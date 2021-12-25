/*NMG Nerfgun Mündungsfeuer Geschwindigkeitsmesser von Mini Revollo 01.11.2017
 * Projekt erstellt im www.erfindergarden.de in München
 * Kontakt: mini@revollo.de   https://github.com//minirevollo
 * Mit Hilfe von Google SketchUp wurden zwei Halter designt die die beiden Messpunkte auf einem
 * Kabelkanahl mit 32 mm Durchmesser in einem Abstand (strecke) aufgeschoben werden. Eine Platine
 * wurde mit einem Arduino Micro, einem Buzzer und einem 8-Segment Display versehen.
 * Nach der Stromversorgung des Arduino ist die Messvorrichtung sofort bereit und kann alle 2 Sekunden 
 * (delay) wiederholt werden.
 */
#include <TM1637Display.h>

const int CLK = 8; //Set the CLK pin connection to the display
const int DIO = 7; //Set the DIO pin connection to the display
TM1637Display display(CLK, DIO);  //set up the 4-Digit Display
const uint8_t SEG_GG[] = {
  SEG_G, // -
  SEG_G, // -
  SEG_G, // -
  SEG_G, // -
  }; 
int messpunkt_1 = 18;  //Pin für die erste Messstelle
int messpunkt_2 = 19;   //Pin für die zweite Messstelle
float geschwindigkeit = 0;   //Ausgabe der 
long startzeit = 0;    //Zeitstempel für die erste Messung
long endzeit = 0;      //Zeitstempel für die zweite Messung
float flugdauer = 0;   //Zeit des Geschossen von Punkt eins zu Punkt zwei
float strecke = 15;    //in cm
int led = 13;
int buzzer = 9;
int sens = 50;   //sensibilität
int senssetup_1 = 0;  //Anfangsmessung zum justieren 
int senssetup_2 = 0;   //Anfangsmessung zum justieren
int sens_1 = 0;       //Triggerpunkt abzüglich der sensibilität
int sens_2 = 0;       //Triggerpunkt abzüglich der sensibilität
boolean aktiviert = false;    //Messung wurde gestartet

void setup() {
  display.setBrightness(0x0a);  //set the diplay to maximum brightness
  display.showNumberDec(strecke,false,4,0); //Geschwindigkeit anzeigen(Wert,führende nullen,ziffern, pos)
  pinMode(messpunkt_1, INPUT);
  pinMode(messpunkt_2, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
 /*while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
   }
 */
  senssetup_1 = analogRead(messpunkt_1);    //setzt den aktuellen Messpunktwert plus der sensibilität als Trigger
  senssetup_2 = analogRead(messpunkt_2);
  sens_1 = senssetup_1 - sens;
  sens_2 = senssetup_2 - sens;
    Serial.print("setup_1:  ");
    Serial.println(senssetup_1);
    Serial.print("setup_2:  ");
    Serial.println(senssetup_2);
    Serial.print("Sensibilität:  ");
    Serial.println(sens);
    Serial.print("Strecke:  ");
    Serial.println(strecke/100);
    
}

void loop() {
  while(analogRead(messpunkt_1) <= sens_1 && aktiviert == false){    //Messung beginnt
   startzeit = millis();
   aktiviert = true;                                //Messung wird als aktiv gesetzt, damit das Projektil auf seiner gesamten Länge nicht mehrfach gemessen wird
   digitalWrite(led, HIGH);
   
   
   }  
  if(analogRead(messpunkt_2) <= sens_2 && aktiviert == true){
   endzeit = millis();                               //Messung endet
   flugdauer = (endzeit - startzeit)+1;
   geschwindigkeit = (strecke/100)/(flugdauer/1000);// in  m/s
   aktiviert = false;                     //Messung wieder deaktiviert, bereit für die nächste Messung
   Serial.print(startzeit);
   Serial.print(", ");
   Serial.println(endzeit);
   Serial.print("Flugdauer: ");
   Serial.print(flugdauer);
   Serial.print(" ms,  Geschwindigkeit: ");
   Serial.print(geschwindigkeit);
   Serial.println("  m/s");
   digitalWrite(led, LOW);
   display.setSegments(SEG_GG,4,0);
   display.showNumberDec(geschwindigkeit,false,4,0); //Geschwindigkeit anzeigen(Wert,führende nullen,ziffern, pos)
  
   for(int i = 0; i <= 1; i++){   // Sound abspielen
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(300);
   }
   
  }
  
 
  
}
