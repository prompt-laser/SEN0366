/************************************************************* 
 *Author: Jacob Gilsdorf (prompt-laser)
 *Version: V 0.1.5
 *Last Edit(D.M.Y): 20.04.2024
 *Description: Library for interfacing with an DFRobot SEN0366
 *             laser rangefinder  
 *************************************************************/
#include <SoftwareSerial.h>

class SEN0366{
 private:
  char _address;
  unsigned char _check;
  unsigned char buff[12];
  float _returnVal;
  SoftwareSerial _serial = SoftwareSerial(2,3);
 
 public:
  
  SEN0366(int tx, int rx){
   _serial = SoftwareSerial(tx,rx);
   _serial.begin(9600);
   _address = 0x080;
  }
  
  SEN0366(int tx, int rx, char address){
   _serial = SoftwareSerial(tx,rx);
   _serial.begin(9600);
   _address = address;
  }
  
  bool LaserOn(){
   _check = 0;
   _serial.write(_address);
   _check += _address;
   _serial.write(0x06);
   _check += 0x06;
   _serial.write(0x05);
   _check += 0x05;
   _serial.write(0x01);
   _check += 0x01;
   _serial.write((unsigned char)~_check + 1);
   delay(50);
   if(_serial.available() > 0){
	_check = 0;
	ClearBuff();
	for(int i = 0; i < 5; i++){
		buff[i] = _serial.read();
	}
	for(int i = 0; i < 4; i++){
	 _check += buff[i];
	}
	
	if(buff[4] == (unsigned char)~_check + 1 && buff[3] == 0x01){
	 return true;
	}else{
     return false;
	}
   }else{
    return false;
   }
  }
  
  bool LaserOff(){
   _check = 0;
   _serial.write(_address);
   _check += _address;
   _serial.write(0x06);
   _check += 0x06;
   _serial.write(0x05);
   _check += 0x05;
   _serial.write((char)0x00);
   _check += 0x00;
   _serial.write((unsigned char)~_check + 1);
   delay(50);
   if(_serial.available() > 0){
	   _check = 0;
	ClearBuff();
	for(int i = 0; i < 5; i++){
		buff[i] = _serial.read();
	}
	for(int i = 0; i < 4; i++){
	 _check += buff[i];
	}
	
	if(buff[4] == (unsigned char)~_check + 1 && buff[3] == 0x01){
	 return true;
	}else{
     return false;
	}
   }else{
    return false;
   }
  }
  
  bool Shutdown(){
   _check = 0;
   _serial.write(_address);
   _check += _address;
   _serial.write(0x04);
   _check += 0x04;
   _serial.write(0x02);
   _check += 0x02;
   _serial.write((unsigned char)~_check + 1);
  }
  
  float SingleMeasurement_1MM(){
   SetResolutionTo1MM();
   _check = 0;
   _serial.write(_address);
   _check += _address;
   _serial.write(0x06);
   _check += 0x06;
   _serial.write(0x02);
   _check += 0x02;
   _serial.write((unsigned char)~_check + 1);
   while(_serial.available() == 0){
   }
   delay(50);
   _check = 0;
   ClearBuff();
   int c = 0;
   while(_serial.available() > 0 && c < 11){
	   buff[c] = _serial.read();
	   c++;
   }
   for(int i = 0; i<=9; i++){
	   _check += buff[i];
   }
   if(buff[10] == (unsigned char)~_check + 1){
	_returnVal = 0;
	_returnVal += (buff[3] - 0x30) * 100;
	_returnVal += (buff[4] - 0x30) * 10;
	_returnVal += (buff[5] - 0x30) * 1;
	_returnVal += (buff[7] - 0x30) * .1;
	_returnVal += (buff[8] - 0x30) * .01;
	_returnVal += (buff[9] - 0x30) * .001;
	return _returnVal;
   }else{
    return 999.999;
   }
  }
  
  float SingleMeasurement_Point1MM(){
   SetResolutionToPoint1MM();
   _check = 0x00;
   _serial.write(_address);
   _check += _address;
   _serial.write(0x06);
   _check += 0x06;
   _serial.write(0x02);
   _check += 0x02;
   _serial.write((unsigned char)~_check + 1);
   while(_serial.available() == 0){
   }
   delay(50);
   _check = 0x00;
   ClearBuff();
   int c = 0;
   while(_serial.available() > 0 && c < 12){
	   buff[c] = _serial.read();
	   c++;
   }
   for(int i = 0; i<11; i++){
	   _check += buff[i];
   }
   if(buff[11] == (unsigned char)~_check + 1){
	_returnVal = 0;
	_returnVal += (buff[3] - 0x30) * 100;
	_returnVal += (buff[4] - 0x30) * 10;
	_returnVal += (buff[5] - 0x30) * 1;
	_returnVal += (buff[7] - 0x30) * .1;
	_returnVal += (buff[8] - 0x30) * .01;
	_returnVal += (buff[9] - 0x30) * .001;
	_returnVal += (buff[10] - 0x30) * .0001;
	return _returnVal;
   }else{
    return 999.999;
   }
  }
	
  bool SetResolutionTo1MM(){
   _check = 0;
   _serial.write(0xFA);
   _check += 0xFA;
   _serial.write(0x04);
   _check += 0x04;
   _serial.write(0x0C);
   _check += 0x0C;
   _serial.write(0x01);
   _check += 0x01;   
   _serial.write((unsigned char)~_check + 1);
   delay(50);
   if(_serial.available() > 0){
	_check = 0;
	ClearBuff();
	for(int i = 0; i < 4; i++){
	 buff[i] = _serial.read();
	}
	for(int i = 0; i<3; i++){
	 _check += buff[i];
	}
	
	if(buff[3] == (unsigned char)~_check + 1 && buff[1] == 0x04){
	 return true;
	}else{
     return false;
	}
   }else{
    return false;
   }
  }
  
  bool SetResolutionToPoint1MM(){
   _check = 0;
   _serial.write(0xFA);
   _check += 0xFA;
   _serial.write(0x04);
   _check += 0x04;
   _serial.write(0x0C);
   _check += 0x0C;
   _serial.write(0x02);
   _check += 0x02;   
   _serial.write((unsigned char)~_check + 1);
   delay(50);
   if(_serial.available() > 0){
	_check = 0;
	ClearBuff();
	for(int i = 0; i < 3; i++){
		buff[i] = _serial.read();
		_check += buff[i];
	}
	
	if(_serial.read() == (unsigned char)~_check + 1 && buff[1] == 0x04){
	 return true;
	}else{
     return false;
	}
   }else{
    return false;
   }
  }
  
  void ClearBuff(){
   for(int i=0; i<12; i++){
    buff[i] = 0;
   }
  }
};
