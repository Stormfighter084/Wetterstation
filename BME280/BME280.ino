#include <Seeed_BME280.h> // Implementieren der Seeed Bibliothek, ermöglicht die Kommunikation mit dem BME
#include <Wire.h>  // Implementieren der Wire Bibliothek, ermöglicht dem Arduino mit Geräten zu kommunizieren, welche das I²C Protokoll verwenden
 
BME280 bme280;

void setup ()
{
   Serial.begin(9600); // Einstellen der Baudrate
   if(!bme280.init()) // Wenn keine Daten vom BME abgefragt werden können...
   
        {
          Serial.println("FEHLER!"); // ...dann soll eine Fehlermeldung ausgegeben werden.
        }
}

void loop() // Starten unserer Schleife

{
  float Druck; // Deklarieren der Variablen
  float Pascal;
  double Bar;
  
          Serial.print("Temperatur : ");   
          Serial.print(bme280.getTemperature()); // Abfrage und Ausgabe der Temperatur
          Serial.println("C");
          Serial.print("Druck : ");
          
  Pascal = bme280.getPressure();  // Abfrage des Drucks in Pascal
  
          Serial.print(Pascal);  // Ausgabe des Drucks in Pascal
          Serial.print(" Pascal oder auch ");
          
  Bar = (Pascal/100000); // Wandlung des Pascal-Werts in Bar
  
          Serial.print(Bar); // Ausgabe des Drucks in Bar
          Serial.println(" Bar");
          
          Serial.print("Feuchtigkeit : "); 
          Serial.print(bme280.getHumidity()); // Abfrage und Ausgabe der Feuchtigkeit
          Serial.println("%");
          Serial.println(" ");
          
  delay(1000); 
  
}
