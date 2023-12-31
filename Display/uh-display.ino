#include <MCUFRIEND_kbv.h> //MCU_FRIEND_kbv by David Prentice
#include <Adafruit_GFX.h> //Adafruit_GFX by Adafruit
#include "pitches.h" //pitches.h by Mike Putnam

#define LCD_RESET A4 
#define LCD_CS A3   
#define LCD_CD A2  
#define LCD_WR A1  
#define LCD_RD A0 

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFD20

MCUFRIEND_kbv tft;

double displayRiffs[18][10] = { 
  { 0, 1, 0, 0, 0, 6, 0, NOTE_F4, 600, 600 },
  { 0, 0, 1, 0, 0, 7, 0, NOTE_G4, 600, 600 },
  { 0, 0, 0, 1, 0, 8, 0, NOTE_GS4, 600, 600 },
  { 0, 0, 0, 0, 1, 9, 0, NOTE_A4, 600, 600 },
  { 0, 0, 0, 1, 0, 10, 0, NOTE_GS4, 600, 600 },
  { 0, 0, 1, 0, 0, 11, 0, NOTE_G4, 600, 600 },
  { 0, 1, 0, 0, 0, 12, 0, NOTE_F4, 600, 600 },
  { 0, 1, 0, 0, 0, 13, 0, NOTE_F4, 600, 600 },

  { 1, 0, 0, 0, 0, 14, 0, NOTE_F4, 600, 600 },
  { 0, 1, 0, 0, 0, 15, 0, NOTE_F4, 600, 600 },
  { 0, 0, 1, 0, 0, 16, 0, NOTE_F4, 600, 600 },
  { 1, 0, 0, 0, 0, 17, 0, NOTE_F4, 600, 600 },
  { 0, 1, 0, 0, 0, 17.25, 0, NOTE_F4, 600, 600 },
  { 0, 0, 1, 0, 0, 17.5, 0, NOTE_F4, 600, 600 },
  { 0, 0, 0, 1, 0, 17.75, 0, NOTE_F4, 600, 600 },
  { 0, 0, 0, 1, 0, 18.25, 0, NOTE_F4, 600, 600 },
  { 1, 0, 0, 0, 0, 18.75, 0, NOTE_F4, 600, 600 },
  { 0, 1, 0, 0, 0, 19, 21, NOTE_F4, 600, 600 }
};

int bpm = 126;
int milliQuarter = round((60000 / (double) bpm));
int startNote = 0;
int noteAmount = sizeof(displayRiffs) / sizeof(displayRiffs[0]);          
int notePos;
int notePos2;
unsigned long currentMillis;
unsigned long startOffset;
double milliPixel = (double) milliQuarter / 80;

void setup() {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9486);
  tft.fillScreen(BLACK);

  tft.drawLine(0, 80, 320, 80, WHITE);
  tft.drawLine(0, 160, 320, 160, WHITE);
  tft.drawLine(0, 240, 320, 240, WHITE);
  tft.drawLine(0, 320, 320, 320, WHITE);
  tft.drawLine(0, 400, 320, 400, MAGENTA);
  
  tft.fillCircle(53, 80, 8, BLACK);
  tft.fillCircle(53, 160, 8, BLACK); 
  tft.fillCircle(53, 240, 8, BLACK);
  tft.fillCircle(53, 320, 8, BLACK);

  tft.fillCircle(106, 80, 8, BLACK);
  tft.fillCircle(106, 160, 8, BLACK);
  tft.fillCircle(106, 240, 8, BLACK);
  tft.fillCircle(106, 320, 8, BLACK);

  tft.fillCircle(159, 80, 8, BLACK);
  tft.fillCircle(159, 160, 8, BLACK);
  tft.fillCircle(159, 240, 8, BLACK);
  tft.fillCircle(159, 320, 8, BLACK);

  tft.fillCircle(213, 80, 8, BLACK);
  tft.fillCircle(213, 160, 8, BLACK);
  tft.fillCircle(213, 240, 8, BLACK);
  tft.fillCircle(213, 320, 8, BLACK);

  tft.fillCircle(266, 80, 8, BLACK);
  tft.fillCircle(266, 160, 8, BLACK);
  tft.fillCircle(266, 240, 8, BLACK);
  tft.fillCircle(266, 320, 8, BLACK);

  tft.drawLine(53, 0, 53, 480, WHITE);
  tft.drawLine(106, 0, 106, 480, WHITE);
  tft.drawLine(159, 0, 159, 480, WHITE);
  tft.drawLine(213, 0, 213, 480, WHITE);
  tft.drawLine(266, 0, 266, 480, WHITE);

  tft.fillCircle(53, 400, 10, GREEN);
  tft.fillCircle(106, 400, 10, RED);
  tft.fillCircle(159, 400, 10, YELLOW);
  tft.fillCircle(213, 400, 10, BLUE);
  tft.fillCircle(266, 400, 10, ORANGE);

  tft.fillCircle(53, 400, 8, BLACK);
  tft.fillCircle(106, 400, 8, BLACK);
  tft.fillCircle(159, 400, 8, BLACK);
  tft.fillCircle(213, 400, 8, BLACK);
  tft.fillCircle(266, 400, 8, BLACK);

  bool start = true;
  while (start) {
    if (Serial.available() > 0) {
      Serial.read();
      start = false;
    }
  }
  
  startOffset = millis();
}

void replacePrevious(int x, int i) {
  tft.fillCircle(x, displayRiffs[i][8], 8, BLACK); 
  tft.drawLine(x, displayRiffs[i][8] - 8, x, displayRiffs[i][8] + 8, WHITE);
}

void replacePreviousLine(int x, int i) {
  tft.fillRect(x - 3, displayRiffs[i][9] - 6, 7, displayRiffs[i][8] - displayRiffs[i][9] + 6, BLACK);
  tft.drawLine(x, displayRiffs[i][9] - 6, x, displayRiffs[i][8], WHITE);
}

void createLine(int x, int pos1, int pos2, uint16_t color) {
  tft.fillCircle(x, pos1, 8, color);
  tft.fillRect(x - 3, pos2, 7, pos1 - pos2, color); 
  tft.fillCircle(x, pos2, 6, color);
  tft.fillRect(x - 6, pos2 - 6, 3, 12, BLACK);
  tft.fillRect(x + 4, pos2 - 6, 3, 12, BLACK);
}

void replaceRiffDots (int x, uint16_t color) {
  tft.fillCircle(x, 400, 10, color); 
  tft.fillCircle(x, 400, 8, BLACK);
}

void loop() {
  currentMillis = millis() - startOffset;
  for (int i = startNote; i < noteAmount; i++) { 
    notePos = round(((400 - 80 * (double) displayRiffs[i][5]) + (currentMillis / (double) milliPixel)));
    if (displayRiffs[i][6]) {
      notePos2 = round(((400 - 80 * (double) displayRiffs[i][6]) + (currentMillis / (double) milliPixel))); 
      if (notePos < -10) {
        i = noteAmount;
      } else if (notePos2 >= 400) {
        startNote++;
        if (displayRiffs[i][0]) { replaceRiffDots(53, GREEN); }
        if (displayRiffs[i][1]) { replaceRiffDots(106, RED); }
        if (displayRiffs[i][2]) { replaceRiffDots(159, YELLOW); }
        if (displayRiffs[i][3]) { replaceRiffDots(213, BLUE); }
        if (displayRiffs[i][4]) { replaceRiffDots(266, ORANGE); }
      } else if (notePos > 400 && notePos2 < 400) {
        displayRiffs[i][8] = 400;
        if (displayRiffs[i][0]) { replacePreviousLine(53, i); createLine(53, 400, notePos2, GREEN); tft.fillCircle(53, 400, 10, GREEN); }
        if (displayRiffs[i][1]) { replacePreviousLine(106, i); createLine(106, 400, notePos2, RED); tft.fillCircle(106, 400, 10, RED); }
        if (displayRiffs[i][2]) { replacePreviousLine(159, i); createLine(159, 400, notePos2, YELLOW); tft.fillCircle(159, 400, 10, YELLOW); }
        if (displayRiffs[i][3]) { replacePreviousLine(213, i); createLine(213, 400, notePos2, BLUE); tft.fillCircle(213, 400, 10, BLUE); }
        if (displayRiffs[i][4]) { replacePreviousLine(266, i); createLine(266, 400, notePos2, ORANGE); tft.fillCircle(266, 400, 10, ORANGE); }
        displayRiffs[i][9] = notePos2;
      } else {
        if (displayRiffs[i][0]) { replacePreviousLine(53, i); replacePrevious(53, i); createLine(53, notePos, notePos2, GREEN); }
        if (displayRiffs[i][1]) { replacePreviousLine(106, i); replacePrevious(106, i); createLine(106, notePos, notePos2, RED); }
        if (displayRiffs[i][2]) { replacePreviousLine(159, i); replacePrevious(159, i); createLine(159, notePos, notePos2, YELLOW); }
        if (displayRiffs[i][3]) { replacePreviousLine(213, i); replacePrevious(213, i); createLine(213, notePos, notePos2, BLUE); }
        if (displayRiffs[i][4]) { replacePreviousLine(266, i); replacePrevious(266, i); createLine(266, notePos, notePos2, ORANGE); }
        displayRiffs[i][8] = notePos;
        displayRiffs[i][9] = notePos2;
      }
    } else {
      if (notePos < -10) {
        i = noteAmount;
      } else if (notePos > 400) {
        startNote++;
        if (displayRiffs[i][0]) { replacePrevious(53, i); replaceRiffDots(53, GREEN); }
        if (displayRiffs[i][1]) { replacePrevious(106, i); replaceRiffDots(106, RED); }
        if (displayRiffs[i][2]) { replacePrevious(159, i); replaceRiffDots(159, YELLOW); }
        if (displayRiffs[i][3]) { replacePrevious(213, i); replaceRiffDots(213, BLUE); }
        if (displayRiffs[i][4]) { replacePrevious(266, i); replaceRiffDots(266, ORANGE); }
      } else {
        if (displayRiffs[i][0]) { replacePrevious(53, i); tft.fillCircle(53, notePos, 8, GREEN); }
        if (displayRiffs[i][1]) { replacePrevious(106, i); tft.fillCircle(106, notePos, 8, RED); }
        if (displayRiffs[i][2]) { replacePrevious(159, i); tft.fillCircle(159, notePos, 8, YELLOW); }
        if (displayRiffs[i][3]) { replacePrevious(213, i); tft.fillCircle(213, notePos, 8, BLUE); }
        if (displayRiffs[i][4]) { replacePrevious(266, i); tft.fillCircle(266, notePos, 8, ORANGE); }
        displayRiffs[i][8] = notePos;
      }
    }
  }
}
