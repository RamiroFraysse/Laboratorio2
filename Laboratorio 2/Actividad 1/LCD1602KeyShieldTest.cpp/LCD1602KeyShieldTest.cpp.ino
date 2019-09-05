/*
 LCD1602 Keypad Shield 1.0 Test Sketch - LiquidCrystal Library
 
 Este sketch demuestra el uso del LCD1602 Key Shield 1.0.
 Para ello se toman los pulsos de los botones mediante la entrada
 analÃ³gica AD0 y, mediante la librerÃ­a LiquidCrystal de Arduino 1.0,
 se muestra en el display la tecla pulsada. La librerÃ­a LiquidCrystal
 permite controlar cualquier display LCD compatible con el 
 controlador Hitachi HD44780.
 
 La configuraciÃ³n de la LibrerÃ­a se realiza en base al esquemÃ¡tico
 del shield.
 
 Este ejemplo estÃ¡ basado en un ejemplo provisto con la documentaciÃ³n del
 LCD1602 Keypad Shield, el cuÃ¡l ha sido adaptado en base a los ejemplos 
 de la librerÃ­a LiquidCrystal de Arduino.
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 http://arduino.cc/en/Reference/LiquidCrystal
 
 
 */
#include <stdbool.h>
#include <stdint.h>
#include "Arduino.h"
// include the library code:
#include <LiquidCrystal.h>

// these constants won't change.  But you can change the size of
// your LCD using them:
const uint8_t numRows = 2;
const uint8_t numCols = 16;//Cantidad de celdas donde se puede escribir en el lcd

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Key message
char msgs[5][17] = 
{
    " Right Key:  OK ", 
    " Up Key:     OK ", 
    " Down Key:   OK ", 
    " Left Key:   OK ", 
    " Select Key: OK "
};
uint16_t adc_key_val[5] ={30, 150, 360, 535, 760 };
uint8_t NUM_KEYS = 5;
uint16_t adc_key_in;
uint16_t key=-1;
uint16_t oldkey=-1;

void setup() 
{
    Serial.begin(9600);
    //pinMode(10, OUTPUT);
    // set up the LCD's number of columns and rows: 
    lcd.begin(numCols,numRows);
    analogWrite(10, 0); //Controla intensidad backlight, le da el brillo
    lcd.setCursor(0, 0);
    lcd.print("Key Pad Test");
    lcd.setCursor(0, 1);
    lcd.print("Sist.Emb. 2019");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Press any key...");
}

void loop()
{
    adc_key_in = analogRead(0);      // read the value from the sensor  

    key = get_key(adc_key_in);       // convert into key press

    if (key != oldkey)             // if keypress is detected
    { 
        delay(50);                 // wait for debounce time
        adc_key_in = analogRead(0);  // read the value from the sensor  
        key = get_key(adc_key_in);   // convert into key press
        if (key != oldkey)        
        {     
            oldkey = key;
            if (key >=0)
            {
                lcd.setCursor(0, 1);      //line=1, x=0
                lcd.print(msgs[key]);
            }
        }
    }
}

// Convert ADC value to key number
uint16_t get_key(unsigned int input)
{
    uint16_t k;
    for (k = 0; k < NUM_KEYS; k++){
        Serial.println(input);
        if (input < adc_key_val[k])
            return k;
    }

    if (k >= NUM_KEYS)
        k = -1;     // No valid key pressed

    return k;
}
