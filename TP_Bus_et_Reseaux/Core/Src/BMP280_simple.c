/*
 * BMP280.c
 *
 *  Created on: Oct 3, 2020
 *      Author: cbares
 */

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

extern I2C_HandleTypeDef hi2c1;

uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;
uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;

BMP280_S32_t t_fine;
uint32_t final_P;
float final_T;

/*This function enables to check whether we can communicate well with the sensor
 * as well as whether we can retrieve its address.
 */
int BMP280_check() {
	uint8_t buf[1];
	HAL_StatusTypeDef ret;
	buf[0] = BMP280_ID_REG;

	ret = HAL_I2C_Master_Transmit(&hi2c1, BMP280_ADDR, buf, 1, HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with check (I2C Transmit)\r\n");
	}

	ret = HAL_I2C_Master_Receive(&hi2c1, BMP280_ADDR, buf, BMP280_ID_LEN,
	HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with check (I2C Receive) \r\n");
	}

	printf("Id: 0x%x...", buf[0]);
	if (buf[0] == BMP280_ID_VAL) {
		printf("Ok\r\n");
		return 0;
	} else {
		printf("not Ok!\r\n");
		return 1;
	}
}

/* This function enables to retrieve the trimming registers, which will help use to compensate the pressure and the pressure.
 * (Please see functions bmp280_compensate_P_int64() and bmp280_compensate_T_int32()).
 */

int BMP280_get_trimming() {
	uint8_t *buf;
	buf = BMP280_Read_Reg(BMP280_TRIM_REG_MSB, BMP280_TRIM_LEN);
	dig_T1 = (buf[0] | buf[1]<<8);
	dig_T2 = (buf[2] | buf[3]<<8);
	dig_T3 = (buf[4] | buf[5]<<8);
	dig_P1 = (buf[6] | buf[7]<<8);
	dig_P2 = (buf[8] | buf[9]<<8);
	dig_P3 = (buf[10] | buf[11]<<8);
	dig_P4 = (buf[12] | buf[13]<<8);
	dig_P5 = (buf[14] | buf[15]<<8);
	dig_P6 = (buf[16] | buf[17]<<8);
	dig_P7 = (buf[18] | buf[19]<<8);
	dig_P8 = (buf[20] | buf[21]<<8);
	dig_P9 = (buf[22] | buf[23]<<8);
	free(buf);
	return 0;
}

/* This function enables to set temperature resolution at 17 bit/0.0025 °C, pressure resolution
 * at 20 bit/0.16 Pa and to set the power mode to normal
 */
int BMP280_init() {
	HAL_StatusTypeDef ret;
	uint8_t ctrl = (0b010 << 5) | (0b101 << 2) | (0b11);
	/* 				osr_t x2       osr_p x16       normal mode   */

	printf("Configure...\r\n");
	ret = BMP280_Write_Reg(BMP280_CTRL_MEAS_REG, ctrl);
	if (ret == 0) {
		printf("Config Ok\r\n");
	} else {
		printf("Config not Ok!\r\n");
		return 1;
	}
	BMP280_get_trimming();
	return 0;
}

/* This function is a general one used to modify a register of the BMP280 memory.
 */

int BMP280_Write_Reg(uint8_t reg, uint8_t value) {
	uint8_t buf[3];
	HAL_StatusTypeDef ret;

	buf[0] = reg;
	buf[1] = value;
	ret = HAL_I2C_Master_Transmit(&hi2c1, BMP280_ADDR, buf, 2, HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with I2C Transmit\r\n");
	}

	ret = HAL_I2C_Master_Receive(&hi2c1, BMP280_ADDR, buf, 1, HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with I2C Receive\r\n");
	}

	if (buf[0] == value) {
		return 0;
	} else {
		return 1;
	}
}

uint8_t* BMP280_Read_Reg(uint8_t reg, uint8_t length) {
	uint8_t *buf;
	HAL_StatusTypeDef ret;

	ret = HAL_I2C_Master_Transmit(&hi2c1, BMP280_ADDR, &reg, 1, HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with I2C Transmit\r\n");
	}

	buf = (uint8_t*) malloc(length);
	ret = HAL_I2C_Master_Receive(&hi2c1, BMP280_ADDR, buf, length,
	HAL_MAX_DELAY);
	if (ret != 0) {
		printf("Problem with I2C Receive\r\n");
	}

	return buf;
}

/* This function enables to retrieve the non compensated temperature.
 */

BMP280_S32_t BMP280_get_temperature() {
	uint8_t *buf;
	BMP280_S32_t adc_T;

	buf = BMP280_Read_Reg(BMP280_TEMP_REG_MSB, BMP280_TEMP_LEN);

	adc_T = ((BMP280_S32_t) (buf[0]) << 12) | ((BMP280_S32_t) (buf[1]) << 4)
			| ((BMP280_S32_t) (buf[2]) >> 4);

	free(buf);

//	printf("Non compensated temperature: ");
//	printf("0X%05lX", adc_T);
//	printf("\r\n");

	return adc_T;
}
/* This function enables to retrieve the non compensated pressure.
 */

BMP280_S32_t BMP280_get_pressure() {
	uint8_t *buf;
	BMP280_S32_t adc_P;

	buf = BMP280_Read_Reg(BMP280_PRES_REG_MSB, BMP280_PRES_LEN);

	adc_P = ((BMP280_S32_t) (buf[0]) << 12) | ((BMP280_S32_t) (buf[1]) << 4)
			| ((BMP280_S32_t) (buf[2]) >> 4);

	free(buf);

//	printf("Pressure:    0x");
//	printf("%05lX", adc_P);
//	printf("\r\n");

	return adc_P;
}

// This function returns temperature with 0.01 deg_C resolution.
float bmp280_compensate_T_int32() {
	BMP280_S32_t adc_T = BMP280_get_temperature();
	BMP280_S32_t var1, var2, T;
	var1 = ((((adc_T >> 3) - ((BMP280_S32_t) dig_T1 << 1)))
			* ((BMP280_S32_t) dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((BMP280_S32_t) dig_T1))
			* ((adc_T >> 4) - ((BMP280_S32_t) dig_T1))) >> 12)
			* ((BMP280_S32_t) dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	final_T = 0.01*T;
	printf("T=+%.2f_C\r\n",final_T);
	return final_T;
}

// This function returns pressure in Pa as unsigned 32 bit integer.
uint32_t bmp280_compensate_P_int64() {
	BMP280_S32_t adc_P = BMP280_get_pressure();
	BMP280_S64_t var1, var2, p;
	var1 = ((BMP280_S64_t) t_fine) - 128000;
	var2 = var1 * var1 * (BMP280_S64_t) dig_P6;
	var2 = var2 + ((var1 * (BMP280_S64_t) dig_P5) << 17);
	var2 = var2 + (((BMP280_S64_t) dig_P4) << 35);
	var1 = ((var1 * var1 * (BMP280_S64_t) dig_P3) >> 8)
			+ ((var1 * (BMP280_S64_t) dig_P2) << 12);
	var1 = (((((BMP280_S64_t) 1) << 47) + var1)) * ((BMP280_S64_t) dig_P1)
			>> 33;
	if (var1 == 0) {
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((BMP280_S64_t) dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((BMP280_S64_t) dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((BMP280_S64_t) dig_P7) << 4);
	final_P = (uint32_t) p/256;
	printf("P=%ldPa\r\n",final_P);
	return final_P;
}
