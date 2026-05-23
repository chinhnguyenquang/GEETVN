#pragma once 
#include <PCF8575.h> 
#include "IotVision.h"
#include <vector> 



namespace Pcf8575_custom {


    class Pcf8575_input {
        PCF8575 pcf8575;
        constexpr static int DI_ACTIVE_TIME =50; //ms
        std::vector<int> _registeredPins;
        bool _trangThaiHienTai[16] = {0};
        bool _trangThaiOnDinh[16] = {0};
        unsigned long _thoiGianCuoi[16] = {0};

        int error=0;

        bool error_status=false;

        bool _inverted_logic =false;

        public:
           Pcf8575_input(uint8_t address, std::initializer_list<int> pins,bool inverted_logic = false,TwoWire *wire = &Wire ) 
            : pcf8575(address,wire), _registeredPins(pins), _inverted_logic(inverted_logic) {}

            bool init()
            {
                if (pcf8575.begin())
                {
                    DBG("PCF8575 input initialized successfully.\n");
                    delay(20);
                      for (int i = 0; i < 16; i++) {
                        pcf8575.write(i, 1);   
                    }
                    delay(20);
                    return 1;
                }
                else
                {
                    DBG("Failed to initialize PCF8575 input.\n");
                    return 0;                
                }
            }
            bool Error_input(void) {return error_status;}

            void CapNhatTrangThai(void);
            //1 co tac dong 0 la khong
            bool readPin(int pin) {return _inverted_logic ? !_trangThaiOnDinh[pin] : _trangThaiOnDinh[pin];}

        };
    

    class Pcf8575_output {
        bool _inverted_logic =false;
        PCF8575 pcf8575;
        bool error=false;
        public:
        Pcf8575_output(uint8_t address,bool inverted_logic = false,TwoWire *wire = &Wire) 
        : pcf8575(address,wire), _inverted_logic(inverted_logic) {}

        bool init()
        {
                if (pcf8575.begin())
                {
                    DBG("PCF8575 input initialized successfully.\n");
                    delay(20);
                    for (int i = 0; i < 16; i++) {
                        Set(i,0);
                    }
                    delay(20);
                    return 1;
                }
                else
                {
                    DBG("Failed to initialize PCF8575 input.\n");
                    return 0;                
                }
        }

        void Set(uint8_t pin,bool state)
        {
            bool status{(!_inverted_logic) ? state : (!state)} ;
            pcf8575.write(pin,status);

            if (pcf8575.lastError() != PCF8575_OK) {
                error =true; 
            }
        }

        bool Error_output() {return error;}



    };
}