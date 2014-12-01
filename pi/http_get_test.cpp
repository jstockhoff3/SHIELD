
#include <stdio.h>
#include <string.h>
 
#include <curl/curl.h>
#include "libs/shieldHttpActionsLib.hpp"

int main(int argc, char *argv[])
{
   if (argc != 2) {
        cout << "usage: " << argv[0] << " <URL> " << endl;
        cout << "\t <URL> -- The URL from which to fetch the JSON data" << endl;
        exit(1);
    }else{

        string URL = string(argv[1]);
        int res = janssonTest(URL);

        if(res){
          //success
          COUT << "the function 'janssonTest()' has succeeded." << endl;

        }else
          //failure
          COUT << "the function 'janssonTest()' has failed." << endl;
        } 

    }

  return 0;
}