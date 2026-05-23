#pragma once 
#include "Arduino.h"
#include "Wire.h"

#include "IotVision.h"
#include "Audio.h"
#include "Pcf8575_custom.h"
#include "Para_inout.h"

#include "Error_handled.h"

#include <esp_task_wdt.h>

#define PCF_ADDR_INPUT 0x20
#define PCF_ADDR_OUT 0x21


void Init_main();