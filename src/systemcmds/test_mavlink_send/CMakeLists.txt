############################################################################
#
#   Copyright (c) 2024 PX4 Development Team. All rights reserved.
#
############################################################################

px4_add_module(
	MODULE systemcmds__test_mavlink_send
	MAIN test_mavlink_send
	SRCS
		test_mavlink_send.cpp
	DEPENDS
		uORB::test_mavlink_tx
)
