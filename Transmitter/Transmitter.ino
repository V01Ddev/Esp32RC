#include <SPI.h>
#include "WiFi.h"
#include <esp_now.h>


/*
   left stick:
   X-axis = D35 (throttle)
   Y-axis = D34 (yaw)

   right stick:
   X-axis = D33 (pitch)
   Y-axis = D32 (roll)

*/

#define LeftXPIN 35
#define LeftYPIN 34
int Yaw = 0;
int Throttle = 0;


#define RightXPIN 33
#define RightYPIN 32
int Roll = 0;
int Pitch = 0;

#define LED 18

uint8_t broadcastAddress[] =  {0xB0, 0xB2, 0x1C, 0x0B, 0x01, 0x94};
esp_now_peer_info_t peerInfo;

// Setting up what the signal would look like
struct Signal {
    byte throttle;
    byte pitch;
    byte roll;
    byte yaw;
};

Signal data;


void DataReset(){
    data.throttle = 128;
    data.pitch = 128;
    data.roll = 128;
    data.yaw = 128;
}



void setup(){
    Serial.begin(115200);

    pinMode(RightXPIN, INPUT);
    pinMode(RightYPIN, INPUT);

    pinMode(LeftXPIN, INPUT);
    pinMode(LeftYPIN, INPUT);

    pinMode(LED, OUTPUT);

    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the send callback

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0; 
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

    delay(3000);

    digitalWrite(LED, HIGH);
    Serial.println("Setup complete...");
}



void loop(){
    // map({pin}, {min_value}, {max_value}, 0, 255)

    Throttle = map(analogRead(LeftXPIN), 0, 4095, 0, 255);
    Yaw = map(analogRead(LeftYPIN), 0, 4095, 0, 255);

    Roll = map(analogRead(RightYPIN), 0, 4095, 0, 255);
    Pitch = map(analogRead(RightXPIN), 0, 4095, 0, 255);

    data.throttle = Throttle;
    data.pitch = Pitch;
    data.roll = Roll;
    data.yaw = Yaw;


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

    esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(Signal));

    delay(50);
}

