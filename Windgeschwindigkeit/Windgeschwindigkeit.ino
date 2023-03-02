float revolutions = 0;
float windSpeed = 0;
bool test = 0;

void setup() {
  Serial.begin(115200);
  pinMode(18, INPUT_PULLUP);
  //pinMode(3, OUTPUT);
}
 
void IRAM_ATTR ISR() {
    revolutions++;
    //ets_printf(".");
}

void loop() {
  attachInterrupt(digitalPinToInterrupt(18), ISR, RISING);
  delay(60000);
  detachInterrupt(18);
  windSpeed = revolutions * 0.18;

  Serial.println("");
  Serial.print("Velocity: ");
 Serial.println(windSpeed);
 
  revolutions = 0;
}

