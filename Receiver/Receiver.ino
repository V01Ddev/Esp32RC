#include <SPI.h>
#include "WiFi.h"
#include <esp_now.h>
#include <ESP32Servo.h>


/*
   throttle = D5
   pitch = D18
   roll = D19
   yaw = D21
 */


int Throttle = 80;
int Roll = 83;
int Pitch = 80;
int Yaw = 83;


Servo Tservo; // Throttle
Servo Pservo; // Pitch
Servo Rservo; // Roll
Servo Yservo; // Yaw

int TPos = 0; // Throttle PWM output
int PPos = 0; // Pitch PWM output
int RPos = 0; // Roll PWM output
int YPos = 0; // Yaw PWM output


unsigned long now;
unsigned long LastRec;
unsigned long NxtRcv;

// Setting up what the signal would look like
struct Signal {
    byte throttle;
    byte pitch;
    byte roll;
    byte yaw;
};

Signal data;


// To run if signal is lost
void DataReset(){
    data.throttle = 90;
    data.pitch = 90;
    data.roll = 90;
    data.yaw = 90;
}



void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

    memcpy(&data, incomingData, sizeof(Signal));

    Throttle = data.throttle;
    Pitch = data.pitch;
    Roll = data.roll;
    Yaw = data.yaw;

    LastRec = millis() / 1000;
    NxtRcv = LastRec + 3;

    Serial.print("Throttle: ");
    Serial.println(data.throttle);

    Serial.print("Yaw: ");
    Serial.println(data.yaw);

    Serial.print("Pitch: ");
    Serial.println(data.pitch);

    Serial.print("Roll: ");
    Serial.println(data.roll);

    now = millis();
}

void setup(){

    Serial.begin(115200);

    delay(500);

    Tservo.attach(5);
    Pservo.attach(18);
    Rservo.attach(19);
    Yservo.attach(21);

    DataReset();

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW...");
        return;
    }

    // Does things on callback (when data is sent over)
    esp_now_register_recv_cb(OnDataRecv);
}


int lastRecv = 0;

void loop(){
    now = millis() / 1000;
    if (now > NxtRcv){
        DataReset();
        Serial.println("Connection lost, data has been reset...");
        delay(500);
    }

    TPos = data.throttle;  // pin D5 (PWM signal)
    PPos = data.pitch;     // pin D18 (PWM signal)
    RPos = data.roll;      // pin D19 (PWM signal)
    YPos = data.yaw;       // pin D21 (PWM signal)

    Tservo.write(TPos);
    Pservo.write(PPos);
    Rservo.write(RPos);
    Yservo.write(YPos);
}
