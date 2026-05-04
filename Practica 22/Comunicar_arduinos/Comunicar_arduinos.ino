#include <SoftwareSerial.h>

#define MAX_MSG 65535

// Variables globales para comunicación del arduino
const int rxpin = 2;
const int txpin = 3;

// Variables que tienen que ver con el protocolo de comunicación
const byte H = 'H';
const byte F = 'F';

bool recibiendo_mensaje = false;
bool conozco_tamanio = false;
byte [MAX_MSG] buffer; 

int estado = 0;
unsigned int size = 0;
unsigned int leidos = 0;


SoftwareSerial serialArduino(rxpin, txpin,true);

void setup() {
  Serial.begin(9600);
  serialArduino.begin(9600);
}


void enviarArduino(unsigned int size){
  serialArduino.write(H);

  // bits mas significativos a menos significativos
  serialArduino.write((size >> 8) & 0xFF);
  //bits menos significativos
  serialArduino.write(size & 0xFF);
  
  for(unsigned int i = 0 ; i < size ; i++){
    serialArduino.write(buffer[i]);
  }

  serialArduino.write(F);
}

void enviarPC(int size){
  // bits mas significativos a menos significativos
  serialArduino.write((size >> 8) & 0xFF);
  //bits menos significativos
  serialArduino.write(size & 0xFF);
  
  for(unsigned int i = 0 ; i < size ; i++){
    serialArduino.write(buffer[i]);
  }
}

void loop() {
  if(Serial.available() > 0){ // Si la computadora a la cuál estoy conectado me envió algo:
    switch(estado){
      case 0; // Esperando H
        byte b = Serial.read();
        if(b == H){
          estado = 1;
          size = 0;
          leidos = 0;
        }
        break;

      case 1; // Esperando el tamanio (corresponde a 2 bytes)
        if(Serial.available() >= 2){// nos aseguramos de leer solo cuando estén los 2 bits del tamaño...
          byte b = Serial.read();
          size = (b << 8);
          b = Serial.read();
          size |= b;
          estado = 2 ;
        }
        break;
      case 2; // esperamos el mensaje a transmitir
        while(Serial.available() > 0 && (leidos < size)){
          byte b = Serial.read();
          buffer[leidos++] = b;
        }

        if(leidos == size){
          estado = 3;
        }

        break;

      case 3; // esperamos la F 
        byte b = Serial.read();
        if(b == F){
          enviarArduino(size);
          estado = 0;
          size = 0;
          leidos = 0;
        }
        break;
    }
  }

  if(serialArduino.available() > 0){
    
    switch(estado){
      case 0; // Esperando H
        byte b = serialArduino.read();
        if(b == H){
          estado = 1;
          size = 0;
          leidos = 0;
        }
        break;

      case 1; // Esperando el tamanio (corresponde a 2 bytes)
        if(serialArduino.available() >= 2){// nos aseguramos de leer solo cuando estén los 2 bits del tamaño...
          byte b = serialArduino.read();
          size = (b << 8);
          b = serialArduino.read();
          size |= b;
          estado = 2 ;
        }
        break;
      case 2; // esperamos el mensaje a transmitir
        while(serialArduino.available() > 0 && (leidos < size)){
          byte b = serialArduino.read();
          buffer[leidos++] = b;
        }

        if(leidos == size){
          estado = 3;
        }

        break;

      case 3; // esperamos la F 
        byte b = serialArduino.read();
        if(b == F){
          enviarPC(size);
          estado = 0;
          size = 0;
          leidos = 0;
        }
        break;
    }
  }
}
