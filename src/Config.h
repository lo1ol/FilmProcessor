#pragma once

#include "MyAssert.h"

#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

#define DISPLAY_ROWS 4
#define DISPLAY_COLS 16

#define MODE_SWITCH_BTN A0
#define BACK_BTN A1

#define ENCODER_DT 6
#define ENCODER_CLK 7

#define BUZZER 9

#define SERVO 10

#define PUMP_FOR A14
#define PUMP_REV A15

#define DEV_VALVE A2
#define BLEACH_VALVE A3
#define FIX_VALVE A4
#define EXTRA_VALVE A7
#define WATER_VALVE A9
#define WASTE_VALVE A13

#define CHEM_LOAD_ML_SPEED 17L
#define PURE_WASH_TIME 15000L
#define PRE_CLEAN_TUBES_TIME 3000L
#define POST_CLEAN_TUBES_TIME 2000L

#define CLEAN_BATH_TIME 10000L
#define FREE_BATH_TIME 15000L
#define CLEAN_PUMP_TIME 3000L
