/************************************************************* 
 *Author: Jacob Gilsdorf (prompt-laser)
 *Version: V 0.1
 *Last Edit(D.M.Y): 20.11.2023
 *Description: Library for interfacing with an DFRobot SEN0366
 *             laser rangefinder  
 *************************************************************/
#include <SoftwareSerial.h>

class SEN0366{
 private:
  char _address;
  int _tx;
  int _rx;
 
 public:
  
  SEN0366(int tx, int rx){
   _tx = tx;
   _rx = rx;
   _address = 0x080;
  }
  
  SEN0366(int tx, int rx, char address){
	  _tx = tx;
	  _rx = rx;
	  _address = address;
  }
  
  bool LaserOn(){
   SoftwareSerial _serial(_tx,_rx);
   _serial.begin(9600);
   unsigned char _check = 0;
   _serial.write(_address);
   _check += _address;
   _serial.write(0x06);
   _check += 0x06;
   _serial.write(0x05);
   _check += 0x05;
   _serial.write(0x01);
   _check += 0x01;
   _serial.write(CalculateCheckByte(_check));
   delay(50);
   if(_serial.available() > 0){
	   _check = 0;
    unsigned char buff[5] = {0};
	for(int i = 0; i < 4; i++){
		buff[i] = _serial.read();
		_check += buff[i];
	}
	
	if(_serial.read() == CalculateCheckByte(_check) && buff[3] == 0x01){
	 return true;
	}else{
     return false;
	}
   }else{
    return false;
   }
  }
  
  bool LaserOff(){
   SoftwareSerial _serial(_tx,_rx);
   _serial.begin(9600);
   unsigned char _check = 0;
   _serial.write(_address);
   _check += _address;
   _serial.write(0x06);
   _check += 0x06;
   _serial.write(0x05);
   _check += 0x05;
   _serial.write((char)0x00);
   _check += 0x00;
   _serial.write(CalculateCheckByte(_check));
   delay(50);
   if(_serial.available() > 0){
	   _check = 0;
    unsigned char buff[5] = {0};
	for(int i = 0; i < 4; i++){
		buff[i] = _serial.read();
		_check += buff[i];
	}
	
	if(_serial.read() == CalculateCheckByte(_check) && buff[3] == 0x01){
	 return true;
	}else{
     return false;
	}
   }else{
    return false;
   }
  }
  
  float SingleMeasurement_1MM(){
   SetResolutionTo1MM();
   SoftwareSerial _serial(_tx,_rx);
   _serial.begin(9600);
   unsigned char _check = 0;
   _serial.write(_address);
   _check += _address;
   _serial.write(0x06);
   _check += 0x06;
   _serial.write(0x02);
   _check += 0x02;
   _serial.write(CalculateCheckByte(_check));
   while(_serial.available() == 0){
   }
   delay(50);
   _check = 0;
   unsigned char buff[11] = {0};
   int c = 0;
   while(_serial.available() > 0 && c < 11){
	   buff[c] = _serial.read();
	   c++;
   }
   for(int i = 0; i<10; i++){
	   _check += buff[i];
   }
   unsigned char _returnedCheckByte = buff[10];
   if(_returnedCheckByte == CalculateCheckByte(_check)){
	float returnVal = 0;
	returnVal += (buff[3] - 0x30) * 100;
	returnVal += (buff[4] - 0x30) * 10;
	returnVal += (buff[5] - 0x30) * 1;
	returnVal += (buff[7] - 0x30) * .1;
	returnVal += (buff[8] - 0x30) * .01;
	returnVal += (buff[9] - 0x30) * .001;
	return returnVal;
   }else{
    return 999.999;
   }
  }
  
  float SingleMeasurement_Point1MM(){
   SetResolutionToPoint1MM();
   SoftwareSerial _serial(_tx,_rx);
   _serial.begin(9600);
   unsigned char _check = 0;
   _serial.write(_address);
   _check += _address;
   _serial.write(0x06);
   _check += 0x06;
   _serial.write(0x02);
   _check += 0x02;
   _serial.write(CalculateCheckByte(_check));
   while(_serial.available() == 0){
   }
   delay(50);
   _check = 0;
   unsigned char buff[12] = {0};
   int c = 0;
   while(_serial.available() > 0 && c < 12){
	   buff[c] = _serial.read();
	   c++;
   }
   for(int i = 0; i<11; i++){
	   _check += buff[i];
   }
   unsigned char _returnedCheckByte = buff[11];
   if(_returnedCheckByte == CalculateCheckByte(_check)){
	float returnVal = 0;
	returnVal += (buff[3] - 0x30) * 100;
	returnVal += (buff[4] - 0x30) * 10;
	returnVal += (buff[5] - 0x30) * 1;
	returnVal += (buff[7] - 0x30) * .1;
	returnVal += (buff[8] - 0x30) * .01;
	returnVal += (buff[9] - 0x30) * .001;
	returnVal += (buff[10] - 0x30) * .0001;
	return returnVal;
   }else{
    return 999.999;
   }
  }
	
  bool SetResolutionTo1MM(){
   SoftwareSerial _serial(_tx,_rx);
   _serial.begin(9600);
   unsigned char _check = 0;
   _serial.write(0xFA);
   _check += 0xFA;
   _serial.write(0x04);
   _check += 0x04;
   _serial.write(0x0C);
   _check += 0x0C;
   _serial.write(0x01);
   _check += 0x01;   
   _serial.write(CalculateCheckByte(_check));
   delay(50);
   if(_serial.available() > 0){
	   _check = 0;
    unsigned char buff[4] = {0};
	for(int i = 0; i < 3; i++){
		buff[i] = _serial.read();
		_check += buff[i];
	}
	
	if(_serial.read() == CalculateCheckByte(_check) && buff[1] == 0x04){
	 return true;
	}else{
     return false;
	}
   }else{
    return false;
   }
  }
  
  bool SetResolutionToPoint1MM(){
   SoftwareSerial _serial(_tx,_rx);
   _serial.begin(9600);
   unsigned char _check = 0;
   _serial.write(0xFA);
   _check += 0xFA;
   _serial.write(0x04);
   _check += 0x04;
   _serial.write(0x0C);
   _check += 0x0C;
   _serial.write(0x02);
   _check += 0x01;   
   _serial.write(CalculateCheckByte(_check));
   delay(50);
   if(_serial.available() > 0){
	   _check = 0;
    unsigned char buff[4] = {0};
	for(int i = 0; i < 3; i++){
		buff[i] = _serial.read();
		_check += buff[i];
	}
	
	if(_serial.read() == CalculateCheckByte(_check) && buff[1] == 0x04){
	 return true;
	}else{
     return false;
	}
   }else{
    return false;
   }
  }
		  
  unsigned char CalculateCheckByte(unsigned char _check){
	  return ~_check + 1;
  }
};