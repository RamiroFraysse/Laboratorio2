#include <LiquidCrystal.h>
#include "device.h"
#include "fnqueue.h"
#include "timer.h"

//Repositorio Github
//Inicializa la library con los numeros de interfaces de los pines.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int brillo = 100;
//Identifican las celdas del lcd
const int numRows = 2;
const int numCols = 16;
static int modo; //modo 0 MCA, modo 1 MP, modo 2 MVT, modo 3 MAD 

static int posCronX = 4; //Arranca en 4 para que quede centrado
static int posCronY = 1; //Empieza a imprimir en la columna de abajo.
static int ms,s,m,cs;
volatile int tiempos[10][3]; //tiempos guardados tanto los m, cs y s.
volatile int savepos; //posición donde se almacena el ultimo tiempo a guardar
volatile int visorpos; //posición del tiempo visualizado en MVT



const int analogOutPin = 10; // Analog output pin that the LED brightness is attached to

void select_key_down(){

  lcd.setCursor(0, 1);//Arranca a escribir en la columna 0 y la fila 1.
  lcd.print("Select key down ");
}

void up_key_down(){
  switch(modo){
    case 0:
      modo=1;
      break;
    case 1:
      modo=0;
      break;
    case 2:
      visorpos=(visorpos+1)%10;
      break;
    case 3:
      if(brillo<100)
         brillo += 20;
         analogWrite(10, brillo); //Controla intensidad backlight 
      break;
  
  }
}
void down_key_down(){
}


void printInicio(void)
{
  lcd.setCursor(0, 0);
   //40 caracteres
  //16 ancho del display, es necesario mostrar los otros 24 tambien
  lcd.print("Sistemas Embebidos-2do Cuatrimestre 2019");
  lcd.setCursor(0, 1);
  //40 caracteres
  lcd.print("Laboratorio 2 - Com: Fraysse / Carignano");
  for (int positionCounter = 0; positionCounter < 24; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft(); 
    // wait a bit:
    delay(200);
  }
  //Espera 2s.
  delay(2000);
  for (int positionCounter = 0; positionCounter < 24; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();   
  }
  
  lcd.clear();

}

void printNumero(int num,int x) {
  if (num <= 9) {
    lcd.setCursor(x, posCronY);
    lcd.print("0");
    lcd.setCursor(x+1, posCronY);
    lcd.print(num);
  } else {
    lcd.setCursor(x, posCronY);
    lcd.print(num);
  }
}

void printCronometro(void){
  printNumero(m,posCronX);
  lcd.setCursor(posCronX+2, posCronY);
  lcd.print(":");
  printNumero(s,posCronX+3);
  lcd.setCursor(posCronX+5, posCronY);
  lcd.print(":");
  printNumero(cs,posCronX+6);
}

void printBrillo(){
  lcd.setCursor(0, 1);
  lcd.print("       ");
  lcd.print(brillo);
  if(brillo == 100){
    lcd.setCursor(10, 1);
    lcd.print("%     ");
  }
   else
     if(brillo > 5){
       lcd.setCursor(9, 1);
       lcd.print("%     ");
     }
     else{
       lcd.setCursor(8, 1);
       lcd.print("%     ");
     }
}

void printUlt10(int i){
  printNumero(tiempos[i][0],posCronX);
  lcd.setCursor(posCronX+2, posCronY);
  lcd.print(":");
  printNumero(tiempos[i][1],posCronX+3);
  lcd.setCursor(posCronX+5, posCronY);
  lcd.print(":");
  printNumero(tiempos[i][2],posCronX+6); 
 }

ISR(TIMER2_OVF_vect) {  //timer1 interrupt 8kHz toggles pin 9
  TCNT2 = gettcnt2();  
  /* Write to a digital pin so that we can confirm our timer */  
  if (modo == 0) {
    if(ms==10){
      cs++;
      ms=0;
    }
    if(cs==100){
      s++;
      cs=0;
    }
    if(s==60){
      m++;
      s=0;
    }
    ms++; 
  }
  
//  if(presionoSelect | MAD){
//    dimmer++;
//  }
//
//
//  toggle = ~toggle;  
}

void setup() {

  // Setup LCD
  pinMode(analogOutPin, OUTPUT);
  lcd.begin(numCols,numRows);
  analogWrite(analogOutPin, brillo); //Controla intensidad backlight
  printInicio();
  modo=3;
  ms=0;
  cs=0;
  m=0;
  s=0;
  savepos=0;
  visorpos=0;

  timer_init();

  //Inicialización driverTeclado
  teclado_init();
  fnqueue_init();

  //Cuando se atienda la función encolada se va a ejecutar el callback correspondiente
  key_down_callback(select_key_down, TECLA_SELECT);
  key_down_callback(up_key_down, TECLA_UP);
  key_down_callback(down_key_down, TECLA_DOWN);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  switch(modo){
    case 0: 
      lcd.print("Conteo Ascendente");
      printCronometro();
      break;
    case 1: 
      lcd.print("      Pausa      ");
      printCronometro();
      break;
    case 2:
      lcd.print("  Visor Tiempos  ");
      printUlt10(visorpos);
      break;
    case 3:
      lcd.print("  Ajuste Dimmer  ");
     //printBrillo();
      break;
  }
  fnqueue_run();
}
