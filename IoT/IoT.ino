const int lightSensorPin = A1;
int lightSensorValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  lightSensorValue = analogRead(lightSensorPin);

  Serial.println(lightSensorValue);
}
