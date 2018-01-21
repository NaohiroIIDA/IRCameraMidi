
#include "Arduino.h"
#include "Wire.h"
#include "MIDIUSB.h"


// @brief Library for DFRobot's IR Position Camera
// @author [Angelo](Angelo.qiao@dfrobot.com)

#include "DFRobotIRPosition.h"

DFRobotIRPosition myDFRobotIRPosition;

int positionX[4];     ///< Store the X position
int positionY[4];     ///< Store the Y position

int now_ch;
int old_ch;

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void setup()
{
  Serial.begin(19200);
  myDFRobotIRPosition.begin();

  now_ch = 0;
  old_ch = 0;
}

void loop()
{
  myDFRobotIRPosition.requestPosition();

  if (myDFRobotIRPosition.available()) {
  
    for (int i = 0; i < 4; i++) {
      positionX[i] = myDFRobotIRPosition.readX(i);
      positionY[i] = myDFRobotIRPosition.readY(i);
    }

    now_ch = map(positionX[0], 0, 650, 35, 85);
    
    Serial.println(now_ch);
    
    if (positionX[0] == 1023) {
      now_ch = 0;
    }

    if (now_ch != old_ch) {
      Serial.println(now_ch);

      noteOn(0,  now_ch, 127);
      MidiUSB.flush();
      noteOff(0,  old_ch, 127);
      MidiUSB.flush();

      old_ch = now_ch;
    }

    }
    else {
      Serial.println("Device not available!");
    }

  }




