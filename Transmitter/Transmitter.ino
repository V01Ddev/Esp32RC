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

#define ThrottlePIN 35
#define YawPIN 34
int Throttle = 0;
int Yaw = 0;


#define PitchPIN 33
#define RollPIN 32
int Pitch = 0;
int Roll = 0;

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


void setup(){
    Serial.begin(115200);

    pinMode(PitchPIN, INPUT);
    pinMode(RollPIN, INPUT);

    pinMode(ThrottlePIN, INPUT);
    pinMode(YawPIN, INPUT);

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

    delay(1000);

    digitalWrite(LED, HIGH);
    Serial.println("Setup complete...");
}



void loop(){
	// map({pin}, {min_value}, {max_value}, 0, 255)

	Throttle = analogRead(ThrottlePIN);
	Throttle = map(Throttle, 0, 4095, 0, 180);

	Roll = analogRead(RollPIN);
	Roll = map(Roll, 0, 4095, 0, 180);

	Pitch = analogRead(PitchPIN);
	Pitch = map(Pitch, 0, 4095, 0, 180);

	Yaw = analogRead(YawPIN);
	Yaw = map(Yaw, 0, 4095, 0, 180);


	// Reversing the pitch
	// Pitch = 255 - Pitch;

	//constrain(Pitch, 45, 135);

	Serial.print("Pitch: ");
	Serial.println(Pitch);

	Serial.print("Roll: ");
	Serial.println(Roll);

	Serial.print("Yaw: ");
	Serial.println(Yaw);

	Serial.print("Throttle: ");
	Serial.println(Throttle);

	data.throttle = Throttle;
	data.pitch = Pitch;
	data.roll = Roll;
	data.yaw = Yaw;

	esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(Signal));

	delay(50);
}
