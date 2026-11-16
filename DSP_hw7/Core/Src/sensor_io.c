/*
 * sensor_io.c
 *
 *  Created on: 2026年11月12日
 *      Author: yihsu
 */

#include "main.h"

extern I2C_HandleTypeDef hi2c2;

/*
 * I2C2 is initialized by MX_I2C2_Init()
 * before BSP_ACCELERO_Init().
 */
void SENSOR_IO_Init(void)
{
    /* No additional initialization required */
}

void SENSOR_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
    HAL_I2C_Mem_Write(
        &hi2c2,
        Addr,
        Reg,
        I2C_MEMADD_SIZE_8BIT,
        &Value,
        1,
        1000
    );
}

uint8_t SENSOR_IO_Read(uint8_t Addr, uint8_t Reg)
{
    uint8_t value = 0;

    HAL_I2C_Mem_Read(
        &hi2c2,
        Addr,
        Reg,
        I2C_MEMADD_SIZE_8BIT,
        &value,
        1,
        1000
    );

    return value;
}

uint16_t SENSOR_IO_ReadMultiple(
    uint8_t Addr,
    uint8_t Reg,
    uint8_t *Buffer,
    uint16_t Length)
{
    return (uint16_t)HAL_I2C_Mem_Read(
        &hi2c2,
        Addr,
        Reg,
        I2C_MEMADD_SIZE_8BIT,
        Buffer,
        Length,
        1000
    );
}
