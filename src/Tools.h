#pragma once

#include "App.h"
#include "Display.h"
#include "MelodyPlayer.h"
#include "Memory.h"
#include "Pump.h"
#include "Rotator.h"
#include "SafeEncButton.h"

#define ADD_TO_ENUM(enumName, current, num) \
    (enumName)(((uint8_t)(current) + (uint8_t)enumName::last_ + num) % (uint8_t)enumName::last_)

#define ARRAY_SIZE(array) sizeof(array) / sizeof((array)[0])

int8_t getEncoderDir();
int8_t getEncoderShift();
bool getTime(uint16_t& time);

void setupEncoder();

void formatTime(uint16_t, char* dst);
uint16_t unformatTime(const char* time);

extern App gApp;
extern EncButton gEncoder;
extern ButtonT<MODE_SWITCH_BTN> gModeSwitchBtn;
extern ButtonT<BACK_BTN> gBackBtn;
extern Display gDisplay;
extern Memory gMemory;
extern Rotator gRotator;
extern Pump gPump;
extern MelodyPlayer gMelodyPlayer;
