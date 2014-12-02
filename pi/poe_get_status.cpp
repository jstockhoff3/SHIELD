
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <curl/curl.h>
#include "libs/shieldHttpActionsLib.hpp"

using namespace std;

int main(int argc, char *argv[]){
   if(argc != 2){
        cout << "usage: " << argv[0] << " <POE Name> " << endl;
        cout << "\t <POE Name> -- The name of the POE that you for which you wish to know the status." << endl;
        exit(1);
   }else{

        string POEName = string(argv[1]);
        int locked,open;
        string errorMsg;
        int res = checkPOEStatus(POEName,&locked,&open,&errorMsg);
        if(res){
          //success
          cout << POEName << " has the following status: " << endl << "   Locked: " << locked << endl << "   Open: " << open << endl;
        }else{
          //failure
          cout << "the function 'checkPOEStatus()' has failed with the following error:" << endl << "  " << errorMsg << endl;
        }  
  
        if(lockPOE(POEName))
          cout << "POE Locked" << endl;
        else
          cout << "Lock Failed" << endl;

        res = checkPOEStatus(POEName,&locked,&open,&errorMsg);
        if(res){
          //success
          cout << POEName << " has the following status: " << endl << "   Locked: " << locked << endl << "   Open: " << open << endl;
        }else{
          //failure
          cout << "the function 'checkPOEStatus()' has failed with the following error:" << endl << "  " << errorMsg << endl;
        }  

        if(unlockPOE(POEName))
          cout << "POE Unlocked" << endl;
        else
          cout << "Unlock Failed" << endl;

        res = checkPOEStatus(POEName,&locked,&open,&errorMsg);
        if(res){
          //success
          cout << POEName << " has the following status: " << endl << "   Locked: " << locked << endl << "   Open: " << open << endl;
        }else{
          //failure
          cout << "the function 'checkPOEStatus()' has failed with the following error:" << endl << "  " << errorMsg << endl;
        }  

        if(openPOE(POEName))
          cout << "POE Opened" << endl;
        else
          cout << "Open Failed" << endl;

        res = checkPOEStatus(POEName,&locked,&open,&errorMsg);
        if(res){
          //success
          cout << POEName << " has the following status: " << endl << "   Locked: " << locked << endl << "   Open: " << open << endl;
        }else{
          //failure
          cout << "the function 'checkPOEStatus()' has failed with the following error:" << endl << "  " << errorMsg << endl;
        }  

        if(closePOE(POEName))
          cout << "POE Closed" << endl;
        else
          cout << "Close Failed" << endl;

        res = checkPOEStatus(POEName,&locked,&open,&errorMsg);
        if(res){
          //success
          cout << POEName << " has the following status: " << endl << "   Locked: " << locked << endl << "   Open: " << open << endl;
        }else{
          //failure
          cout << "the function 'checkPOEStatus()' has failed with the following error:" << endl << "  " << errorMsg << endl;
        }
  }

  return 0;
}