/* Stereo peak meter example, assumes Audio adapter but just uses terminal so no more parts required.

This example code is in the public domain

*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

const int myInput = AUDIO_INPUT_LINEIN;
// const int myInput = AUDIO_INPUT_MIC;

AudioInputI2S        audioInput;         // audio shield: mic or line-in
AudioAnalyzePeak     peak_L;
AudioAnalyzePeak     peak_R;
AudioOutputI2S       audioOutput;        // audio shield: headphones & line-out

AudioConnection c1(audioInput,0,peak_L,0);
AudioConnection c2(audioInput,1,peak_R,0);
AudioConnection c3(audioInput,0,audioOutput,0);
AudioConnection c4(audioInput,1,audioOutput,1);

AudioControlSGTL5000 audioShield;

#define RPWM 20
#define REN 1

#define MAX_VOLUME_IN 40
#define MAX_POWER_OUT 255
#define MIN_POWER_OUT 100
#define TIME_BETWEEN_READS 200


void setup() {
  AudioMemory(6);
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);
  Serial.begin(9600);
  pinMode(RPWM,OUTPUT);
  pinMode(REN,OUTPUT);
  digitalWrite(REN,HIGH);
}

elapsedMillis fps;
uint8_t cnt=0;
uint8_t left = 0;
uint8_t right = 0;
uint8_t out;

void loop() {
  delay(TIME_BETWEEN_READS);
  if(fps > 24) {
    if (peak_L.available() && peak_R.available()) {
      fps=0;
      left=0;
      right=0;
      uint8_t leftPeak=peak_L.read() * 30.0;
      uint8_t rightPeak=peak_R.read() * 30.0;
      while(cnt++<30) {
        left++;
      }
      for(cnt=0;cnt<rightPeak;cnt++) {
        right++;
      }
      if (left+right>MAXVOLUMEIN){left=MAXVOLUMEIN;right=0;}
      out=map(right+left,0,MAX_VOLUME_IN,MIN_POWER_OUT,MAX_POWER_OUT);
      if (MIN_POWER_OUT==100){out=0;}
      analogWrite(RPWM,out);
      Serial.println("");
      Serial.print(out1);
    }
  }
}
