#include <SoftwareSerial.h>

// Variables globales para comunicación del arduino
const int rxpin = 2;
const int txpin = 3;

// Variables que tienen que ver con el protocolo de comunicación
const byte H = 'H';
const byte F = 'F';

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
    unsigned long start = millis();

    while(!Serial.available()){//si no ha terminado de llegar los datos
      if(millis() - start > 5000){ // 5 segundos timeout por byte
      Serial.println("#Timeout leyendo datos");
      return;
  }
    } 
    byte b = Serial.read();
    serialArduino.write(b);
  }

  serialArduino.write(F);
}

void enviarPC(byte b){
  switch(estado){
      case 0: // El automata espera H
        if(b == H){
          estado = 1;
          size = 0;
          leidos = 0;
        }
        break;
      
      case 1: // El automata espera el tamaño de byte de los datos a recibir
        size = (size << 8 ) | b;
        leidos++;
        
        // cuando se leyeron los 2 bytes de tamaño, seguimos con el siguiente estado
        if(leidos == 2){
          leidos = 0;
          estado = 2;
        }
        break;
        
      case 2: // El automata retransmite los datos recibidos
        Serial.write(b);
        leidos++;
        
        if(leidos == size){
          estado = 3;
        }
        break;

      case 3: // El automata espera la terminación del mensaje
        if(b == F){
          Serial.println("#Archivo recibido completo");
      }
      estado = 0;
      break;
    }
}

void loop() {
  if(Serial.available() >= 2){
    Serial.println("#e recibio el mensaje de processing en el arduino UNO\n");
    byte b = Serial.read();
    size = (b << 8);
    b = Serial.read();
    size |= b;

    Serial.println("se enviara el mensaje de processing con el tamaño");
    Serial.println(size);
    enviarArduino(size);
    Serial.println("se transmitio el mensaje de processing al arduino mega");
  }

  if(serialArduino.available() > 0){
    byte b = serialArduino.read();

    enviarPC(b);
  }
}
