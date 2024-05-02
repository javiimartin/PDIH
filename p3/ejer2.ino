void setup() { // código de inicialización (se ejecuta una vez al principio)
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(7, INPUT_PULLUP); // Configura el pin 7 como entrada (interruptor)
}

void loop() {
  if (digitalRead(7) == LOW) { // Verifica si el interruptor está presionado 
    digitalWrite(11, HIGH); // Enciende el LED rojo
    digitalWrite(12, LOW);  // Apaga el LED amarillo
    digitalWrite(13, LOW);  // Apaga el LED verde
  } else {
    digitalWrite(11, LOW);  
    // Los LEDs amarillo y verde parpadean alternativamente cuando el interruptor no está presionado
    digitalWrite(12, HIGH); 
    delay(1000);            
    digitalWrite(12, LOW);  
    
    digitalWrite(13, HIGH); 
    delay(1000);            
    digitalWrite(13, LOW);  
  }
}