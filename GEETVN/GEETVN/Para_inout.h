#pragma once 
#include "Arduino.h"



enum class SensorType : uint8_t {
    //công tắc 2 trạng thái (limit switch / door switch) TRANG THAI DONG CUA
    SWITCH_1_STATE = 15,

    // cảm biến quét tay không chạm (IR / ToF / radar) BEN NGOAI
    //HAND_SCAN1 = 13,
    //BEN TRONG
    //HAND_SCAN2 =12,

    // công tắc 2 trạng thái (limit switch / door switch) TRANG THAI MO CUA
    SWITCH_2_STATE = 14,

    // cảm biến quang (photoelectric beam / diffuse / curtain) cam bien chong ket 
    PHOTO_SENSOR1 = 3,
    // cảm biến quang (photoelectric beam / diffuse / curtain) cam bien chong ket 
    PHOTO_SENSOR2 = 4,
    //Cam cảm biến ra đa (radar sensor)
    RADAR_SENSOR = 5,
    //Cam bien khong cham vao 
    HAND_SENSOR_1 =12,
    //Cam bien khong cham ra
    HAND_SENSOR_2 =13,
    //Cam bien quang
    CBQ1=10,
    CBQ2=11,
    ////Cambien mui
    CBM=9

};

enum class ActuatorType : int {
    // rơle 1 kênh (1 relay channel) ĐIỀU KHIỂN MỞ CỬA
    RELAY_MOTOR = 6,

    RELAY_MOTOR_INVERTED = 7,

    // rơle 2 kênh (2 relay channel) ĐIỀU KHIỂN MỞ CỬA VÀ ĐÈN
    RELAY_2_CHANNEL = 2,

    RELAY_1_CHANNEL = 3,


    // đèn cảnh báo (warning light)
    WARNING_LIGHT = 3,

    LIGHT =5,

    Maybom =4,
    
};



enum class DoorState : uint8_t {
    CLOSED_K = 0,        // cửa đã đóng hoàn toàn khong co nguoi ben trong
    CLOSED_C = 1,        // cửa đã đóng hoàn toàn có người bên trong
    CLOSING_C =   2,         // đang đóng co nguoi ben trong 
    CLOSING_K =8,           //dang dong khong co nguoi ben trong
    OPENNED_IN = 3,        // cửa đã mở hoàn toàn
    OPENNED_OUT =10,       //
    OPENING_IN = 4,       // đang mở de di vao 
    OPENING_OUT =9,
    STOPPED = 5,       // dừng giữa chừng
    BLOCKED = 6,       // gặp vật cản / chống kẹt
    ERROR = 7,          // lỗi hệ thống
    UNKNOW=12

};