#pragma once

#include "gainput/gainput.h"

struct HIDDeviceInfo;
struct HIDController;

bool HIDIsSupportedPS4Controller(HIDDeviceInfo* devInfo);
int HIDOpenPS4Controller(HIDDeviceInfo* devInfo, HIDController* controller, uint8_t playerNum);
