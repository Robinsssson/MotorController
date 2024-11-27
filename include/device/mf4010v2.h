#ifndef __MF4010V2__H__
#define __MF4010V2__H__
#include "mirco_lib/mirco_stdint.h"
#include <cjson/cJSON.h>
#include <stdint.h>
/**
 * @brief 电机关闭命令
 */
#define COMMAND_MOTOR_CLOSE                                                                                            \
    {                                                                                                                  \
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                                                                 \
    }
/**
 * @brief 电机运行命令
 */
#define COMMAND_MOTOR_RUNNING                                                                                          \
    {                                                                                                                  \
        0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                                                                 \
    }
/**
 * @brief 电机停止命令
 */
#define COMMAND_MOTOR_STOP                                                                                             \
    {                                                                                                                  \
        0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                                                                 \
    }
/**
 * @brief 转矩闭环控制
 * @arg   int16_t iqControl -2048->2048
 */
#define COMMAND_TORQUE_CONTROL(iqControl)                                                                              \
    {                                                                                                                  \
        0xA1, 0x00, 0x00, 0x00, *(uint8_t*) (&iqControl), *((uint8_t*) (&iqControl) + 1), 0x00, 0x00                   \
    }
/**
 * @brief 单圈位置闭环控制
 * @arg   uint8_t spinDirection 0x00顺时针，0x01逆时针
 *        uint32_t angleControl 36000->360°
 */
#define COMMAND_SINGLERING_CONTROL_1(spinDirection, angleControl)                                                      \
    {                                                                                                                  \
        0xA5, spinDirection, 0x00, 0x00, *(uint8_t*) (&angleControl), *((uint8_t*) (&angleControl) + 1),               \
            *((uint8_t*) (&angleControl) + 2), *((uint8_t*) (&angleControl) + 3)                                       \
    }

/**
 * @brief 读取PID参数
 * @return
 */
#define COMMAND_READ_PID_PARAMS                                                                                        \
    {                                                                                                                  \
        0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                                                                 \
    }

#define COMMAND_WRITE_PID_PARAMS(location, anglePidKp, anglePidKi, speedPidKp, speedPidKi, iqPidKp, iqPidKi)           \
    {                                                                                                                  \
        location, 0x00, anglePidKp, anglePidKi, speedPidKp, speedPidKi, iqPidKp, iqPidKi                               \
    }
/**
 * @brief PID写入RAM
 */
#define COMMAND_WRITE_PID_PARAMS_RAM(anglePidKp, anglePidKi, speedPidKp, speedPidKi, iqPidKp, iqPidKi)                 \
    COMMAND_WRITE_PID_PARAMS(0x31, anglePidKp, anglePidKi, speedPidKp, speedPidKi, iqPidKp, iqPidKi)
/**
 * @brief PID写入ROM
 */
#define COMMAND_WRITE_PID_PARAMS_ROM(anglePidKp, anglePidKi, speedPidKp, speedPidKi, iqPidKp, iqPidKi)                 \
    COMMAND_WRITE_PID_PARAMS(0x32, anglePidKp, anglePidKi, speedPidKp, speedPidKi, iqPidKp, iqPidKi)
/**
 * @brief 速度闭环控制
 * @arg   int32_t speedControl为电机速度 speedControl * 0.01dps/LSB
 */ // 1个字节，8位 0x00-0xff 接收32位的输入 0x00000000 - 0xffffffff
    // 0x12345678 -> 0x12 0x34 0x56 0x78
#define COMMAND_SPEED_CONTROL(speedControl)                                                                            \
    {                                                                                                                  \
        0xA2, 0x00, 0x00, 0x00, *(uint8_t*) (&speedControl), *((uint8_t*) (&speedControl) + 1),                        \
            *((uint8_t*) (&speedControl) + 2), *((uint8_t*) (&speedControl) + 3)                                       \
    }
/**
 * @brief 增量闭环角度控制
 * @arg   uint8_t spinDirection 电机转动方向 0x00顺时针 0x01逆时针
 *        uint32_t angleControl 设置电机转动角度如36000代表360°
 *        uint16_t maxSpeed     设置最大转速，即360代表360dps。
 */
#define COMMAND_SINGLERING_CONTROL_2(spinDirection, maxSpeed, angleControl)                                            \
    {                                                                                                                  \
        0xA6, spinDirection, *(uint8_t*) (&maxSpeed), *((uint8_t*) (&maxSpeed) + 1), *(uint8_t*) (&angleControl),      \
            *((uint8_t*) (&angleControl) + 1), *((uint8_t*) (&angleControl) + 2), *((uint8_t*) (&angleControl) + 3)    \
    }
/**
 * @brief 增量闭环角度控制
 * @arg   int32_t角度增量，如36000代表360°
 */
#define COMMAND_POSITION_ADD_CONTROL(angleIncrement)                                                                   \
    {                                                                                                                  \
        0xA7, 0x00, 0x00, 0x00, *(uint8_t*) (&angleIncrement), *((uint8_t*) (&angleIncrement) + 1),                    \
            *((uint8_t*) (&angleIncrement) + 2), *((uint8_t*) (&angleIncrement) + 3)                                   \
    }
/**
 * @brief 电机命令发送与接收程序
 * @arg   mf4010v2_t为电机结构体对象，输入对应的COMMAND_MIRCO宏
 */
#define COMMAND_USEAGE(motor, COMMAND_MIRCO)                                                                           \
    {                                                                                                                  \
        uint8_t command[] = COMMAND_MIRCO;                                                                             \
        mf4010v2_send_command(motor, command, 1);                                                                         \
        char* str = mf4010v2_print_json(motor);                                                                        \
        printf("%s\n", str);                                                                                           \
        cJSON_free(str);                                                                                               \
        mf4010v2_free_json(motor);                                                                                     \
    }

typedef struct mf4010v2_t mf4010v2_t;
struct mf4010v2_t {
    uint8_t  command[8];
    uint8_t  return_code[8];
    uint16_t id;
    cJSON*   json_data;
};
mf4010v2_t mf4010v2_init(int id);
void       mf4010v2_send_command(mf4010v2_t* mf4010v2, uint8_t command[8], uint8_t create_json);
void       mf4010v2_to_json(mf4010v2_t* mf4010v2);
void       mf4010v2_free_json(mf4010v2_t* mf4010v2);
char*      mf4010v2_print_json(mf4010v2_t* mf4010v2);
#endif //!__MF4010V2__H__
