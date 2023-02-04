/**
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#include "MLX90621_I2C_Driver.h"
#include "i2c.h"

void MLX90621_I2CInit(){
	// .ioc
}

void MLX90621_I2CFreqSet(int freq){
	// .ioc
}

int MLX90621_I2CReadEEPROM(uint8_t slaveAddr, uint8_t startAddress, uint16_t nMemAddressRead, uint8_t *data)
{
    int cnt = 0;
    uint8_t i2cData[256] = {0};
    
    if(HAL_I2C_Mem_Read(&hi2c2, (slaveAddr << 1), startAddress, 1, i2cData, nMemAddressRead, 100) != HAL_OK)
    {
    	return -1;
    }
    
    for(cnt=0; cnt < nMemAddressRead; cnt++)
    {
        *data++ = i2cData[cnt];
    }

    return 0;
} 

int MLX90621_I2CRead(uint8_t slaveAddr,uint8_t command, uint8_t startAddress, uint8_t addressStep, uint8_t nMemAddressRead, uint16_t *data)
{
    int cnt = 0;
    int i = 0;
    uint8_t i2cData[132] = {0};
    

    if(Custom_HAL_I2C_Mem_Read(&hi2c2, (slaveAddr << 1), command, startAddress, addressStep, nMemAddressRead, i2cData, 2*nMemAddressRead, 100) != HAL_OK)
    {
    	return -1;
    }
    
    for(cnt=0; cnt < nMemAddressRead; cnt++)
    {
        i = cnt << 1;
        *data++ = (uint16_t)i2cData[i+1]*256 + (uint16_t)i2cData[i];
    }

    return 0;   
}

int MLX90621_I2CRead_Polling(uint8_t slaveAddr,uint8_t command, uint8_t startAddress, uint8_t addressStep, uint8_t nMemAddressRead, uint16_t *data)
{
    int cnt = 0;
    int i = 0;
    uint8_t i2cData[132] = {0};


    if(Custom_HAL_I2C_Mem_Read(&hi2c2, (slaveAddr << 1), command, startAddress, addressStep, nMemAddressRead, i2cData, 2*nMemAddressRead, 100) != HAL_OK)
    {
    	return -1;
    }

    for(cnt=0; cnt < nMemAddressRead; cnt++)
    {
        i = cnt << 1;
        *data++ = (uint16_t)i2cData[i+1]*256 + (uint16_t)i2cData[i];
    }

    return 0;
} 

int MLX90621_I2CWrite(uint8_t slaveAddr, uint8_t command, uint8_t checkValue, uint16_t data)
{
    uint8_t cmd[5] = {0,0,0,0,0};
    static uint16_t dataCheck;
    
    cmd[0] = command;
    cmd[2] = data & 0x00FF;
    cmd[1] = cmd[2] - checkValue;
    cmd[4] = data >> 8;
    cmd[3] = cmd[4] - checkValue;

    if(HAL_I2C_Master_Transmit(&hi2c2, (slaveAddr << 1), cmd, 5, 100) != HAL_OK)
    {
        return -1;
    }

    MLX90621_I2CRead_Polling(slaveAddr, 0x02, 0x8F+command, 0, 1, &dataCheck);
    
    if (dataCheck != data)
    {
        return -2;
    }    
    
    return 0;
}












