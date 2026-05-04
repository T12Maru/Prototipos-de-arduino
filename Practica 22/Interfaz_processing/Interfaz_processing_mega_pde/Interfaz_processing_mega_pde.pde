import processing.serial.*;

Serial myPort;

byte[] file;

byte[] buffer;
int estado = 0;
int size = 0;
int leidos = 0;

public static final short portIndex = 0;

void setup(){
  println(Serial.list());
  println("Conectando a ->" + Serial.list()[portIndex]);
  myPort = new Serial(this,Serial.list()[portIndex],9600);
  
  file = loadBytes("b.bin");
  
  println("Escribe X para enviar el archivo");
}

void draw(){
}

void keyPressed(){
  if(key == 'X' || key == 'x'){
    enviar_archivo(file);
  }
}

void enviar_archivo(byte [] file){  
  println("Enviando archivo...");
  
  myPort.write((file.length >> 8) & 0xFF);
  myPort.write(file.length & 0xFF);
  
  for(int i = 0; i < file.length; i++){
    myPort.write(file[i]);
  }
}

void serialEvent(Serial p){
  while(p.available() > 0){
    byte b = (byte)p.read();

    switch(estado){

      case 0:
        size = (b & 0xFF) << 8;
        estado = 1;
        break;

      case 1:
        size |= (b & 0xFF);
        
        if(size <= 0 || size > 4096){
          println("Tamaño inválido: " + size);
          estado = 0;
          leidos = 0;
          size = 0;
          break;
        }
        
        buffer = new byte[size];
        leidos = 0;
        estado = 2;
        break;

      case 2:
        buffer[leidos++] = b;

        if(leidos == size){
          println("Archivo recibido");
          saveBytes("recibido.bin", buffer);
          estado = 0;
        }
        break;
    }
  }
}
