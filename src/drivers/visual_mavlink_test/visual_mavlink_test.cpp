/****************************************************************************
 *
 *   Copyright (c) 2024 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file visual_mavlink_test.cpp
 *
 * Minimal MAVLink round-trip test module. It is used to verify that the ground
 * station can talk to both the simulation vehicle and the real flight
 * controller over MAVLink.
 *
 * The module receives a number from the ground station (through an nsh shell
 * command delivered over MAVLink SERIAL_CONTROL), increments or decrements it
 * by one and reports the result back to the ground station as a STATUSTEXT
 * (mavlink_log) message of the form:
 *
 *   VMT:inc:<value>     (forward / increment direction)
 *   VMT:dec:<value>     (reverse / decrement direction)
 *
 * The ground station (QGroundControl) is responsible for relaying the value
 * between the two vehicles. See VisualMavlinkTestController on the QGC side.
 *
 * @author PX4 Development Team
 */

#include <px4_platform_common/log.h>
#include <px4_platform_common/module.h>
#include <drivers/drv_hrt.h>
#include <uORB/uORB.h>
#include <uORB/topics/mavlink_log.h>

#include <cstdlib>
#include <cstring>
#include <cstdio>

extern "C" __EXPORT int visual_mavlink_test_main(int argc, char *argv[]);

// Kept alive across shell invocations so the mavlink_log topic stays advertised.
static orb_advert_t g_mavlink_log_pub = nullptr;

static void publish_result(const char *direction, long value)
{
	mavlink_log_s mavlink_log{};
	mavlink_log.timestamp = hrt_absolute_time();
	mavlink_log.severity = 6; // MAV_SEVERITY_INFO

	snprintf(mavlink_log.text, sizeof(mavlink_log.text), "VMT:%s:%ld", direction, value);
	mavlink_log.text[sizeof(mavlink_log.text) - 1] = '\0';

	if (g_mavlink_log_pub == nullptr) {
		// A normal advertise seeds the first sample, which mavlink forwards as
		// a STATUSTEXT message to the ground station.
		g_mavlink_log_pub = orb_advertise(ORB_ID(mavlink_log), &mavlink_log);

		if (g_mavlink_log_pub == nullptr) {
			PX4_ERR("orb_advertise(mavlink_log) failed");
		}

		return;
	}

	if (orb_publish(ORB_ID(mavlink_log), g_mavlink_log_pub, &mavlink_log) != PX4_OK) {
		PX4_WARN("orb_publish(mavlink_log) failed");
	}
}

static void usage()
{
	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
Minimal MAVLink round-trip test module used to verify ground station <-> vehicle
communication for both the simulation vehicle and the real flight controller.

The module receives a number, applies +1 (inc) or -1 (dec) and reports the result
back to the ground station as a STATUSTEXT message "VMT:<inc|dec>:<value>".

### Examples
Increment a value (forward direction):
$ visual_mavlink_test process 5 inc

Decrement a value (reverse direction):
$ visual_mavlink_test process 5 dec

Emit a self test message:
$ visual_mavlink_test test
)DESCR_STR");

	PRINT_MODULE_USAGE_NAME("visual_mavlink_test", "driver");
	PRINT_MODULE_USAGE_COMMAND_DESCR("process", "Process a value: process <value> <inc|dec>");
	PRINT_MODULE_USAGE_COMMAND_DESCR("test", "Emit a self test STATUSTEXT message");
}

int visual_mavlink_test_main(int argc, char *argv[])
{
	if (argc < 2) {
		usage();
		return 1;
	}

	const char *verb = argv[1];

	if (strcmp(verb, "process") == 0) {
		if (argc < 4) {
			PX4_ERR("Usage: visual_mavlink_test process <value> <inc|dec>");
			return 1;
		}

		long value = strtol(argv[2], nullptr, 10);
		const char *direction = argv[3];
		long result;

		if (strcmp(direction, "inc") == 0) {
			result = value + 1;

		} else if (strcmp(direction, "dec") == 0) {
			result = value - 1;

		} else {
			PX4_ERR("direction must be 'inc' or 'dec'");
			return 1;
		}

		PX4_INFO("VMT process: %ld %s -> %ld", value, direction, result);
		publish_result(direction, result);
		return 0;

	} else if (strcmp(verb, "test") == 0) {
		publish_result("inc", 1);
		PX4_INFO("VMT self test sent: VMT:inc:1");
		return 0;
	}

	usage();
	return 1;
}
