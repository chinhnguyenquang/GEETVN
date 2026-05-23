#include "main.h"

#pragma region init_class 
Pcf8575_custom::Pcf8575_input _Input_pcf(PCF_ADDR_INPUT,{12,13,14,15});
Pcf8575_custom::Pcf8575_output _Output_pcf(PCF_ADDR_OUT,true);
Audio _Audio(26,25,33);
int Maybom=0;
#pragma endregion init_class

#pragma region declare_task
void Task_UpdatePCF_MP3(void* pvParameter);
void Task_Event(void* pvParameter);
#pragma endregion declare_task


#pragma region declare_variable

bool Error_check=false;
DoorState _currentState;

#pragma endregion declare_variable



const char* DoorStateToString(DoorState state)
{
    switch (state)
    {
    case DoorState::CLOSED_K:
        return "CLOSED_K";

    case DoorState::CLOSED_C:
        return "CLOSED_C";

    case DoorState::CLOSING_K:
        return "CLOSING_K";

    case DoorState::CLOSING_C:
        return "CLOSING_C";

    case DoorState::OPENING_IN:
        return "OPENING_IN";

    case DoorState::OPENING_OUT:
        return "OPENING_OUT";

    case DoorState::OPENNED_IN:
        return "OPENNED_IN";

    case DoorState::OPENNED_OUT:
        return "OPENNED_OUT";

    default:
        return "UNKNOWN";
    }
}


void TaskDebug(void* pvParameter) {

    for (;;) {
        DBG("State: %s\n", DoorStateToString(_currentState));

       
        vTaskDelay(pdMS_TO_TICKS(2000)); 
    }
}


void Open_is(int status) 
{
    /*
    status = 1  mo cua 
    status =2 dong cua
    status =0 dung lai
    */
    if(status == 1)
    {
        _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_MOTOR),0);
        delay(50);
        _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_MOTOR_INVERTED),1);
    }
    else if (status ==2)
    {
        _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_MOTOR_INVERTED),0);
        delay(50);
        _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_MOTOR),1);
    }

    else 
    {
        _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_MOTOR_INVERTED),0);
        _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_MOTOR),0);
    }
}

    // //công tắc 2 trạng thái (limit switch / door switch) TRANG THAI DONG CUA
    // SWITCH_1_STATE = 0,

    // // cảm biến quét tay không chạm (IR / ToF / radar)
    // HAND_SCAN = 1,

    // // công tắc 2 trạng thái (limit switch / door switch) TRANG THAI MO CUA
    // SWITCH_2_STATE = 2,

    // // cảm biến quang (photoelectric beam / diffuse / curtain) cam bien chong ket 
    // PHOTO_SENSOR1 = 3,
    // // cảm biến quang (photoelectric beam / diffuse / curtain) cam bien chong ket 
    // PHOTO_SENSOR2 = 4,
    // //Cam cảm biến ra đa (radar sensor)
    // RADAR_SENSOR = 5


void KhoiTao_TrangThaidau(){
    if (_Input_pcf.readPin(static_cast<int>(SensorType::SWITCH_1_STATE)) == 0){
        _currentState = DoorState::CLOSING_K;
        Open_is(2);
    }
    else
    {
        Open_is(0);
        _currentState = DoorState::CLOSED_K; 
    }
}



void Init_main() {
    Serial.begin(115200); delay(500);
    DBG("boot oke ;;;;;;;;;;;;;;;;;;;;;;;;");


    Wire.setTimeOut(100);
    Wire.begin(21,22,100000);
    _Input_pcf.init();
    _Output_pcf.init();

    _Audio.begin();
    Open_is(0);   // Tắt motor ngay từ đầu
     // Cập nhật input vài lần trước khi xác định trạng thái đầu
    for (int i = 0; i < 10; i++) {
        _Input_pcf.CapNhatTrangThai();
        delay(20);
    }

    KhoiTao_TrangThaidau();
    #pragma region create_task
    xTaskCreatePinnedToCore(
            Task_UpdatePCF_MP3,
            "Task_Update",
            4000,
            NULL,
            8,  // Ưu tiên cao hơn task debug
            NULL,
            1); // Chạy trên Core 1
    xTaskCreatePinnedToCore(
            Task_Event,
            "Task_xuly_su_kien",
            4000,
            NULL,
            7,  // Ưu tiên cao hơn task debug
            NULL,
            1); // Chạy trên Core 1

    
    xTaskCreatePinnedToCore(
            TaskDebug,
            "Taskdebug",
            5000,
            NULL,
            4,
            NULL,
            0);
    
    
    #pragma endregion create_task
}


    /*
    status = 1  mo cua 
    status =2 dong cua
    status =0 dung lai
    */


void Task_Event(void* pvParameter) {
    static bool status_dongcua=false;
    uint32_t openedAt = 0;
    uint32_t timeMaybom=0;
    static bool status_maybom=false;
    for (;;) {
        
        switch (_currentState)
        {
        case DoorState::CLOSED_K:
            /* code */
            if(_Input_pcf.readPin(static_cast<int>(SensorType::HAND_SENSOR_1))==1)
            {
                Open_is(1);
                _Output_pcf.Set(static_cast<int>(ActuatorType::LIGHT), 1);
                _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_2_CHANNEL),1);
                _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_1_CHANNEL),1);
                _currentState = DoorState::OPENNED_IN;
                _Audio.play();
                DBG("hello world");
                status_maybom = false;
                _Output_pcf.Set(static_cast<int>(ActuatorType::Maybom), 0);
            }

            if(_Input_pcf.readPin(static_cast<int>(SensorType::HAND_SENSOR_2))==1)
            {
                Open_is(1);
                _Output_pcf.Set(static_cast<int>(ActuatorType::LIGHT), 0);
                _currentState = DoorState::OPENNED_OUT;
            }

            if(Maybom >= 2)
            {
                Maybom =0;
                _Output_pcf.Set(static_cast<int>(ActuatorType::Maybom), 1);
                status_maybom = true;
                timeMaybom=millis();
            }

            if(status_maybom)
            {
                if(millis() - timeMaybom >= 5000)
                {
                    _Output_pcf.Set(static_cast<int>(ActuatorType::Maybom), 0);
                    status_maybom = false;
                }
            }
            break;
        
        case DoorState::CLOSED_C:
            if(_Input_pcf.readPin(static_cast<int>(SensorType::HAND_SENSOR_2))==1)
            {
                Open_is(1);
                _Output_pcf.Set(static_cast<int>(ActuatorType::LIGHT), 0);
                _currentState = DoorState::OPENNED_OUT;
            }
            break;

        case DoorState::CLOSING_K:
            if(_Input_pcf.readPin(static_cast<int>(SensorType::SWITCH_1_STATE)))
            {
                Open_is(0);
                // Them hit cua
                _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_2_CHANNEL),0);
                _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_1_CHANNEL),0);
                _Audio.stop();
                _currentState = DoorState::CLOSED_K;
            }
            break;
        case DoorState::CLOSING_C:
            if(_Input_pcf.readPin(static_cast<int>(SensorType::SWITCH_1_STATE)))
            {
                Open_is(0);
                // Them hit cua
                _currentState = DoorState::CLOSED_C;
            }
            break;
        case DoorState::OPENNED_IN:
            if(_Input_pcf.readPin(static_cast<int>(SensorType::SWITCH_2_STATE)))
            {
                Open_is(0);
                //_currentState = DoorState::OPENNED;
                if(!status_dongcua)
                {
                    openedAt = millis();
                    status_dongcua = true;
                }
                if(millis() - openedAt >= 15000)
                {
                    Open_is(2);
                    status_dongcua =false;
                    _currentState=DoorState::CLOSING_K;
                    _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_2_CHANNEL),0);
                    _Output_pcf.Set(static_cast<int>(ActuatorType::RELAY_1_CHANNEL),0);
                    _Audio.stop();

                }
            }

            if(_Input_pcf.readPin(static_cast<int>(SensorType::HAND_SENSOR_2)))
            {
               Open_is(2);
                ++Maybom;
               status_dongcua =false;
               _currentState = DoorState::CLOSING_C; 
            }
            break;
        case DoorState::OPENNED_OUT:
            //Dieu chinh them radar
            if(_Input_pcf.readPin(static_cast<int>(SensorType::SWITCH_2_STATE)))
            {
                Open_is(0);
                if(!status_dongcua)
                {
                    openedAt = millis();
                    status_dongcua = true;
                }
                if(millis() - openedAt >= 7500)
                {
                    Open_is(2);
                    status_dongcua =false;
                    _currentState=DoorState::CLOSING_K;
                }
            }
            
            break;

        default:
            break;
        }
        

        
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}


void Task_UpdatePCF_MP3(void* pvParameter) {
   

        
    for (;;) {
        _Input_pcf.CapNhatTrangThai();
        Error_check = _Input_pcf.Error_input();
        Error_check |= _Output_pcf.Error_output();
        if (Error_check){
            DBG("HELLO WORLD");
            //recoverI2C();
            //esp_restart();
        } 
        _Audio.update();
        
        vTaskDelay(pdMS_TO_TICKS(5)); 
    }
}