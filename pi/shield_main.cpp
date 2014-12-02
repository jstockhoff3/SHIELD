
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <curl/curl.h>
#include "libs/shieldLib.hpp"

using namespace std;

int main(int argc, char *argv[]){
   if(argc != 4){
        cout << "usage: " << argv[0] << " <Door POE Name> <System Name> <Sensor Name> " << endl;
        cout << "\t <POE Name> -- The name of the POE attached to the MAGLOCK." << endl;
        cout << "\t <System Name> -- The name of the system." << endl;
        cout << "\t <Sensor Name> -- The name of motion sensor in the database." << endl;
        exit(1);
   }else{

      string POEName = string(argv[1]);
      string systemName = string(argv[2]);
      string sensorName = string(argv[3]);

      int locked,open,lastLocked,lastOpen,armed,alarm,lastArmed,
          lastAlarm,motionStatus,lastMotionStatus,confirmedMotionStatus,
          lastConfirmedMotionStatus,motionStatCounter;

      string errorMsg;
      lastLocked = -1;
      lastOpen = -1;
      lastArmed = -1;
      lastAlarm = -1;
      lastMotionStatus = -1;
      lastConfirmedMotionStatus = -1;
      motionStatCounter = 0;
      int iter = 0;


      if(!wiringPiSetupGpio()){
        cout << "WiringPi Initialized" << endl;
      }else{
        cout << "WiringPi Failed to Initialize" << endl;
        return 0;
      }

      for(;;){

      //CHECK THE POE STATUS (i.e. locked/unlocked opened/closed)
      //makes a call to the database to retrieve the values, and 
      //updates the maglock accordingly

        if(!iter) cout << "Initial POE status check..." << endl;

        if(checkPOEStatus(POEName,&locked,&open,&errorMsg)){
          //success
          if(locked != lastLocked || open != lastOpen){
            cout << POEName << " has the following status: " << endl << "   Locked: " << locked << endl << "   Open: " << open << endl;
          }
        }else{
          //failure
          cout << "the function 'checkPOEStatus()' has failed with the following error:" << endl << "  " << errorMsg << endl;
        }  
        lastOpen = open;
        lastLocked = locked;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        //CHECK THE SYSTEM STATUS (i.e. aremed/disarmed alarm active/passive)
        //makes a call to the database to retrieve the values, and 
        //updates the alarm/armed LEDs accordingly
         
        if(!iter) cout << "Initial system status check on" << systemName << "..." << endl;

        if(checkSystemStatus(systemName,&armed,&alarm)){
          //success
          if(armed != lastArmed || alarm != lastAlarm){
            cout << systemName << " has the following status: " << endl << "   Armed: " << armed << endl << "   Alarm: " << alarm << endl;
          }
        }else{
          //failure
          cout << "the function 'checkSystemStatus()' has failed" << endl;
        }  
          
        lastArmed = armed;
        lastAlarm = alarm;


       ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //CHECK THE MOTION SENSOR AND UPDATE THE DATABASE ACCORDINGLY

        motionStatus = checkMotionStatus();

        if(!iter) cout << "Initial motion status check..." << endl;

        if(motionStatus == lastMotionStatus){
          motionStatCounter ++;
        }else{
          motionStatCounter = 0;
        }

        if(motionStatCounter == 10){
          confirmedMotionStatus = motionStatus;
          if(confirmedMotionStatus != lastConfirmedMotionStatus){
            if(setMotionStatus(sensorName, motionStatus)){
              cout << "Motion status set to: " << confirmedMotionStatus << endl;
            }else{
              cout << "ERROR: Motion status changed, but was not able to be set." << endl;
            }
          }
          lastConfirmedMotionStatus = confirmedMotionStatus;
        }

        lastMotionStatus = motionStatus;

        iter ++;
      }
  }

  return 0;
}