// Title: Analog data in/out
// Author: Kang Taewook
// 
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  // sensorValue = analogRead(analogInPin);
  sensorValue++;
  if(sensorValue > 500)
    sensorValue = 0;
  
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 500, 0, 255);
  analogWrite(analogOutPin, outputValue);

  // print the results to the Serial Monitor:
  Serial.write(outputValue);
  //delay(50);
  //Serial.write(500);
  delay(50);
}
