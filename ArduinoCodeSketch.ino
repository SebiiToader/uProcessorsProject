#include <SoftwareSerial.h>

// Pins
#define LED 10
#define BT_TX_PIN 11
#define BT_RX_PIN 12
#define anPin 0

// Variabile
float celsius = 0;     // Temperatura in grade C
float fahrenheit = 0;     // Temperatura in grade F
float voltage;      // Voltage variable
int sensor;       // Valoarea senzorului [0-1023]
float sumC = 0.0;
float sumF = 0.0;
int frames = 0;
int countC = 0;
int countF = 0;

// Bluetooth
byte input = 0;     // Trimite valoarea prin bluetooth
float outputC = 0;
float outputF = 0;
SoftwareSerial bluetoothModule =  SoftwareSerial(BT_RX_PIN, BT_TX_PIN);

void setup() {
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);
  pinMode(LED, OUTPUT);

  bluetoothModule.begin(9600);
}

void loop() {
  
  // Primeste valoarea prin bluetooth
  while (bluetoothModule.available() > 0) {
    input = bluetoothModule.read();
  }
  pinPower(input);    // Seteaza LED-ul la ultima valoare primita   

  // Temperature in celsius
  if(frames%1000 == 0) {    // Every 1000 frames get the value and sum
    sensor = analogRead(anPin);
    voltage = (sensor*5000.0)/1024;   // 1023:5000=sensor:x
    celsius = voltage/10;
    
    sumC += celsius;
    countC++;
    outputC=sumC/countC;
    sendDataC(outputC);
   
  }
  
  
//Temperature in Fahrenheit
   if(frames%1000 == 0) {     // La fiecare 1000 frames ia valorile si face calculul.
    sensor = analogRead(anPin);
    voltage = (sensor*5000.0)/1023;   // 1023:5000=sensor:x
    celsius = voltage/10;
    fahrenheit = (celsius*1.8) + 32;
    
    sumF += fahrenheit;
    countF++;
    outputF=sumF/countF;
    sendDataF(outputF);
  }

  delay(100);       //Intarzie aparitia valorii cu 100 frames (aproximativ 1 secunda)
}
 
void pinPower(int power) {
  analogWrite(LED, power);
}

//Trimite temperatura catre APLICATIE la aparitia caracterelor specifice
void sendDataC(float value) {
  String string = String(value);  
  bluetoothModule.print(' ' + string + '#');   // Cand apare caracterul #, aplicatia trimite temperatura in grade C
}

void sendDataF(float value) {
  String string = String(value);  
  bluetoothModule.print(' ' + string + '$');  // Cand apare caracterul $, aplicatia trimite temperatura in grade F
}
