#pragma once

#include "App.h"
#include "Display.h"
#include "Memory.h"
#include "SafeEncButton.h"

#define ADD_TO_ENUM(enumName, current, num) \
    (enumName)(((uint8_t)(current) + (uint8_t)enumName::last_ + num) % (uint8_t)enumName::last_)

#define ARRAY_SIZE(array) sizeof(array) / sizeof((array)[0])

int8_t getEncoderDir();
int8_t getEncoderShift();
bool getTime(uint16_t& time);

void setupEncoder();

String formatTime(uint16_t);
uint16_t unformatTime(String time);

extern App gApp;
extern EncButton gEncoder;
extern ButtonT<MODE_SWITCH_BTN> gModeSwitchBtn;
extern ButtonT<BACK_BTN> gBackBtn;
extern Display gDisplay;
extern Memory gMemory;
