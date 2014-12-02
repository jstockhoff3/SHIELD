
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <curl/curl.h>
#include "libs/shieldLib.hpp"

using namespace std;

int main(int argc, char *argv[]){
   if(argc != 3){
        cout << "usage: " << argv[0] << " <Door POE Name> <System Name> " << endl;
        cout << "\t <POE Name> -- The name of the POE attached to the MAGLOCK." << endl;
        cout << "\t <System Name> -- The name of the system." << endl;
        exit(1);
   }else{

      string POEName = string(argv[1]);
      string systemName = string(argv[2]);
      int locked,open,lastLocked,lastOpen,armed,alarm,lastArmed,lastAlarm;
      string errorMsg;
      lastLocked = -1;
      lastOpen = -1;
      lastArmed = -1;
      lastAlarm = -1;


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
          int res = checkPOEStatus(POEName,&locked,&open,&errorMsg);
          if(res){
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

        //CHECK THE SYSTEM STATUS (i.e. aremed/disarmed alarm active/passive)
        //makes a call to the database to retrieve the values, and 
        //updates the alarm/armed LEDs accordingly
        
          res = checkSystemStatus(systemName,&armed,&alarm);
          if(res){
            //success
            if(armed != lastArmed || alarm != lastAlarm){
              cout << systemName << " has the following status: " << endl << "   Armed: " << armed << endl << "   Alarm: " << alarm << endl;
            }
          }else{
            //failure
            cout << "the function 'checkPOEStatus()' has failed with the following error:" << endl << "  " << errorMsg << endl;
          }  
          
        lastArmed = armed;
        lastAlarm = alarm;

        }
  }

  return 0;
}