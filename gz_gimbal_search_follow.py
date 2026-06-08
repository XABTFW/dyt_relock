#!/usr/bin/env python3
"""Sweep a Gazebo x500 gimbal and follow target LOS offsets when available.

The script publishes directly to Gazebo's gimbal joint command topics. During
search it scans yaw in rows. When a detector sends a fresh target packet over
UDP, it adjusts yaw/pitch to center the target.

Accepted UDP JSON examples:
  {"valid": true, "los_x_rad": 0.03, "los_y_rad": -0.01}
  {"target_valid": true, "los_x_deg": 2.0, "los_y_deg": -1.0}

Accepted UDP text example:
  1 0.03 -0.01
"""

import argparse
import json
import math
import socket
import subprocess
import sys
import time


def clamp(value, lower, upper):
    return max(lower, min(upper, value))


def wrap_deg(angle):
    """Wrap an angle to the [-180, 180) range for continuous yaw rotation."""
    return (angle + 180.0) % 360.0 - 180.0


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--model", default="x500_gimbal", help="Gazebo model name, or 'auto'")
    parser.add_argument("--gz-bin", default="gz", help="Gazebo CLI executable")
    parser.add_argument("--rate", type=float, default=20.0, help="command publish rate in Hz")
    parser.add_argument("--yaw-min", type=float, default=-70.0, help="search yaw min in deg")
    parser.add_argument("--yaw-max", type=float, default=70.0, help="search yaw max in deg")
    parser.add_argument("--pitch-top", type=float, default=20.0, help="search top pitch in deg")
    parser.add_argument("--pitch-bottom", type=float, default=-45.0, help="search bottom pitch in deg")
    parser.add_argument("--pitch-step", type=float, default=15.0, help="pitch row step in deg")
    parser.add_argument("--yaw-speed", type=float, default=25.0, help="search yaw speed in deg/s")
    parser.add_argument("--udp-bind", default="127.0.0.1", help="target packet bind address")
    parser.add_argument("--udp-port", type=int, default=15200, help="target packet UDP port")
    parser.add_argument("--no-target-udp", action="store_true", help="disable UDP target input and search forever")
    parser.add_argument("--target-timeout", type=float, default=1.0, help="target freshness timeout in seconds")
    parser.add_argument("--follow-gain", type=float, default=0.35, help="LOS-to-gimbal proportional gain (fraction of error corrected per measurement)")
    parser.add_argument("--max-follow-step", type=float, default=12.0, help="max follow correction per measurement in deg")
    parser.add_argument("--follow-deadzone", type=float, default=0.3, help="LOS deadzone in deg to suppress jitter near center")
    parser.add_argument("--follow-yaw-360", dest="follow_yaw_360", action="store_true", default=True,
                        help="allow continuous 360deg yaw while following (default on; yaw joint is unlimited)")
    parser.add_argument("--no-follow-yaw-360", dest="follow_yaw_360", action="store_false",
                        help="disable 360deg yaw and clamp yaw to --follow-yaw-limit instead")
    parser.add_argument("--follow-yaw-limit", type=float, default=120.0,
                        help="max yaw magnitude while following in deg (only used with --no-follow-yaw-360)")
    parser.add_argument("--follow-pitch-up", type=float, default=85.0,
                        help="max upward pitch while following in deg (gimbal joint upper limit)")
    parser.add_argument("--follow-pitch-down", type=float, default=-135.0,
                        help="max downward pitch while following in deg (gimbal joint lower limit)")
    parser.add_argument("--coast-time", type=float, default=1.5, help="keep moving from last target offset after target loss")
    parser.add_argument("--coast-gain", type=float, default=1.0, help="coast correction scale after target loss")
    parser.add_argument("--reacquire-timeout", type=float, default=3.0, help="local search time around last lock before global search")
    parser.add_argument("--reacquire-yaw-range", type=float, default=18.0, help="local reacquire yaw range around last lock in deg")
    parser.add_argument("--reacquire-pitch-range", type=float, default=10.0, help="local reacquire pitch range around last lock in deg")
    parser.add_argument("--reacquire-speed", type=float, default=50.0, help="local reacquire scan speed in deg/s")
    parser.add_argument("--yaw-sign", type=float, default=1.0, help="set -1 if horizontal tracking moves the wrong way")
    parser.add_argument("--pitch-sign", type=float, default=-1.0, help="set 1 if vertical tracking moves the wrong way")
    parser.add_argument("--gimbal-feedback", dest="gimbal_feedback", action="store_true", default=True,
                        help="send current gimbal yaw/pitch back to the detector over UDP (default on)")
    parser.add_argument("--no-gimbal-feedback", dest="gimbal_feedback", action="store_false",
                        help="disable gimbal angle UDP feedback to the detector")
    parser.add_argument("--gimbal-feedback-host", default="127.0.0.1",
                        help="detector host to receive gimbal angle feedback")
    parser.add_argument("--gimbal-feedback-port", type=int, default=15201,
                        help="detector UDP port to receive gimbal angle feedback")
    parser.add_argument("--dry-run", action="store_true", help="print commands without publishing to Gazebo")
    return parser.parse_args()


def list_gz_topics(gz_bin):
    try:
        result = subprocess.run(
            [gz_bin, "topic", "-l"],
            check=True,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
    except (OSError, subprocess.CalledProcessError) as exc:
        raise RuntimeError(f"failed to list Gazebo topics with '{gz_bin} topic -l': {exc}") from exc

    return result.stdout.splitlines()


def resolve_topics(gz_bin, model):
    if model != "auto":
        return (
            f"/model/{model}/command/gimbal_yaw",
            f"/model/{model}/command/gimbal_pitch",
        )

    yaw_topic = None
    pitch_topic = None

    for topic in list_gz_topics(gz_bin):
        if topic.endswith("/command/gimbal_yaw"):
            yaw_topic = topic

        elif topic.endswith("/command/gimbal_pitch"):
            pitch_topic = topic

    if yaw_topic is None or pitch_topic is None:
        raise RuntimeError("could not auto-detect gimbal command topics; pass --model x500_gimbal")

    return yaw_topic, pitch_topic


class GzPublisher:
    def __init__(self, gz_bin, yaw_topic, pitch_topic, dry_run):
        self._gz_bin = gz_bin
        self._yaw_topic = yaw_topic
        self._pitch_topic = pitch_topic
        self._dry_run = dry_run
        self._warned = False

    def publish(self, yaw_deg, pitch_deg):
        yaw_rad = math.radians(yaw_deg)
        pitch_rad = math.radians(pitch_deg)

        if self._dry_run:
            print(f"yaw={yaw_deg:+7.2f}deg pitch={pitch_deg:+7.2f}deg", flush=True)
            return

        processes = (
            (self._yaw_topic, self._start_publish(self._yaw_topic, yaw_rad)),
            (self._pitch_topic, self._start_publish(self._pitch_topic, pitch_rad)),
        )

        for topic, process in processes:
            self._finish_publish(topic, process)

    def _start_publish(self, topic, value_rad):
        payload = f"data: {value_rad:.7f}"

        try:
            return subprocess.Popen(
                [self._gz_bin, "topic", "-t", topic, "-m", "gz.msgs.Double", "-p", payload],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.PIPE,
                text=True,
            )
        except OSError as exc:
            if not self._warned:
                print(f"Gazebo publish failed on {topic}: {exc}", file=sys.stderr)
                self._warned = True
            return None

    def _finish_publish(self, topic, process):
        if process is None:
            return

        _stdout, stderr = process.communicate()

        if process.returncode != 0 and not self._warned:
            message = stderr.strip() if stderr else f"exit code {process.returncode}"
            print(f"Gazebo publish failed on {topic}: {message}", file=sys.stderr)
            self._warned = True


class GimbalFeedbackSender:
    """Send the current commanded gimbal yaw/pitch (deg) back to the detector.

    The detector writes these into the DYT frame's gimbal angle fields so the
    flight controller can rotate the camera LOS into the body/NED frame
    correctly (otherwise it assumes the camera points straight forward).
    """

    def __init__(self, host, port):
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._addr = (host, port)
        self._warned = False

    def send(self, yaw_deg, pitch_deg):
        msg = {
            "gimbal_yaw_deg": float(yaw_deg),
            "gimbal_pitch_deg": float(pitch_deg),
        }

        try:
            self._sock.sendto(
                json.dumps(msg, separators=(",", ":")).encode("utf-8"),
                self._addr,
            )
        except OSError as exc:
            if not self._warned:
                print(f"gimbal feedback UDP send failed: {exc}", file=sys.stderr)
                self._warned = True


class TargetReceiver:
    def __init__(self, bind_addr, port):
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._sock.bind((bind_addr, port))
        self._sock.setblocking(False)
        self.valid = False
        self.los_x_rad = 0.0
        self.los_y_rad = 0.0
        self.last_update = 0.0
        self.last_valid_update = 0.0

    def poll(self):
        while True:
            try:
                data, _addr = self._sock.recvfrom(2048)
            except BlockingIOError:
                return

            self._parse(data.decode("utf-8", errors="replace").strip())

    def fresh(self, now, timeout_s):
        return self.valid and (now - self.last_update) <= timeout_s

    def valid_age(self, now):
        if self.last_valid_update <= 0.0:
            return float("inf")

        return now - self.last_valid_update

    def _parse(self, text):
        try:
            msg = json.loads(text)
            valid = bool(msg.get("valid", msg.get("target_valid", False)))

            if "los_x_rad" in msg:
                los_x = float(msg["los_x_rad"])
            else:
                los_x = math.radians(float(msg.get("los_x_deg", 0.0)))

            if "los_y_rad" in msg:
                los_y = float(msg["los_y_rad"])
            else:
                los_y = math.radians(float(msg.get("los_y_deg", 0.0)))

        except (ValueError, TypeError, json.JSONDecodeError):
            parts = text.replace(",", " ").split()

            if len(parts) < 3:
                return

            try:
                valid = bool(int(float(parts[0])))
                los_x = float(parts[1])
                los_y = float(parts[2])
            except ValueError:
                return

        self.valid = valid
        self.los_x_rad = los_x
        self.los_y_rad = los_y
        self.last_update = time.monotonic()

        if valid:
            self.last_valid_update = self.last_update


class SearchPattern:
    def __init__(self, args):
        self._yaw_min = args.yaw_min
        self._yaw_max = args.yaw_max
        self._pitch_top = args.pitch_top
        self._pitch_bottom = args.pitch_bottom
        self._pitch_step = max(1.0, args.pitch_step)
        self._yaw_speed = max(1.0, args.yaw_speed)
        self.yaw = self._yaw_min
        self.pitch = self._pitch_top
        self._direction = 1.0

    def update(self, dt):
        self.yaw += self._direction * self._yaw_speed * dt

        if self.yaw >= self._yaw_max:
            self.yaw = self._yaw_max
            self._next_row()

        elif self.yaw <= self._yaw_min:
            self.yaw = self._yaw_min
            self._next_row()

        return self.yaw, self.pitch

    def _next_row(self):
        self._direction *= -1.0
        self.pitch -= self._pitch_step

        if self.pitch < self._pitch_bottom:
            self.pitch = self._pitch_top

    def set_pose(self, yaw, pitch):
        self.yaw = clamp(yaw, self._yaw_min, self._yaw_max)
        self.pitch = clamp(pitch, self._pitch_bottom, self._pitch_top)

    def set_pose_follow(self, yaw, pitch, yaw_limit, pitch_down, pitch_up, yaw_360=False):
        # While tracking, the gimbal must be free to move across its full
        # mechanical travel rather than being capped at the search scan window.
        if yaw_360:
            # Yaw joint is unlimited in the model: allow continuous rotation and
            # wrap the commanded angle so it never grows without bound.
            self.yaw = wrap_deg(yaw)
        else:
            self.yaw = clamp(yaw, -yaw_limit, yaw_limit)

        self.pitch = clamp(pitch, pitch_down, pitch_up)


class LocalReacquirePattern:
    def __init__(self, args):
        self._yaw_range = max(0.0, args.reacquire_yaw_range)
        self._pitch_range = max(0.0, args.reacquire_pitch_range)
        self._speed = max(1.0, args.reacquire_speed)
        self._start_time = 0.0
        self._anchor_yaw = 0.0
        self._anchor_pitch = 0.0

    def reset(self, yaw, pitch, now):
        self._anchor_yaw = yaw
        self._anchor_pitch = pitch
        self._start_time = now

    def clear(self):
        self._start_time = 0.0

    def active(self):
        return self._start_time > 0.0

    def update(self, now):
        elapsed = max(0.0, now - self._start_time)
        yaw_omega = self._speed / max(self._yaw_range, 1.0)
        pitch_omega = self._speed / max(self._pitch_range * 2.0, 1.0)
        yaw = self._anchor_yaw + self._yaw_range * math.sin(elapsed * yaw_omega)
        pitch = self._anchor_pitch + self._pitch_range * math.sin(elapsed * pitch_omega)
        return yaw, pitch


def main():
    args = parse_args()

    if args.rate <= 0.0:
        raise SystemExit("--rate must be greater than zero")

    try:
        yaw_topic, pitch_topic = resolve_topics(args.gz_bin, args.model)
    except RuntimeError as exc:
        print(exc, file=sys.stderr)
        raise SystemExit(1) from exc

    print(f"publishing gimbal commands: yaw={yaw_topic}, pitch={pitch_topic}", flush=True)
    if args.no_target_udp:
        print("target UDP input disabled; search mode only", flush=True)

    else:
        print(f"listening for target offsets on udp://{args.udp_bind}:{args.udp_port}", flush=True)

    publisher = GzPublisher(args.gz_bin, yaw_topic, pitch_topic, args.dry_run)
    target = None if args.no_target_udp else TargetReceiver(args.udp_bind, args.udp_port)
    feedback = GimbalFeedbackSender(args.gimbal_feedback_host, args.gimbal_feedback_port) if args.gimbal_feedback else None

    if feedback is not None:
        print(
            f"sending gimbal angle feedback to udp://{args.gimbal_feedback_host}:{args.gimbal_feedback_port}",
            flush=True,
        )
    search = SearchPattern(args)
    reacquire = LocalReacquirePattern(args)

    period = 1.0 / args.rate
    last_time = time.monotonic()
    last_log = 0.0
    last_consumed_update = 0.0
    target_yaw_rate = 0.0
    target_pitch_rate = 0.0
    last_lock_yaw = search.yaw
    last_lock_pitch = search.pitch

    while True:
        now = time.monotonic()
        dt = max(0.0, now - last_time)
        last_time = now
        if target is not None:
            target.poll()

        if target is not None and target.fresh(now, args.target_timeout):
            new_measurement = target.last_update != last_consumed_update

            if new_measurement:
                los_x_deg = math.degrees(target.los_x_rad)
                los_y_deg = math.degrees(target.los_y_rad)

                # Deadzone keeps the gimbal still when the target is already centered.
                if abs(los_x_deg) < args.follow_deadzone:
                    los_x_deg = 0.0

                if abs(los_y_deg) < args.follow_deadzone:
                    los_y_deg = 0.0

                yaw_correction = args.yaw_sign * los_x_deg * args.follow_gain
                pitch_correction = args.pitch_sign * los_y_deg * args.follow_gain
                yaw_correction = clamp(yaw_correction, -args.max_follow_step, args.max_follow_step)
                pitch_correction = clamp(pitch_correction, -args.max_follow_step, args.max_follow_step)

                # Estimate target angular rate from the gap between measurements
                # so we can keep tracking smoothly while waiting for the next packet.
                # The rate is low-pass filtered so a single noisy frame cannot
                # whip the gimbal around; this keeps the lock glued to the target.
                meas_dt = target.last_update - last_consumed_update

                if 0.0 < last_consumed_update and 1e-3 < meas_dt <= args.target_timeout:
                    rate_alpha = 0.5
                    new_yaw_rate = yaw_correction / meas_dt
                    new_pitch_rate = pitch_correction / meas_dt
                    target_yaw_rate = rate_alpha * target_yaw_rate + (1.0 - rate_alpha) * new_yaw_rate
                    target_pitch_rate = rate_alpha * target_pitch_rate + (1.0 - rate_alpha) * new_pitch_rate
                else:
                    target_yaw_rate = 0.0
                    target_pitch_rate = 0.0

                yaw = search.yaw + yaw_correction
                pitch = search.pitch + pitch_correction
                search.set_pose_follow(yaw, pitch, args.follow_yaw_limit,
                                       args.follow_pitch_down, args.follow_pitch_up,
                                       args.follow_yaw_360)
                last_consumed_update = target.last_update
                last_lock_yaw = search.yaw
                last_lock_pitch = search.pitch

            else:
                # No new packet yet: feed forward the estimated target motion
                # instead of re-applying the stale offset (which caused overshoot).
                # Cap the per-step feed-forward so a long gap cannot fling the
                # gimbal far off the last known target direction.
                ff_yaw = clamp(target_yaw_rate * dt, -args.max_follow_step, args.max_follow_step)
                ff_pitch = clamp(target_pitch_rate * dt, -args.max_follow_step, args.max_follow_step)
                yaw = search.yaw + ff_yaw
                pitch = search.pitch + ff_pitch
                search.set_pose_follow(yaw, pitch, args.follow_yaw_limit,
                                       args.follow_pitch_down, args.follow_pitch_up,
                                       args.follow_yaw_360)

            reacquire.clear()
            mode = "follow"

        elif target is not None and target.valid_age(now) <= args.coast_time:
            age_s = target.valid_age(now)
            coast_scale = args.coast_gain * (1.0 - age_s / max(args.coast_time, 1e-3))
            yaw = search.yaw + target_yaw_rate * dt * coast_scale
            pitch = search.pitch + target_pitch_rate * dt * coast_scale
            search.set_pose_follow(yaw, pitch, args.follow_yaw_limit,
                                   args.follow_pitch_down, args.follow_pitch_up,
                                   args.follow_yaw_360)
            mode = "coast"

        elif target is not None and target.valid_age(now) <= args.reacquire_timeout:
            if not reacquire.active():
                reacquire.reset(last_lock_yaw, last_lock_pitch, now)

            yaw, pitch = reacquire.update(now)
            search.set_pose(yaw, pitch)
            mode = "reacq"

        else:
            reacquire.clear()
            last_consumed_update = 0.0
            target_yaw_rate = 0.0
            target_pitch_rate = 0.0
            yaw, pitch = search.update(dt)
            mode = "search"

        publisher.publish(search.yaw, search.pitch)

        if feedback is not None:
            feedback.send(search.yaw, search.pitch)

        if now - last_log > 1.0:
            print(f"{mode:6s} yaw={search.yaw:+7.2f}deg pitch={search.pitch:+7.2f}deg", flush=True)
            last_log = now

        sleep_s = period - (time.monotonic() - now)

        if sleep_s > 0.0:
            time.sleep(sleep_s)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nstopped")
