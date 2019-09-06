#include <LiquidCrystal.h>
#include "device.h"
#include "fnqueue.h"

//Repositorio Github
//Inicializa la library con los numeros de interfaces de los pines.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int brightness = 150;
//Identifican las celdas del lcd
const int numRows = 2;
const int numCols = 16;

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 10; // Analog output pin that the LED brightness is attached to

void lcdCleanFila(int fila){
  //Imprime 16 espacios en la fila indicada (0 o 1)
  lcd.setCursor(0, fila);
  lcd.print("                ");
}

void select_key_down(){

  lcd.setCursor(0, 1);//Arranca a escribir en la columna 0 y la fila 1.
  lcd.print("Select key down ");
}
void left_key_down(){
  lcd.setCursor(0, 1);
  lcd.print("Left key down   ");
}
void right_key_down(){
  lcd.setCursor(0, 1);
  lcd.print("Right key down  ");
}
void up_key_down(){
  lcd.setCursor(0, 1);
  lcd.print("Up key down     ");
}
void down_key_down(){
  lcd.setCursor(0, 1);
  lcd.print("Down key down   ");
}
void down_key_up(){
  lcd.setCursor(0, 1);
  lcd.print("Down key up     ");
}
void up_key_up(){
  lcd.setCursor(0, 1);
  lcd.print("Up key up       ");
}
void left_key_up(){
  lcd.setCursor(0, 1);
  lcd.print("Left key up     ");
}
void right_key_up(){
  lcd.setCursor(0, 1);
  lcd.print("Right key up    ");
}
void select_key_up(){
  lcd.setCursor(0, 1);
  lcd.print("Select key up   ");
}


void setup() {

  // Setup LCD
  pinMode(analogOutPin, OUTPUT);
  lcd.begin(numCols,numRows);
  analogWrite(analogOutPin, brightness); //Controla intensidad backlight
  lcd.setCursor(0, 0);
  lcd.print(" Driver de Teclado ");
  lcd.setCursor(0, 1);
  lcd.print("Press any key...");

  //Inicialización driverTeclado
  teclado_init();
  fnqueue_init();

  //Cuando se atienda la función encolada se va a ejecutar el callback correspondiente
  key_down_callback(select_key_down, TECLA_SELECT);
  key_down_callback(left_key_down, TECLA_LEFT);
  key_down_callback(right_key_down, TECLA_RIGHT);
  key_down_callback(up_key_down, TECLA_UP);
  key_down_callback(down_key_down, TECLA_DOWN);
  
  key_up_callback(select_key_up, TECLA_SELECT);
  key_up_callback(left_key_up, TECLA_LEFT);
  key_up_callback(right_key_up, TECLA_RIGHT);
  key_up_callback(up_key_up, TECLA_UP);
  key_up_callback(down_key_up, TECLA_DOWN);
  
}

void loop() {
  // put your main code here, to run repeatedly:


  fnqueue_run();
}
