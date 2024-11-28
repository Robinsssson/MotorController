#include "bsp/can1.h"
#include "bsp/tim2.h"
#include "bsp/usart1.h"
#include "cjson/cJSON.h"
#include "device/inv_mpu.h"
#include "device/mf4010v2.h"
#include "mirco_lib/mirco_delay.h"
#include <app.h>
#include <device/mpu6050.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pid/PID.h"

#define PID_KP1 3.5f
#define PID_KI1 1.2f
#define PID_KD1 0.0f
#define PID_TAU1 0.02f
#define PID_LIM_MIN1 -500.0f
#define PID_LIM_MAX1 500.0f
#define PID_LIM_MIN_INT1 -100.0f
#define PID_LIM_MAX_INT1 100.0f

#define PID_KP2 3.5f
#define PID_KI2 0.3f
#define PID_KD2 0.0f
#define PID_TAU2 0.02f
#define PID_LIM_MIN2 -500.0f
#define PID_LIM_MAX2 500.0f
#define PID_LIM_MIN_INT2 -100.0f
#define PID_LIM_MAX_INT2 100.0f

#define SAMPLE_TIME_S 0.1f


PIDController     pidControllerMotor1, pidControllerMotor2;
extern mf4010v2_t motor1, motor2;
/* private functions declaration */
/* public functions realization */
void app_init()
{
    mirco_delay_init();
    led_init();
    key_init();
    can1_init();
    usart1_init(115200);
    cJSON_InitHooks(NULL);

    pidControllerMotor1 = (PIDController){.Kp        = PID_KP1,
                                          .Ki        = PID_KI1,
                                          .Kd        = PID_KD1,
                                          .tau       = PID_TAU1,
                                          .limMin    = PID_LIM_MIN1,
                                          .limMax    = PID_LIM_MAX1,
                                          .limMinInt = PID_LIM_MIN_INT1,
                                          .limMaxInt = PID_LIM_MAX_INT1,
                                          .T         = SAMPLE_TIME_S};

    pidControllerMotor2 = (PIDController){.Kp        = PID_KP2,
                                          .Ki        = PID_KI2,
                                          .Kd        = PID_KD2,
                                          .tau       = PID_TAU2,
                                          .limMin    = PID_LIM_MIN2,
                                          .limMax    = PID_LIM_MAX2,
                                          .limMinInt = PID_LIM_MIN_INT2,
                                          .limMaxInt = PID_LIM_MAX_INT2,
                                          .T         = SAMPLE_TIME_S};
    PIDController_Init(&pidControllerMotor1);
    PIDController_Init(&pidControllerMotor2);
    motor1 = mf4010v2_init(0x141); // 上面的电机
    motor2 = mf4010v2_init(0x142);
    COMMAND_USEAGE(&motor1, COMMAND_MOTOR_RUNNING);
    COMMAND_USEAGE(&motor2, COMMAND_MOTOR_RUNNING);
    COMMAND_USEAGE(&motor1, COMMAND_READ_PID_PARAMS);
    COMMAND_USEAGE(&motor2, COMMAND_READ_PID_PARAMS);
    // COMMAND_USEAGE(&motor1, COMMAND_WRITE_PID_PARAMS_RAM(20, 5, 60, 1, 60, 30));
    // COMMAND_USEAGE(&motor2, COMMAND_WRITE_PID_PARAMS_RAM(20, 5, 60, 5, 60, 30));
    MPU_Init();
    while (mpu_dmp_init()) {
        mirco_delay_ms(200);
        printf("MPU DMP INIT ERROR\r\n");
    }
    printf("MPU DMP INIT OK\r\n");
    tim2_init();
    led_toggle();
}
// 电机1是否使能， 当前测量值， 目标值  电机2是否使能， 当前测量值， 目标值
//
#define max_val_motor_2 400
void app_control(uint8_t enable_motor1, float measurement1, float setpoint1, uint8_t enable_motor2, float measurement2,
                 float setpoint2)
{
    float output_motor1 = PIDController_Update(&pidControllerMotor1, setpoint1, measurement1);
    float output_motor2 = PIDController_Update(&pidControllerMotor2, setpoint2, measurement2);
    // Motor 1 control // 测量值和目标值
    if (enable_motor1) {
        int32_t add_motor1 = (int32_t) output_motor1 * 100; // 0x12345678
        uint8_t command_motor1[8] =
            COMMAND_SPEED_CONTROL(add_motor1); // 把 output(t) 作为电机转速 //can发一次消息可以发送八个字节 int8_t [8]
        mf4010v2_send_command(&motor1, command_motor1, 0);
    }
    // 使用PID算法，计算一个PID的输出，output(t)
    if (enable_motor2) {
        int32_t add_motor2 = (int32_t) output_motor2 * 100;
        if (measurement2 < 0.8 && measurement2 > -0.8)
            add_motor2 = 0;
        uint8_t command_motor2[8] = COMMAND_SPEED_CONTROL(add_motor2);
        mf4010v2_send_command(&motor2, command_motor2, 0);
    }
}