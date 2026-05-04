import processing.serial.*;

Serial myPort;

byte[] file;

byte[] buffer;
int size = 0; 
int leidos = 0;
int estado = 0;
int data = 0;

public static final short portIndex = 2;

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

  println("enviando: " + str(file.length));
  
  myPort.write('H');
  myPort.write((file.length >> 8) & 0xFF);
  myPort.write(file.length & 0xFF);
  
  for(int i = 0; i < file.length; i++){
    myPort.write(file[i]);
  }
  myPort.write('F');
}

void serialEvent(Serial p){
  if(p.available() > 0){
    switch(estado){
      case 0:
        if(p.available() >= 2){
          data = p.read();
          size = ((byte)data << 8);
          data = p.read();
          size |= (byte)data;
          
          leidos = 0;
          buffer = new byte[size];
          estado = 1;
        }
        break;
      case 1:
        while(p.available() > 0){
          data = p.read();
          buffer[leidos++] = (byte)data;
          println("Recibi: " + binary(data,8));
        }
        if(leidos == size){
          println("Guardando el archivo");
          saveBytes("recibido.bin",buffer);
          estado = 0;
          println("Archivo guardado, regresando al estado 0");
        }
        break;
    }
  }
}
