/****************************************************************************
 *
 *   Copyright (c) 2026 PX4 Development Team. All rights reserved.
 *
 ****************************************************************************/

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <px4_platform_common/getopt.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/SubscriptionInterval.hpp>
#include <uORB/topics/dyt_command.h>
#include <uORB/topics/dyt_status_reply.h>
#include <uORB/topics/dyt_target.h>
#include <uORB/topics/parameter_update.h>

#include <lib/mathlib/mathlib.h>

using namespace time_literals;

class DytGimbal : public ModuleBase<DytGimbal>, public ModuleParams, public px4::ScheduledWorkItem
{
public:
	explicit DytGimbal(const char *device_path);
	~DytGimbal() override;

	static int task_spawn(int argc, char *argv[]);
	static int custom_command(int argc, char *argv[]);
	static int print_usage(const char *reason = nullptr);

	bool init();
	int print_status() override;
	void show_status();

private:
	static constexpr uint8_t FRAME_SYNC_1{0xFC};
	static constexpr uint8_t FRAME_SYNC_2_TELEMETRY{0x2C};
	static constexpr uint8_t FRAME_SYNC_2_STATUS_REPLY{0x2C};
	static constexpr uint8_t FRAME_TAIL{0xF0};
	static constexpr uint8_t COMMAND_SYNC_1{0xFB};
	static constexpr size_t FIXED_FRAME_LEN{64};
	static constexpr size_t MAX_FRAME_LEN{64};
	static constexpr size_t COMMAND_LEN{44};
	static constexpr hrt_abstime CONTROL_FRAME_INTERVAL{40_ms};
	static constexpr unsigned CENTER_SEQUENCE_DELAY_US{80000};
	

	void Run() override;

	bool open_serial();
	void close_serial();
	int configure_serial(int fd, int baud);
	speed_t baud_to_speed(int baud) const;

	void read_serial();
	void process_byte(uint8_t byte);
	void process_primary_byte(uint8_t byte);
	void reset_primary_frame_parser();
	bool validate_frame(const uint8_t *frame, size_t frame_len) const;
	void handle_primary_frame(const uint8_t *frame, size_t frame_len, hrt_abstime now);
	void handle_frame(const uint8_t *frame, hrt_abstime now);
	void handle_status_reply_frame(const uint8_t *frame, size_t frame_len, hrt_abstime now);
	void maybe_log_target(const dyt_target_s &target, const uint8_t *frame);
	void log_status_reply(const dyt_status_reply_s &reply, const uint8_t *frame, size_t frame_len);
	void maybe_log_raw_frame(const char *label, const uint8_t *frame, size_t frame_len);
	void publish_link_state(hrt_abstime now, uint8_t tracking_state);
	const char *control_name(uint8_t control) const;
	const char *geotrack_cmd_name(uint8_t command) const;
	const char *lift_state_name(uint8_t state) const;

	void handle_command_updates();
	void send_startup_home_if_needed(hrt_abstime now);
	void send_periodic_neutral_if_needed(hrt_abstime now);
	void send_protocol_command(const dyt_command_s &cmd);
	void publish_shell_command(uint8_t command);
	void send_center_sequence();
	int16_t angle_deg_to_cdeg(float angle_deg) const;
	bool write_command_buffer(const uint8_t *buffer, size_t buffer_len);
	void send_command_frame(uint8_t control, int16_t param_x, int16_t param_y, uint8_t param3, int8_t zoom_rate,
				bool record_command = true);

	void update_params_if_needed();

	int _uart_fd{-1};
	char _device_path[32]{};
	uint8_t _frame[MAX_FRAME_LEN]{};
	size_t _frame_index{0};
	bool _awaiting_sync_2{false};
	size_t _expected_frame_len{0};
	uint8_t _frame_type{0};

	hrt_abstime _last_rx_time{0};
	hrt_abstime _last_status_reply_time{0};
	hrt_abstime _last_open_attempt{0};
	hrt_abstime _last_state_publish{0};
	hrt_abstime _last_command_time{0};
	hrt_abstime _last_control_frame_time{0};
	hrt_abstime _last_target_log_time{0};
	hrt_abstime _startup_home_time{0};
	hrt_abstime _startup_home_end_time{0};
	hrt_abstime _startup_home_next_time{0};

	uint32_t _frame_counter{0};
	uint32_t _status_reply_counter{0};
	uint16_t _parse_error_count{0};
	uint16_t _read_error_count{0};
	uint16_t _write_error_count{0};
	uint64_t _rx_byte_count{0};
	uint32_t _sync1_count{0};
	uint32_t _sync2_count{0};
	uint32_t _status_reply_sync2_count{0};
	uint32_t _other_frame_type_count{0};
	uint8_t _last_sync2_byte{0};
	uint32_t _command_tx_count{0};
	uint32_t _startup_home_count{0};
	uint8_t _last_command_control{0};
	int _last_write_errno{0};
	int _last_write_result{0};
	bool _startup_home_sent{false};
	bool _startup_home_canceled{false};

	dyt_target_s _last_target{};
	dyt_status_reply_s _last_status_reply{};

	uORB::Subscription _dyt_command_sub{ORB_ID(dyt_command)};
	uORB::SubscriptionInterval _parameter_update_sub{ORB_ID(parameter_update), 1_s};
	uORB::Publication<dyt_command_s> _dyt_command_pub{ORB_ID(dyt_command)};
	uORB::Publication<dyt_status_reply_s> _dyt_status_reply_pub{ORB_ID(dyt_status_reply)};
	uORB::Publication<dyt_target_s> _dyt_target_pub{ORB_ID(dyt_target)};

	DEFINE_PARAMETERS(
		(ParamInt<px4::params::DYT_BAUD>) _param_dyt_baud,
		(ParamInt<px4::params::DYT_TO_MS>) _param_dyt_timeout_ms,
		(ParamInt<px4::params::DYT_RTRY_MS>) _param_dyt_retry_ms,
		(ParamInt<px4::params::DYT_LOG_MS>) _param_dyt_log_ms,
		(ParamInt<px4::params::DYT_RAWLOG>) _param_dyt_rawlog,
		(ParamInt<px4::params::DYT_HOME_EN>) _param_dyt_home_en,
		(ParamInt<px4::params::DYT_HOME_DLY>) _param_dyt_home_delay_ms,
		(ParamInt<px4::params::DYT_HOME_DUR>) _param_dyt_home_duration_ms,
		(ParamInt<px4::params::DYT_HOME_INT>) _param_dyt_home_interval_ms,
		(ParamFloat<px4::params::DYT_HOME_YAW>) _param_dyt_home_yaw_deg,
		(ParamFloat<px4::params::DYT_HOME_PIT>) _param_dyt_home_pitch_deg
	);
};

DytGimbal::DytGimbal(const char *device_path) :
	ModuleParams(nullptr),
	ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::hp_default)
{
	strncpy(_device_path, device_path, sizeof(_device_path) - 1);
	_device_path[sizeof(_device_path) - 1] = '\0';
	memset(&_last_target, 0, sizeof(_last_target));
	_last_target.range_m = NAN;
}

DytGimbal::~DytGimbal()
{
	close_serial();
}

bool DytGimbal::init()
{
	ScheduleOnInterval(5_ms);
	return true;
}

int DytGimbal::print_status()
{
	show_status();
	return 0;
}

void DytGimbal::show_status()
{
	PX4_INFO("port: %s", _device_path);
	PX4_INFO("fd: %d", _uart_fd);
	PX4_INFO("rx bytes: %llu", static_cast<unsigned long long>(_rx_byte_count));
	PX4_INFO("sync 0xFC: %lu", static_cast<unsigned long>(_sync1_count));
	PX4_INFO("sync 0x2C: %lu", static_cast<unsigned long>(_sync2_count));
	PX4_INFO("json replies: %lu", static_cast<unsigned long>(_status_reply_sync2_count));
	PX4_INFO("other after 0xFC: %lu", static_cast<unsigned long>(_other_frame_type_count));
	PX4_INFO("last byte after 0xFC: 0x%02x", _last_sync2_byte);
	PX4_INFO("frames: %lu", static_cast<unsigned long>(_frame_counter));
	PX4_INFO("status replies: %lu", static_cast<unsigned long>(_status_reply_counter));
	PX4_INFO("parse errors: %u", _parse_error_count);
	PX4_INFO("read errors: %u", _read_error_count);
	PX4_INFO("write errors: %u", _write_error_count);
	PX4_INFO("last write result: %d errno: %d", _last_write_result, _last_write_errno);
	PX4_INFO("tx commands: %lu", static_cast<unsigned long>(_command_tx_count));
	PX4_INFO("last command control: 0x%02x", _last_command_control);
	PX4_INFO("last command tx: %.3f s", static_cast<double>(_last_command_time > 0 ?
			(hrt_absolute_time() - _last_command_time) * 1e-6 : -1.0));
	PX4_INFO("last target: state=%u valid=%u follow=%u motor=%u elect_lock=%u yaw=%.2f deg",
		 static_cast<unsigned>(_last_target.tracking_state),
		 static_cast<unsigned>(_last_target.target_valid),
		 static_cast<unsigned>((_last_target.status2 & (1 << 2)) != 0),
		 static_cast<unsigned>((_last_target.status2 & (1 << 3)) != 0),
		 static_cast<unsigned>((_last_target.status2 & (1 << 1)) != 0),
		 static_cast<double>(math::degrees(_last_target.gimbal_yaw_rad)));
	PX4_INFO("startup home: en=%ld sent=%u canceled=%u count=%lu yaw=%.1f pitch=%.1f next=%.3f s",
		 static_cast<long>(_param_dyt_home_en.get()),
		 static_cast<unsigned>(_startup_home_sent),
		 static_cast<unsigned>(_startup_home_canceled),
		 static_cast<unsigned long>(_startup_home_count),
		 static_cast<double>(_param_dyt_home_yaw_deg.get()),
		 static_cast<double>(_param_dyt_home_pitch_deg.get()),
		 static_cast<double>(_startup_home_next_time > 0 && hrt_absolute_time() < _startup_home_next_time ?
			(_startup_home_next_time - hrt_absolute_time()) * 1e-6 : 0.0));
	PX4_INFO("debug log period: %ld ms", static_cast<long>(_param_dyt_log_ms.get()));
	PX4_INFO("raw frame log: %ld", static_cast<long>(_param_dyt_rawlog.get()));
	PX4_INFO("last rx: %.3f s", static_cast<double>(_last_rx_time > 0 ?
			(hrt_absolute_time() - _last_rx_time) * 1e-6 : -1.0));
	PX4_INFO("last reply rx: %.3f s", static_cast<double>(_last_status_reply_time > 0 ?
			(hrt_absolute_time() - _last_status_reply_time) * 1e-6 : -1.0));
}

void DytGimbal::Run()
{
	if (should_exit()) {
		ScheduleClear();
		close_serial();
		exit_and_cleanup();
		return;
	}

	update_params_if_needed();

	const hrt_abstime now = hrt_absolute_time();

	if (_uart_fd < 0) {
		if ((now - _last_open_attempt) >= static_cast<hrt_abstime>(_param_dyt_retry_ms.get()) * 1000ULL) {
			_last_open_attempt = now;
			open_serial();
		}

		publish_link_state(now, dyt_target_s::TRACKING_STATE_TIMEOUT);
		return;
	}

	read_serial();
	handle_command_updates();
	send_startup_home_if_needed(now);
	send_periodic_neutral_if_needed(now);

	const hrt_abstime timeout_us = static_cast<hrt_abstime>(_param_dyt_timeout_ms.get()) * 1000ULL;
	const hrt_abstime timeout_check_time = hrt_absolute_time();

	if ((_last_rx_time == 0) || ((timeout_check_time - _last_rx_time) > timeout_us)) {
		publish_link_state(timeout_check_time, dyt_target_s::TRACKING_STATE_TIMEOUT);
	}
}

void DytGimbal::update_params_if_needed()
{
	if (_parameter_update_sub.updated()) {
		parameter_update_s update{};
		_parameter_update_sub.copy(&update);
		updateParams();

		if (_uart_fd >= 0) {
			configure_serial(_uart_fd, _param_dyt_baud.get());
		}
	}
}

bool DytGimbal::open_serial()
{
	_uart_fd = ::open(_device_path, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if (_uart_fd < 0) {
		PX4_WARN("open %s failed (%d)", _device_path, errno);
		return false;
	}

	if (configure_serial(_uart_fd, _param_dyt_baud.get()) != PX4_OK) {
		close_serial();
		return false;
	}

	PX4_INFO("opened %s @ %ld", _device_path, static_cast<long>(_param_dyt_baud.get()));
	const int32_t delay_ms = math::constrain(_param_dyt_home_delay_ms.get(), int32_t{0}, int32_t{30000});
	const int32_t duration_ms = math::constrain(_param_dyt_home_duration_ms.get(), int32_t{0}, int32_t{30000});
	_startup_home_time = hrt_absolute_time() + static_cast<hrt_abstime>(delay_ms) * 1000ULL;
	_startup_home_end_time = _startup_home_time + static_cast<hrt_abstime>(duration_ms) * 1000ULL;
	_startup_home_next_time = _startup_home_time;
	_startup_home_sent = false;
	_startup_home_canceled = false;
	_startup_home_count = 0;
	return true;
}

void DytGimbal::close_serial()
{
	if (_uart_fd >= 0) {
		::close(_uart_fd);
		_uart_fd = -1;
	}

	_startup_home_time = 0;
	_startup_home_end_time = 0;
	_startup_home_next_time = 0;
	_startup_home_sent = false;
	_startup_home_canceled = false;
	_startup_home_count = 0;
}

speed_t DytGimbal::baud_to_speed(int baud) const
{
	switch (baud) {
	case 9600: return B9600;
	case 19200: return B19200;
	case 38400: return B38400;
	case 57600: return B57600;
	case 115200: return B115200;
#ifdef B230400
	case 230400: return B230400;
#endif
#ifdef B460800
	case 460800: return B460800;
#endif
#ifdef B921600
	case 921600: return B921600;
#endif
	default: return 0;
	}
}

int DytGimbal::configure_serial(int fd, int baud)
{
	const speed_t speed = baud_to_speed(baud);

	if (speed == 0) {
		PX4_ERR("unsupported baud %d", baud);
		return PX4_ERROR;
	}

	struct termios uart_config {};

	if (tcgetattr(fd, &uart_config) < 0) {
		PX4_ERR("tcgetattr failed (%d)", errno);
		return PX4_ERROR;
	}

	cfmakeraw(&uart_config);
	uart_config.c_cflag |= (CLOCAL | CREAD);
	uart_config.c_cflag &= ~CSIZE;
	uart_config.c_cflag |= CS8;
	uart_config.c_cflag &= ~PARENB;
	uart_config.c_cflag &= ~CSTOPB;
#ifdef CRTSCTS
	uart_config.c_cflag &= ~CRTSCTS;
#endif
	uart_config.c_cc[VMIN] = 0;
	uart_config.c_cc[VTIME] = 0;

	if (cfsetispeed(&uart_config, speed) < 0 || cfsetospeed(&uart_config, speed) < 0) {
		PX4_ERR("baud config failed (%d)", errno);
		return PX4_ERROR;
	}

	if (tcsetattr(fd, TCSANOW, &uart_config) < 0) {
		PX4_ERR("tcsetattr failed (%d)", errno);
		return PX4_ERROR;
	}

	return PX4_OK;
}

void DytGimbal::read_serial()
{
	uint8_t buffer[128]{};

	for (;;) {
		const ssize_t nread = ::read(_uart_fd, buffer, sizeof(buffer));
		bool would_block = false;

		if (nread < 0) {
			would_block = (errno == EAGAIN);
#if defined(EWOULDBLOCK) && (EWOULDBLOCK != EAGAIN)
			would_block = would_block || (errno == EWOULDBLOCK);
#endif
		}

		if (nread > 0) {
			_rx_byte_count += static_cast<uint64_t>(nread);

			for (ssize_t i = 0; i < nread; ++i) {
				process_byte(buffer[i]);
			}

		} else if (nread == 0 || would_block) {
			break;

		} else {
			++_read_error_count;
			close_serial();
			break;
		}
	}
}

void DytGimbal::process_byte(uint8_t byte)
{
	process_primary_byte(byte);
}

void DytGimbal::reset_primary_frame_parser()
{
	_frame_index = 0;
	_awaiting_sync_2 = false;
	_expected_frame_len = 0;
	_frame_type = 0;
}

void DytGimbal::process_primary_byte(uint8_t byte)
{
	if (_frame_index == 0) {
		if (byte == FRAME_SYNC_1) {
			++_sync1_count;
			_frame[_frame_index++] = byte;
			_awaiting_sync_2 = true;
			_expected_frame_len = 0;
			_frame_type = 0;
		}

		return;
	}

	if (_awaiting_sync_2) {
		_last_sync2_byte = byte;

		if (byte == FRAME_SYNC_2_TELEMETRY) {
			_frame_type = byte;
			_frame[_frame_index++] = byte;
			_awaiting_sync_2 = false;
			++_sync2_count;
			_expected_frame_len = FIXED_FRAME_LEN;

		} else if (byte == FRAME_SYNC_1) {
			++_sync1_count;
			_frame[0] = FRAME_SYNC_1;
			_frame_index = 1;

		} else {
			++_other_frame_type_count;
			_frame_index = 0;
			_awaiting_sync_2 = false;
		}

		return;
	}

	if (_frame_index >= MAX_FRAME_LEN) {
		++_parse_error_count;
		publish_link_state(hrt_absolute_time(), dyt_target_s::TRACKING_STATE_ERROR);
		reset_primary_frame_parser();
		return;
	}

	_frame[_frame_index++] = byte;

	if ((_expected_frame_len > 0) && (_frame_index == _expected_frame_len)) {
		const hrt_abstime now = hrt_absolute_time();

		if (validate_frame(_frame, _expected_frame_len)) {
			handle_primary_frame(_frame, _expected_frame_len, now);

		} else {
			++_parse_error_count;
			publish_link_state(now, dyt_target_s::TRACKING_STATE_ERROR);
		}

		reset_primary_frame_parser();
	}
}

bool DytGimbal::validate_frame(const uint8_t *frame, size_t frame_len) const
{
	if (frame_len != FIXED_FRAME_LEN || frame[0] != FRAME_SYNC_1 || frame[1] != FRAME_SYNC_2_TELEMETRY ||
	    frame[frame_len - 1] != FRAME_TAIL) {
		return false;
	}

	uint8_t checksum = 0;

	for (size_t i = 2; i <= 61; ++i) {
		checksum = static_cast<uint8_t>(checksum ^ frame[i]);
	}

	return checksum == frame[62];
}

void DytGimbal::handle_primary_frame(const uint8_t *frame, size_t frame_len, hrt_abstime now)
{
	if (frame_len != FIXED_FRAME_LEN) {
		return;
	}

	if (frame[2] == 0xFF) {
		++_status_reply_sync2_count;
		handle_status_reply_frame(frame, frame_len, now);

	} else {
		handle_frame(frame, now);
	}
}

void DytGimbal::handle_frame(const uint8_t *frame, hrt_abstime now)
{
	const auto u16_at = [frame](size_t index) -> uint16_t {
		return static_cast<uint16_t>(frame[index]) | (static_cast<uint16_t>(frame[index + 1]) << 8);
	};

	const auto s16_at = [&u16_at](size_t index) -> int16_t {
		return static_cast<int16_t>(u16_at(index));
	};

	const uint16_t pod_status = u16_at(4);
	const uint16_t camera_status = u16_at(6);
	const uint8_t servo_state = frame[8];
	const uint8_t lock_state = static_cast<uint8_t>((pod_status >> 9) & 0x3);
	const bool locked = (lock_state == 1) || (lock_state == 2) || (servo_state == 0x07);
	const int16_t bbox_width = s16_at(54);
	const int16_t bbox_height = s16_at(56);
	const uint16_t range_raw = u16_at(16);
	const uint8_t current_image = frame[47];

	dyt_target_s target{};
	target.timestamp = now;
	target.timestamp_sample = now;
	target.frame_counter = ++_frame_counter;

	target.status1 = frame[3];
	target.status2 = static_cast<uint8_t>(pod_status & 0xFF);
	target.status3 = servo_state;
	target.self_test_raw = frame[3];
	target.parse_error_count = _parse_error_count;

	switch (current_image) {
	case 1:
		target.video_source = dyt_target_s::VIDEO_SOURCE_IR_1;
		break;

	case 2:
		target.video_source = dyt_target_s::VIDEO_SOURCE_VIS_2;
		break;

	default:
		target.video_source = dyt_target_s::VIDEO_SOURCE_VIS_1;
		break;
	}

	target.tracking_algorithm = dyt_target_s::TRACK_ALGO_ADAPTIVE;
	target.auto_hint = (frame[27] != 0) || (frame[28] != 0) || (bbox_width > 0 && bbox_height > 0);
	target.tracking_state = locked ? dyt_target_s::TRACKING_STATE_LOCKED : dyt_target_s::TRACKING_STATE_SEARCH;
	target.target_valid = locked;

	target.image_enhance = false;
	target.recording = (pod_status & (1 << 4)) != 0;
	target.motor_on = servo_state != 0x01;
	target.follow_mode = (servo_state == 0x07) || (servo_state == 0x0B);
	target.laser_on = range_raw > 0;

	target.zoom_ratio = static_cast<float>(frame[48]) * 0.1f;

	target.los_x_rad = math::radians(static_cast<float>(s16_at(58)) * 0.01f);
	target.los_y_rad = math::radians(static_cast<float>(s16_at(60)) * 0.01f);

	target.gimbal_roll_rad = math::radians(static_cast<float>(s16_at(13)) * 0.01f);
	target.gimbal_pitch_rad = math::radians(static_cast<float>(s16_at(11)) * 0.01f);
	target.gimbal_yaw_rad = math::radians(static_cast<float>(s16_at(9)) * 0.01f);

	target.bbox_width_px = bbox_width > 0 ? static_cast<float>(bbox_width) : 0.f;
	target.bbox_height_px = bbox_height > 0 ? static_cast<float>(bbox_height) : 0.f;

	target.gimbal_roll_rate_rad_s = math::radians(static_cast<float>(s16_at(45)) * 0.01f);
	target.gimbal_pitch_rate_rad_s = math::radians(static_cast<float>(s16_at(43)) * 0.01f);
	target.gimbal_yaw_rate_rad_s = math::radians(static_cast<float>(s16_at(41)) * 0.01f);

	target.range_m = (range_raw > 0 && range_raw < UINT16_MAX) ? static_cast<float>(range_raw) : NAN;

	target.selftest_done = (frame[3] & (1 << 7)) != 0;
	target.gyro_calib_failed = ((frame[3] >> 2) & 0x3) == 0x2;
	target.servo_fault = ((frame[3] >> 4) & 0x3) == 0x1;
	target.image_board_fault = ((frame[3] & 0x3) == 0) || ((camera_status & 0x3) == 0);

	target.frame_dt_s = (_last_rx_time > 0) ? (now - _last_rx_time) * 1e-6f : 0.f;
	target.last_rx_age_s = 0.f;

	_last_rx_time = now;
	_last_state_publish = now;
	_last_target = target;
	maybe_log_target(target, frame);
	_dyt_target_pub.publish(target);
}

void DytGimbal::handle_status_reply_frame(const uint8_t *frame, size_t frame_len, hrt_abstime now)
{
	dyt_status_reply_s reply{};
	reply.timestamp = now;
	reply.timestamp_sample = now;
	reply.control_code = frame[2];
	reply.parse_error_count = _parse_error_count;

	size_t payload_length = 0;

	for (size_t i = 3; i < 62; ++i) {
		if (frame[i] != 0) {
			payload_length = i - 2;
		}
	}

	reply.param_length = static_cast<uint8_t>(payload_length);
	reply.truncated = payload_length > sizeof(reply.params);

	const size_t max_params = sizeof(reply.params) / sizeof(reply.params[0]);
	const size_t copy_length = (payload_length < max_params) ? payload_length : max_params;

	for (size_t i = 0; i < copy_length; ++i) {
		reply.params[i] = frame[3 + i];
	}

	_last_status_reply_time = now;
	++_status_reply_counter;
	_last_status_reply = reply;
	_dyt_status_reply_pub.publish(reply);
	log_status_reply(reply, frame, frame_len);
}

void DytGimbal::maybe_log_target(const dyt_target_s &target, const uint8_t *frame)
{
	const int32_t log_period_ms = _param_dyt_log_ms.get();

	if (log_period_ms <= 0) {
		return;
	}

	const hrt_abstime log_period_us = static_cast<hrt_abstime>(log_period_ms) * 1000ULL;

	if ((_last_target_log_time != 0) && ((target.timestamp - _last_target_log_time) < log_period_us)) {
		return;
	}

	_last_target_log_time = target.timestamp;

	const double range_m = PX4_ISFINITE(target.range_m) ? static_cast<double>(target.range_m) : -1.0;

	PX4_INFO("DYT rx #%lu state=%u valid=%u los=(%.2f, %.2f) deg att=(%.2f, %.2f, %.2f) deg range=%.2f m zoom=%.1f bbox=%.0fx%.0f servo=0x%02x dt=%.3f s",
		 static_cast<unsigned long>(target.frame_counter),
		 static_cast<unsigned>(target.tracking_state),
		 static_cast<unsigned>(target.target_valid),
		 static_cast<double>(math::degrees(target.los_x_rad)),
		 static_cast<double>(math::degrees(target.los_y_rad)),
		 static_cast<double>(math::degrees(target.gimbal_roll_rad)),
		 static_cast<double>(math::degrees(target.gimbal_pitch_rad)),
		 static_cast<double>(math::degrees(target.gimbal_yaw_rad)),
		 range_m,
		 static_cast<double>(target.zoom_ratio),
		 static_cast<double>(target.bbox_width_px),
		 static_cast<double>(target.bbox_height_px),
		 static_cast<unsigned>(target.status3),
		 static_cast<double>(target.frame_dt_s));

	maybe_log_raw_frame("DYT raw 0xFC", frame, FIXED_FRAME_LEN);
}

void DytGimbal::log_status_reply(const dyt_status_reply_s &reply, const uint8_t *frame, size_t frame_len)
{
	if ((reply.control_code == 0x3a) && (reply.param_length >= 2)) {
		PX4_INFO("DYT reply ctrl=0x%02x(%s) geotrack_cmd=0x%02x(%s) status=%u",
			 static_cast<unsigned>(reply.control_code),
			 control_name(reply.control_code),
			 static_cast<unsigned>(reply.params[0]),
			 geotrack_cmd_name(reply.params[0]),
			 static_cast<unsigned>(reply.params[1]));

	} else if ((reply.control_code == 0xb0) && (reply.param_length >= 1)) {
		PX4_INFO("DYT reply ctrl=0x%02x(%s) lift_state=0x%02x(%s)",
			 static_cast<unsigned>(reply.control_code),
			 control_name(reply.control_code),
			 static_cast<unsigned>(reply.params[0]),
			 lift_state_name(reply.params[0]));

	} else {
		PX4_INFO("DYT reply ctrl=0x%02x(%s) len=%u p0=0x%02x p1=0x%02x",
			 static_cast<unsigned>(reply.control_code),
			 control_name(reply.control_code),
			 static_cast<unsigned>(reply.param_length),
			 static_cast<unsigned>(reply.params[0]),
			 static_cast<unsigned>(reply.params[1]));
	}

	maybe_log_raw_frame("DYT raw 0xFF", frame, frame_len);
}

void DytGimbal::maybe_log_raw_frame(const char *label, const uint8_t *frame, size_t frame_len)
{
	if (_param_dyt_rawlog.get() <= 0) {
		return;
	}

	char line[MAX_FRAME_LEN * 3 + 1]{};
	size_t offset = 0;

	for (size_t i = 0; i < frame_len; ++i) {
		const int written = snprintf(&line[offset], sizeof(line) - offset, "%02X%s",
					     frame[i], (i + 1 < frame_len) ? " " : "");

		if ((written <= 0) || (static_cast<size_t>(written) >= (sizeof(line) - offset))) {
			break;
		}

		offset += static_cast<size_t>(written);
	}

	PX4_INFO("%s: %s", label, line);
}

const char *DytGimbal::control_name(uint8_t control) const
{
	switch (control) {
	case 0x00: return "neutral";
	case 0x11: return "aircraft_state";
	case 0x23: return "track_template";
	case 0x25: return "video_mode";
	case 0x2d: return "protocol_switch";
	case 0x2f: return "stream_config";
	case 0x31: return "video_switch";
	case 0x32: return "capture";
	case 0x33: return "record";
	case 0x34: return "continuous_capture";
	case 0x37: return "osd";
	case 0x3a: return "point_track";
	case 0x3b: return "stop_track";
	case 0x3d: return "laser_once";
	case 0x3e: return "laser_continuous";
	case 0x3f: return "laser_stop";
	case 0x45: return "visible_zoom";
	case 0x4a: return "visible_brightness";
	case 0x4b: return "visible_contrast";
	case 0x54: return "digital_zoom";
	case 0x59: return "ir_brightness";
	case 0x5a: return "ir_contrast";
	case 0x5b: return "capture";
	case 0x5c: return "focus_mode";
	case 0x5d: return "focus_position";
	case 0x60: return "shutter_auto";
	case 0x61: return "shutter_manual";
	case 0x71: return "center";
	case 0x72: return "frame_angle";
	case 0x73: return "vertical_down";
	case 0x74: return "stow";
	case 0x76: return "gyro_calib";
	case 0x77: return "yaw_scan_param";
	case 0x78: return "pitch_scan_param";
	case 0x79: return "scan";
	case 0x7a: return "yaw_lock";
	case 0x7b: return "yaw_follow";
	case 0x80: return "attitude_guide";
	case 0x81: return "lock_frame_angle";
	case 0x90: return "json";
	case 0x93: return "green_or_id_track";
	case 0xfa: return "manual_calib";
	case 0xff: return "json_reply";
	default: return "unknown";
	}
}

const char *DytGimbal::geotrack_cmd_name(uint8_t command) const
{
	switch (command) {
	case 0x00: return "exit";
	case 0x01: return "center_point";
	case 0x02: return "target_point";
	case 0x0a: return "target_calib";
	default: return "unknown";
	}
}

const char *DytGimbal::lift_state_name(uint8_t state) const
{
	switch (state) {
	case 0x00: return "stop";
	case 0x01: return "up";
	case 0x02: return "down";
	case 0x03: return "up_limit";
	case 0x04: return "down_limit";
	case 0xff: return "error";
	default: return "unknown";
	}
}

void DytGimbal::publish_link_state(hrt_abstime now, uint8_t tracking_state)
{
	if ((now - _last_state_publish) < 100_ms) {
		return;
	}

	dyt_target_s target = _last_target;
	target.timestamp = now;
	target.tracking_state = tracking_state;
	target.target_valid = false;
	target.timestamp_sample = _last_rx_time;
	target.frame_counter = _frame_counter;
	target.parse_error_count = _parse_error_count;

	if (_last_rx_time > 0) {
		target.last_rx_age_s = (now >= _last_rx_time) ? (now - _last_rx_time) * 1e-6f : 0.f;

	} else {
		target.last_rx_age_s = NAN;
	}

	_dyt_target_pub.publish(target);
	_last_state_publish = now;
}

void DytGimbal::handle_command_updates()
{
	dyt_command_s cmd{};

	while (_dyt_command_sub.update(&cmd)) {
		_startup_home_canceled = true;
		send_protocol_command(cmd);
	}
}

void DytGimbal::send_startup_home_if_needed(hrt_abstime now)
{
	if (_startup_home_sent || _startup_home_canceled || _uart_fd < 0 || _param_dyt_home_en.get() == 0) {
		return;
	}

	if (_startup_home_time == 0 || _startup_home_next_time == 0 || now < _startup_home_next_time) {
		return;
	}

	const int16_t yaw_cmd = angle_deg_to_cdeg(_param_dyt_home_yaw_deg.get());
	const int16_t pitch_cmd = angle_deg_to_cdeg(_param_dyt_home_pitch_deg.get());
	send_command_frame(0x72, yaw_cmd, pitch_cmd, 0, 0);
	++_startup_home_count;

	PX4_INFO("startup home angle sent yaw=%.1f pitch=%.1f count=%lu",
		 static_cast<double>(yaw_cmd) * 0.01,
		 static_cast<double>(pitch_cmd) * 0.01,
		 static_cast<unsigned long>(_startup_home_count));

	const int32_t interval_ms = math::constrain(_param_dyt_home_interval_ms.get(), int32_t{100}, int32_t{5000});
	_startup_home_next_time = now + static_cast<hrt_abstime>(interval_ms) * 1000ULL;

	if (_startup_home_end_time == 0 || _startup_home_next_time > _startup_home_end_time) {
		_startup_home_sent = true;
	}
}

void DytGimbal::send_periodic_neutral_if_needed(hrt_abstime now)
{
	if (_uart_fd < 0 || (_last_control_frame_time != 0 && (now - _last_control_frame_time) < CONTROL_FRAME_INTERVAL)) {
		return;
	}

	send_command_frame(0x00, 0, 0, 0, 0, false);
}

void DytGimbal::send_protocol_command(const dyt_command_s &cmd)
{
	switch (cmd.command) {
	case dyt_command_s::CMD_AUTO_LOCK: {
			send_command_frame(0x93, 0, 0, cmd.param3, cmd.zoom_rate);
		}
		break;

	case dyt_command_s::CMD_STOP_TRACK:
		send_command_frame(0x3B, cmd.param_x, cmd.param_y, cmd.param3, cmd.zoom_rate);
		break;

	case dyt_command_s::CMD_NOFOLLOW:
		send_command_frame(0x7A, cmd.param_x, cmd.param_y, cmd.param3, cmd.zoom_rate);
		break;

	case dyt_command_s::CMD_YAW_FOLLOW:
		send_command_frame(0x7B, cmd.param_x, cmd.param_y, cmd.param3, cmd.zoom_rate);
		break;

	case dyt_command_s::CMD_ELECT_LOCK:
		send_command_frame(0x81, 1, 0, cmd.param3, cmd.zoom_rate);
		break;

	case dyt_command_s::CMD_ELECT_UNLOCK:
		send_command_frame(0x81, 0, 0, cmd.param3, cmd.zoom_rate);
		break;

	case dyt_command_s::CMD_LOCK_VIEW:
		send_command_frame(0x7A, 0, 0, 0, 0);
		usleep(CENTER_SEQUENCE_DELAY_US);
		send_command_frame(0x81, 1, 0, 0, 0);
		break;

	case dyt_command_s::CMD_CENTER:
		send_center_sequence();
		break;

	case dyt_command_s::CMD_CENTER_GIMBAL:
		send_command_frame(0x3B, 0, 0, 0, 0);
		usleep(CENTER_SEQUENCE_DELAY_US);
		send_command_frame(0x72, cmd.param_x, cmd.param_y, cmd.param3, cmd.zoom_rate);
		break;

	case dyt_command_s::CMD_SET_FRAME_ANGLE:
		send_command_frame(0x72, cmd.param_x, cmd.param_y, cmd.param3, cmd.zoom_rate);
		break;

	case dyt_command_s::CMD_SEARCH_RATE:
		send_command_frame(0x79, cmd.param_x, cmd.param_y, cmd.param3, cmd.zoom_rate);
		break;

	case dyt_command_s::CMD_RETRIGGER:
		send_command_frame(0x3B, 0, 0, 0, 0);
		usleep(CENTER_SEQUENCE_DELAY_US);
		send_command_frame(0x93, 0, 0, cmd.param3, cmd.zoom_rate);
		break;

	default:
		break;
	}
}

void DytGimbal::publish_shell_command(uint8_t command)
{
	dyt_command_s cmd{};
	cmd.timestamp = hrt_absolute_time();
	cmd.command = command;

	if (command == dyt_command_s::CMD_AUTO_LOCK || command == dyt_command_s::CMD_RETRIGGER) {
		cmd.param_x = -100;
	}

	_dyt_command_pub.publish(cmd);
}

void DytGimbal::send_center_sequence()
{
	send_command_frame(0x3B, 0, 0, 0, 0);
	usleep(CENTER_SEQUENCE_DELAY_US);
	send_command_frame(0x7A, 0, 0, 0, 0);
	usleep(CENTER_SEQUENCE_DELAY_US);
	send_command_frame(0x81, 0, 0, 0, 0);
	usleep(CENTER_SEQUENCE_DELAY_US);
	send_command_frame(0x71, 0, 0, 0, 0);
}

int16_t DytGimbal::angle_deg_to_cdeg(float angle_deg) const
{
	if (!PX4_ISFINITE(angle_deg)) {
		angle_deg = 0.f;
	}

	const float limited_deg = math::constrain(angle_deg, -180.f, 180.f);
	return static_cast<int16_t>(roundf(limited_deg * 100.f));
}

bool DytGimbal::write_command_buffer(const uint8_t *buffer, size_t buffer_len)
{
	size_t written_total = 0;
	_last_write_errno = 0;
	_last_write_result = 0;

	const hrt_abstime deadline = hrt_absolute_time() + 100_ms;

	while (written_total < buffer_len) {
		const ssize_t written = ::write(_uart_fd, buffer + written_total, buffer_len - written_total);
		_last_write_result = static_cast<int>(written);

		if (written > 0) {
			written_total += static_cast<size_t>(written);
			continue;
		}

		if (written < 0) {
			_last_write_errno = errno;

			if ((errno == EINTR) || (errno == EAGAIN)
#if defined(EWOULDBLOCK) && (EWOULDBLOCK != EAGAIN)
			    || (errno == EWOULDBLOCK)
#endif
			   ) {
				if (hrt_absolute_time() < deadline) {
					usleep(1000);
					continue;
				}
			}

			return false;
		}

		if (hrt_absolute_time() >= deadline) {
			return false;
		}

		usleep(1000);
	}

	(void)tcdrain(_uart_fd);
	_last_write_result = static_cast<int>(written_total);
	return true;
}

void DytGimbal::send_command_frame(uint8_t control, int16_t param_x, int16_t param_y, uint8_t param3, int8_t zoom_rate,
				   bool record_command)
{
	(void)param3;
	(void)zoom_rate;

	if (_uart_fd < 0) {
		return;
	}

	uint8_t buffer[COMMAND_LEN]{};
	buffer[0] = COMMAND_SYNC_1;
	buffer[1] = FRAME_SYNC_2_TELEMETRY;
	buffer[2] = control;
	buffer[3] = static_cast<uint8_t>(param_x & 0xFF);
	buffer[4] = static_cast<uint8_t>((param_x >> 8) & 0xFF);
	buffer[5] = static_cast<uint8_t>(param_y & 0xFF);
	buffer[6] = static_cast<uint8_t>((param_y >> 8) & 0xFF);

	for (size_t i = 2; i <= 41; ++i) {
		buffer[42] = static_cast<uint8_t>(buffer[42] ^ buffer[i]);
	}

	buffer[43] = FRAME_TAIL;

	if (!write_command_buffer(buffer, sizeof(buffer))) {
		++_write_error_count;
		PX4_WARN("DYT tx 0x%02x failed result=%d errno=%d",
			 static_cast<unsigned>(control), _last_write_result, _last_write_errno);

	} else {
		++_command_tx_count;
		_last_control_frame_time = hrt_absolute_time();

		if (record_command) {
			_last_command_control = control;
			_last_command_time = _last_control_frame_time;
		}
	}
}

int DytGimbal::task_spawn(int argc, char *argv[])
{
	const char *device = "/dev/ttyS3";

	int ch;
	int myoptind = 1;
	const char *myoptarg = nullptr;

	while ((ch = px4_getopt(argc, argv, "d:", &myoptind, &myoptarg)) != EOF) {
		switch (ch) {
		case 'd':
			device = myoptarg;
			break;

		default:
			return print_usage("unknown option");
		}
	}

	DytGimbal *instance = new DytGimbal(device);

	if (instance != nullptr) {
		_object.store(instance);
		_task_id = task_id_is_work_queue;

		if (instance->init()) {
			return PX4_OK;
		}
	}

	delete instance;
	_object.store(nullptr);
	_task_id = -1;
	return PX4_ERROR;
}

int DytGimbal::custom_command(int argc, char *argv[])
{
	if (!is_running()) {
		return print_usage("module not running");
	}

	if (!strcmp(argv[0], "status")) {
		get_instance()->show_status();
		return PX4_OK;
	}

	if (!strcmp(argv[0], "autolock")) {
		get_instance()->publish_shell_command(dyt_command_s::CMD_AUTO_LOCK);
		return PX4_OK;
	}

	if (!strcmp(argv[0], "stoptrk")) {
		get_instance()->publish_shell_command(dyt_command_s::CMD_STOP_TRACK);
		return PX4_OK;
	}

	if (!strcmp(argv[0], "nofollow")) {
		get_instance()->publish_shell_command(dyt_command_s::CMD_NOFOLLOW);
		return PX4_OK;
	}

	if (!strcmp(argv[0], "yawfollow")) {
		get_instance()->publish_shell_command(dyt_command_s::CMD_YAW_FOLLOW);
		return PX4_OK;
	}

	if (!strcmp(argv[0], "elock")) {
		get_instance()->publish_shell_command(dyt_command_s::CMD_ELECT_LOCK);
		return PX4_OK;
	}

	if (!strcmp(argv[0], "eunlock")) {
		get_instance()->publish_shell_command(dyt_command_s::CMD_ELECT_UNLOCK);
		return PX4_OK;
	}

	if (!strcmp(argv[0], "lockview")) {
		get_instance()->publish_shell_command(dyt_command_s::CMD_LOCK_VIEW);
		return PX4_OK;
	}

	if (!strcmp(argv[0], "center")) {
		get_instance()->publish_shell_command(dyt_command_s::CMD_CENTER);
		return PX4_OK;
	}

	if (!strcmp(argv[0], "centergimbal")) {
		get_instance()->publish_shell_command(dyt_command_s::CMD_CENTER_GIMBAL);
		return PX4_OK;
	}

	return print_usage("unknown command");
}

int DytGimbal::print_usage(const char *reason)
{
	if (reason != nullptr) {
		PX4_WARN("%s", reason);
	}

	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
DYT UART driver for periodic telemetry parsing and one-shot tracking commands.

)DESCR_STR");

	PRINT_MODULE_USAGE_NAME("dyt_gimbal", "driver");
	PRINT_MODULE_USAGE_COMMAND("start");
	PRINT_MODULE_USAGE_ARG("-d <device>", "UART device path, default /dev/ttyS3", true);
	PRINT_MODULE_USAGE_COMMAND("status");
	PRINT_MODULE_USAGE_COMMAND("autolock");
	PRINT_MODULE_USAGE_COMMAND("stoptrk");
	PRINT_MODULE_USAGE_COMMAND("nofollow");
	PRINT_MODULE_USAGE_COMMAND("yawfollow");
	PRINT_MODULE_USAGE_COMMAND("elock");
	PRINT_MODULE_USAGE_COMMAND("eunlock");
	PRINT_MODULE_USAGE_COMMAND("lockview");
	PRINT_MODULE_USAGE_COMMAND("center");
	PRINT_MODULE_USAGE_COMMAND("centergimbal");
	PRINT_MODULE_USAGE_DEFAULT_COMMANDS();
	return 0;
}

extern "C" __EXPORT int dyt_gimbal_main(int argc, char *argv[])
{
	return DytGimbal::main(argc, argv);
}
