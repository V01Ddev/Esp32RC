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
3 (CE) 22   MAYBE 17
4 (CSN) 21  MAYBE 5
5 (SCK) 18
6 (MOSI) 23
7 (MISO) 19
*/


int Yaw = 128;
int Throttle = 128;
int Roll = 128;
int Pitch = 128;


#define CEpin 17
#define CSNpin 5
#define SCKpin 18
#define MOSIpin 23
#define MISOpin 19

const uint64_t pipeIn = 2006;
RF24 radio(CEpin, CSNpin);

// Setting up what the signal would look like
struct Signal {
    byte throttle;
    byte pitch;
    byte roll;
    byte yaw;
};


/*

Signal data;
// To run if signal is lost
void DataReset(){
    data.throttle = 128;
    data.pitch = 128;
    data.roll = 128;
    data.yaw = 128;
}

*/


void setup(){

    Serial.begin(115200);
    
    delay(1000);

    // Radio config
    radio.begin();
    radio.openReadingPipe(1, pipeIn);
    radio.startListening();

    // DataReset();

    Serial.println("Radio started...");
}


int lastRecv = 0;

void loop(){

    /*
    int now = millis();

    if (now - lastRecv > 1000){
        DataReset();
    }

    if (radio.available()){
        radio.read(&data, sizeof(data));
        lastRecv = millis();
    }

    Throttle = data.throttle;
    Pitch = data.pitch;
    Roll = data.roll;
    Yaw = data.yaw;

    Serial.println(data.throttle);

    Serial.print("Throttle: ");
    Serial.println(Throttle);

    Serial.print("Pitch: ");
    Serial.println(Pitch);

    Serial.print("Roll: ");
    Serial.println(Roll);

    Serial.print("Yaw: ");
    Serial.println(Yaw);
    */

    char txt[32] = {0};
    
    Serial.println(txt);

    while(radio.available()){
        radio.read(&txt, sizeof(txt));
        Serial.println(txt);
        delay(100);
    }

    if(radio.available() != true){
        Serial.println("no fucking signal");
        delay(100);
    }
}
