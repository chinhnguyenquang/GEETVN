#pragma once 


#define DEBUG 1

#if DEBUG
  #define DBG(...) do { Serial.printf(__VA_ARGS__); } while(0)
#else
  #define DBG(...) do {} while(0)
#endif
