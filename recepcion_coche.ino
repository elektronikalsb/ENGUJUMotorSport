#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <Servo.h>

const int analogOutPin = 5; // Analog output pin
const int analogOutPin2 = 6;
        

int acelerar_map;
int val_map;
int frenar_map;
Servo myservo;


// instantiate an object for the nRF24L01 transceiver
RF24 radio(7, 8); // using pin 7 for the CE pin, and pin 8 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = {"1Node", "2Node"};
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
bool role = false;  // true = TX role, false = RX role

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
float payload = 0.0;
byte datos[2];

void setup() {

  pinMode(6,OUTPUT);

  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  myservo.attach(9);
 
  radioNumber = 0;
  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload)); // float datatype occupies 4 bytes

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1


  radio.startListening(); // put radio in RX mode

  digitalWrite(6,LOW);
myservo.write (83);
  digitalWrite(5,LOW);

  
} // setup

void loop() {


    // This device is a RX node
uint8_t pipe;
if (radio.available()) {             // is there a payload? get the pipe number that recieved it
//      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      radio.read(&datos, sizeof(datos));            // fetch payload from FIFO
      Serial.print("datos[0]");
      Serial.println(datos[0]);
       Serial.print("datos[1]");
      Serial.println(datos[1]);
}
val_map = map (datos[1], 9, 245, 55, 110);
myservo.write (val_map);

 // role
acelerar_map = map (datos[0],50,255,0,255);
analogWrite(analogOutPin, acelerar_map);
//analogWrite(analogOutPin2, datos[1]);

myservo.write (val_map);

} // loop
