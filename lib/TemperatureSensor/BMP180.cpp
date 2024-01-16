/* An STM32 HAL library written for the BMP180 temperature/pressure sensor. */
/* Library by @eepj www.github.com/eepj */

#ifdef STM32_TARGET
#include "stm32f4xx_hal.h"
#endif

#include "BMP180.hpp"

extern "C"
{
}

#define BMP180_I2C_ADDR 0x77
#define BMP180_I2C_TIMEOUT 1000
#define BMP180_CONTROL_REG 0xf4
#define BMP180_MSB_REG 0xf6
#define BMP180_LSB_REG 0xf7
#define BMP180_XLSB_REG 0xf8
#define BMP180_SOFT_RESET_REG 0xe0
#define BMP180_SOFT_RESET 0xb6

typedef enum BMP180_OSS
{
  BMP180_LOW,
  BMP180_STANDARD,
  BMP180_HIGH,
  BMP180_ULTRA,
} BMP180_OSS;

typedef enum BMP180_CalIndex
{
  BMP180_INDEX_AC1,
  BMP180_INDEX_AC2,
  BMP180_INDEX_AC3,
  BMP180_INDEX_AC4,
  BMP180_INDEX_AC5,
  BMP180_INDEX_AC6,
  BMP180_INDEX_B1,
  BMP180_INDEX_B2,
  BMP180_INDEX_MB,
  BMP180_INDEX_MC,
  BMP180_INDEX_MD,
} BMP180_CalIndex;

typedef struct BMP180_EEPROM
{
  short BMP180_AC1;
  short BMP180_AC2;
  short BMP180_AC3;
  unsigned short BMP180_AC4;
  unsigned short BMP180_AC5;
  unsigned short BMP180_AC6;
  short BMP180_B1;
  short BMP180_B2;
  short BMP180_MB;
  short BMP180_MC;
  short BMP180_MD;
} BMP180_EEPROM;

BMP180_EEPROM _bmp180_eeprom;
BMP180_OSS _bmp180_oss;

const uint8_t BMP180_EEPROM_ADDR_MSB[11] = {0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe};
const uint8_t BMP180_EEPROM_ADDR_LSB[11] = {0xab, 0xad, 0xaf, 0xb1, 0xb3, 0xb5, 0xb7, 0xb9, 0xbb, 0xbd, 0xbf};

const uint8_t BMP180_CMD_TEMP = 0x2e;
const uint8_t BMP180_DELAY_TEMP = 5;
const uint8_t BMP180_CMD_PRES[4] = {0x34, 0x74, 0xb4, 0xf4};
const uint8_t BMP180_DELAY_PRES[4] = {5, 8, 14, 26};

I2C_HandleTypeDef hi2c1;

/**
 * @brief Reads from a specific register.
 * @param reg Address of register to read from.
 * @return Byte read.
 */
uint8_t BMP180_ReadReg(uint8_t reg)
{
  HAL_StatusTypeDef op_result = HAL_I2C_Master_Transmit(&hi2c1, BMP180_I2C_ADDR << 1, &reg, 1, BMP180_I2C_TIMEOUT);
  if (op_result != HAL_OK)
  {
    printf("HAL_I2C_Master_Transmit result is NOT OK\n");
  }
  uint8_t result;
  op_result = HAL_I2C_Master_Receive(&hi2c1, BMP180_I2C_ADDR << 1, &result, 1, BMP180_I2C_TIMEOUT);
  if (op_result != HAL_OK)
  {
    printf("HAL_I2C_Master_Receive result is NOT OK\n");
  }
  return result;
}

int32_t BMP180_GetUT(void)
{
  return (BMP180_ReadReg(BMP180_MSB_REG) << 8) | BMP180_ReadReg(BMP180_LSB_REG);
}

int32_t BMP180_GetUP(void)
{
  return ((BMP180_ReadReg(BMP180_MSB_REG) << 16) | (BMP180_ReadReg(BMP180_LSB_REG) << 8) | BMP180_ReadReg(BMP180_XLSB_REG)) >> (8 - _bmp180_oss);
}

void init_i2c()
{
  // hi2c1.Instance = I2C1;
  // // hi2c1.Init.Timing = 0x2000090E;
  // hi2c1.Init.OwnAddress1 = 0;
  // hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  // hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  // hi2c1.Init.OwnAddress2 = 0;
  // // hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  // hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  // hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  __HAL_RCC_I2C1_CLK_ENABLE();

  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    while (1)
    {
    }
  }
}
/**
 * @brief Initializes the BMP180 temperature/pressure sensor.
 * @param hi2c User I2C handle pointer.
 */
HAL_StatusTypeDef BMP180_Init(I2C_HandleTypeDef *hi2c)
{
  init_i2c();
  while (HAL_I2C_IsDeviceReady(&hi2c1, BMP180_I2C_ADDR << 1, 1, HAL_MAX_DELAY) != HAL_OK)
  {
    HAL_Delay(100);
  }

  return HAL_OK;
}

/**
 * @param oss Enum, oversampling setting.
 * @note Available resolutions: BMP180_LOW, BMP180_STANDARD, BMP180_HIGH, BMP180_ULTRA.
 * @note Refer to section 3.3.1 of datasheet.
 */
void BMP180_SetOversampling(BMP180_OSS oss)
{
  _bmp180_oss = oss;
}

/**
 * @brief Updates calibration data.
 * @note Must be called once before main loop.
 */
void BMP180_UpdateCalibrationData(void)
{
  _bmp180_eeprom.BMP180_AC1 = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_AC1]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_AC1]);
  _bmp180_eeprom.BMP180_AC2 = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_AC2]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_AC2]);
  _bmp180_eeprom.BMP180_AC3 = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_AC3]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_AC3]);
  _bmp180_eeprom.BMP180_AC4 = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_AC4]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_AC4]);
  _bmp180_eeprom.BMP180_AC5 = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_AC5]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_AC5]);
  _bmp180_eeprom.BMP180_AC6 = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_AC6]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_AC6]);
  _bmp180_eeprom.BMP180_B1 = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_B1]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_B1]);
  _bmp180_eeprom.BMP180_B2 = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_B2]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_B2]);
  _bmp180_eeprom.BMP180_MB = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_MB]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_MB]);
  _bmp180_eeprom.BMP180_MC = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_MC]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_MC]);
  _bmp180_eeprom.BMP180_MD = (BMP180_ReadReg(BMP180_EEPROM_ADDR_MSB[BMP180_INDEX_MD]) << 8) | BMP180_ReadReg(BMP180_EEPROM_ADDR_LSB[BMP180_INDEX_MD]);
}

/**
 * @brief Writes to a specific register.
 * @param reg Address of register to write to.
 * @param cmd Byte to write.
 */
void BMP180_WriteReg(uint8_t reg, uint8_t cmd)
{
  uint8_t arr[2] = {reg, cmd};
  HAL_I2C_Master_Transmit(&hi2c1, BMP180_I2C_ADDR << 1, arr, 2, BMP180_I2C_TIMEOUT);
}

/**
 * @brief Measures and calculates temperature.
 * @return Temperature in 0.1 (1/10) degrees Celsius.
 */
int32_t BMP180_GetRawTemperature(void)
{
  BMP180_WriteReg(BMP180_CONTROL_REG, BMP180_CMD_TEMP);
  HAL_Delay(BMP180_DELAY_TEMP);
  int32_t ut = (BMP180_ReadReg(BMP180_MSB_REG) << 8) | BMP180_ReadReg(BMP180_LSB_REG);
  int32_t x1 = (ut - _bmp180_eeprom.BMP180_AC6) * _bmp180_eeprom.BMP180_AC5 / (1 << 15);
  int32_t x2 = (_bmp180_eeprom.BMP180_MC * (1 << 11)) / (x1 + _bmp180_eeprom.BMP180_MD);
  int32_t b5 = x1 + x2;
  return (b5 + 8) / (1 << 4);
}

/**
 * @brief Measures and calculates temperature.
 * @return Temperature in degrees Celsius.
 */
float BMP180_GetTemperature(void)
{
  int32_t temp = BMP180_GetRawTemperature();
  return temp / 10.0;
}

/**
 * @brief Measures and calculates pressure.
 * @return Pressure in Pascal(Pa).
 */
int32_t BMP180_GetPressure(void)
{
  BMP180_WriteReg(BMP180_CONTROL_REG, BMP180_CMD_TEMP);
  HAL_Delay(BMP180_DELAY_TEMP);
  int32_t ut = BMP180_GetUT();
  BMP180_WriteReg(BMP180_CONTROL_REG, BMP180_CMD_PRES[_bmp180_oss]);
  HAL_Delay(BMP180_DELAY_PRES[_bmp180_oss]);
  int32_t up = BMP180_GetUP();
  int32_t x1 = (ut - _bmp180_eeprom.BMP180_AC6) * _bmp180_eeprom.BMP180_AC5 / (1 << 15);
  int32_t x2 = (_bmp180_eeprom.BMP180_MC * (1 << 11)) / (x1 + _bmp180_eeprom.BMP180_MD);
  int32_t b5 = x1 + x2;
  int32_t b6 = b5 - 4000;
  x1 = (_bmp180_eeprom.BMP180_B2 * (b6 * b6 / (1 << 12))) / (1 << 11);
  x2 = _bmp180_eeprom.BMP180_AC2 * b6 / (1 << 11);
  int32_t x3 = x1 + x2;
  int32_t b3 = (((_bmp180_eeprom.BMP180_AC1 * 4 + x3) << _bmp180_oss) + 2) / 4;
  x1 = _bmp180_eeprom.BMP180_AC3 * b6 / (1 << 13);
  x2 = (_bmp180_eeprom.BMP180_B1 * (b6 * b6 / (1 << 12))) / (1 << 16);
  x3 = ((x1 + x2) + 2) / 4;
  uint32_t b4 = _bmp180_eeprom.BMP180_AC4 * (uint32_t)(x3 + 32768) / (1 << 15);
  uint32_t b7 = ((uint32_t)up - b3) * (50000 >> _bmp180_oss);
  int32_t p;
  if (b7 < 0x80000000)
    p = (b7 * 2) / b4;
  else
    p = (b7 / b4) * 2;
  x1 = (p / (1 << 8)) * (p / (1 << 8));
  x1 = (x1 * 3038) / (1 << 16);
  x2 = (-7357 * p) / (1 << 16);
  p = p + (x1 + x2 + 3791) / (1 << 4);
  return p;
}

BMP180::BMP180()
{
  BMP180_Init(&hi2c1);
  BMP180_SetOversampling(BMP180_ULTRA);
  BMP180_UpdateCalibrationData();
}

int32_t BMP180::getTemperature()
{
  return BMP180_GetRawTemperature() / 10;
}

uint32_t BMP180::getHumidity()
{
  return 0;
}
