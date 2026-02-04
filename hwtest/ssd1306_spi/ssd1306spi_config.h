
#pragma once

#include "main.h"

#define SSD1306_HEIGHT (64U)

#define cs_set()    do{HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, GPIO_PIN_RESET);}while(0)   //CS_притягиваем к земле
#define cs_reset()  do{HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, GPIO_PIN_SET);}while(0)     //CS притягиваем к 3.3v
#define RST_set()   do{HAL_GPIO_WritePin(DISP_RES_GPIO_Port, DISP_RES_Pin, GPIO_PIN_RESET);}while(0) //RST притягиваем к земле
#define RST_reset() do{HAL_GPIO_WritePin(DISP_RES_GPIO_Port, DISP_RES_Pin, GPIO_PIN_SET);}while(0)   //RST притягиваем к 3.3v
#define DC_set()    do{HAL_GPIO_WritePin(DISP_DC_GPIO_Port, DISP_DC_Pin, GPIO_PIN_RESET);}while(0)    //DC притягиваем к земле
#define DC_reset()  do{HAL_GPIO_WritePin(DISP_DC_GPIO_Port, DISP_DC_Pin, GPIO_PIN_SET);}while(0)      //DC притягиваем к 3.3v

#define DELAY_MS(x) do{HAL_Delay(x);}while (0)

#define SPI_HANDLE (hspi1)
extern SPI_HandleTypeDef SPI_HANDLE; //Используемая шина spi

