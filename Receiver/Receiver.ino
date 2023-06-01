#include <SPI.h>
#include "WiFi.h"
#include <esp_now.h>
#include <Servo.h>


/*
   throttle = D5
   pitch = D18
   roll = D19
   yaw = D21
 */


int Throttle = 128;
int Roll = 128;
int Pitch = 128;
int Yaw = 128;


Servo Tservo;
Servo Pservo;
Servo Rservo;
Servo Yservo;

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
    data.throttle = 128;
    data.pitch = 128;
    data.roll = 128;
    data.yaw = 128;
}



void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

    memcpy(&data, incomingData, sizeof(Signal));

    Throttle = data.throttle;
    Pitch = data.pitch;
    Roll = data.roll;
    Yaw = data.yaw;

    LastRec = millis() / 1000;
    NxtRcv = LastRec + 3;
/*
    Serial.print("Throttle: ");
    Serial.println(data.throttle);

    Serial.print("Yaw: ");
    Serial.println(data.yaw);

    Serial.print("Pitch: ");
    Serial.println(data.pitch);

    Serial.print("Roll: ");
    Serial.println(data.roll);
*/

    now = millis();
}

void setup(){

    Serial.begin(115200);

    delay(1000);

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

    Serial.println("Setup complete...");
}


int lastRecv = 0;

void loop(){
    now = millis() / 1000;
    if (now > NxtRcv){
        DataReset();
        Serial.println("Connection lost, data has been reset...");
        delay(500);
    }

    TPos = map(data.throttle, 0, 255, 0, 180);  // pin D4 (PWM signal)
    PPos = map(data.pitch, 0, 255, 0, 180);     // pin D3 (PWM signal)
    RPos = map(data.roll, 0, 255, 0, 180);      // pin D2 (PWM signal)
    YPos = map(data.yaw, 0, 255, 0, 180);       // pin D5 (PWM signal)

    Serial.println(RPos);

    Tservo.write(TPos);
    Pservo.write(PPos);
    Rservo.write(RPos);
    Yservo.write(YPos);
}
