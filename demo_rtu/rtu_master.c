#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <modbus.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>

long get_system_time_microsecond()
{
    struct timeval tv = {};
    if (0 == gettimeofday(&tv, NULL))
        return tv.tv_sec * 1e6 + tv.tv_usec;
    else
        return 0;
}

uint16_t tab_reg[64] = {
    0, 1, 2, 3, 4,
    5, 6, 7, 8, 9,
    10, 11, 12, 13, 14,
    15, 16, 17, 18, 19};

int _write(modbus_t *ctx)
{
    static int num = 0;
    int rc = 0;
    long start_time = 0, end_time = 0;

    start_time = get_system_time_microsecond();
    rc = modbus_write_registers(ctx, 0, 20, tab_reg);
    end_time = get_system_time_microsecond();
    if (rc == -1)
    {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
        return rc;
    }
    printf("[%4d][write num = %d][time escap = %ld]\n",
        num++, rc, end_time - start_time);
    return 0;
}

int _read(modbus_t *ctx)
{
    static int num = 0;
    int rc = 0;
    long start_time = 0, end_time = 0;

    memset(tab_reg, 0, sizeof(tab_reg));
    // 读寄存器设置：寄存器地址、数量、数据缓冲
    start_time = get_system_time_microsecond();
    rc = modbus_read_registers(ctx, 0, 20, tab_reg);
    end_time = get_system_time_microsecond();
    if (rc == -1)
    {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
        return -1;
    }
    printf("[%4d][read num = %d][time escap = %ld]",
        num++, rc, end_time - start_time);
    for (int i = 0; i < 20; ++i)
    {
        printf("<%#x>", tab_reg[i]);
    }
    printf("\n");
    return 0;
}

int main(void)
{
    modbus_t *ctx = NULL;

    //打开端口: 端口，波特率，校验位，数据位，停止位
    ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1);
    //设置从机地址
    modbus_set_slave(ctx, 1);
    //设置串口模式(可选)
    // modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS232);
    //设置RTS(可选)
    // modbus_rtu_set_rts(ctx, MODBUS_RTU_RTS_UP);
    //建立连接
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connexion failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    //设置应答延时(可选)
    modbus_set_response_timeout(ctx, 0, 1000000);

    while (1)
    {
        _read(ctx);
        // _write(ctx);
        sleep(1);
    }
}
