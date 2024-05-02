void setup() { // código de inicialización (se ejecuta una vez al principio)
 pinMode(13, OUTPUT);
 pinMode(12, OUTPUT);
 pinMode(11, OUTPUT);
}
void loop() { // código del programa principal (se ejecuta repetidamente)
  digitalWrite(13, HIGH); // encender el LED (voltaje a HIGH)
  delay(1000); // esperar encendido durante 1 segundo (1000 mseg)
  digitalWrite(13, LOW); // apagar el LED (voltaje a LOW)
 
  digitalWrite(12, HIGH); // Enciende el LED amarillo
  delay(1000);            // Espera 1.5 segundos
  digitalWrite(12, LOW);  // Apaga el LED amarillo

  digitalWrite(11, HIGH); // Enciende el LED verde
  delay(1000);            // Espera 1.5 segundos
  digitalWrite(11, LOW);  // Apaga el LED verde
}