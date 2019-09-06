#include "Arduino.h"
#include "device.h"
#include "stdio.h"
#include "fnqueue.h"


volatile int adcValue;
volatile uint8_t low, high;
volatile int conversionFlag = 0;
int canal;

//Handler key down functions
void (*handler_key_down_tecla_up)();
void (*handler_key_down_tecla_down)();
void (*handler_key_down_tecla_right)();
void (*handler_key_down_tecla_left)();
void (*handler_key_down_tecla_select)();
//Handler key up functions
void (*handler_key_up_tecla_up)();
void (*handler_key_up_tecla_down)();
void (*handler_key_up_tecla_right)();
void (*handler_key_up_tecla_left)();
void (*handler_key_up_tecla_select)();

//Used to convert adcValue to key number
int adc_key_val[5] ={30, 180, 360, 535, 760};
int NUM_KEYS = 5;

// Variables auxiliares
int lastKeyDown = -1;
int teclaPresionada = -1;

//Encabezado de funciones
int get_key(int adcValue);
void key_down_function(int teclaPresionada);
void key_up_function(int lastKeyDown);

int adc_init(void){
    int exito = 0;
    canal = 0;
    int tensionRef =1;
    if(canal >= 0 && canal <6){

        ADMUX |= canal;
        if(tensionRef >= 0 && tensionRef < 4){
            if(tensionRef == 3){
                ADMUX |= (1 << REFS1) | (1 << REFS0);
            }
            else{
                ADMUX |= (tensionRef << 6);
            }
        }
        else{ //Tension de referencia por default
            ADMUX |= (1 << REFS0);
            ADMUX &= ~(1 << REFS1);
        }
        //PRR – Power Reduction Register
        //Bit 0 – PRADC: Power Reduction ADC
        //Writing a logic one to this bit shuts down the ADC. The ADC must be disabled before shut down.
        PRR &= ~(1 << PRADC);
        
        //ADCSRA – ADC Control and Status Register A
        //Bit 7 – ADEN: ADC Enable
        //Writing this bit to one enables the ADC (this don't start de ADC). 
        ADCSRA |= (1 << ADEN);
        //Bits 2:0 – ADPS[2:0]: ADC Prescaler Select Bits
        //128 Preescaler
        ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
        //Bit 3 – ADIE: ADC Interrupt Enable (if global interruptions are enable)
        ADCSRA |= (1 << ADIE);
        //Bit 5 – ADATE: ADC Auto Trigger Enable
        ADCSRA |= (1 << ADATE);

        //ADCSRB – ADC Control and Status Register B
        //Bit 2:0 – ADTS[2:0]: ADC Auto Trigger Source - ADTS2 = 0, ADTS1 = 0, ADTS0 = 0 -> Free Running Mode
        ADCSRB &= ~(1 << ADTS2) & ~(1 << ADTS1) & ~(1 << ADTS0);

        //Inicializar Conversion
        //Bit 6 – ADSC: ADC Start Conversion
        ADCSRA |= (1 << ADSC);

        exito = 1;
    }
    return exito;
}


void teclado_init(void){
      Serial.begin(9600);

    adc_init();    
}



// Convert ADC value to key number
int get_key(int input){
    int k;
    for (k = 0; k < NUM_KEYS; k++)
        if (input < adc_key_val[k])
            return k;

    if (k >= NUM_KEYS)
        k = -1;     // No valid key pressed

    return k;
}


void key_down_function(int teclaPresionada){
    switch(teclaPresionada){
    case TECLA_UP:
      handler_key_down_tecla_up();
      break;
    case TECLA_DOWN:
      handler_key_down_tecla_down();
      break;
    case TECLA_LEFT:
      handler_key_down_tecla_left();
      break;
    case TECLA_RIGHT:
      handler_key_down_tecla_right();
      break;
    case TECLA_SELECT:
      handler_key_down_tecla_select();
      break;

  }
}

void key_up_function(int lastKeyDown){
  switch(lastKeyDown){
    case TECLA_UP:
      handler_key_up_tecla_up();
      break;
    case TECLA_DOWN:
      handler_key_up_tecla_down();
      break;
    case TECLA_LEFT:
      handler_key_up_tecla_left();
      break;
    case TECLA_RIGHT:
      handler_key_up_tecla_right();
      break;
    case TECLA_SELECT:
      handler_key_up_tecla_select();
      break;
  }
}

void ProcesarAdc(){
  //Funcion que determina que tecla fue pulsada.
 
  int key = get_key(adcValue);
  Serial.println("adcValue: "+String(adcValue));
  if (key != -1){
    key_down_function(key);
    lastKeyDown = key;
  }
  else{
    if(lastKeyDown != -1){
      key_up_function(lastKeyDown);
    }
  }

}

ISR(ADC_vect){ //ADC conversion complete
    low = ADCL;
    high = ADCH;
    adcValue = (high << 8) | low;
    conversionFlag = 1;
    fnqueue_add(ProcesarAdc);
}

void key_down_callback(void (*handler)(), int tecla){
  switch(tecla){
    case TECLA_UP:
      handler_key_down_tecla_up = handler;
      break;
    case TECLA_DOWN:
      handler_key_down_tecla_down = handler;
      break;
    case TECLA_LEFT:
      handler_key_down_tecla_left = handler;
      break;
    case TECLA_RIGHT:
      handler_key_down_tecla_right = handler;
      break;
    case TECLA_SELECT:
      handler_key_down_tecla_select = handler;
      break;
  
  }
}

void key_up_callback(void (*handler)(), int tecla){
  switch(tecla){
    case TECLA_UP:
      handler_key_up_tecla_up = handler;
      break;
    case TECLA_DOWN:
      handler_key_up_tecla_down = handler;
      break;
    case TECLA_LEFT:
      handler_key_up_tecla_left = handler;
      break;
    case TECLA_RIGHT:
      handler_key_up_tecla_right = handler;
      break;
    case TECLA_SELECT:
      handler_key_up_tecla_select = handler;
      break;

  }
}
