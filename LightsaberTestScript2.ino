
#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"

// Choose any two pins that can be used with SoftwareSerial to RX & TX
#define SFX_TX 5
#define SFX_RX 6

// Connect to the RST pin on the Sound Board
#define SFX_RST 4

// You can also monitor the ACT pin for when audio is playing!

// we'll be using software serial
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

// pass the software serial to Adafruit_soundboard, the second
// argument is the debug port (not used really) and the third
// arg is the reset pin
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);
// can also try hardware serial with
// Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);



// these constants describe the pins. They won't change:
const int xPin = A1;                  // x-axis of the accelerometer
const int yPin = A2;                  // y-axis
const int zPin = A3;                  // z-axis (only on 3-axis models)
const int redPin = 7;
const int greenPin = 8;
const int bluePin = 9;
const int ACTPin = 10;

/************** Motion Thresholds **************/
/*       Thresholds of Swings and Clashes      */

int swingThreshold = 50;
int clashThreshold = 75;

int currentTotal;
int previousTotal;

int currentDistance;
int previousDistance;

int deltaDistance;
int deltaTotal;

int deltaX;
int deltaY;
int deltaZ;

int xRaw;
int yRaw;
int zRaw;

int previousX;
int previousY;
int previousZ;

/***************** Sound Lists *****************/
/*              Sound Effect Names             */

char on[] = "T00     WAV";
char hum[] = "T01LATCHWAV";
char off[] = "T02     WAV";
char* swingList[] = {"T03RAND0WAV", "T03RAND1WAV", "T03RAND2WAV", "T03RAND3WAV", "T03RAND4WAV", "T03RAND5WAV", "T03RAND6WAV", "T03RAND7WAV", "T03RAND8WAV", "T03RAND9WAV"};
char lock[] = "T04HOLDl";
char* clashList[] = {"T05RAND0WAV", "T05RAND1WAV", "T05RAND2WAV", "T05RAND3WAV", "T05RAND4WAV", "T05RAND5WAV", "T05RAND6WAV", "T05RAND7WAV", "T05RAND8WAV"};
char blasterBlock1[] = "T06     WAV";
char blasterBlock2[] = "T07     WAV";

void setup() {
  // initialize the serial communications:
  Serial.begin(9600);
  // softwareserial at 9600 baud
  ss.begin(9600);

  Serial.println("Rishi's Lightsaber Test Script");
  if (!sfx.reset()) {
    Serial.println("Not found");
    while (1);
  }
  Serial.println("SFX board found");

  xRaw = analogRead(xPin);
  yRaw = analogRead(yPin);
  zRaw = analogRead(zPin);

  currentTotal = xRaw + yRaw + zRaw;
  previousTotal = currentTotal;
  previousX = xRaw;
  previousY = yRaw;
  previousZ = zRaw;
  pinMode(ACTPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}

void loop() {
  xRaw = analogRead(xPin);
  yRaw = analogRead(yPin);
  zRaw = analogRead(zPin);

  currentDistance = sqrt(pow(previousX - xRaw, 2) + pow(previousY - yRaw, 2) + pow(previousZ - zRaw, 2));
  currentTotal = xRaw + yRaw + zRaw;
 
  deltaTotal = currentTotal - previousTotal;
  deltaDistance = currentDistance - previousDistance;
  
  deltaX = abs(xRaw - previousX);
  deltaY = abs(yRaw - previousY);
  deltaZ = abs(zRaw - previousZ);
 
  if (deltaX >= swingThreshold || deltaY >= swingThreshold || deltaX >= swingThreshold) {
    previousX = xRaw;
    previousY = yRaw;
    previousZ = zRaw;
    Serial.print("***** SWING *****");
    Serial.println();
    sfx.stop();
    sfx.playTrack(swingList[random(0, 8)]);
  }

   
  if (deltaX >= clashThreshold || deltaY >= clashThreshold || deltaX >= clashThreshold) {
    previousX = xRaw;
    previousY = yRaw;
    previousZ = zRaw;
    Serial.print("***** CLASH *****");
    Serial.println();
    sfx.stop();
    sfx.playTrack(clashList[random(0, 8)]);
   }

  
  if (deltaX < swingThreshold && deltaY < swingThreshold && deltaX < swingThreshold) {
    previousX = xRaw;
    previousY = yRaw;
    previousZ = zRaw;
    if (digitalRead(ACTPin) == HIGH) {
      sfx.playTrack(hum);
    }
  }
/*
  if (deltaX > swingThreshold || deltaY > swingThreshold || deltaX > swingThreshold) {
    previousTotal = currentTotal;
    Serial.print("***** SWING *****");
    Serial.println();
    
    
  }
  

  
 
  }
  */

  // print the sensor values:
  Serial.print(deltaX);
  Serial.print("\t");
  Serial.print(deltaY);
  Serial.print("\t");
  Serial.print(deltaZ);
  Serial.print("\t");
  Serial.println();
  // delay before next reading:
  // delay(100);

}


