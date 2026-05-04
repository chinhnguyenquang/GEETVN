#include "main.h"


Pcf8575_custom::Pcf8575_input _Input_pcf(PCF_ADDR_INPUT,{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15});
Pcf8575_custom::Pcf8575_output _Output_pcf(PCF_ADDR_OUT,true);
void Task_UpdatePCF(void* pvParameter);

#pragma region Declare_var
bool Error_check=false;
#pragma endregion Declare_var


#pragma region Debug_only
#if DEBUG 
void Task_Debug(void* pvParameter) {
    bool status{false};
    for (;;) {
        // Hiển thị các chân đang có tín hiệu (mức 1 sau khi qua logic đảo)
        Serial.print("Input Active: [ ");
        bool hasActive = false;
        
        for (int i = 0; i < 16; i++) {
            if (_Input_pcf.readPin(i)) {
                Serial.print(i);
                Serial.print(" ");
                hasActive = true;
                
                

            }
            _Output_pcf.Set(i,status);
        }
        status= !status;
        if (!hasActive) Serial.print("NONE");
        Serial.println(" ]");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Cập nhật log mỗi 0.5s để tránh tràn Serial
    }
}
#endif 

#pragma endregion Debug_only



void Init(){
    #if DEBUG
    Serial.begin(115200); delay(10);
    #endif


    Wire.setTimeOut(100);
    Wire.begin(21,22,100000);

    _Input_pcf.init();
    _Output_pcf.init();
    xTaskCreatePinnedToCore(
            Task_UpdatePCF,
            "Task_UpdatePCF",
            4000,
            NULL,
            2,  // Ưu tiên cao hơn task debug
            NULL,
            1); // Chạy trên Core 1

        #if DEBUG
        xTaskCreatePinnedToCore(
        Task_Debug,    /* Task function. */
        "Task Debug", /* name of task. */
        2000,                               /* Stack size of task */
        NULL,                                /* parameter of the task */
        1,                                   /* priority of the task */
        NULL,                               /* Task handle to keep track of created task */
        1);  
        #endif
}




void Task_UpdatePCF(void* pvParameter) {
    for (;;) {
        _Input_pcf.CapNhatTrangThai();

        Error_check = _Input_pcf.Error_input();
        Error_check |= _Output_pcf.Error_output();

        if (Error_check){
            DBG("HELLO WORLD");
        } 
        

        // Tốc độ quét nên nhanh hơn DI_ACTIVE_TIME (50ms)
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}


