
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
        cout << "usage: " << argv[0] << " <URL> " << endl;
        cout << "\t <URL> -- The URL from which to fetch the JSON data" << endl;
        exit(1);
   }else{

        string URL = string(argv[1]);
        int res = janssonTest(URL);

        if(res){
          //success
          cout << "the function 'janssonTest()' has succeeded." << endl;

        }else{
          //failure
          cout << "the function 'janssonTest()' has failed." << endl;
        }  
  }
  return 0;
}