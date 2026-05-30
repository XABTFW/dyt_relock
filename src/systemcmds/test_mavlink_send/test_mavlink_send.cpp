/****************************************************************************
 *
 *   Test MAVLink Send Command
 *   Send test_mavlink_tx messages from PX4 terminal
 *
 ****************************************************************************/

#include <px4_platform_common/cli.h>
#include <px4_platform_common/getopt.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/module.h>
#include <uORB/Publication.hpp>
#include <uORB/topics/test_mavlink_tx.h>
#include <string.h>
#include <stdlib.h>

static void print_usage()
{
	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
Send test_mavlink_tx messages for testing communication between simulation and real hardware.

### Examples
Send test values:
$ test_mavlink_send 10 20 30.5
)DESCR_STR");

	PRINT_MODULE_USAGE_NAME_SIMPLE("test_mavlink_send", "command");
	PRINT_MODULE_USAGE_ARG("<test1> <test2> <test3>", "Three test values (uint8, int16, float)", false);
}

extern "C" __EXPORT int test_mavlink_send_main(int argc, char *argv[])
{
	if (argc == 2 && strcmp(argv[1], "help") == 0) {
		print_usage();
		return 0;
	}

	if (argc < 4) {
		PX4_ERR("Not enough arguments. Usage: test_mavlink_send <test1> <test2> <test3>");
		print_usage();
		return 1;
	}

	// Parse arguments
	uint8_t test1 = (uint8_t)atoi(argv[1]);
	int16_t test2 = (int16_t)atoi(argv[2]);
	float test3 = atof(argv[3]);

	// Create and publish message
	uORB::Publication<test_mavlink_tx_s> test_mavlink_tx_pub{ORB_ID(test_mavlink_tx)};
	test_mavlink_tx_s msg{};
	
	msg.timestamp = hrt_absolute_time();
	msg.test1 = test1;
	msg.test2 = test2;
	msg.test3 = test3;

	test_mavlink_tx_pub.publish(msg);

	PX4_INFO("Sent test_mavlink_tx: test1=%u, test2=%d, test3=%.2f", 
	         (unsigned)test1, (int)test2, (double)test3);

	return 0;
}
