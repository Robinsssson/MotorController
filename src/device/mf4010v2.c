#include "device/mf4010v2.h"
#include "bsp/can1.h"
#include "bsp/usart1.h"
#include "cjson/cJSON.h"
#include "stm32f10x_can.h"
#include <stdint.h>
#include <stdio.h>

mf4010v2_t mf4010v2_init(int id)
{
    // TODO: Implement this function
    mf4010v2_t mf4010v2;
    mf4010v2.id = id;
    for (int i = 0; i < 8; i++) {
        mf4010v2.command[i]     = 0;
        mf4010v2.return_code[i] = 0;
    }
    mf4010v2.json_data = NULL;
    return mf4010v2;
}

void mf4010v2_send_command(mf4010v2_t* mf4010v2, uint8_t command[8], uint8_t create_json)
{
    for (int i = 0; i < 8; i++) {
        mf4010v2->command[i] = command[i];
    }
    can1_send_message(mf4010v2->id, mf4010v2->command, 8);
    CanRxMsg rx = can1_receive_message();
    if (rx.StdId == 0xFFFF) {
        printf("No response from device\n");
    }
    for (int i = 0; i < 8; i++) {
        mf4010v2->return_code[i] = rx.Data[i];
    }
    if (create_json) {
        mf4010v2_to_json(mf4010v2);
    }
}
// struct mf4010v2_t {
//     uint8_t  command[8];
//     uint8_t  return_code[8];
//     uint16_t id;
//     cJSON*   json_data;
// };
void mf4010v2_to_json(mf4010v2_t* mf4010v2)
{
    // TODO: Implement this function
    cJSON* json_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_root, "id", mf4010v2->id);
    cJSON* json_commands = cJSON_CreateArray();
    for (int i = 0; i < 8; i++) {
        cJSON_AddItemToArray(json_commands, cJSON_CreateNumber(mf4010v2->command[i]));
    }
    cJSON_AddItemToObject(json_root, "commands", json_commands);
    cJSON* json_return_codes = cJSON_CreateArray();
    for (int i = 0; i < 8; i++) {
        cJSON_AddItemToArray(json_return_codes, cJSON_CreateNumber(mf4010v2->return_code[i]));
    }
    cJSON_AddItemToObject(json_root, "return_codes", json_return_codes);
    mf4010v2->json_data = json_root;
}

char* mf4010v2_print_json(mf4010v2_t* mf4010v2)
{
    // TODO: Implement this function
    return cJSON_PrintUnformatted(mf4010v2->json_data);
}

void mf4010v2_free_json(mf4010v2_t* mf4010v2)
{
    // TODO: Implement this function
    if (mf4010v2->json_data != NULL) {
        cJSON_Delete(mf4010v2->json_data);
    }
}