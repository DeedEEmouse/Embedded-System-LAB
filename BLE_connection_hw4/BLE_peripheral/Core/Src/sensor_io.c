#include "main.h"

extern I2C_HandleTypeDef hi2c2;

/* AI wrote this, basically doing some of the job of hw2's stm32l475e_ioc01.c
 * where accounts of SENSOR_IO_XXX function in lsm6dsl.h*/

void SENSOR_IO_Init(void)
{
}

void SENSOR_IO_DeInit(void)
{
    HAL_I2C_DeInit(&hi2c2);
}

void SENSOR_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
    HAL_I2C_Mem_Write(
        &hi2c2,
        Addr,
        (uint16_t)Reg,
        I2C_MEMADD_SIZE_8BIT,
        &Value,
        1,
        1000
    );
}

uint8_t SENSOR_IO_Read(uint8_t Addr, uint8_t Reg)
{
    uint8_t read_value = 0;

    HAL_I2C_Mem_Read(
        &hi2c2,
        Addr,
        (uint16_t)Reg,
        I2C_MEMADD_SIZE_8BIT,
        &read_value,
        1,
        1000
    );

    return read_value;
}

uint16_t SENSOR_IO_ReadMultiple(
    uint8_t Addr,
    uint8_t Reg,
    uint8_t *Buffer,
    uint16_t Length
)
{
    return (uint16_t)HAL_I2C_Mem_Read(
        &hi2c2,
        Addr,
        (uint16_t)Reg,
        I2C_MEMADD_SIZE_8BIT,
        Buffer,
        Length,
        1000
    );
}

void SENSOR_IO_WriteMultiple(
    uint8_t Addr,
    uint8_t Reg,
    uint8_t *Buffer,
    uint16_t Length
)
{
    HAL_I2C_Mem_Write(
        &hi2c2,
        Addr,
        (uint16_t)Reg,
        I2C_MEMADD_SIZE_8BIT,
        Buffer,
        Length,
        1000
    );
}

HAL_StatusTypeDef SENSOR_IO_IsDeviceReady(
    uint16_t DevAddress,
    uint32_t Trials
)
{
    return HAL_I2C_IsDeviceReady(
        &hi2c2,
        DevAddress,
        Trials,
        1000
    );
}

void SENSOR_IO_Delay(uint32_t Delay)
{
    HAL_Delay(Delay);
}
