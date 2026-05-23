#include "Pcf8575_custom.h"

namespace Pcf8575_custom {
    void Pcf8575_input::CapNhatTrangThai(void) {
        unsigned long thoiGianHienTai = millis();

        uint16_t digitalRaw = pcf8575.read16();
        
        if (pcf8575.lastError() != PCF8575_OK) {
            error < 10? ++ error : (error_status=true);
            return; 
        }
        else {error =0; error_status=false;}

        for (int pin : _registeredPins) {

            bool raw = (digitalRaw >> pin) & 0x01;

            if (raw != _trangThaiHienTai[pin]) {
                _trangThaiHienTai[pin] = raw;
                _thoiGianCuoi[pin] = thoiGianHienTai;
            }

            if ((thoiGianHienTai - _thoiGianCuoi[pin]) >= DI_ACTIVE_TIME) {
                _trangThaiOnDinh[pin] =  raw;
            }
        }

        }
    
}