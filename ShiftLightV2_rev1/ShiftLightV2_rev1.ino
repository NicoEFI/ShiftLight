// ok le 23/04/2017 Ronan

/*   pixels.setPixelColor(0, pixels.Color(255,0,0)) ; //vert
   pixels.setPixelColor(1, pixels.Color(0,255,0)) ; //rouge
   pixels.setPixelColor(2, pixels.Color(0,0,255)) ;  //bleu     
   pixels.setPixelColor(3, pixels.Color(255,255,0)) ; //jaune
   pixels.setPixelColor(4, pixels.Color(0,255,255)) ; //violet
   pixels.setPixelColor(5, pixels.Color(255,255,255)) ; //blanc
   pixels.setPixelColor(4, pixels.Color(0,255,127)) ; //rose
   pixels.setPixelColor(5, pixels.Color(127,255,0)) ; //orange
*/

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

//configuration for the Tachometer variables 
const int sensorPin = 2; 
const int sensorInterrupt = 0; 
const int timeoutValue = 5; 
  
volatile unsigned long lastPulseTime; 
volatile unsigned long interval = 0; 
volatile int timeoutCounter; 

float rpm; 
int rpmlast = 3000; 
int val_btnprog_shift; // Etat haut ou bas du bouton marche arrière
int i = 0;
int choix_shift;
//int k = 0;
//int NB_clig_shift;
float rpm_mem;
//float ms_lue_shift; // Valeur à mettre en mémoire
float ms_lue_led1; // Valeur A en mémoire
float ms_lue_led2;
float ms_lue_led3;
float ms_lue_led4;
float ms_lue_led5;
float ms_lue_led6;
//float ms_lue_led7;
//float ms_lue_led8;
float ms_lue_max;

float ms_lue_led10; // Valeur A en mémoire
float ms_lue_led20;
float ms_lue_led30;
//float ms_lue_led40;
float ms_lue_max2;

float ms_lue_led100; // Valeur A en mémoire
float ms_lue_led200;
float ms_lue_led300;
float ms_lue_led400;
float ms_lue_led500;
float ms_lue_led600;
float ms_lue_max3;
int last_choix_shift;
int ADD_shift;
float histeris1; // Valeur calculé entre 2 valeurs

#define PIN            12 // Which pin on the Arduino is connected to the NeoPixels?
#define NUMPIXELS      6  // How many NeoPixels are attached to the Arduino? 
#define BTN_PROG_shift 3  // Boutton poussoir PROGRAMMATION sur pin 3
byte luminosite; 
byte choixluminosite;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // pour WS2812

unsigned long keyPrevMillis = 0;
float Time=0;
float Time2;
const unsigned long keySampleIntervalMs = 40;

int ExtralongKeyPressCountMax = 160;    // 160 * 50 = 8000 ms
int longKeyPressCountMax = 100;    // 100 * 50 = 5000 ms
int mediumKeyPressCountMin = 60;    // 60 * 50 = 3000 ms
int KeyPressCount = 0;

byte prevKeyState = HIGH;         // button is active low

// ************************************************    SETUP    ******************************************************************
void setup() { 
 Serial.begin(9600); // This initializes the serial monitor.
 pixels.begin(); // This initializes the NeoPixel library.
 //pixels.show();
 //delay(10);
 effacer_led ();

   
// config for the Tach 
  pinMode(sensorPin, INPUT); 
  pinMode(BTN_PROG_shift, INPUT_PULLUP);
 

    
attachInterrupt(sensorInterrupt, sensorIsr, RISING); 
lastPulseTime = micros(); 
timeoutCounter = 0;
/*
EEPROM.put(20, 0);
EEPROM.put(24, 0);
EEPROM.put(28, 0);
EEPROM.put(32, 0);
EEPROM.put(36, 0);
EEPROM.put(40, 0);
EEPROM.put(44, 0);
EEPROM.put(48, 0);
EEPROM.put(52, 0);
EEPROM.put(56, 0);
EEPROM.put(60, 0);
EEPROM.put(64, 0);
EEPROM.put(68, 0);
EEPROM.put(72, 0);
EEPROM.put(76, 0);
EEPROM.put(80, 0);
EEPROM.put(84, 0);
EEPROM.put(88, 0);
EEPROM.put(92, 0);
EEPROM.put(96, 0);
EEPROM.put(100, 0);
EEPROM.put(104, 0);

*/
//EEPROM.put(108, 0);
// config for the Neopixel 
pinMode(12, OUTPUT);
EEPROM.get(108, choixluminosite);  
if (choixluminosite==0) {choixluminosite=100;}
pixels.setBrightness(choixluminosite);
pixels.show();
delay(10);

EEPROM.get(80, last_choix_shift);
delay (100);
Serial.println("last_choix_shift : debut");Serial.print(last_choix_shift,DEC);

if (last_choix_shift != 1 && last_choix_shift != 2 && last_choix_shift != 3) {prog_init ();}


}

// ************************************************    sensorIsr()    ******************************************************************
void sensorIsr() 
{ 
unsigned long now = micros(); 
interval = now - lastPulseTime; 
lastPulseTime = now; 
timeoutCounter = timeoutValue;      
} 

// ************************************************    LOOP    ******************************************************************
void loop() {    
   while (timeoutCounter != 0)
      {         mesure_rpm();
        //--------------------------------
     EEPROM.get(20, ms_lue_led1); // Obtenir la valeur A dans la mémoire 0
     EEPROM.get(24, ms_lue_led2);  
     EEPROM.get(28, ms_lue_led3); 
     EEPROM.get(32, ms_lue_led4); 
     EEPROM.get(36, ms_lue_led5); 
     EEPROM.get(40, ms_lue_led6); 
//     EEPROM.get(44, ms_lue_led7); 
//     EEPROM.get(48, ms_lue_led8); 
     EEPROM.get(52, ms_lue_max); 

     EEPROM.get(56, ms_lue_led10); // Obtenir la valeur A dans la mémoire 0
     EEPROM.get(60, ms_lue_led20);  
     EEPROM.get(64, ms_lue_led30); 
//     EEPROM.get(68, ms_lue_led40); 
     EEPROM.get(72, ms_lue_max2);

     EEPROM.get(84, ms_lue_led100); // Obtenir la valeur A dans la mémoire 0
     EEPROM.get(88, ms_lue_led200);  
     EEPROM.get(92, ms_lue_led300); 
//     EEPROM.get(96, ms_lue_led400); 
     EEPROM.get(100, ms_lue_led500); 
     EEPROM.get(104, ms_lue_max3);
   
     
      val_btnprog_shift = digitalRead(BTN_PROG_shift);Time = millis();
      while (val_btnprog_shift == LOW)  // Attente appuis BOUTON prog
         { bouton(); }  
    
     }
}

// ***************************************************   shift()    ***********************************************************************
void shift() {
                  if (rpm>=0 && rpm<=ms_lue_led1) {pixels.setPixelColor(0, pixels.Color(0,0,0)) ; }// Moderately bright 0 color.
                  //if (rpm==0 || rpm<=ms_lue_led1 ) {effacer_led () ; }// Moderately bright 0 color.                  
                  //pixels.setPixelColor(0, (rpm==0 || rpm<=500  ? pixels.Color(0,0,0): pixels.Color(0,0,0))); // Moderately bright green color.

                  if (rpm>=ms_lue_led1) {pixels.setPixelColor(0, pixels.Color(255,0,0)) ; }// Moderately bright green color.
                  else {pixels.setPixelColor(0, pixels.Color(0,0,0)) ; }
                  //pixels.setPixelColor(0, (rpm>=ms_lue_led1  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright green color.

                  if (rpm>=ms_lue_led2) {pixels.setPixelColor(1, pixels.Color(255,0,0)) ; }// Moderately bright green color.
                  else {pixels.setPixelColor(1, pixels.Color(0,0,0)) ; } 
                  //pixels.setPixelColor(1, (rpm >= ms_lue_led2  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright green color.

                  if (rpm>=ms_lue_led3) {pixels.setPixelColor(2, pixels.Color(255,0,0)) ; }// Moderately bright green color.
                  else {pixels.setPixelColor(2, pixels.Color(0,0,0)) ; }
                  //pixels.setPixelColor(2, (rpm >= ms_lue_led3  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright green color.

                  if (rpm>=ms_lue_led4) {pixels.setPixelColor(3, pixels.Color(255,0,0)) ; }// Moderately bright green color.
                  else {pixels.setPixelColor(3, pixels.Color(0,0,0)) ; }
                  //pixels.setPixelColor(3, (rpm >= ms_lue_led4  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright green color.                 

                  if (rpm>=ms_lue_led5) {pixels.setPixelColor(4, pixels.Color(255,0,0)) ; }// Moderately bright green color.
                  else {pixels.setPixelColor(4, pixels.Color(0,0,0)) ; }
                  //pixels.setPixelColor(4, (rpm >= ms_lue_led5  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright green color.

                  if (rpm>=ms_lue_led6) {pixels.setPixelColor(5, pixels.Color(0,255,0)) ; }// Moderately bright rouge color.
                  else {pixels.setPixelColor(5, pixels.Color(0,0,0)) ; }
                  //pixels.setPixelColor(5, (rpm >= ms_lue_led6  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright green color.

//                  if (rpm>=ms_lue_led7) {pixels.setPixelColor(6, pixels.Color(255,127,0)) ; }// Moderately bright orange color.
//                  else {pixels.setPixelColor(6, pixels.Color(0,0,0)) ; }
//                  //pixels.setPixelColor(6, (rpm >= ms_lue_led7  ? pixels.Color(255,127,0) : pixels.Color(0,0,0))); // Moderately bright jaune color.

//                  if (rpm>=ms_lue_led8) {pixels.setPixelColor(7,  pixels.Color(255,0,0)) ; }// Moderately bright rouge color.
//                  else {pixels.setPixelColor(7, pixels.Color(0,0,0)) ; }
//                  //pixels.setPixelColor(7, (rpm >= ms_lue_led8  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright rouge color.

                  if  (rpm >= ms_lue_max) {cligo_led_bleu();}

//pixels.setBrightness(128);
pixels.show();
delay(10);
}

// ***************************************************   shift2()    ***********************************************************************
void shift2() {
                  pixels.setPixelColor(0, (rpm==0 || rpm<=500  ? pixels.Color(0,0,0): pixels.Color(0,0,0))); // Moderately bright green color.
                  //if (rpm==0 || rpm<=ms_lue_led10) {effacer_led () ; }// Moderately bright 0 color.

                  pixels.setPixelColor(2, (rpm>=ms_lue_led10  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.
                  pixels.setPixelColor(3, (rpm>=ms_lue_led10  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.
                  
                  pixels.setPixelColor(1, (rpm >= ms_lue_led20  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.
                  pixels.setPixelColor(4, (rpm >= ms_lue_led20  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.

                  pixels.setPixelColor(0, (rpm >= ms_lue_led30  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright rouge color.
                  pixels.setPixelColor(5, (rpm >= ms_lue_led30  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright rouge color.

//                  pixels.setPixelColor(0, (rpm >= ms_lue_led40  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright rouge color.
//                  pixels.setPixelColor(7, (rpm >= ms_lue_led40  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright rouge color.

                  if  (rpm >= ms_lue_max2) {cligo_led_bleu();}
                  
//pixels.setBrightness(128);
pixels.show();
delay(10);
}

// ***************************************************   shift3()    ***********************************************************************
void shift3() {
                  pixels.setPixelColor(0, (rpm==0 || rpm<=500  ? pixels.Color(0,0,0): pixels.Color(0,0,0))); // Moderately bright green color.
                  //if (rpm==0 || rpm<=ms_lue_led100) {effacer_led () ; }// Moderately bright 0 color.

                  pixels.setPixelColor(0, (rpm >= ms_lue_led100  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.} 
                  pixels.setPixelColor(1, (rpm >= ms_lue_led100  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.

                  pixels.setPixelColor(2, (rpm >= ms_lue_led200  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.
                  pixels.setPixelColor(3, (rpm >= ms_lue_led200  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.                 

                  pixels.setPixelColor(4, (rpm >= ms_lue_led300  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.
                  pixels.setPixelColor(5, (rpm >= ms_lue_led300  ? pixels.Color(255,0,0) : pixels.Color(0,0,0))); // Moderately bright green color.

//                  pixels.setPixelColor(6, (rpm >= ms_lue_led400  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright vert color.
//                  pixels.setPixelColor(7, (rpm >= ms_lue_led400  ? pixels.Color(0,255,0) : pixels.Color(0,0,0))); // Moderately bright vert color.
                  
                  if  (rpm >= ms_lue_led500 && rpm <ms_lue_max3) {
                  for (int m=0; m<6;m++) {
                  pixels.setPixelColor( m, pixels.Color(0,255,0)) ; }// Moderately bright rouge color.
                  pixels.show();
                  }
                                     
                  if  (rpm >= ms_lue_max3) {cligo_led_bleu();}

//pixels.setBrightness(128);
pixels.show();
delay(10);
}

// ***************************************************   Gestion bouton    ***********************************************************************
// called when key goes from not pressed to pressed
// called when button is kept pressed for less than .5 seconds
void shortKeyPress() {
    Serial.println("short");
    prog_shift();
}


// called when button is kept pressed for more than 2 seconds
void mediumKeyPress() {
    Serial.println("medium");
    prog_shift2();
}


// called when button is kept pressed for 2 seconds or more
void longKeyPress() {
    Serial.println("long");
    prog_shift3();    
}

void ExtralongKeyPress() {
    Serial.println("Extralong");
    prog_luminosite();    
} 

// called when key goes from not pressed to pressed
void keyPress() {
    Serial.println("key press");
    KeyPressCount = 0;
}



//void keyPress() {
//    //Serial.println("key press");
//    KeyPressCount = 0;
//}

// called when key goes from pressed to not pressed
void keyRelease() {
      if (KeyPressCount >= ExtralongKeyPressCountMax) {
        ExtralongKeyPress();
    }
    if (KeyPressCount < ExtralongKeyPressCountMax && KeyPressCount >=longKeyPressCountMax ) {
        longKeyPress();
    }
  
    if (KeyPressCount < longKeyPressCountMax && KeyPressCount >= mediumKeyPressCountMin) {
        mediumKeyPress();
        //prog_shift2();
    }
    else {
      if (KeyPressCount < mediumKeyPressCountMin) {
        shortKeyPress();
        //prog_shift();
      }
    }
}

void bouton() {
 
    // key management section
    /*if (millis() - keyPrevMillis >= keySampleIntervalMs) {
        keyPrevMillis = millis();
        
        byte currKeyState = digitalRead(BTN_PROG_shift);
        
        if ((prevKeyState == HIGH) && (currKeyState == LOW)) {
            KeyPressCount = 0;
        }
        else if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
            keyRelease();
        }
        else if (currKeyState == LOW) {
            KeyPressCount++;
        }
        prevKeyState = currKeyState;
    }*/
    if (millis() - keyPrevMillis >= keySampleIntervalMs) {
        keyPrevMillis = millis();
        
        byte currKeyState = digitalRead(BTN_PROG_shift);
        
        if ((prevKeyState == HIGH) && (currKeyState == LOW)) {
            keyPress();
        }
        else if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
            keyRelease();
        }
        else if (currKeyState == LOW) {
            KeyPressCount++;
            //Serial.println(ExtralongKeyPressCountMax,DEC);
            //Serial.println(longKeyPressCountMax,DEC);
            //Serial.println(mediumKeyPressCountMin,DEC);
            Time2=keyPrevMillis-Time;
            Time2=Time2/1000;
            Serial.println(Time2,2);            
            Serial.println(KeyPressCount,DEC);
                
   // if (KeyPressCount >= longKeyPressCountMax) {
   //     longKeyPress();
   // }
   // if (KeyPressCount >= ExtralongKeyPressCountMax) {
   //     ExtralongKeyPress();
   // }
   
        }
        
        prevKeyState = currKeyState;
    }    
}

// *******************************************************   prog_init    *********************************************************************
void prog_init (){
      do {
       val_btnprog_shift = digitalRead(BTN_PROG_shift);
       cligo_led_rouge ();}  while (val_btnprog_shift == HIGH);  // Attente appuis BOUTON prog
       val_btnprog_shift = digitalRead(BTN_PROG_shift);       
      while (val_btnprog_shift == LOW)  // Attente appuis BOUTON prog
         { bouton();}  
} 




// *******************************************************   prog_shift   ********************************************************************
void prog_shift() {
    choix_shift = 1;
    last_choix_shift=choix_shift;
    EEPROM.put(80,last_choix_shift);   //Mémorisation de la  valeur valeurpot à l'adresse 0
    EEPROM.get(80, last_choix_shift);
    Serial.println(last_choix_shift,DEC);
    effacer_led(); delay(200);
    pixels.setPixelColor(0, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.show(); delay(200);
    effacer_led(); delay(200);
    pixels.setPixelColor(0, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.show(); delay(200);    
    effacer_led(); delay(200);
    //LED1*******************************
        pixels.setPixelColor(0, pixels.Color(255,0,0)); // Moderately bright green color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 20;
        mesure_rpm_shift();
        //rpm_mem=rpm*2;
        //delay(500);
        //ADD_shift = 20;
        eeprom_shift ();
    //LED2*******************************
        pixels.setPixelColor(1, pixels.Color(255,0,0)); // Moderately bright green color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 24;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED3*******************************
        pixels.setPixelColor(2, pixels.Color(255,0,0)); // Moderately bright green color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 28;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED4*******************************
        pixels.setPixelColor(3, pixels.Color(255,0,0)); // Moderately bright green color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 32;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED5*******************************
        pixels.setPixelColor(4,pixels.Color(255,0,0)); // Moderately bright green color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 36;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED6*******************************
        pixels.setPixelColor(5, pixels.Color(0,255,0)); // Moderately bright rouge color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 40;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED7*******************************
//        pixels.setPixelColor(6, pixels.Color(255,255,0)); // Moderately bright jaune color.
//        pixels.show();
//        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
//        ADD_shift = 44;
//        mesure_rpm_shift();
//        eeprom_shift ();
    //LED8*******************************
//        pixels.setPixelColor(7, pixels.Color(255,0,0)); // Moderately bright rouge color.
//        pixels.show();
//        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
//        ADD_shift = 48;
//        mesure_rpm_shift();
//        eeprom_shift ();
    //LEDmax*******************************
       for (int j=0; j<6;j++) {               
            pixels.setPixelColor( j, pixels.Color(0, 0, 255));}  // even pixels bleue
            pixels.show();    
            while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
            ADD_shift = 52;
            mesure_rpm_shift();
            eeprom_shift ();
    EEPROM.get(80, last_choix_shift);
    Serial.println("choix_shift :");Serial.print(last_choix_shift,DEC);
    val_btnprog_shift=!val_btnprog_shift ;
    int val_btnprog_shift;
    delay(1000);
  }

// *******************************************************   prog_shift 2  ********************************************************************
void prog_shift2() {
    choix_shift=2;
    last_choix_shift=choix_shift;
    EEPROM.put(80,last_choix_shift);   //Mémorisation de la  valeur valeurpot à l'adresse 0 
    EEPROM.get(80, last_choix_shift);
    Serial.println("choix_shift :");Serial.print(last_choix_shift,DEC);
    effacer_led(); delay(200);
    pixels.setPixelColor(2, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.setPixelColor(3, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.show(); delay(200);
    effacer_led(); delay(200);
    pixels.setPixelColor(2, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.setPixelColor(3, pixels.Color(255,0,0)); // Moderately bright green color..
    pixels.show(); delay(200);    
    effacer_led(); delay(200);
    //LED1*******************************
    pixels.setPixelColor(2, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.setPixelColor(3, pixels.Color(255,0,0)); // Moderately bright green color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 56;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED2*******************************
    pixels.setPixelColor(1, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.setPixelColor(4, pixels.Color(255,0,0)); // Moderately bright green color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 60;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED3*******************************
        pixels.setPixelColor(0, pixels.Color(0,255,0)); // Moderately bright rouge color.
        pixels.setPixelColor(5, pixels.Color(0,255,0)); // Moderately bright rouge color.    
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 64;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED4*******************************
//        pixels.setPixelColor(0, pixels.Color(255,0,0)); // Moderately bright green color.
//        pixels.setPixelColor(7, pixels.Color(255,0,0)); // Moderately bright green color.
//        pixels.show();
//        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
//        ADD_shift = 68;
//        mesure_rpm_shift();
//        eeprom_shift ();
    //LEDmax2*******************************
       for (int j=0; j<6;j++) {               
            pixels.setPixelColor( j, pixels.Color(0, 0, 255));}  // even pixels bleue
            pixels.show();    
            while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
            delay(250);
            ADD_shift = 72;
            mesure_rpm_shift();
            eeprom_shift ();

    val_btnprog_shift=!val_btnprog_shift ;
    int val_btnprog_shift;
    delay(1000);
  }

// *******************************************************   prog_shift 3  ********************************************************************
void prog_shift3() {
    choix_shift=3;
    last_choix_shift=choix_shift;
    EEPROM.put(80,last_choix_shift);   //Mémorisation de la  valeur valeurpot à l'adresse 0 
    EEPROM.get(80, last_choix_shift);
    Serial.println("choix_shift :");Serial.print(last_choix_shift,DEC);
    effacer_led(); delay(200);
    pixels.setPixelColor(0, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.setPixelColor(1, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.show(); delay(200);
    effacer_led(); delay(200);
    pixels.setPixelColor(0, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.setPixelColor(1, pixels.Color(255,0,0)); // Moderately bright green color..
    pixels.show(); delay(200);    
    effacer_led(); delay(200);
    //LED1*******************************
    pixels.setPixelColor(0, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.setPixelColor(1, pixels.Color(255,0,0)); // Moderately bright green color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 84;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED2*******************************
    pixels.setPixelColor(2, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.setPixelColor(3, pixels.Color(255,0,0)); // Moderately bright green color.
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 88;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED3*******************************
        pixels.setPixelColor(4, pixels.Color(255,0,0)); // Moderately bright vert color.
        pixels.setPixelColor(5, pixels.Color(255,0,0)); // Moderately bright vert color.    
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 92;
        mesure_rpm_shift();
        eeprom_shift ();
    //LED4*******************************
//        pixels.setPixelColor(6, pixels.Color(0,255,0)); // Moderately bright vert color.
//        pixels.setPixelColor(7, pixels.Color(0,255,0)); // Moderately bright vert color.    
//        pixels.show();
//        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
//        ADD_shift = 96;
//        mesure_rpm_shift();
//        eeprom_shift ();
    //LED5*******************************
        for (int m=0; m<6;m++) {
            pixels.setPixelColor( m, pixels.Color(0, 255, 0));}  // even pixels rouge
        pixels.show();
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 100;
        mesure_rpm_shift();
        eeprom_shift ();
    //LEDmax3*******************************
        for (int j=0; j<6;j++) {               
            pixels.setPixelColor( j, pixels.Color(0, 0, 255));}  // even pixels bleue
        pixels.show();    
        while(digitalRead(BTN_PROG_shift) == HIGH) // Attente appuis BOUTON prog
        ADD_shift = 104;
        mesure_rpm_shift();
        eeprom_shift ();

    val_btnprog_shift=!val_btnprog_shift ;
    int val_btnprog_shift;
    delay(1000);
  }
  
// *******************************************************   mesure_rpm   **********************************************************************
void mesure_rpm() {
          --timeoutCounter; 
          rpm = 60e6/(float)interval; // ORIGINAL 
          
          EEPROM.get(80, last_choix_shift);
          Serial.println("choix_shift :");Serial.println(last_choix_shift,DEC);
          choix_shift=last_choix_shift;
          
          if (choix_shift==1) { 
          shift();  // affiche _LED 
          Serial.println(rpm,0); 
          }
          
          else if (choix_shift==2) {
          shift2();  // affiche _LED 
          Serial.println(rpm,0);            
          }

          else if (choix_shift==3) {
          shift3();  // affiche _LED 
          Serial.println(rpm,0);            
          }

          if (rpm > rpmlast+500){rpm=rpmlast;} //remove erroneous results 
          rpmlast = rpm;
}

// *********************************************************   mesure_rpm_shift   ****************************************************************
void mesure_rpm_shift() {
          --timeoutCounter; 
          rpm = 60e6/(float)interval; // ORIGINAL 
          //if (rpm > rpmlast+500){rpm=rpmlast;} //remove erroneous results 
          rpmlast = rpm;
          rpm_mem=rpm*2;
          delay(500);
}

// **********************************************************   eeprom_shift   *******************************************************************
void eeprom_shift (){
    EEPROM.put(ADD_shift,rpm_mem);   //Mémorisation de la  valeur valeurpot à l'adresse 0 
    //EEPROM.get(ADD_shift, ms_lue_led1);
    Serial.println();
    Serial.println(rpm,0);    // Affiche la Valeur valeurpot dans le moniteur série
    Serial.println(rpm_mem,0);
    //Serial.println(ms_lue_led1,0);     // Affiche la Valeur ms_lue dans le moniteur série
    delay(500);
}

// **************************************************************  cligo_led_bleu    **************************************************************
void cligo_led_bleu (){
       for (int j=0; j<6;j++) {               
            pixels.setPixelColor( j, pixels.Color(0, 0, 0));}  // even pixels 0
            pixels.show();
            delay (150); 
       for (int j=0; j<6;j++) {               
            pixels.setPixelColor( j, pixels.Color(0, 0, 255));}  // even pixels bleue
            pixels.show(); 
            delay (150);  
}

// **************************************************************  cligo_led_rouge    **************************************************************
void cligo_led_rouge (){
       for (int j=0; j<6;j++) {               
            pixels.setPixelColor( j, pixels.Color(0, 0, 0));}  // even pixels 0
            pixels.show();
            delay (150); 
       for (int j=0; j<6;j++) {               
            pixels.setPixelColor( j, pixels.Color(0, 255, 0));}  // even pixels rouge
            pixels.show(); 
            delay (150);  
}

// **************************************************************  allumé_TOUTE_led_vert    **************************************************************
void led_vert (){
/*       for (int j=0; j<6;j++) {               
            pixels.setPixelColor( j, pixels.Color(0, 0, 0));}  // even pixels 0
            pixels.show();
            delay (150); */
            pixels.setBrightness(choixluminosite);  
       for (int j=0; j<6;j++) {               
            pixels.setPixelColor( j, pixels.Color(255, 0, 0));}  // even pixels rouge
            pixels.show(); 
            delay (500);  
}

// *********************************************************   effacer_led      ********************************************************************
void effacer_led (){
                      for (i=0; i<6;i++) {
                            pixels.setPixelColor(i, pixels.Color(0,0,0));} // Moderately bright rouge color.
                            pixels.show();delay(10);
}

// *********************************************************   prog_luminosite      ********************************************************************
void prog_luminosite (){
  choixluminosite=210;led_vert();
         do {
                //for (int k=0; k<25;k++) {               
                  choixluminosite=choixluminosite-10;  // even pixels rouge
                  Serial.println(choixluminosite);
                  led_vert(); 
                            //while(digitalRead(BTN_PROG_shift) == LOW) ; prog_luminosite_valide(); // Attente appuis BOUTON prog 
                  if (choixluminosite==0) {choixluminosite=210;};
            }
          while(digitalRead(BTN_PROG_shift) == HIGH) ; // Attente appuis BOUTON prog 
          prog_luminosite_valide();
          
    //EEPROM.put(108,choixluminosite);   //Mémorisation de la  valeur valeurpot à l'adresse 0 
    //EEPROM.get(108, choixluminosite);
    //            pixels.setBrightness(choixluminosite); 
    //             pixels.show(); 
  //}

    val_btnprog_shift=!val_btnprog_shift ;
}

void prog_luminosite_valide (){
      EEPROM.put(108,choixluminosite);   //Mémorisation de la  valeur valeurpot à l'adresse 0 
    EEPROM.get(108, choixluminosite);
                pixels.setBrightness(choixluminosite); 
                                  Serial.println(choixluminosite);
                 pixels.show(); 
                     val_btnprog_shift=!val_btnprog_shift ;
                     delay(1500);
                     KeyPressCount=0;
                 loop();
  }
