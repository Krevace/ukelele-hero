const int thumbPin = A1;
int thumbPinState;
unsigned long startOffset;
unsigned long currentMillis;
byte buf[4];

void setup() {
  Serial.begin(9600);
  DDRA = B00000000;

  bool start = true;
  while (start) {
    if (PINA || analogRead(thumbPin) < 150 || analogRead(thumbPin) > 850) {
      Serial.write('A');
      start = false;
    }
  }
  
  startOffset = millis();
}

void loop() {
  currentMillis = millis() - startOffset; 
  thumbPinState = analogRead(thumbPin);
  if (thumbPinState < 150 || thumbPinState > 850) {
    Serial.write(PINA);
    buf[0] = currentMillis & 255; 
    buf[1] = (currentMillis >> 8) & 255;
    buf[2] = (currentMillis >> 16) & 255;
    buf[3] = (currentMillis >> 24) & 255;
    Serial.write(buf, 4);
  } 
  delay(20);
}
