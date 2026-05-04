#include <SoftwareSerial.h>

#define MAX_SIZE 32

// Variables globales para comunicación del arduino
const int rxpin = 2;
const int txpin = 3;

// Variables que tienen que ver con el protocolo de comunicación
const byte H = 'H';
const byte F = 'F';
byte b;
byte buffer[MAX_SIZE]; 

unsigned int estado_miPC = 0;
unsigned int estado_arduino = 0;
unsigned int size = 0;
unsigned int id = 0;
unsigned int leidos = 0;


SoftwareSerial serialArduino(rxpin, txpin,true);

void setup() {
  Serial.begin(9600);
  serialArduino.begin(9600);
}


void enviarArduino(unsigned int size){
  serialArduino.write(H);
  serialArduino.write(id);

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
  Serial.write(id);
  Serial.write((size >> 8) & 0xFF);
  //bits menos significativos
  Serial.write(size & 0xFF);
  
  for(unsigned int i = 0 ; i < size ; i++){
    Serial.write(buffer[i]);
  }
}

void loop() {
  if(Serial.available() > 0){ // Si la computadora a la cuál estoy conectado me envió algo:
    switch(estado_miPC){
      case 0: // Esperando H
        b = Serial.read();
        if(b == H){
          estado_miPC = 1;
          size = 0;
          leidos = 0;
          serialArduino.write(255);
        }
        break;

      case 1: // esperando el identificador de la pc
        if(Serial.available() > 0){
          id = Serial.read();
          estado_miPC = 2 ;
        }
        break;

      case 2: // Esperando el tamanio (corresponde a 2 bytes)
        if(Serial.available() >= 2){// nos aseguramos de leer solo cuando estén los 2 bits del tamaño...
          b = Serial.read();
          size = (b << 8);
          b = Serial.read();
          size |= b;
          estado_miPC = 3;
        }
        break;
      case 3: // esperamos el mensaje a transmitir
        while(Serial.available() > 0 && (leidos < size)){
          b = Serial.read();
          buffer[leidos++] = b;
        }

        if(leidos == size){
          estado_miPC = 4;
        }

        break;

      case 4: // esperamos la F 
        b = Serial.read();
        if(b == F){
          enviarArduino(size);
          estado_miPC = 0;
          size = 0;
          id = 0;
          leidos = 0;
        }
        break;
    }
  }

  if(serialArduino.available() > 0){
    
    switch(estado_arduino){
      case 0: // Esperando 255 (Un pc esta usando el canal)
        b = serialArduino.read();
        if(b == 255){
          estado_arduino = 1;
          Serial.write(255);
        }
        break;

      case 1: // Esperando H
        b = serialArduino.read();
        if(b == H){
          estado_arduino = 2;
          size = 0;
          leidos = 0;
        }
        break;

      case 2:
        id = serialArduino.read();
        estado_arduino = 3;
        break;

      case 3: // Esperando el tamanio (corresponde a 2 bytes)
        if(serialArduino.available() >= 2){// nos aseguramos de leer solo cuando estén los 2 bits del tamaño...
          b = serialArduino.read();
          size = (b << 8);
          b = serialArduino.read();
          size |= b;
          estado_arduino = 4 ;
        }
        break;
      case 4: // esperamos el mensaje a transmitir
        while(serialArduino.available() > 0 && (leidos < size)){
          b = serialArduino.read();
          buffer[leidos++] = b;
        }

        if(leidos == size){
          estado_arduino = 5;
        }

        break;

      case 5: // esperamos la F 
        b = serialArduino.read();
        if(b == F){
          enviarPC(size);
          estado_arduino = 0;
          size = 0;
          leidos = 0;
        }
        break;
    }
  }
}
