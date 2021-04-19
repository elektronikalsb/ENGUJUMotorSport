#define cuadrado 2
#define triangulo 9
#define circulo 10
#define X 4

int giro ;
int acelerar ;
int frenar;


#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(7, 8);

uint8_t address[][6] = {"1Node", "2Node"};

bool radioNumber = 1;

bool role = true;
int payload = 0;
byte datos[3];
void setup() {

pinMode(cuadrado,INPUT);
pinMode(triangulo,INPUT);
pinMode(circulo,INPUT);
pinMode(X,INPUT);
Serial.begin(115200);
  while (!Serial) {
  }

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} 
  }
  radioNumber = 1;
 

  radio.setPALevel(RF24_PA_LOW);  
  radio.setPayloadSize(sizeof(payload)); 
  radio.openWritingPipe(address[radioNumber]);     
  radio.openReadingPipe(1, address[!radioNumber]); 
  if (role) {
    radio.stopListening();  
}
}
void loop() {
  
giro = analogRead(A5);//giro
acelerar = analogRead(A0);//acelerar
frenar = analogRead(A3);
int nada = analogRead(A5);//giro

datos[0] = map (acelerar,0,1023,0,255);
datos[1] = map (giro,0,1023,0,255);
datos[2] = map (frenar,0,1023,0,255);
//unsigned long start_timer = micros();                    // start the timer
    bool report = radio.write(&datos, sizeof(datos));      // transmit & save the report
//    unsigned long end_timer = micros();                      // end the timer
   
    Serial.println(datos[0]);
    Serial.println(datos[1]);
    Serial.println(datos[2]);
    
}



  
