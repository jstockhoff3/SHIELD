#include <wiringPi.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "shieldHttpActionsLib.hpp"

#define MAGLOCK 4
#define MOTIONSENSOR 18
#define CONTACTSENSOR 0
#define ARMED 14
#define ALARM 15

using namespace std;

int checkPOEStatus(string POEname, int *locked, int *open, string *errorMsg);
int lockPOE(string POEname);
int unlockPOE(string POEname);
int openPOE(string POEname);
int closePOE(string POEName);
int toggleSystemArmed(string systemName);
int checkSystemStatus(string systemName, int* armed, int* alarmActive);
void setGPIOpin(int pinNum, int value);
int readGPIOpin(int pinNum);
int setMotionStatus(string sensorName, string motionStatStr);
int checkMotionStatus(void);

void setGPIOpin(int pinNum, int value){
		pinMode(pinNum,OUTPUT);
		if(value){
			digitalWrite(pinNum,HIGH);
		}else{
			digitalWrite(pinNum,LOW);
		}
}

int readGPIOpin(int pinNum){
		pinMode(pinNum,INPUT);
		if(digitalRead(pinNum) == HIGH){
			return 1;
		}else{
			return 0;
		}
}

//Make a call to the API to check the status of a POE and update the locked/open variables accordingly
int checkPOEStatus(string POEname, int *locked, int *open, string *errorMsg){

  string rawData;
  json_t *root, *theData;
  json_error_t error;
  int returnVal = 0;

  string URL = "http://192.168.1.3/poe/getPOE.php?poeName=" + POEname;

  rawData = http_GET(URL);
  root = json_loads(rawData.c_str(), 0, &error);

  if(isSuccess(rawData,errorMsg)){
    theData = json_array_get(root, 0);
    if(json_is_object(theData)){
      *locked = !strcmp(json_string_value(json_object_get(theData,"Locked")),"1");
      if(*locked){
      	setGPIOpin(MAGLOCK,0);
      }else{
      	setGPIOpin(MAGLOCK,1);
      }
      *open = !strcmp(json_string_value(json_object_get(theData,"Open")),"1");
      returnVal = 1;
    }else{
      cout << "returned data JSON is not object" << endl;
    }
  }else{
    cout << "isSuccess has failed" << endl;
  }
  json_decref(root);
  return returnVal;
}


//lock a POE by name: updates the DB and locks the maglock
int lockPOE(string POEname){
  string *errorMsg;
  string URL = "http://192.168.1.3/poe/setPOE.php?entryName="
               + POEname + "&action=lock";
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
  	setGPIOpin(MAGLOCK,0);
    return 1;
  }else{
    return 0;
  }
}

//unlock a POE by name: updates the DB and unlocks the maglock
int unlockPOE(string POEname){
  string *errorMsg;
  string URL = "http://192.168.1.3/poe/setPOE.php?entryName="
               + POEname + "&action=unlock";
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
  	setGPIOpin(MAGLOCK,1);
    return 1;
  }else{
    return 0;
  }
}

//set POE 'open' value in DB to 1
int openPOE(string POEname){
  string *errorMsg;
  string URL = "http://192.168.1.3/poe/setPOE.php?entryName=" 
                + POEname + "&action=open";
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
    return 1;
  }else{
    return 0;
  }
}

//set POE 'open' value in DB to 0
int closePOE(string POEname){
  string *errorMsg;
  string URL = "http://192.168.1.3/poe/setPOE.php?entryName=" 
                + POEname + "&action=close";
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
    return 1;
  }else{
    return 0;
  }
}

int toggleSystemArmed(string systemName){
  string *errorMsg;
  string URL = "http://192.168.1.3/motion/setMotionStatus.php?systemName=" + systemName;
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
    return 1;
  }else{
  	cout << "toggleSystemArmed failed with: " << errorMsg << endl;
    return 0;
  }
}

int checkSystemStatus(string systemName, int* armed, int* alarmActive){
  string rawData;
  json_t *root, *theData;
  json_error_t error;
  int returnVal = 0;
  string *errorMsg;
  string URL = "http://192.168.1.3/system/getSystemStatus.php?systemName=" + systemName;

  rawData = http_GET(URL);
 
  root = json_loads(rawData.c_str(), 0, &error);

  if(isSuccess(rawData,errorMsg)){
    theData = json_array_get(root, 0);
    if(json_is_object(theData)){
      *armed = !strcmp(json_string_value(json_object_get(theData,"Armed")),"1");
      if(*armed){
      	setGPIOpin(ARMED,1);
      }else{
      	setGPIOpin(ARMED,0);
      }
      *alarmActive = !strcmp(json_string_value(json_object_get(theData,"AlarmActive")),"1");
      if(*alarmActive){
      	setGPIOpin(ALARM,1);
      }else{
      	setGPIOpin(ALARM,0);
      }
      returnVal = 1;
    }else{
      cout << "returned data JSON is not object" << endl;
    }
  }else{
    cout << "isSuccess has failed" << endl;
  }

  json_decref(root);
  return returnVal;
}

int setMotionStatus(string sensorName, int motionStatus){
  string *errorMsg;
  string motionStatStr;

  if(motionStatus){
  	motionStatStr = "1";
  }else{
  	motionStatStr = "0";
  }

  string URL = "http://192.168.1.3/motion/setMotionStatus.php?sensorName="
               + sensorName + "&motionFound=" + motionStatStr;
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
    return 1;
  }else{
  	cout << "Error While Setting Motion Status: " << errorMsg << endl;
    return 0;
  }
}

int checkMotionStatus(void){
  if(readGPIOpin(MOTIONSENSOR) == HIGH){
  	return 1;
  }else{
  	return 0;
  }
}
