#include <SoftwareSerial.h>
#include <TinyGPS++.h>
TinyGPSPlus gps;
SoftwareSerial serial_neo(6, 7); // RX, TX
SoftwareSerial mySerial(9, 10); // RX, TX
String Write_API_key = "OMITIDO_PARA_APENDICE";
String lat_long;
float latitude = 99;
float longitude;
String longitude_maior;
String latitude_maior;
String date_time;
void setup() {
 //Baud rate do objeto serial
 Serial.begin(9600);
 // Definindo baud rate dos objetos da software serial
 serial_neo.begin(9600);
 mySerial.begin(9600);
 Serial.println("Iniciando GPS");
}
void loop() { //Loop principal do programa
 serial_neo.listen();
 //Serial.print(latitude);
 //Serial.print(longitude);
 Serial.print(latitude_maior);
 Serial.print(longitude_maior);
 if(latitude == 99){
 while (serial_neo.available() > 0){ // While de aquisição da posição
 if (gps.encode(serial_neo.read())){
 sinalGPS();
 }
 if (latitude != 99){ //Quando a latitude é populada pelo valor correto rodamos a
rotina de enviar o dado para o servidor
 mySerial.listen();
 if (Serial.available()>0)
 delay(1000);
 SetupModule();
 mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
 delay(3000);
 MostraSerial();
 mySerial.println("AT+CIPSEND");
42
 delay(2000);
 Serial.println();
 MostraSerial();
 String str="GET
https://api.thingspeak.com/update?api_key="+Write_API_key+"&field2="+latitude_ma
ior+"&field3="+longitude_maior+"&field4="+date_time+"&field5=https://maps.google.c
om/?q="+latitude_maior+","+longitude_maior;
 Serial.println(str); delay(2000);
 mySerial.println(str); delay(4000);
 MostraSerial();
 mySerial.println((char)26); delay(4000);
 mySerial.println();
 MostraSerial();
 mySerial.println("AT+CIPSHUT");//close the connection
 delay(500);
 MostraSerial();
 str="";
 Serial.print("Vou entrar no delay");
 delay(180000); // Rodando o loop a cada 3 minutos
 Serial.print("Passei delay");
 latitude = 99;
 break;
 }
 }
 }
 Serial.println();

 } // Fim do loop principal do programa

void sinalGPS(){
 if (gps.location.isValid()){
 latitude = gps.location.lat();
 longitude= gps.location.lng();
 String lat_long = "";
 latitude_maior = String(latitude,6);
 longitude_maior = String(longitude,6);
 lat_long += latitude_maior;
 lat_long += longitude_maior;
 Serial.print(gps.location.lat(), 6);
 }else{
 Serial.print(F("INVALID"));
 }

 if (gps.date.isValid()){
 date_time = "";
 date_time += gps.date.day();
 //date_time += "/";
 date_time += gps.date.month();
 //date_time += "/";
 date_time += gps.date.year();
43
 //date_time += " ";
 }else{
 Serial.print(F("INVALID"));
 }

 Serial.print(F(" "));
 if (gps.time.isValid()){
 date_time += gps.time.hour();
 //date_time += ":";
 date_time += gps.time.minute();
 //date_time += ":";
 date_time += gps.time.second();
 Serial.print(date_time);
 }else{
 Serial.print(F("INVALID"));
 }
 Serial.println();
}
void SetupModule(){
 if (mySerial.available())Serial.write(mySerial.read());
 mySerial.println("AT"); delay(1000);
 mySerial.println("AT+CPIN?"); delay(1000);
 mySerial.println("AT+CREG?"); delay(1000);
 mySerial.println("AT+CGATT?"); delay(1000);
 mySerial.println("AT+CIPSHUT");delay(1000);
 mySerial.println("AT+CIPSTATUS"); delay(2000);
 mySerial.println("AT+CIPMUX=0"); delay(2000);

 //setting the APN,
 mySerial.println("AT+CSTT=\"timbrasil.br\",\"tim\",\"tim\""); delay(1000);
 MostraSerial();
 mySerial.println("AT+CIICR"); delay(2000);
 MostraSerial();
 //get local IP adress
 mySerial.println("AT+CIFSR"); delay(2000);
 MostraSerial();

 mySerial.println("AT+CIPSPRT=0");delay(2000);
 MostraSerial();
 }
void MostraSerial(){
 while(mySerial.available()!=0)
 Serial.write(mySerial.read());
 delay(2000);
