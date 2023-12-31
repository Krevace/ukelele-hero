#include "pitches.h" //pitches.h by Mike Putnam

double displayRiffs[18][11] = { 
  { 0, 1, 0, 0, 0, 6, 0, NOTE_F4, 600, 600, 0 },
  { 0, 0, 1, 0, 0, 7, 0, NOTE_G4, 600, 600, 0 },
  { 0, 0, 0, 1, 0, 8, 0, NOTE_GS4, 600, 600, 0 },
  { 0, 0, 0, 0, 1, 9, 0, NOTE_A4, 600, 600, 0 },
  { 0, 0, 0, 1, 0, 10, 0, NOTE_GS4, 600, 600, 0 },
  { 0, 0, 1, 0, 0, 11, 0, NOTE_G4, 600, 600, 0 },
  { 0, 1, 0, 0, 0, 12, 0, NOTE_F4, 600, 600, 0 },
  { 0, 1, 0, 0, 0, 13, 0, NOTE_F4, 600, 600, 0 },
  
  { 1, 0, 0, 0, 0, 14, 0, NOTE_E4, 600, 600, 0 },
  { 0, 1, 0, 0, 0, 15, 0, NOTE_F4, 600, 600, 0 },
  { 0, 0, 1, 0, 0, 16, 0, NOTE_G4, 600, 600, 0 },
  { 1, 0, 0, 0, 0, 17, 0, NOTE_E4, 600, 600, 0 },
  { 0, 1, 0, 0, 0, 17.25, 0, NOTE_F4, 600, 600, 0 },
  { 0, 0, 1, 0, 0, 17.5, 0, NOTE_FS4, 600, 600, 0 },
  { 0, 0, 0, 1, 0, 17.75, 0, NOTE_G4, 600, 600, 0 },
  { 0, 0, 0, 1, 0, 18.25, 0, NOTE_FS4, 600, 600, 0 },
  { 1, 0, 0, 0, 0, 18.75, 0, NOTE_E4, 600, 600, 0 },
  { 0, 1, 0, 0, 0, 19, 21, NOTE_F4, 600, 600, 0 }
};
                          
int bpm = 126;
int milliQuarter = round((60000 / (double) bpm));
int noteAmount = sizeof(displayRiffs) / sizeof(displayRiffs[0]);      
int startNote = 0;       
int buzzPin = 31;
unsigned long currentMillis;
unsigned long startOffset;
unsigned long inputMillis;
byte allButtons;
byte buf[4];

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < noteAmount; i++) {
    if (displayRiffs[i][6]) {
      displayRiffs[i][9] = round((displayRiffs[i][6] * milliQuarter) + 50);
      displayRiffs[i][8] = round((displayRiffs[i][6] * milliQuarter) - 50);
    }
    displayRiffs[i][9] = 0;
    displayRiffs[i][8] = 0;
    displayRiffs[i][6] = round((displayRiffs[i][5] * milliQuarter) + 50);
    displayRiffs[i][5] = round((displayRiffs[i][5] * milliQuarter) - 50);
  }

  bool start = true;
  while (start) {
    if (Serial.available() > 0) {
      Serial.read();
      Serial.write('A');
      start = false;
    }
  }
  
  startOffset = millis();
}

void loop() {
  currentMillis = millis() - startOffset; 
  if (Serial.available() >= 5) {
    allButtons = Serial.read();
    Serial.readBytes(buf, 4); 
    inputMillis = 0;
    for (int i = 0; i < 4; i++) {
      inputMillis += (uint32_t) buf[i] << (i*8); 
    }
    int tempCount = 0;
    for (int i = startNote; i < noteAmount; i++) {
      if (inputMillis > displayRiffs[i][9] && inputMillis > displayRiffs[i][6]) {
        startNote++;
      } else if (inputMillis < displayRiffs[i][5]) {
        i = noteAmount; //this could also mean that this is a failed note
      } else {
        if ((bool) (allButtons & 1) == displayRiffs[i][0] && (bool) (allButtons & 2) == displayRiffs[i][1] && (bool) (allButtons & 4) == displayRiffs[i][2] && (bool) (allButtons & 8) == displayRiffs[i][3] && (bool) (allButtons & 16) == displayRiffs[i][4] && displayRiffs[i][10] != 2) {
          displayRiffs[i][10] = 1;
          tone(buzzPin, displayRiffs[i][7], 40);
          tempCount++;
        } else {
          displayRiffs[i][10] = 2;
          //tone(buzzPin, NOTE_F3, 8);
          tempCount++;
        }
        i = noteAmount;
      }
    }
    if (!tempCount && inputMillis > 100) {
      //tone(buzzPin, NOTE_F3, 8);
    }
  }
  /*
  for (int i = startNote; i < noteAmount; i++) {
    if (currentMillis > displayRiffs[i][9] && currentMillis > displayRiffs[i][6] && displayRiffs[i][10] == 0) {
      startNote++;
      Serial.print("Missed");
      Serial.println(currentMillis);
    } else if (currentMillis < displayRiffs[i][5]) {
      i = noteAmount;
    } 
  }
  */  //Nearly 200 milliseconds ahead, causing inputs to be canceled out
  //Serial.println(inputMillis);
}
