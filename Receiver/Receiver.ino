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

   transmitter pins:
   1 ground
   2 3.3V Note (10uF cap across ground and 3.3V)
   3 (CE) 22   MAYBE 17
   4 (CSN) 21  MAYBE 5
   5 (SCK) 18
   6 (MOSI) 23
   7 (MISO) 19
 */


int Throttle = 0;
int Roll = 128;
int Pitch = 128;
int Yaw = 128;

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
    data.throttle = 0;
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
       now = millis();

       delay(50);
     */
}

void setup(){

    Serial.begin(115200);

    delay(1000);

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
}
