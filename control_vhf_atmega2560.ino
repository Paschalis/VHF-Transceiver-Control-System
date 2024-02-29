/*##############################################################################
 # PROJECT       :  Control VHF (KTR 905 COMM TRANSCEIVER)
 # AUTHOR        :  Moschogiannis Paschalis
 # FILENAME      :  control_vhf_atmega2560.ino
 #
 #
 # This file is part of the Control VHF project. More information about
 # this project can be found here: pasxalis.8@hotmail.gr
 ################################################################################
 */

//We always have to include the library
#include "LedControl.h"
#include <Keypad.h>
#include <math.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
//connect to the row pinouts of the keypad
byte rowPins[ROWS] = {53, 52, 51, 50};
//connect to the column pinouts of the keypad
byte colPins[COLS] = {49, 48, 47, 46};
//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int keyNum = 0;
/*
   Now we need a LedControl to work with.
   pin 12 is connected to the DataIn
   pin 11 is connected to the CLK
   pin 10 is connected to LOAD
   We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=500;
/*Variable welcome used to show "Hello... ----- 12345678" one time.*/
int welcome=0;
int i=5;
int z=0;
int wire=22;
int dec[5];
String bcd;
int digit;
int dec_counter=0;
int bcd_counter=0;
void setup() {
    /*
     * The MAX72XX is in power-saving mode on startup,
     * we have to do a wakeup call
     */
    /*
     * Set the shutdown (power saving) mode for the device
     * Params :
     * addr	The address of the display to control
     * status	If true the device goes into power-down mode. Set to false
     *		for normal operation.
     */
    lc.shutdown(0,false);
    /*
     * Set the brightness of the display.
     * Params:
     * addr		the address of the display to control
     * intensity	the brightness of the display. (0..15)
     */
    lc.setIntensity(0,4);
    /*
     * Switch all Leds on the display off.
     * Params:
     * addr	address of the display to control
     */
    lc.clearDisplay(0);
    Serial.begin(9600);
    pinMode(22, OUTPUT);
    pinMode(23, OUTPUT);
    pinMode(24, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(27, OUTPUT);
    pinMode(28, OUTPUT);
    pinMode(29, OUTPUT);
    pinMode(30, OUTPUT);
    pinMode(31, OUTPUT);
    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(34, OUTPUT);
    pinMode(35, OUTPUT);
    pinMode(36, OUTPUT);
    pinMode(37, OUTPUT);

}

void hello(){


    /*
     * Display a character on a 7-Segment display.
     * There are only a few characters that make sense here :
     *	'0','1','2','3','4','5','6','7','8','9','0',
     *  'A','b','c','d','E','F','H','L','P',
     *  '.','-','_',' '
     * Params:
     * addr	address of the display
     * digit	the position of the character on the display (0..7)
     * value	the character to be displayed.
     * dp	sets the decimal point.
     */
    lc.setChar(0,7,'H',false);
    lc.setChar(0,6,'E',false);
    lc.setChar(0,5,'L',false);
    lc.setChar(0,4,'L',false);
    lc.setChar(0,3,'0',false);
    lc.setChar(0,2,'.',false);
    lc.setChar(0,1,'.',false);
    lc.setChar(0,0,'.',false);
    delay(delaytime+1000);
    lc.clearDisplay(0);
    for(int k=0; k<=7; k++) {
        lc.setChar(0,k,'-',false);
    }
    delay(delaytime+1000);
    lc.clearDisplay(0);
    int l=1;
    for(int k=7; k>=0; k--) {
        lc.setDigit(0,k,l,false);
        delay(delaytime);
        l++;
    }
    lc.clearDisplay(0);
    //Default frequency
    lc.setChar(0,7,'-',false);
    lc.setDigit(0,6,1,false);
    lc.setChar(0,0,'-',false);
}

int power(int base, int exponent){
    long long result = 1;
    while (exponent != 0) {
        result *= base;
        --exponent;
    }
    return result;

}


String digitToBcd(int digit){
    String str="";
    for(int n=3; n>=0; n--){
        int d = power(2,n);

        if(digit/d!=0){
            str+="1";
            digit-=d;
        }
        else
            str+="0";
    }
    return str;
}


void loop() {
    if (welcome==0) {
        hello();
        welcome=1;
    }

    /*
     * Set the status of a single Led.
     * Params :
     * addr	address of the display
     * row	the row of the Led (0..7)
     * col	the column of the Led (0..7)
     * state	If true the led is switched on,
     *		if false it is switched off
     */
    lc.setLed(0,4,0,true); //set led of dot pin on
    char key = keypad.getKey();
    if(key) {
        if(key=='*'){
            /*
            int decimal = 0;
            for (int z = 0; z < 5; z++) {
                int num = dec[z];
                if (num != 0) {
                    while (num > 0) {
                        decimal *= 10;
                        num /= 10;
                    }
                decimal += dec[z];
                }else{
                    decimal *= 10;
                }
            }
            */
            int a;
            long decimal=0;
            for(a=0; a<5; a++) {
                decimal=10*decimal+dec[a];
            }
            Serial.print("Decimal number:");
            Serial.println(decimal);

            String bcd="";
            while(decimal!=0) {
                digit=decimal%10;
                bcd=digitToBcd(digit)+bcd;
                decimal/=10;
            }

            Serial.print("BCD number:");
            Serial.println(bcd);

            for(int bcd_counter=2; bcd_counter<=20; bcd_counter++) {
                if(bcd[bcd_counter]=='1') {
                    digitalWrite(wire, HIGH);
                }
                else if(bcd[bcd_counter]=='0') {
                    digitalWrite(wire, LOW);
                }
                wire++;
            }
            wire=22;
            int i=5;
            int z=0;
        }
        else if(key=='A'){

            //Show the frequency A=132.725MHz
            lc.setChar(0,7,'-',false);
            lc.setDigit(0,6,1,false);
            lc.setDigit(0,5,2,false);
            lc.setDigit(0,4,1,false);
            lc.setDigit(0,3,5,false);
            lc.setDigit(0,2,0,false);
            lc.setDigit(0,1,0,false);
            lc.setChar(0,0,'-',false);

            long decimal=21500;
            while(decimal!=0) {
                digit=decimal%10;
                bcd=digitToBcd(digit)+bcd;
                decimal/=10;
            }

            Serial.println(decimal);
            Serial.println(bcd);

            for(int bcd_counter=2; bcd_counter<=20; bcd_counter++) {
                if(bcd[bcd_counter]=='1') {
                    digitalWrite(wire, HIGH);
                }
                else if(bcd[bcd_counter]=='0') {
                    digitalWrite(wire, LOW);
                }
                wire++;
            }
            wire=22;
            String bcd="";
        }

        else if(key=='B') {

            //Show the frequency B=130.600MHz
            lc.setChar(0,7,'-',false);
            lc.setDigit(0,6,1,false);
            lc.setDigit(0,5,2,false);
            lc.setDigit(0,4,2,false);
            lc.setDigit(0,3,1,false);
            lc.setDigit(0,2,0,false);
            lc.setDigit(0,1,0,false);
            lc.setChar(0,0,'-',false);
            long decimal=22100;
            while(decimal!=0) {
                digit=decimal%10;
                bcd=digitToBcd(digit)+bcd;
                decimal/=10;
            }

            Serial.println(decimal);
            Serial.println(bcd);

            for(int bcd_counter=2; bcd_counter<=20; bcd_counter++) {
                if(bcd[bcd_counter]=='1') {
                    digitalWrite(wire, HIGH);
                }
                else if(bcd[bcd_counter]=='0') {
                    digitalWrite(wire, LOW);
                }
                wire++;
            }
            wire=22;
            String bcd="";
        }

        else if(key=='C') {

            //Show the frequency C=123.500MHz
            lc.setChar(0,7,'-',false);
            lc.setDigit(0,6,1,false);
            lc.setDigit(0,5,2,false);
            lc.setDigit(0,4,9,false);
            lc.setDigit(0,3,8,false);
            lc.setDigit(0,2,0,false);
            lc.setDigit(0,1,0,false);
            lc.setChar(0,0,'-',false);
            long decimal=29800;
            while(decimal!=0) {
                digit=decimal%10;
                bcd=digitToBcd(digit)+bcd;
                decimal/=10;
            }

            Serial.println(decimal);
            Serial.println(bcd);

            for(int bcd_counter=2; bcd_counter<=20; bcd_counter++) {
                if(bcd[bcd_counter]=='1') {
                    digitalWrite(wire, HIGH);
                }
                else if(bcd[bcd_counter]=='0') {
                    digitalWrite(wire, LOW);
                }
                wire++;
            }
            wire=22;
            String bcd="";
        }

        else if(key=='D') {

            //Show the frequency D=133.000MHz
            lc.setChar(0,7,'-',false);
            lc.setDigit(0,6,1,false);
            lc.setDigit(0,5,3,false);
            lc.setDigit(0,4,2,false);
            lc.setDigit(0,3,7,false);
            lc.setDigit(0,2,2,false);
            lc.setDigit(0,1,5,false);
            lc.setChar(0,0,'-',false);
            long decimal=32725;
            Serial.println(decimal);

            while(decimal!=0) {
                digit=decimal%10;
                bcd=digitToBcd(digit)+bcd;
                decimal/=10;
            }
            Serial.println(bcd);

            for(int bcd_counter=2; bcd_counter<=20; bcd_counter++) {
                if(bcd[bcd_counter]=='1') {
                    digitalWrite(wire, HIGH);
                }
                else if(bcd[bcd_counter]=='0') {
                    digitalWrite(wire, LOW);
                }
                wire++;
            }
            wire=22;
            String bcd="";
        }

        else if(key=='#') {

            lc.clearDisplay(0);
            //Default frequency
            lc.setChar(0,7,'-',false);
            lc.setDigit(0,6,1,false);
            lc.setChar(0,0,'-',false);

            wire=22;
            String bcd="";
            i=5;
            z=0;
        }

        else{

            //convert keypad button to display integer number
            if(keyNum<=999) {
                keyNum = (keyNum*10) + (int(key)-48);
            }
            if (key) {
                Serial.print("The button that you have pressed:");
                Serial.println(key);
            }
            Serial.print("The KeyNum:");
            Serial.println(keyNum);

            dec[z]=keyNum;
            //display frequency between -132.725-
            while(i>=1) {
                lc.setDigit(0,i,keyNum,false);
                break;
            }
            z = z + 1;
            i = i - 1;
            keyNum = 0;

        }
    }
}
