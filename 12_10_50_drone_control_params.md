# 12_10_50.ulg 无人机控制参数整理

来源日志: `/home/btfw/1/12_10_50.ulg`

- 控制相关参数总数: 594 个
- 本表不重复列出导引头 `DYTG_*` 和 cooperative_rendezvous `CRDZ_*`，它们已在专项表中整理。
- `system默认` 和 `setup默认` 是 ULog 内记录到的默认值；空白表示日志没有记录该项默认值。
- `不同于日志默认值` 表示当前值和日志中某个默认值不同。

## 分类统计

- 遥控/手动输入: 136
- 控制分配/机架几何: 121
- 执行器/电机输出: 85
- Commander模式/解锁/安全约束: 72
- 多旋翼位置/速度/轨迹控制: 64
- 任务/导航控制约束: 20
- 多旋翼角速度控制: 20
- 故障检测控制约束: 12
- 多旋翼慢速模式控制: 10
- 多旋翼 Acro 控制: 7
- 多旋翼姿态控制: 7
- 降落检测/着陆控制: 7
- 安全断路器: 6
- 精准降落控制: 6
- 固定翼控制参数: 5
- 地理围栏控制约束: 5
- 多旋翼自整定: 5
- 多旋翼控制其他: 4
- VTOL控制参数: 2

## 关键控制参数摘录

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MPC_XY_VEL_MAX|45.0|100.0|100.0|m/s|Multicopter Position Control|Maximum horizontal velocity|不同于日志默认值|
|MPC_XY_CRUISE|3.0|5.0|5.0|m/s|Multicopter Position Control|Default horizontal velocity in autonomous modes|不同于日志默认值|
|MPC_VEL_MANUAL|3.0|10.0|10.0|m/s|Multicopter Position Control|Maximum horizontal velocity setpoint in Position mode|不同于日志默认值|
|MPC_ACC_HOR|6.0|3.0|3.0|m/s^2|Multicopter Position Control|Acceleration for autonomous and for manual modes|不同于日志默认值|
|MPC_ACC_HOR_MAX|10.0|2.0|2.0|m/s^2|Multicopter Position Control|Maximum horizontal acceleration|不同于日志默认值|
|MPC_JERK_MAX|12.0|3.0|3.0|m/s^3|Multicopter Position Control|Maximum horizontal and vertical jerk in Position/Altitude mode|不同于日志默认值|
|MPC_JERK_AUTO|2.0|4.0|4.0|m/s^3|Multicopter Position Control|Jerk limit in autonomous modes|不同于日志默认值|
|MPC_Z_VEL_MAX_UP|30.0|50.0|50.0|m/s|Multicopter Position Control|Maximum ascent velocity|不同于日志默认值|
|MPC_Z_VEL_MAX_DN|20.0|50.0|50.0|m/s|Multicopter Position Control|Maximum descent velocity|不同于日志默认值|
|MPC_TKO_SPEED|3.0|1.5|1.5|m/s|Multicopter Position Control|Takeoff climb rate|不同于日志默认值|
|MPC_LAND_SPEED|0.699999988079071|||m/s|Multicopter Position Control|Landing descend rate||
|MPC_YAWRAUTO_MAX|60.0|||deg/s|Multicopter Attitude Control|Maximum yaw rate in autonomous modes||
|MPC_YAWRAUTO_ACC|20.0|||deg/s^2|Multicopter Attitude Control|Maximum yaw acceleration in autonomous modes||
|MC_ROLLRATE_P|0.10000000149011612|0.15000000596046448|0.15000000596046448||Multicopter Rate Control|Roll rate P gain|不同于日志默认值|
|MC_ROLLRATE_I|0.11999999731779099|0.20000000298023224|0.20000000298023224||Multicopter Rate Control|Roll rate I gain|不同于日志默认值|
|MC_ROLLRATE_D|0.0|0.003000000026077032|0.003000000026077032||Multicopter Rate Control|Roll rate D gain|不同于日志默认值|
|MC_ROLLRATE_MAX|180.0|220.0|220.0|deg/s|Multicopter Attitude Control|Max roll rate|不同于日志默认值|
|MC_PITCHRATE_P|0.10000000149011612|0.15000000596046448|0.15000000596046448||Multicopter Rate Control|Pitch rate P gain|不同于日志默认值|
|MC_PITCHRATE_I|0.11999999731779099|0.20000000298023224|0.20000000298023224||Multicopter Rate Control|Pitch rate I gain|不同于日志默认值|
|MC_PITCHRATE_D|0.0|0.003000000026077032|0.003000000026077032||Multicopter Rate Control|Pitch rate D gain|不同于日志默认值|
|MC_PITCHRATE_MAX|180.0|220.0|220.0|deg/s|Multicopter Attitude Control|Max pitch rate|不同于日志默认值|
|MC_YAWRATE_P|0.20000000298023224||||Multicopter Rate Control|Yaw rate P gain||
|MC_YAWRATE_I|0.10000000149011612||||Multicopter Rate Control|Yaw rate I gain||
|MC_YAWRATE_D|0.0||||Multicopter Rate Control|Yaw rate D gain||
|MC_YAWRATE_MAX|90.0|200.0|200.0|deg/s|Multicopter Attitude Control|Max yaw rate|不同于日志默认值|
|MC_ROLL_P|4.5|4.0|4.0||Multicopter Attitude Control|Roll P gain|不同于日志默认值|
|MC_PITCH_P|4.5|4.0|4.0||Multicopter Attitude Control|Pitch P gain|不同于日志默认值|
|MC_YAW_P|2.0|2.799999952316284|2.799999952316284||Multicopter Attitude Control|Yaw P gain|不同于日志默认值|
|MC_AIRMODE|0||||Mixer Output|Multicopter air-mode||
|CA_AIRFRAME|0||||Geometry|Airframe selection||
|CA_METHOD|2||||Geometry|Control allocation method||
|CA_ROTOR_COUNT|4|0|||Geometry|Total number of rotors|不同于日志默认值|
|PWM_MAIN_FUNC1|101|0|0||Actuator Outputs|PWM Main 1 Output Function|不同于日志默认值|
|PWM_MAIN_FUNC2|102|0|0||Actuator Outputs|PWM Main 2 Output Function|不同于日志默认值|
|PWM_MAIN_FUNC3|103|0|0||Actuator Outputs|PWM Main 3 Output Function|不同于日志默认值|
|PWM_MAIN_FUNC4|104|0|0||Actuator Outputs|PWM Main 4 Output Function|不同于日志默认值|
|PWM_MAIN_MIN1|1100|1000|1000||Actuator Outputs|PWM Main 1 Minimum Value|不同于日志默认值|
|PWM_MAIN_MAX1|1900|2000|2000||Actuator Outputs|PWM Main 1 Maximum Value|不同于日志默认值|
|PWM_MAIN_DIS1|1000||||Actuator Outputs|PWM Main 1 Disarmed Value||
|PWM_MAIN_FAIL1|-1||||Actuator Outputs|PWM Main 1 Failsafe Value||
|DSHOT_MIN|0.054999999701976776|||%|DShot|Minimum DShot Motor Output||
|DSHOT_3D_ENABLE|0||||DShot|Allows for 3d mode when using DShot and suitable mixer||
|COM_RC_IN_MODE|1|3|3||Commander|RC control input mode|不同于日志默认值|
|COM_FLTMODE1|-1||||Commander|Mode slot 1||
|COM_FLTMODE2|-1||||Commander|Mode slot 2||
|COM_FLTMODE3|-1||||Commander|Mode slot 3||
|NAV_ACC_RAD|2.0|10.0||m|Mission|Acceptance Radius|不同于日志默认值|
|LNDMC_XY_VEL_MAX|1.5|||m/s|Land Detector|Multicopter max horizontal velocity||
|LNDMC_Z_VEL_MAX|0.25|||m/s|Land Detector|Multicopter vertical velocity threshold||
|LNDMC_ROT_MAX|20.0|||deg/s|Land Detector|Multicopter max rotational speed||

## 全部控制参数

### Commander模式/解锁/安全约束

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|COM_ACT_FAIL_ACT|0||||Commander|Set the actuator failure failsafe mode||
|COM_ARMABLE|1||||Commander|Flag to allow arming||
|COM_ARM_AUTH_ID|10||||Commander|Arm authorizer system id||
|COM_ARM_AUTH_MET|0||||Commander|Arm authorization method||
|COM_ARM_AUTH_REQ|0||||Commander|Require arm authorization to arm||
|COM_ARM_AUTH_TO|1.0|||s|Commander|Arm authorization timeout||
|COM_ARM_BAT_MIN|-1.0|||norm|Commander|Minimum battery level for arming||
|COM_ARM_CHK_ESCS|0||||Commander|Enable checks on ESCs that report telemetry||
|COM_ARM_HFLT_CHK|1||||Commander|Enable FMU SD card hardfault detection check||
|COM_ARM_IMU_ACC|0.699999988079071|||m/s^2|Commander|Maximum accelerometer inconsistency between IMU units that will allow arming||
|COM_ARM_IMU_GYR|0.25|||rad/s|Commander|Maximum rate gyro inconsistency between IMU units that will allow arming||
|COM_ARM_MAG_ANG|60|||deg|Commander|Maximum magnetic field inconsistency between units that will allow arming||
|COM_ARM_MAG_STR|2||||Commander|Enable mag strength preflight check||
|COM_ARM_MIS_REQ|0||||Commander|Require valid mission to arm||
|COM_ARM_ODID|0||||Commander|Enable Drone ID system detection and health check||
|COM_ARM_SDCARD|1||||Commander|Enable FMU SD card detection check||
|COM_ARM_SWISBTN|0||||Commander|Arm switch is a momentary button||
|COM_ARM_WO_GPS|1||||Commander|GPS preflight check||
|COM_CPU_MAX|95.0|||%|Commander|Maximum allowed CPU load to still arm||
|COM_DISARM_LAND|2.0|||s|Commander|Time-out for auto disarm after landing||
|COM_DISARM_MAN|1||||Commander|Allow disarming via switch/stick/button on multicopters in manual thrust modes||
|COM_DISARM_PRFLT|10.0|||s|Commander|Time-out for auto disarm if not taking off||
|COM_DLL_EXCEPT|0||||Commander|Datalink loss exceptions||
|COM_DL_LOSS_T|10|||s|Commander|GCS connection loss time threshold||
|COM_FAIL_ACT_T|5.0|||s|Commander|Delay between failsafe condition triggered and failsafe reaction||
|COM_FLIGHT_UUID|152||||Commander|Next flight UUID||
|COM_FLTMODE1|-1||||Commander|Mode slot 1||
|COM_FLTMODE2|-1||||Commander|Mode slot 2||
|COM_FLTMODE3|-1||||Commander|Mode slot 3||
|COM_FLTMODE4|-1||||Commander|Mode slot 4||
|COM_FLTMODE5|-1||||Commander|Mode slot 5||
|COM_FLTMODE6|-1||||Commander|Mode slot 6||
|COM_FLTT_LOW_ACT|3||||Commander|Remaining flight time low failsafe||
|COM_FLT_PROFILE|0||||Commander|User Flight Profile||
|COM_FLT_TIME_MAX|-1|||s|Commander|Maximum allowed flight time||
|COM_FORCE_SAFETY|0||||Commander|Enable force safety||
|COM_HLDL_LOSS_T|120|||s|Commander|High Latency Datalink loss time threshold||
|COM_HLDL_REG_T|0|||s|Commander|High Latency Datalink regain time threshold||
|COM_HOME_EN|1||||Commander|Home position enabled||
|COM_HOME_IN_AIR|0||||Commander|Allows setting the home position after takeoff||
|COM_IMB_PROP_ACT|0||||Commander|Imbalanced propeller failsafe mode||
|COM_KILL_DISARM|5.0|||s|Commander|Timeout value for disarming when kill switch is engaged||
|COM_LKDOWN_TKO|3.0|||s|Commander|Timeout for detecting a failure after takeoff||
|COM_LOW_BAT_ACT|0||||Commander|Battery failsafe mode||
|COM_MODE_ARM_CHK|0||||Commander|Allow external mode registration while armed||
|COM_MOT_TEST_EN|1||||Commander|Enable Actuator Testing||
|COM_OBC_LOSS_T|5.0|||s|Commander|Time-out to wait when onboard computer connection is lost before warning about loss connection||
|COM_OBL_RC_ACT|0||||Commander|Set offboard loss failsafe mode||
|COM_OF_LOSS_T|1.0|||s|Commander|Time-out to wait when offboard connection is lost before triggering offboard lost action||
|COM_PARACHUTE|0||||Commander|Expect and require a healthy MAVLink parachute system||
|COM_POSCTL_NAVL|0||||Commander|Position mode navigation loss response||
|COM_POS_FS_EPH|5.0|||m|Commander|Horizontal position error threshold for hovering systems||
|COM_POS_LOW_ACT|3||||Commander|Low position accuracy action||
|COM_POS_LOW_EPH|-1.0|||m|Commander|Low position accuracy failsafe threshold||
|COM_POWER_COUNT|1||||Commander|Required number of redundant power modules||
|COM_PREARM_MODE|0||||Commander|Condition to enter prearmed mode||
|COM_QC_ACT|0||||Commander|Set action after a quadchute||
|COM_RAM_MAX|95.0|||%|Commander|Maximum allowed RAM usage to pass checks||
|COM_RCL_EXCEPT|7||||Commander|RC loss exceptions||
|COM_RC_ARM_HYST|1000|||ms|Commander|RC input arm/disarm command duration||
|COM_RC_IN_MODE|1|3|3||Commander|RC control input mode|不同于日志默认值|
|COM_RC_LOSS_T|3.0|0.5|0.5|s|Commander|Manual control loss timeout|不同于日志默认值|
|COM_RC_OVERRIDE|1||||Commander|Enable RC stick override of auto and/or offboard modes||
|COM_RC_STICK_OV|30.0|||%|Commander|RC stick override threshold||
|COM_SPOOLUP_TIME|1.0|||s|Commander|Enforced delay between arming and further navigation||
|COM_TAKEOFF_ACT|0||||Commander|Action after TAKEOFF has been accepted||
|COM_THROW_EN|0||||Commander|Enable throw-start||
|COM_THROW_SPEED|5.0|||m/s|Commander|Minimum speed for the throw start||
|COM_VEL_FS_EVH|1.0|||m/s|Commander|Horizontal velocity error threshold||
|COM_WIND_MAX|-1.0|||m/s|Commander|High wind speed failsafe threshold||
|COM_WIND_MAX_ACT|0||||Commander|High wind failsafe mode||
|COM_WIND_WARN|-1.0|||m/s|Commander|Wind speed warning threshold||

### VTOL控制参数

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|VT_B_DEC_I|0.10000000149011612|||rad s/m|VTOL Attitude Control|Backtransition deceleration setpoint to pitch I gain||
|VT_B_DEC_MSS|2.0|||m/s^2|VTOL Attitude Control|Approximate deceleration during back transition||

### 任务/导航控制约束

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MIS_COMMAND_TOUT|0.0|||s|Mission|Timeout to allow the payload to execute the mission command||
|MIS_LND_ABRT_ALT|30|||m|Mission|Landing abort min altitude||
|MIS_MNT_YAW_CTL|0||||Mission|Enable yaw control of the mount. (Only affects multicopters and ROI mission items)||
|MIS_TAKEOFF_ALT|2.5|||m|Mission|Default take-off altitude||
|MIS_YAW_ERR|12.0|||deg|Mission|Max yaw error in degrees needed for waypoint heading acceptance||
|MIS_YAW_TMT|-1.0|||s|Mission|Time in seconds we wait on reaching target heading at a waypoint if it is forced||
|NAV_ACC_RAD|2.0|10.0||m|Mission|Acceptance Radius|不同于日志默认值|
|NAV_DLL_ACT|0||||Commander|Set GCS connection loss failsafe mode||
|NAV_FORCE_VT|1||||Mission|Force VTOL mode takeoff and land||
|NAV_FW_ALTL_RAD|5.0|||m|Mission|FW Altitude Acceptance Radius before a landing||
|NAV_FW_ALT_RAD|10.0|||m|Mission|FW Altitude Acceptance Radius||
|NAV_LOITER_RAD|80.0|||m|Mission|Loiter radius (FW only)||
|NAV_MC_ALT_RAD|0.800000011920929|||m|Mission|MC Altitude Acceptance Radius||
|NAV_MIN_GND_DIST|-1.0|||m|Mission|Minimum height above ground during Mission and RTL||
|NAV_MIN_LTR_ALT|-1.0|||m|Mission|Minimum Loiter altitude||
|NAV_RCL_ACT|2||||Commander|Set RC loss failsafe mode||
|NAV_TRAFF_AVOID|1||||Mission|Set traffic avoidance mode||
|NAV_TRAFF_A_HOR|500.0|||m|Mission|Set NAV TRAFFIC AVOID horizontal distance||
|NAV_TRAFF_A_VER|500.0|||m|Mission|Set NAV TRAFFIC AVOID vertical distance||
|NAV_TRAFF_COLL_T|60|||s|Mission|Estimated time until collision||

### 固定翼控制参数

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|FW_AIRSPD_MAX|20.0|||m/s|FW Performance|Maximum Airspeed (CAS)||
|FW_AIRSPD_TRIM|15.0|||m/s|FW Performance|Trim (Cruise) Airspeed||
|FW_PSP_OFF|0.0|||deg|FW Attitude Control|Pitch setpoint offset (pitch at level flight)||
|FW_T_CLMB_R_SP|3.0|||m/s|FW General|Default target climbrate||
|FW_T_SINK_R_SP|2.0|||m/s|FW General|Default target sinkrate||

### 地理围栏控制约束

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|GF_ACTION|2||||Geofence|Geofence violation action||
|GF_MAX_HOR_DIST|0.0|||m|Geofence|Max horizontal distance from Home||
|GF_MAX_VER_DIST|0.0|||m|Geofence|Max vertical distance from Home||
|GF_PREDICT|0||||Geofence|[EXPERIMENTAL] Use Pre-emptive geofence triggering||
|GF_SOURCE|0||||Geofence|Geofence source||

### 多旋翼 Acro 控制

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MC_ACRO_EXPO|0.0||||Multicopter Acro Mode|Acro mode roll, pitch expo factor||
|MC_ACRO_EXPO_Y|0.0||||Multicopter Acro Mode|Acro mode yaw expo factor||
|MC_ACRO_P_MAX|100.0|||deg/s|Multicopter Acro Mode|Acro mode maximum pitch rate||
|MC_ACRO_R_MAX|100.0|||deg/s|Multicopter Acro Mode|Acro mode maximum roll rate||
|MC_ACRO_SUPEXPO|0.0||||Multicopter Acro Mode|Acro mode roll, pitch super expo factor||
|MC_ACRO_SUPEXPOY|0.0||||Multicopter Acro Mode|Acro mode yaw super expo factor||
|MC_ACRO_Y_MAX|100.0|||deg/s|Multicopter Acro Mode|Acro mode maximum yaw rate||

### 多旋翼位置/速度/轨迹控制

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MPC_ACC_DECOUPLE|1||||Multicopter Position Control|Acceleration to tilt coupling||
|MPC_ACC_DOWN_MAX|3.0|||m/s^2|Multicopter Position Control|Maximum downwards acceleration in climb rate controlled modes||
|MPC_ACC_HOR|6.0|3.0|3.0|m/s^2|Multicopter Position Control|Acceleration for autonomous and for manual modes|不同于日志默认值|
|MPC_ACC_HOR_MAX|10.0|2.0|2.0|m/s^2|Multicopter Position Control|Maximum horizontal acceleration|不同于日志默认值|
|MPC_ACC_UP_MAX|7.0|4.0|4.0|m/s^2|Multicopter Position Control|Maximum upwards acceleration in climb rate controlled modes|不同于日志默认值|
|MPC_ALT_MODE|2||||Multicopter Position Control|Altitude reference mode||
|MPC_HOLD_DZ|0.10000000149011612||||Multicopter Position Control|Deadzone for sticks in manual piloted modes||
|MPC_HOLD_MAX_XY|0.800000011920929|||m/s|Multicopter Position Control|Maximum horizontal velocity for which position hold is enabled (use 0 to disable check)||
|MPC_HOLD_MAX_Z|0.6000000238418579|||m/s|Multicopter Position Control|Maximum vertical velocity for which position hold is enabled (use 0 to disable check)||
|MPC_JERK_AUTO|2.0|4.0|4.0|m/s^3|Multicopter Position Control|Jerk limit in autonomous modes|不同于日志默认值|
|MPC_JERK_MAX|12.0|3.0|3.0|m/s^3|Multicopter Position Control|Maximum horizontal and vertical jerk in Position/Altitude mode|不同于日志默认值|
|MPC_LAND_ALT1|10.0|||m|Multicopter Position Control|Altitude for 1. step of slow landing (descend)||
|MPC_LAND_ALT2|5.0|||m|Multicopter Position Control|Altitude for 2. step of slow landing (landing)||
|MPC_LAND_ALT3|1.0|||m|Multicopter Position Control|Altitude for 3. step of slow landing||
|MPC_LAND_CRWL|0.30000001192092896|||m/s|Multicopter Position Control|Land crawl descend rate||
|MPC_LAND_RADIUS|1000.0|||m|Multicopter Position Control|User assisted landing radius||
|MPC_LAND_RC_HELP|0||||Multicopter Position Control|Enable nudging based on user input during autonomous land routine||
|MPC_LAND_SPEED|0.699999988079071|||m/s|Multicopter Position Control|Landing descend rate||
|MPC_MANTHR_MIN|0.07999999821186066|||norm|Multicopter Position Control|Minimum collective thrust in Stabilized mode||
|MPC_MAN_TILT_MAX|60.0|35.0|35.0|deg|Multicopter Position Control|Maximal tilt angle in Stabilized or Altitude mode|不同于日志默认值|
|MPC_MAN_Y_MAX|150.0|||deg/s|Multicopter Position Control|Max manual yaw rate for Stabilized, Altitude, Position mode||
|MPC_MAN_Y_TAU|0.07999999821186066|||s|Multicopter Position Control|Manual yaw rate input filter time constant||
|MPC_POS_MODE|4||||Multicopter Position Control|Position/Altitude mode variant||
|MPC_THR_CURVE|0||||Multicopter Position Control|Thrust curve mapping in Stabilized Mode||
|MPC_THR_HOVER|0.36000001430511475|0.5|0.5|norm|Multicopter Position Control|Vertical thrust required to hover|不同于日志默认值|
|MPC_THR_MAX|1.0|||norm|Multicopter Position Control|Maximum collective thrust in climb rate controlled modes||
|MPC_THR_MIN|0.11999999731779099|||norm|Multicopter Position Control|Minimum collective thrust in climb rate controlled modes||
|MPC_THR_XY_MARG|0.30000001192092896|||norm|Multicopter Position Control|Horizontal thrust margin||
|MPC_TILTMAX_AIR|60.0|45.0|45.0|deg|Multicopter Position Control|Maximum tilt angle in air|不同于日志默认值|
|MPC_TILTMAX_LND|8.0|12.0|12.0|deg|Multicopter Position Control|Maximum tilt during inital takeoff ramp|不同于日志默认值|
|MPC_TKO_RAMP_T|3.0|||s|Multicopter Position Control|Smooth takeoff ramp time constant||
|MPC_TKO_SPEED|3.0|1.5|1.5|m/s|Multicopter Position Control|Takeoff climb rate|不同于日志默认值|
|MPC_USE_HTE|1||||Multicopter Position Control|Use hover thrust estimate for altitude control||
|MPC_VELD_LP|5.0|||Hz|Multicopter Position Control|Velocity derivative low pass cutoff frequency||
|MPC_VEL_LP|0.0|||Hz|Multicopter Position Control|Velocity low pass cutoff frequency||
|MPC_VEL_MANUAL|3.0|10.0|10.0|m/s|Multicopter Position Control|Maximum horizontal velocity setpoint in Position mode|不同于日志默认值|
|MPC_VEL_MAN_BACK|-1.0|||m/s|Multicopter Position Control|Maximum backward velocity in Position mode||
|MPC_VEL_MAN_SIDE|-1.0|||m/s|Multicopter Position Control|Maximum sideways velocity in Position mode||
|MPC_VEL_NF_BW|5.0|||Hz|Multicopter Position Control|Velocity notch filter bandwidth||
|MPC_VEL_NF_FRQ|0.0|||Hz|Multicopter Position Control|Velocity notch filter frequency||
|MPC_XY_CRUISE|3.0|5.0|5.0|m/s|Multicopter Position Control|Default horizontal velocity in autonomous modes|不同于日志默认值|
|MPC_XY_ERR_MAX|2.0||||Multicopter Position Control|Maximum horizontal error allowed by the trajectory generator||
|MPC_XY_MAN_EXPO|0.6000000238418579||||Multicopter Position Control|Manual position control stick exponential curve sensitivity||
|MPC_XY_P|1.100000023841858|0.949999988079071|0.949999988079071||Multicopter Position Control|Proportional gain for horizontal position error|不同于日志默认值|
|MPC_XY_TRAJ_P|0.5||||Multicopter Position Control|Proportional gain for horizontal trajectory position error||
|MPC_XY_VEL_ALL|-10.0||||Multicopter Position Control|Overall Horizontal Velocity Limit||
|MPC_XY_VEL_D_ACC|0.20000000298023224||||Multicopter Position Control|Differential gain for horizontal velocity error||
|MPC_XY_VEL_I_ACC|0.5|0.4000000059604645|0.4000000059604645||Multicopter Position Control|Integral gain for horizontal velocity error|不同于日志默认值|
|MPC_XY_VEL_MAX|45.0|100.0|100.0|m/s|Multicopter Position Control|Maximum horizontal velocity|不同于日志默认值|
|MPC_XY_VEL_P_ACC|2.0|1.7999999523162842|1.7999999523162842||Multicopter Position Control|Proportional gain for horizontal velocity error|不同于日志默认值|
|MPC_YAWRAUTO_ACC|20.0|||deg/s^2|Multicopter Attitude Control|Maximum yaw acceleration in autonomous modes||
|MPC_YAWRAUTO_MAX|60.0|||deg/s|Multicopter Attitude Control|Maximum yaw rate in autonomous modes||
|MPC_YAW_EXPO|0.6000000238418579||||Multicopter Position Control|Manual control stick yaw rotation exponential curve||
|MPC_YAW_MODE|0||||Mission|Heading behavior in autonomous modes||
|MPC_Z_MAN_EXPO|0.6000000238418579||||Multicopter Position Control|Manual control stick vertical exponential curve||
|MPC_Z_P|1.0||||Multicopter Position Control|Proportional gain for vertical position error||
|MPC_Z_VEL_ALL|-3.0||||Multicopter Position Control|Overall Vertical Velocity Limit||
|MPC_Z_VEL_D_ACC|0.0||||Multicopter Position Control|Differential gain for vertical velocity error||
|MPC_Z_VEL_I_ACC|2.0||||Multicopter Position Control|Integral gain for vertical velocity error||
|MPC_Z_VEL_MAX_DN|20.0|50.0|50.0|m/s|Multicopter Position Control|Maximum descent velocity|不同于日志默认值|
|MPC_Z_VEL_MAX_UP|30.0|50.0|50.0|m/s|Multicopter Position Control|Maximum ascent velocity|不同于日志默认值|
|MPC_Z_VEL_P_ACC|4.0||||Multicopter Position Control|Proportional gain for vertical velocity error||
|MPC_Z_V_AUTO_DN|3.0|1.5|1.5|m/s|Multicopter Position Control|Descent velocity in autonomous modes|不同于日志默认值|
|MPC_Z_V_AUTO_UP|5.0|3.0|3.0|m/s|Multicopter Position Control|Ascent velocity in autonomous modes|不同于日志默认值|

### 多旋翼姿态控制

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MC_PITCHRATE_MAX|180.0|220.0|220.0|deg/s|Multicopter Attitude Control|Max pitch rate|不同于日志默认值|
|MC_PITCH_P|4.5|4.0|4.0||Multicopter Attitude Control|Pitch P gain|不同于日志默认值|
|MC_ROLLRATE_MAX|180.0|220.0|220.0|deg/s|Multicopter Attitude Control|Max roll rate|不同于日志默认值|
|MC_ROLL_P|4.5|4.0|4.0||Multicopter Attitude Control|Roll P gain|不同于日志默认值|
|MC_YAWRATE_MAX|90.0|200.0|200.0|deg/s|Multicopter Attitude Control|Max yaw rate|不同于日志默认值|
|MC_YAW_P|2.0|2.799999952316284|2.799999952316284||Multicopter Attitude Control|Yaw P gain|不同于日志默认值|
|MC_YAW_WEIGHT|0.4000000059604645||||Multicopter Attitude Control|Yaw weight||

### 多旋翼慢速模式控制

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MC_SLOW_DEF_HVEL|3.0|||m/s|Multicopter Position Slow Mode|Default horizontal velocity limit||
|MC_SLOW_DEF_VVEL|1.0|||m/s|Multicopter Position Slow Mode|Default vertical velocity limit||
|MC_SLOW_DEF_YAWR|45.0|||deg/s|Multicopter Position Slow Mode|Default yaw rate limit||
|MC_SLOW_MAP_HVEL|0||||Multicopter Position Slow Mode|Manual input mapped to scale horizontal velocity in position slow mode||
|MC_SLOW_MAP_PTCH|0||||Multicopter Position Slow Mode|RC_MAP_AUX{N} to allow for gimbal pitch rate control in position slow mode||
|MC_SLOW_MAP_VVEL|0||||Multicopter Position Slow Mode|Manual input mapped to scale vertical velocity in position slow mode||
|MC_SLOW_MAP_YAWR|0||||Multicopter Position Slow Mode|Manual input mapped to scale yaw rate in position slow mode||
|MC_SLOW_MIN_HVEL|0.30000001192092896|||m/s|Multicopter Position Slow Mode|Horizontal velocity lower limit||
|MC_SLOW_MIN_VVEL|0.30000001192092896|||m/s|Multicopter Position Slow Mode|Vertical velocity lower limit||
|MC_SLOW_MIN_YAWR|3.0|||deg/s|Multicopter Position Slow Mode|Yaw rate lower limit||

### 多旋翼控制其他

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MC_AIRMODE|0||||Mixer Output|Multicopter air-mode||
|MC_MAN_TILT_TAU|0.0|||s|Multicopter Position Control|Manual tilt input filter time constant||
|MC_ORBIT_RAD_MAX|1000.0|||m|Flight Task Orbit|Maximum radius of orbit||
|MC_ORBIT_YAW_MOD|0||||Flight Task Orbit|Yaw behaviour during orbit flight||

### 多旋翼自整定

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MC_AT_APPLY|1||||Autotune|Controls when to apply the new gains||
|MC_AT_EN|1|0|||Autotune|Multicopter autotune module enable|不同于日志默认值|
|MC_AT_RISE_TIME|0.14000000059604645|||s|Autotune|Desired angular rate closed-loop rise time||
|MC_AT_START|0||||Autotune|Start the autotuning sequence||
|MC_AT_SYSID_AMP|0.699999988079071||||Autotune|Amplitude of the injected signal||

### 多旋翼角速度控制

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MC_BAT_SCALE_EN|0||||Multicopter Rate Control|Battery power level scaler||
|MC_PITCHRATE_D|0.0|0.003000000026077032|0.003000000026077032||Multicopter Rate Control|Pitch rate D gain|不同于日志默认值|
|MC_PITCHRATE_FF|0.0||||Multicopter Rate Control|Pitch rate feedforward||
|MC_PITCHRATE_I|0.11999999731779099|0.20000000298023224|0.20000000298023224||Multicopter Rate Control|Pitch rate I gain|不同于日志默认值|
|MC_PITCHRATE_K|1.0||||Multicopter Rate Control|Pitch rate controller gain||
|MC_PITCHRATE_P|0.10000000149011612|0.15000000596046448|0.15000000596046448||Multicopter Rate Control|Pitch rate P gain|不同于日志默认值|
|MC_PR_INT_LIM|0.30000001192092896||||Multicopter Rate Control|Pitch rate integrator limit||
|MC_ROLLRATE_D|0.0|0.003000000026077032|0.003000000026077032||Multicopter Rate Control|Roll rate D gain|不同于日志默认值|
|MC_ROLLRATE_FF|0.0||||Multicopter Rate Control|Roll rate feedforward||
|MC_ROLLRATE_I|0.11999999731779099|0.20000000298023224|0.20000000298023224||Multicopter Rate Control|Roll rate I gain|不同于日志默认值|
|MC_ROLLRATE_K|1.0||||Multicopter Rate Control|Roll rate controller gain||
|MC_ROLLRATE_P|0.10000000149011612|0.15000000596046448|0.15000000596046448||Multicopter Rate Control|Roll rate P gain|不同于日志默认值|
|MC_RR_INT_LIM|0.30000001192092896||||Multicopter Rate Control|Roll rate integrator limit||
|MC_YAWRATE_D|0.0||||Multicopter Rate Control|Yaw rate D gain||
|MC_YAWRATE_FF|0.0||||Multicopter Rate Control|Yaw rate feedforward||
|MC_YAWRATE_I|0.10000000149011612||||Multicopter Rate Control|Yaw rate I gain||
|MC_YAWRATE_K|1.0||||Multicopter Rate Control|Yaw rate controller gain||
|MC_YAWRATE_P|0.20000000298023224||||Multicopter Rate Control|Yaw rate P gain||
|MC_YAW_TQ_CUTOFF|2.0|||Hz|Multicopter Rate Control|Low pass filter cutoff frequency for yaw torque setpoint||
|MC_YR_INT_LIM|0.30000001192092896||||Multicopter Rate Control|Yaw rate integrator limit||

### 安全断路器

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|CBRK_BUZZER|0||||Circuit Breaker|Circuit breaker for disabling buzzer||
|CBRK_FLIGHTTERM|121212||||Circuit Breaker|Circuit breaker for flight termination||
|CBRK_IO_SAFETY|22027||||Circuit Breaker|Circuit breaker for IO safety||
|CBRK_SUPPLY_CHK|0||||Circuit Breaker|Circuit breaker for power supply check||
|CBRK_USB_CHK|197848||||Circuit Breaker|Circuit breaker for USB link check||
|CBRK_VTOLARMING|0||||Circuit Breaker|Circuit breaker for arming in fixed-wing mode check||

### 执行器/电机输出

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|DSHOT_3D_DEAD_H|1000||||DShot|DSHOT 3D deadband high||
|DSHOT_3D_DEAD_L|1000||||DShot|DSHOT 3D deadband low||
|DSHOT_3D_ENABLE|0||||DShot|Allows for 3d mode when using DShot and suitable mixer||
|DSHOT_BIDIR_EN|0||||DShot|Enable bidirectional DShot||
|DSHOT_MIN|0.054999999701976776|||%|DShot|Minimum DShot Motor Output||
|DSHOT_TEL_CFG|0||||DShot|Serial Configuration for DShot Driver||
|MOT_POLE_COUNT|14||||DShot|Number of magnetic poles of the motors||
|PWM_LEVEL_CONT|0||||PWM Outputs|Control PWM output voltage||
|PWM_MAIN_DIS1|1000||||Actuator Outputs|PWM Main 1 Disarmed Value||
|PWM_MAIN_DIS10|1000||||Actuator Outputs|PWM Capture 2 Disarmed Value||
|PWM_MAIN_DIS11|1000||||Actuator Outputs|PWM Capture 3 Disarmed Value||
|PWM_MAIN_DIS12|1000||||Actuator Outputs|PWM Capture 4 Disarmed Value||
|PWM_MAIN_DIS13|1000||||Actuator Outputs|PWM Capture 5 Disarmed Value||
|PWM_MAIN_DIS14|1000||||Actuator Outputs|PWM Capture 6 Disarmed Value||
|PWM_MAIN_DIS2|1000||||Actuator Outputs|PWM Main 2 Disarmed Value||
|PWM_MAIN_DIS3|1000||||Actuator Outputs|PWM Main 3 Disarmed Value||
|PWM_MAIN_DIS4|1000||||Actuator Outputs|PWM Main 4 Disarmed Value||
|PWM_MAIN_DIS5|1000||||Actuator Outputs|PWM Main 5 Disarmed Value||
|PWM_MAIN_DIS6|1000||||Actuator Outputs|PWM Main 6 Disarmed Value||
|PWM_MAIN_DIS7|1000||||Actuator Outputs|PWM Main 7 Disarmed Value||
|PWM_MAIN_DIS8|1000||||Actuator Outputs|PWM Main 8 Disarmed Value||
|PWM_MAIN_DIS9|1000||||Actuator Outputs|PWM Capture 1 Disarmed Value||
|PWM_MAIN_FAIL1|-1||||Actuator Outputs|PWM Main 1 Failsafe Value||
|PWM_MAIN_FAIL10|-1||||Actuator Outputs|PWM Capture 2 Failsafe Value||
|PWM_MAIN_FAIL11|-1||||Actuator Outputs|PWM Capture 3 Failsafe Value||
|PWM_MAIN_FAIL12|-1||||Actuator Outputs|PWM Capture 4 Failsafe Value||
|PWM_MAIN_FAIL13|-1||||Actuator Outputs|PWM Capture 5 Failsafe Value||
|PWM_MAIN_FAIL14|-1||||Actuator Outputs|PWM Capture 6 Failsafe Value||
|PWM_MAIN_FAIL2|-1||||Actuator Outputs|PWM Main 2 Failsafe Value||
|PWM_MAIN_FAIL3|-1||||Actuator Outputs|PWM Main 3 Failsafe Value||
|PWM_MAIN_FAIL4|-1||||Actuator Outputs|PWM Main 4 Failsafe Value||
|PWM_MAIN_FAIL5|-1||||Actuator Outputs|PWM Main 5 Failsafe Value||
|PWM_MAIN_FAIL6|-1||||Actuator Outputs|PWM Main 6 Failsafe Value||
|PWM_MAIN_FAIL7|-1||||Actuator Outputs|PWM Main 7 Failsafe Value||
|PWM_MAIN_FAIL8|-1||||Actuator Outputs|PWM Main 8 Failsafe Value||
|PWM_MAIN_FAIL9|-1||||Actuator Outputs|PWM Capture 1 Failsafe Value||
|PWM_MAIN_FUNC1|101|0|0||Actuator Outputs|PWM Main 1 Output Function|不同于日志默认值|
|PWM_MAIN_FUNC10|0||||Actuator Outputs|PWM Capture 2 Output Function||
|PWM_MAIN_FUNC11|0||||Actuator Outputs|PWM Capture 3 Output Function||
|PWM_MAIN_FUNC12|0||||Actuator Outputs|PWM Capture 4 Output Function||
|PWM_MAIN_FUNC13|0||||Actuator Outputs|PWM Capture 5 Output Function||
|PWM_MAIN_FUNC14|0||||Actuator Outputs|PWM Capture 6 Output Function||
|PWM_MAIN_FUNC2|102|0|0||Actuator Outputs|PWM Main 2 Output Function|不同于日志默认值|
|PWM_MAIN_FUNC3|103|0|0||Actuator Outputs|PWM Main 3 Output Function|不同于日志默认值|
|PWM_MAIN_FUNC4|104|0|0||Actuator Outputs|PWM Main 4 Output Function|不同于日志默认值|
|PWM_MAIN_FUNC5|0||||Actuator Outputs|PWM Main 5 Output Function||
|PWM_MAIN_FUNC6|0||||Actuator Outputs|PWM Main 6 Output Function||
|PWM_MAIN_FUNC7|0||||Actuator Outputs|PWM Main 7 Output Function||
|PWM_MAIN_FUNC8|0||||Actuator Outputs|PWM Main 8 Output Function||
|PWM_MAIN_FUNC9|0||||Actuator Outputs|PWM Capture 1 Output Function||
|PWM_MAIN_MAX1|1900|2000|2000||Actuator Outputs|PWM Main 1 Maximum Value|不同于日志默认值|
|PWM_MAIN_MAX10|2000||||Actuator Outputs|PWM Capture 2 Maximum Value||
|PWM_MAIN_MAX11|2000||||Actuator Outputs|PWM Capture 3 Maximum Value||
|PWM_MAIN_MAX12|2000||||Actuator Outputs|PWM Capture 4 Maximum Value||
|PWM_MAIN_MAX13|2000||||Actuator Outputs|PWM Capture 5 Maximum Value||
|PWM_MAIN_MAX14|2000||||Actuator Outputs|PWM Capture 6 Maximum Value||
|PWM_MAIN_MAX2|1900|2000|2000||Actuator Outputs|PWM Main 2 Maximum Value|不同于日志默认值|
|PWM_MAIN_MAX3|1900|2000|2000||Actuator Outputs|PWM Main 3 Maximum Value|不同于日志默认值|
|PWM_MAIN_MAX4|1900|2000|2000||Actuator Outputs|PWM Main 4 Maximum Value|不同于日志默认值|
|PWM_MAIN_MAX5|2000||||Actuator Outputs|PWM Main 5 Maximum Value||
|PWM_MAIN_MAX6|2000||||Actuator Outputs|PWM Main 6 Maximum Value||
|PWM_MAIN_MAX7|2000||||Actuator Outputs|PWM Main 7 Maximum Value||
|PWM_MAIN_MAX8|2000||||Actuator Outputs|PWM Main 8 Maximum Value||
|PWM_MAIN_MAX9|2000||||Actuator Outputs|PWM Capture 1 Maximum Value||
|PWM_MAIN_MIN1|1100|1000|1000||Actuator Outputs|PWM Main 1 Minimum Value|不同于日志默认值|
|PWM_MAIN_MIN10|1000||||Actuator Outputs|PWM Capture 2 Minimum Value||
|PWM_MAIN_MIN11|1000||||Actuator Outputs|PWM Capture 3 Minimum Value||
|PWM_MAIN_MIN12|1000||||Actuator Outputs|PWM Capture 4 Minimum Value||
|PWM_MAIN_MIN13|1000||||Actuator Outputs|PWM Capture 5 Minimum Value||
|PWM_MAIN_MIN14|1000||||Actuator Outputs|PWM Capture 6 Minimum Value||
|PWM_MAIN_MIN2|1100|1000|1000||Actuator Outputs|PWM Main 2 Minimum Value|不同于日志默认值|
|PWM_MAIN_MIN3|1100|1000|1000||Actuator Outputs|PWM Main 3 Minimum Value|不同于日志默认值|
|PWM_MAIN_MIN4|1100|1000|1000||Actuator Outputs|PWM Main 4 Minimum Value|不同于日志默认值|
|PWM_MAIN_MIN5|1000||||Actuator Outputs|PWM Main 5 Minimum Value||
|PWM_MAIN_MIN6|1000||||Actuator Outputs|PWM Main 6 Minimum Value||
|PWM_MAIN_MIN7|1000||||Actuator Outputs|PWM Main 7 Minimum Value||
|PWM_MAIN_MIN8|1000||||Actuator Outputs|PWM Main 8 Minimum Value||
|PWM_MAIN_MIN9|1000||||Actuator Outputs|PWM Capture 1 Minimum Value||
|PWM_MAIN_REV|0||||Actuator Outputs|Reverse Output Range for PWM MAIN||
|PWM_MAIN_TIM0|-4|400|400||Actuator Outputs|Output Protocol Configuration for PWM Main 1-4|不同于日志默认值|
|PWM_MAIN_TIM1|400||||Actuator Outputs|Output Protocol Configuration for PWM Main 5-6||
|PWM_MAIN_TIM2|400||||Actuator Outputs|Output Protocol Configuration for PWM Main 7-8||
|PWM_MAIN_TIM3|400||||Actuator Outputs|Output Protocol Configuration for PWM Capture 1-3||
|PWM_MAIN_TIM4|400||||Actuator Outputs|Output Protocol Configuration for PWM Capture 4||
|PWM_MAIN_TIM5|400||||Actuator Outputs|Output Protocol Configuration for PWM Capture 5-6||

### 控制分配/机架几何

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|CA_AIRFRAME|0||||Geometry|Airframe selection||
|CA_FAILURE_MODE|0||||Geometry|Motor failure handling mode||
|CA_METHOD|2||||Geometry|Control allocation method||
|CA_R0_SLEW|0.0|||s|Geometry|Motor 0 slew rate limit||
|CA_R10_SLEW|0.0|||s|Geometry|Motor 10 slew rate limit||
|CA_R11_SLEW|0.0|||s|Geometry|Motor 11 slew rate limit||
|CA_R1_SLEW|0.0|||s|Geometry|Motor 1 slew rate limit||
|CA_R2_SLEW|0.0|||s|Geometry|Motor 2 slew rate limit||
|CA_R3_SLEW|0.0|||s|Geometry|Motor 3 slew rate limit||
|CA_R4_SLEW|0.0|||s|Geometry|Motor 4 slew rate limit||
|CA_R5_SLEW|0.0|||s|Geometry|Motor 5 slew rate limit||
|CA_R6_SLEW|0.0|||s|Geometry|Motor 6 slew rate limit||
|CA_R7_SLEW|0.0|||s|Geometry|Motor 7 slew rate limit||
|CA_R8_SLEW|0.0|||s|Geometry|Motor 8 slew rate limit||
|CA_R9_SLEW|0.0|||s|Geometry|Motor 9 slew rate limit||
|CA_ROTOR0_AX|0.0||||Geometry|Axis of rotor 0 thrust vector, X body axis component||
|CA_ROTOR0_AY|0.0||||Geometry|Axis of rotor 0 thrust vector, Y body axis component||
|CA_ROTOR0_AZ|-1.0||||Geometry|Axis of rotor 0 thrust vector, Z body axis component||
|CA_ROTOR0_CT|6.5||||Geometry|Thrust coefficient of rotor 0||
|CA_ROTOR0_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 0||
|CA_ROTOR0_PX|1.0|0.0||m|Geometry|Position of rotor 0 along X body axis relative to center of gravity|不同于日志默认值|
|CA_ROTOR0_PY|1.0|0.0||m|Geometry|Position of rotor 0 along Y body axis relative to center of gravity|不同于日志默认值|
|CA_ROTOR0_PZ|0.0|||m|Geometry|Position of rotor 0 along Z body axis relative to center of gravity||
|CA_ROTOR10_AX|0.0||||Geometry|Axis of rotor 10 thrust vector, X body axis component||
|CA_ROTOR10_AY|0.0||||Geometry|Axis of rotor 10 thrust vector, Y body axis component||
|CA_ROTOR10_AZ|-1.0||||Geometry|Axis of rotor 10 thrust vector, Z body axis component||
|CA_ROTOR10_CT|6.5||||Geometry|Thrust coefficient of rotor 10||
|CA_ROTOR10_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 10||
|CA_ROTOR10_PX|0.0|||m|Geometry|Position of rotor 10 along X body axis relative to center of gravity||
|CA_ROTOR10_PY|0.0|||m|Geometry|Position of rotor 10 along Y body axis relative to center of gravity||
|CA_ROTOR10_PZ|0.0|||m|Geometry|Position of rotor 10 along Z body axis relative to center of gravity||
|CA_ROTOR11_AX|0.0||||Geometry|Axis of rotor 11 thrust vector, X body axis component||
|CA_ROTOR11_AY|0.0||||Geometry|Axis of rotor 11 thrust vector, Y body axis component||
|CA_ROTOR11_AZ|-1.0||||Geometry|Axis of rotor 11 thrust vector, Z body axis component||
|CA_ROTOR11_CT|6.5||||Geometry|Thrust coefficient of rotor 11||
|CA_ROTOR11_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 11||
|CA_ROTOR11_PX|0.0|||m|Geometry|Position of rotor 11 along X body axis relative to center of gravity||
|CA_ROTOR11_PY|0.0|||m|Geometry|Position of rotor 11 along Y body axis relative to center of gravity||
|CA_ROTOR11_PZ|0.0|||m|Geometry|Position of rotor 11 along Z body axis relative to center of gravity||
|CA_ROTOR1_AX|0.0||||Geometry|Axis of rotor 1 thrust vector, X body axis component||
|CA_ROTOR1_AY|0.0||||Geometry|Axis of rotor 1 thrust vector, Y body axis component||
|CA_ROTOR1_AZ|-1.0||||Geometry|Axis of rotor 1 thrust vector, Z body axis component||
|CA_ROTOR1_CT|6.5||||Geometry|Thrust coefficient of rotor 1||
|CA_ROTOR1_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 1||
|CA_ROTOR1_PX|-1.0|0.0||m|Geometry|Position of rotor 1 along X body axis relative to center of gravity|不同于日志默认值|
|CA_ROTOR1_PY|-1.0|0.0||m|Geometry|Position of rotor 1 along Y body axis relative to center of gravity|不同于日志默认值|
|CA_ROTOR1_PZ|0.0|||m|Geometry|Position of rotor 1 along Z body axis relative to center of gravity||
|CA_ROTOR2_AX|0.0||||Geometry|Axis of rotor 2 thrust vector, X body axis component||
|CA_ROTOR2_AY|0.0||||Geometry|Axis of rotor 2 thrust vector, Y body axis component||
|CA_ROTOR2_AZ|-1.0||||Geometry|Axis of rotor 2 thrust vector, Z body axis component||
|CA_ROTOR2_CT|6.5||||Geometry|Thrust coefficient of rotor 2||
|CA_ROTOR2_KM|-0.05000000074505806|0.05000000074505806|||Geometry|Moment coefficient of rotor 2|不同于日志默认值|
|CA_ROTOR2_PX|1.0|0.0||m|Geometry|Position of rotor 2 along X body axis relative to center of gravity|不同于日志默认值|
|CA_ROTOR2_PY|-1.0|0.0||m|Geometry|Position of rotor 2 along Y body axis relative to center of gravity|不同于日志默认值|
|CA_ROTOR2_PZ|0.0|||m|Geometry|Position of rotor 2 along Z body axis relative to center of gravity||
|CA_ROTOR3_AX|0.0||||Geometry|Axis of rotor 3 thrust vector, X body axis component||
|CA_ROTOR3_AY|0.0||||Geometry|Axis of rotor 3 thrust vector, Y body axis component||
|CA_ROTOR3_AZ|-1.0||||Geometry|Axis of rotor 3 thrust vector, Z body axis component||
|CA_ROTOR3_CT|6.5||||Geometry|Thrust coefficient of rotor 3||
|CA_ROTOR3_KM|-0.05000000074505806|0.05000000074505806|||Geometry|Moment coefficient of rotor 3|不同于日志默认值|
|CA_ROTOR3_PX|-1.0|0.0||m|Geometry|Position of rotor 3 along X body axis relative to center of gravity|不同于日志默认值|
|CA_ROTOR3_PY|1.0|0.0||m|Geometry|Position of rotor 3 along Y body axis relative to center of gravity|不同于日志默认值|
|CA_ROTOR3_PZ|0.0|||m|Geometry|Position of rotor 3 along Z body axis relative to center of gravity||
|CA_ROTOR4_AX|0.0||||Geometry|Axis of rotor 4 thrust vector, X body axis component||
|CA_ROTOR4_AY|0.0||||Geometry|Axis of rotor 4 thrust vector, Y body axis component||
|CA_ROTOR4_AZ|-1.0||||Geometry|Axis of rotor 4 thrust vector, Z body axis component||
|CA_ROTOR4_CT|6.5||||Geometry|Thrust coefficient of rotor 4||
|CA_ROTOR4_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 4||
|CA_ROTOR4_PX|0.0|||m|Geometry|Position of rotor 4 along X body axis relative to center of gravity||
|CA_ROTOR4_PY|0.0|||m|Geometry|Position of rotor 4 along Y body axis relative to center of gravity||
|CA_ROTOR4_PZ|0.0|||m|Geometry|Position of rotor 4 along Z body axis relative to center of gravity||
|CA_ROTOR5_AX|0.0||||Geometry|Axis of rotor 5 thrust vector, X body axis component||
|CA_ROTOR5_AY|0.0||||Geometry|Axis of rotor 5 thrust vector, Y body axis component||
|CA_ROTOR5_AZ|-1.0||||Geometry|Axis of rotor 5 thrust vector, Z body axis component||
|CA_ROTOR5_CT|6.5||||Geometry|Thrust coefficient of rotor 5||
|CA_ROTOR5_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 5||
|CA_ROTOR5_PX|0.0|||m|Geometry|Position of rotor 5 along X body axis relative to center of gravity||
|CA_ROTOR5_PY|0.0|||m|Geometry|Position of rotor 5 along Y body axis relative to center of gravity||
|CA_ROTOR5_PZ|0.0|||m|Geometry|Position of rotor 5 along Z body axis relative to center of gravity||
|CA_ROTOR6_AX|0.0||||Geometry|Axis of rotor 6 thrust vector, X body axis component||
|CA_ROTOR6_AY|0.0||||Geometry|Axis of rotor 6 thrust vector, Y body axis component||
|CA_ROTOR6_AZ|-1.0||||Geometry|Axis of rotor 6 thrust vector, Z body axis component||
|CA_ROTOR6_CT|6.5||||Geometry|Thrust coefficient of rotor 6||
|CA_ROTOR6_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 6||
|CA_ROTOR6_PX|0.0|||m|Geometry|Position of rotor 6 along X body axis relative to center of gravity||
|CA_ROTOR6_PY|0.0|||m|Geometry|Position of rotor 6 along Y body axis relative to center of gravity||
|CA_ROTOR6_PZ|0.0|||m|Geometry|Position of rotor 6 along Z body axis relative to center of gravity||
|CA_ROTOR7_AX|0.0||||Geometry|Axis of rotor 7 thrust vector, X body axis component||
|CA_ROTOR7_AY|0.0||||Geometry|Axis of rotor 7 thrust vector, Y body axis component||
|CA_ROTOR7_AZ|-1.0||||Geometry|Axis of rotor 7 thrust vector, Z body axis component||
|CA_ROTOR7_CT|6.5||||Geometry|Thrust coefficient of rotor 7||
|CA_ROTOR7_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 7||
|CA_ROTOR7_PX|0.0|||m|Geometry|Position of rotor 7 along X body axis relative to center of gravity||
|CA_ROTOR7_PY|0.0|||m|Geometry|Position of rotor 7 along Y body axis relative to center of gravity||
|CA_ROTOR7_PZ|0.0|||m|Geometry|Position of rotor 7 along Z body axis relative to center of gravity||
|CA_ROTOR8_AX|0.0||||Geometry|Axis of rotor 8 thrust vector, X body axis component||
|CA_ROTOR8_AY|0.0||||Geometry|Axis of rotor 8 thrust vector, Y body axis component||
|CA_ROTOR8_AZ|-1.0||||Geometry|Axis of rotor 8 thrust vector, Z body axis component||
|CA_ROTOR8_CT|6.5||||Geometry|Thrust coefficient of rotor 8||
|CA_ROTOR8_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 8||
|CA_ROTOR8_PX|0.0|||m|Geometry|Position of rotor 8 along X body axis relative to center of gravity||
|CA_ROTOR8_PY|0.0|||m|Geometry|Position of rotor 8 along Y body axis relative to center of gravity||
|CA_ROTOR8_PZ|0.0|||m|Geometry|Position of rotor 8 along Z body axis relative to center of gravity||
|CA_ROTOR9_AX|0.0||||Geometry|Axis of rotor 9 thrust vector, X body axis component||
|CA_ROTOR9_AY|0.0||||Geometry|Axis of rotor 9 thrust vector, Y body axis component||
|CA_ROTOR9_AZ|-1.0||||Geometry|Axis of rotor 9 thrust vector, Z body axis component||
|CA_ROTOR9_CT|6.5||||Geometry|Thrust coefficient of rotor 9||
|CA_ROTOR9_KM|0.05000000074505806||||Geometry|Moment coefficient of rotor 9||
|CA_ROTOR9_PX|0.0|||m|Geometry|Position of rotor 9 along X body axis relative to center of gravity||
|CA_ROTOR9_PY|0.0|||m|Geometry|Position of rotor 9 along Y body axis relative to center of gravity||
|CA_ROTOR9_PZ|0.0|||m|Geometry|Position of rotor 9 along Z body axis relative to center of gravity||
|CA_ROTOR_COUNT|4|0|||Geometry|Total number of rotors|不同于日志默认值|
|CA_R_REV|0||||Geometry|Bidirectional/Reversible motors||
|CA_SV0_SLEW|0.0|||s|Geometry|Servo 0 slew rate limit||
|CA_SV1_SLEW|0.0|||s|Geometry|Servo 1 slew rate limit||
|CA_SV2_SLEW|0.0|||s|Geometry|Servo 2 slew rate limit||
|CA_SV3_SLEW|0.0|||s|Geometry|Servo 3 slew rate limit||
|CA_SV4_SLEW|0.0|||s|Geometry|Servo 4 slew rate limit||
|CA_SV5_SLEW|0.0|||s|Geometry|Servo 5 slew rate limit||
|CA_SV6_SLEW|0.0|||s|Geometry|Servo 6 slew rate limit||
|CA_SV7_SLEW|0.0|||s|Geometry|Servo 7 slew rate limit||

### 故障检测控制约束

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|FD_ACT_EN|1||||Failure Detector|Enable Actuator Failure check||
|FD_ACT_MOT_C2T|2.0|||A/%|Failure Detector|Motor Failure Current/Throttle Threshold||
|FD_ACT_MOT_THR|0.20000000298023224|||norm|Failure Detector|Motor Failure Throttle Threshold||
|FD_ACT_MOT_TOUT|100|||ms|Failure Detector|Motor Failure Time Threshold||
|FD_ESCS_EN|1||||Failure Detector|Enable checks on ESCs that report their arming state||
|FD_EXT_ATS_EN|0||||Failure Detector|Enable PWM input on for engaging failsafe from an external automatic trigger system (ATS)||
|FD_EXT_ATS_TRIG|1900|||us|Failure Detector|The PWM threshold from external automatic trigger system for engaging failsafe||
|FD_FAIL_P|80|60|60|deg|Failure Detector|FailureDetector Max Pitch|不同于日志默认值|
|FD_FAIL_P_TTRI|0.30000001192092896|||s|Failure Detector|Pitch failure trigger time||
|FD_FAIL_R|80|60|60|deg|Failure Detector|FailureDetector Max Roll|不同于日志默认值|
|FD_FAIL_R_TTRI|0.30000001192092896|||s|Failure Detector|Roll failure trigger time||
|FD_IMB_PROP_THR|30||||Failure Detector|Imbalanced propeller check threshold||

### 精准降落控制

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|PLD_BTOUT|5.0|||s|Precision Land|Landing Target Timeout||
|PLD_FAPPR_ALT|0.10000000149011612|||m|Precision Land|Final approach altitude||
|PLD_HACC_RAD|0.20000000298023224|||m|Precision Land|Horizontal acceptance radius||
|PLD_MAX_SRCH|3||||Precision Land|Maximum number of search attempts||
|PLD_SRCH_ALT|10.0|||m|Precision Land|Search altitude||
|PLD_SRCH_TOUT|10.0|||s|Precision Land|Search timeout||

### 遥控/手动输入

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|MAN_ARM_GESTURE|1||||Manual Control|Enable arm/disarm stick gesture||
|MAN_KILL_GEST_T|-1.0|||s|Manual Control|Trigger time for kill stick gesture||
|RC10_DZ|0.0||||Radio Calibration|RC channel 10 dead zone||
|RC10_MAX|2000.0|||us|Radio Calibration|RC channel 10 maximum||
|RC10_MIN|1000.0|||us|Radio Calibration|RC channel 10 minimum||
|RC10_REV|1.0||||Radio Calibration|RC channel 10 reverse||
|RC10_TRIM|1500.0|||us|Radio Calibration|RC channel 10 trim||
|RC11_DZ|0.0||||Radio Calibration|RC channel 11 dead zone||
|RC11_MAX|2000.0|||us|Radio Calibration|RC channel 11 maximum||
|RC11_MIN|1000.0|||us|Radio Calibration|RC channel 11 minimum||
|RC11_REV|1.0||||Radio Calibration|RC channel 11 reverse||
|RC11_TRIM|1500.0|||us|Radio Calibration|RC channel 11 trim||
|RC12_DZ|0.0||||Radio Calibration|RC channel 12 dead zone||
|RC12_MAX|2000.0|||us|Radio Calibration|RC channel 12 maximum||
|RC12_MIN|1000.0|||us|Radio Calibration|RC channel 12 minimum||
|RC12_REV|1.0||||Radio Calibration|RC channel 12 reverse||
|RC12_TRIM|1500.0|||us|Radio Calibration|RC channel 12 trim||
|RC13_DZ|0.0||||Radio Calibration|RC channel 13 dead zone||
|RC13_MAX|2000.0|||us|Radio Calibration|RC channel 13 maximum||
|RC13_MIN|1000.0|||us|Radio Calibration|RC channel 13 minimum||
|RC13_REV|1.0||||Radio Calibration|RC channel 13 reverse||
|RC13_TRIM|1500.0|||us|Radio Calibration|RC channel 13 trim||
|RC14_DZ|0.0||||Radio Calibration|RC channel 14 dead zone||
|RC14_MAX|2000.0|||us|Radio Calibration|RC channel 14 maximum||
|RC14_MIN|1000.0|||us|Radio Calibration|RC channel 14 minimum||
|RC14_REV|1.0||||Radio Calibration|RC channel 14 reverse||
|RC14_TRIM|1500.0|||us|Radio Calibration|RC channel 14 trim||
|RC15_DZ|0.0||||Radio Calibration|RC channel 15 dead zone||
|RC15_MAX|2000.0|||us|Radio Calibration|RC channel 15 maximum||
|RC15_MIN|1000.0|||us|Radio Calibration|RC channel 15 minimum||
|RC15_REV|1.0||||Radio Calibration|RC channel 15 reverse||
|RC15_TRIM|1500.0|||us|Radio Calibration|RC channel 15 trim||
|RC16_DZ|0.0||||Radio Calibration|RC channel 16 dead zone||
|RC16_MAX|2000.0|||us|Radio Calibration|RC channel 16 maximum||
|RC16_MIN|1000.0|||us|Radio Calibration|RC channel 16 minimum||
|RC16_REV|1.0||||Radio Calibration|RC channel 16 reverse||
|RC16_TRIM|1500.0|||us|Radio Calibration|RC channel 16 trim||
|RC17_DZ|0.0||||Radio Calibration|RC channel 17 dead zone||
|RC17_MAX|2000.0|||us|Radio Calibration|RC channel 17 maximum||
|RC17_MIN|1000.0|||us|Radio Calibration|RC channel 17 minimum||
|RC17_REV|1.0||||Radio Calibration|RC channel 17 reverse||
|RC17_TRIM|1500.0|||us|Radio Calibration|RC channel 17 trim||
|RC18_DZ|0.0||||Radio Calibration|RC channel 18 dead zone||
|RC18_MAX|2000.0|||us|Radio Calibration|RC channel 18 maximum||
|RC18_MIN|1000.0|||us|Radio Calibration|RC channel 18 minimum||
|RC18_REV|1.0||||Radio Calibration|RC channel 18 reverse||
|RC18_TRIM|1500.0|||us|Radio Calibration|RC channel 18 trim||
|RC1_DZ|10.0|||us|Radio Calibration|RC channel 1 dead zone||
|RC1_MAX|1933.0|2000.0|2000.0|us|Radio Calibration|RC channel 1 maximum|不同于日志默认值|
|RC1_MIN|1094.0|1000.0|1000.0|us|Radio Calibration|RC channel 1 minimum|不同于日志默认值|
|RC1_REV|1.0||||Radio Calibration|RC channel 1 reverse||
|RC1_TRIM|1514.0|1500.0|1500.0|us|Radio Calibration|RC channel 1 trim|不同于日志默认值|
|RC2_DZ|10.0|||us|Radio Calibration|RC channel 2 dead zone||
|RC2_MAX|1933.0|2000.0|2000.0|us|Radio Calibration|RC channel 2 maximum|不同于日志默认值|
|RC2_MIN|1094.0|1000.0|1000.0|us|Radio Calibration|RC channel 2 minimum|不同于日志默认值|
|RC2_REV|-1.0|1.0|1.0||Radio Calibration|RC channel 2 reverse|不同于日志默认值|
|RC2_TRIM|1514.0|1500.0|1500.0|us|Radio Calibration|RC channel 2 trim|不同于日志默认值|
|RC3_DZ|10.0|||us|Radio Calibration|RC channel 3 dead zone||
|RC3_MAX|1927.0|2000.0|2000.0|us|Radio Calibration|RC channel 3 maximum|不同于日志默认值|
|RC3_MIN|1094.0|1000.0|1000.0|us|Radio Calibration|RC channel 3 minimum|不同于日志默认值|
|RC3_REV|1.0||||Radio Calibration|RC channel 3 reverse||
|RC3_TRIM|1094.0|1500.0|1500.0|us|Radio Calibration|RC channel 3 trim|不同于日志默认值|
|RC4_DZ|10.0|||us|Radio Calibration|RC channel 4 dead zone||
|RC4_MAX|1933.0|2000.0|2000.0|us|Radio Calibration|RC channel 4 maximum|不同于日志默认值|
|RC4_MIN|1094.0|1000.0|1000.0|us|Radio Calibration|RC channel 4 minimum|不同于日志默认值|
|RC4_REV|1.0||||Radio Calibration|RC channel 4 reverse||
|RC4_TRIM|1514.0|1500.0|1500.0|us|Radio Calibration|RC channel 4 trim|不同于日志默认值|
|RC5_DZ|10.0||||Radio Calibration|RC channel 5 dead zone||
|RC5_MAX|2000.0|||us|Radio Calibration|RC channel 5 maximum||
|RC5_MIN|1000.0|||us|Radio Calibration|RC channel 5 minimum||
|RC5_REV|1.0||||Radio Calibration|RC channel 5 reverse||
|RC5_TRIM|1500.0|||us|Radio Calibration|RC channel 5 trim||
|RC6_DZ|10.0||||Radio Calibration|RC channel 6 dead zone||
|RC6_MAX|2000.0|||us|Radio Calibration|RC channel 6 maximum||
|RC6_MIN|1000.0|||us|Radio Calibration|RC channel 6 minimum||
|RC6_REV|1.0||||Radio Calibration|RC channel 6 reverse||
|RC6_TRIM|1500.0|||us|Radio Calibration|RC channel 6 trim||
|RC7_DZ|10.0||||Radio Calibration|RC channel 7 dead zone||
|RC7_MAX|2000.0|||us|Radio Calibration|RC channel 7 maximum||
|RC7_MIN|1000.0|||us|Radio Calibration|RC channel 7 minimum||
|RC7_REV|1.0||||Radio Calibration|RC channel 7 reverse||
|RC7_TRIM|1500.0|||us|Radio Calibration|RC channel 7 trim||
|RC8_DZ|10.0||||Radio Calibration|RC channel 8 dead zone||
|RC8_MAX|2000.0|||us|Radio Calibration|RC channel 8 maximum||
|RC8_MIN|1000.0|||us|Radio Calibration|RC channel 8 minimum||
|RC8_REV|1.0||||Radio Calibration|RC channel 8 reverse||
|RC8_TRIM|1500.0|||us|Radio Calibration|RC channel 8 trim||
|RC9_DZ|0.0||||Radio Calibration|RC channel 9 dead zone||
|RC9_MAX|2000.0|||us|Radio Calibration|RC channel 9 maximum||
|RC9_MIN|1000.0|||us|Radio Calibration|RC channel 9 minimum||
|RC9_REV|1.0||||Radio Calibration|RC channel 9 reverse||
|RC9_TRIM|1500.0|||us|Radio Calibration|RC channel 9 trim||
|RC_ARMSWITCH_TH|0.75||||Radio Switches|Threshold for the arm switch||
|RC_CHAN_CNT|18|0|0||Radio Calibration|RC channel count|不同于日志默认值|
|RC_ENG_MOT_TH|0.75||||Radio Switches|Threshold for selecting main motor engage||
|RC_FAILS_THR|0|||us|Radio Calibration|Failsafe channel PWM threshold||
|RC_GEAR_TH|0.75||||Radio Switches|Threshold for the landing gear switch||
|RC_INPUT_PROTO|2|-1|-1||RC Input|RC input protocol|不同于日志默认值|
|RC_KILLSWITCH_TH|0.75||||Radio Switches|Threshold for the kill switch||
|RC_LOITER_TH|0.75||||Radio Switches|Threshold for selecting loiter mode||
|RC_MAP_ARM_SW|0||||Radio Switches|Arm switch channel||
|RC_MAP_AUX1|0||||Radio Calibration|AUX1 Passthrough RC channel||
|RC_MAP_AUX2|0||||Radio Calibration|AUX2 Passthrough RC channel||
|RC_MAP_AUX3|0||||Radio Calibration|AUX3 Passthrough RC channel||
|RC_MAP_AUX4|0||||Radio Calibration|AUX4 Passthrough RC channel||
|RC_MAP_AUX5|0||||Radio Calibration|AUX5 Passthrough RC channel||
|RC_MAP_AUX6|0||||Radio Calibration|AUX6 Passthrough RC channel||
|RC_MAP_ENG_MOT|0||||Radio Calibration|RC channel to engage the main motor (for helicopters)||
|RC_MAP_FAILSAFE|0||||Radio Calibration|Failsafe channel mapping||
|RC_MAP_FLAPS|0||||Radio Switches|Flaps channel||
|RC_MAP_FLTMODE|0||||Radio Switches|Single channel flight mode selection||
|RC_MAP_FLTM_BTN|0||||Radio Switches|Button flight mode selection||
|RC_MAP_GEAR_SW|0||||Radio Switches|Landing gear switch channel||
|RC_MAP_KILL_SW|0||||Radio Switches|Emergency Kill switch channel||
|RC_MAP_LOITER_SW|0||||Radio Switches|Loiter switch channel||
|RC_MAP_MODE_SW|0||||Radio Switches|Mode switch channel mapping (deprecated)||
|RC_MAP_OFFB_SW|0||||Radio Switches|Offboard switch channel||
|RC_MAP_PARAM1|0||||Radio Calibration|PARAM1 tuning channel||
|RC_MAP_PARAM2|0||||Radio Calibration|PARAM2 tuning channel||
|RC_MAP_PARAM3|0||||Radio Calibration|PARAM3 tuning channel||
|RC_MAP_PAY_SW|0||||Radio Switches|Payload Power Switch RC channel||
|RC_MAP_PITCH|2|0|0||Radio Calibration|Pitch control channel mapping|不同于日志默认值|
|RC_MAP_RETURN_SW|0||||Radio Switches|Return switch channel||
|RC_MAP_ROLL|1|0|0||Radio Calibration|Roll control channel mapping|不同于日志默认值|
|RC_MAP_THROTTLE|3|0|0||Radio Calibration|Throttle control channel mapping|不同于日志默认值|
|RC_MAP_TRANS_SW|0||||Radio Switches|VTOL transition switch channel mapping||
|RC_MAP_YAW|4|0|0||Radio Calibration|Yaw control channel mapping|不同于日志默认值|
|RC_OFFB_TH|0.75||||Radio Switches|Threshold for selecting offboard mode||
|RC_PAYLOAD_MIDTH|0.25||||Radio Switches|Threshold for mid position of payload power switch||
|RC_PAYLOAD_TH|0.75||||Radio Switches|Threshold for on position of payload power switch||
|RC_PORT_CONFIG|0||||Serial|Serial Configuration for RC Input Driver||
|RC_RETURN_TH|0.75||||Radio Switches|Threshold for selecting return to launch mode||
|RC_RSSI_PWM_CHAN|0||||Radio Calibration|PWM input channel that provides RSSI||
|RC_RSSI_PWM_MAX|2000||||Radio Calibration|Max input value for RSSI reading||
|RC_RSSI_PWM_MIN|1000||||Radio Calibration|Min input value for RSSI reading||
|RC_TRANS_TH|0.75||||Radio Switches|Threshold for the VTOL transition switch||

### 降落检测/着陆控制

|参数|当前值|system默认|setup默认|单位|PX4分组|说明|备注|
|---|---|---|---|---|---|---|---|
|LNDMC_ALT_GND|2.0|||m|Land Detector|Ground effect altitude for multicopters||
|LNDMC_ROT_MAX|20.0|||deg/s|Land Detector|Multicopter max rotational speed||
|LNDMC_TRIG_TIME|1.0|||s|Land Detector|Multicopter land detection trigger time||
|LNDMC_XY_VEL_MAX|1.5|||m/s|Land Detector|Multicopter max horizontal velocity||
|LNDMC_Z_VEL_MAX|0.25|||m/s|Land Detector|Multicopter vertical velocity threshold||
|LND_FLIGHT_T_HI|2||||Land Detector|Total flight time in microseconds||
|LND_FLIGHT_T_LO|-1103393143||||Land Detector|Total flight time in microseconds||
