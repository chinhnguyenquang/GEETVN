#pragma once 
#include <Pcf8575_custom.h>
#include "IotVision.h"
#include "Wire.h"


#define PCF_ADDR_INPUT 0x20
#define PCF_ADDR_OUT 0x21
#define _SDA 21
#define _SCL 22

void Init();