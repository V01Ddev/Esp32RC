#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>



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
3 (CE) 22
4 (CSN) 21
5 (SCK) 18
6 (MOSI) 23
7 (MISO) 19
*/


#define LeftXPIN 35
#define LeftYPIN 34
int Yaw = 0;
int Throttle = 0;


#define RightXPIN 33
#define RightYPIN 32
int Roll = 0;
int Pitch = 0;

const uint64_t pipeOut = 0xE9E8F0F0E1LL;
RF24 radio(22, 21);

// Setting up what the signal would look like
struct Signal {
    byte throttle;
    byte pitch;
    byte roll;
    byte yaw;
};


// To run if signal is lost
Signal data;
void DataReset(){
    data.throttle = 128;
    data.pitch = 128;
    data.roll = 128;
    data.yaw = 128;
}



void setup(){
    Serial.begin(115200);

    radio.begin();
    radio.setAutoAck(false);
    radio.setDataRate(RF24_250KBPS);
    radio.openWritingPipe(pipeOut);

    DataReset();
}



void loop(){
    // map({pin}, {min_value}, {max_value}, 0, 255)

    Throttle = map(analogRead(LeftXPIN), 0, 4095, 0, 255);
    Yaw = map(analogRead(LeftYPIN), 0, 4095, 0, 255);

    Roll = map(analogRead(RightYPIN), 0, 4095, 0, 255);
    Pitch = map(analogRead(RightXPIN), 0, 4095, 0, 255);

    Serial.print("Throttle: ");
    Serial.println(Throttle);

    Serial.print("Yaw: ");
    Serial.println(Yaw);

    Serial.print("Pitch: ");
    Serial.println(Pitch);

    Serial.print("Roll: ");
    Serial.println(Roll);

    data.throttle = Throttle;
    data.pitch = Pitch;
    data.roll = Roll;
    data.yaw = Yaw;

    radio.write(&data, sizeof(Signal));
}
