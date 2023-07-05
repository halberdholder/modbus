#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <memory.h>
#include <modbus.h>
#include <errno.h>

#include "config.h"

int main(void)
{
    int rc = 0;
    modbus_t *ctx = NULL;
    modbus_mapping_t *mb_mapping = NULL;
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];

    // 打开端口: 端口，波特率，校验位，数据位，停止位
    ctx = modbus_new_rtu(SLAVE_DEV, BAUD_RATE, 'N', 8, 1);
    // 设置从机地址
    modbus_set_slave(ctx, 1);
    // 设置串口模式
    // modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS232);
    // 设置RTS
    // modbus_rtu_set_rts(ctx, MODBUS_RTU_RTS_UP);
    // 建立连接
    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connexion failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    mb_mapping = modbus_mapping_new(MODBUS_MAX_READ_BITS, 0,
                                    MODBUS_MAX_READ_REGISTERS, 0);
    if (mb_mapping == NULL)
    {
        fprintf(stderr, "Failed to allocate the mapping: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // 初始化几个寄存器
    mb_mapping->tab_registers[0] = 1;
    mb_mapping->tab_registers[1] = 2;
    mb_mapping->tab_registers[2] = 3;
    mb_mapping->tab_registers[3] = 4;
    mb_mapping->tab_registers[4] = 5;
    mb_mapping->tab_registers[5] = 6;

    while (1)
    {
        memset(query, 0, sizeof(query));
        rc = modbus_receive(ctx, query);
        if (rc > 0)
        {
            modbus_reply(ctx, query, rc, mb_mapping);
        }
        else if (rc == -1)
        {
            // Connection closed by the client or error
            break;
        }
    }

    printf("Quit the loop: %s\n", modbus_strerror(errno));

    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);
}