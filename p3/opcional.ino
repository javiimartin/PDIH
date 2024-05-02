// Define los pines a los que están conectados los LEDs
int ledPins[] = {10, 11, 12, 13};
int numLeds = 4;

void setup() {
  // Configura cada pin como salida
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // Enciende los LEDs de izquierda a derecha
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100); // Ajusta el tiempo de encendido
    digitalWrite(ledPins[i], LOW);
  }
  
  // Enciende los LEDs de derecha a izquierda
  for (int i = numLeds - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(100); // Ajusta el tiempo de encendido
    digitalWrite(ledPins[i], LOW);
  }
}
