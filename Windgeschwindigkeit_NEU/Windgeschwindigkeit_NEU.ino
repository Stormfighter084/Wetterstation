double revolutions = 0;
double windSpeed = 0;
void IRAM_ATTR fonction_ISR() {
    revolutions++;  
    ets_printf("Button pressed \n");
}

void setup() {
    Serial.begin(115200);
    pinMode(18, INPUT_PULLUP);  
}

void loop() {
    attachInterrupt(18, fonction_ISR, FALLING);
    delay(60000);
    detachInterrupt(18);
    windSpeed = revolutions * 0.18;
    Serial.println(windSpeed);
    revolutions = 0;
}