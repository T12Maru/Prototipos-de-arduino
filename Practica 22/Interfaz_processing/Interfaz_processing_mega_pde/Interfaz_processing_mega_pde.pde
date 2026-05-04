import processing.serial.*;

Serial myPort;

byte[] file;

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
  while(p.available() > 0){
    int data = p.read();
    print("Recibi: " + binary(data,8));
  }
}
