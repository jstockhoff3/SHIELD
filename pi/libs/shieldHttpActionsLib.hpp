#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include <iostream>
#include <fstream>

using namespace std;

void uploadImage(char* imagePath); 
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);
string http_GET(string URL);
int checkPOEStatus(string POEname, int *locked, int *open, string *errorMsg);
int isSuccess(string data, string *errorMsg);
int lockPOE(string POEname);
int unlockPOE(string POEname);
int openPOE(string POEname);
int closePOE(string POEName);
int toggleSystemArmed(string systemName);
int checkSystemStatus(string systemName, int* armed, int* alarmActive);
int setMotionStatus(string sensorName, int motionStatus);

string callBackData; //will hold the contents of the url

void uploadImage(char* imagePath){
  CURL *curl;
  CURLcode res;
 
  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* Fill in the file upload field */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "fileToUpload",
               CURLFORM_FILE, imagePath,
               CURLFORM_END);
 
  /* Fill in the filename field */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename",
               CURLFORM_COPYCONTENTS, imagePath,
               CURLFORM_END);
 
 
  /* Fill in the submit field too, even if this is rarely needed */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "submit",
               CURLFORM_COPYCONTENTS, "send",
               CURLFORM_END);
 
  curl = curl_easy_init();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */ 
  headerlist = curl_slist_append(headerlist, buf);
  if(curl) {
    /* what URL that receives this POST */ 
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.3/upload.php");
      /* only disable 100-continue header if explicitly requested */ 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    char *errorBuffer = new char [1000];
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK){
      cout <<  "curl_easy_perform() failed:" << errorBuffer << endl;
    }else{
      cout << "Successfully uploaded" << endl;
    }
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 
    /* then cleanup the formpost chain */ 
    curl_formfree(formpost);
    /* free slist */ 
    curl_slist_free_all (headerlist);
  }
}

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up){ 

    //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        callBackData.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}


string http_GET(string URL){

  CURL *curl;
  CURLcode res;
  string result;

  curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
  curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
  //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

  curl_easy_perform(curl);

  result = callBackData;
  callBackData.clear();

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return result;
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


int isSuccess(string data, string *errorMsg){

  json_t *successData, *success, *root;
  json_error_t error;
  int returnVal = 0;

  root = json_loads(data.c_str(), 0, &error);
  if(!root){
      *errorMsg = "error: on line: ";
  }else if(!json_is_array(root)){
      *errorMsg = "json is not an array!";
  }else if(json_array_size(root) != 2){
      *errorMsg = "json array size is expected to be 2.";
  }else{
    successData = json_array_get(root, 1);
    success = json_object_get(successData,"Successful");
  
      if(!strcmp(json_string_value(success),"1")){
        //the query was successful
        returnVal = 1;
      }else{
        *errorMsg = json_string_value(json_object_get(successData,"Error"));
      } 
  }

  json_decref(root);
  return returnVal;

}

int lockPOE(string POEname){
  string *errorMsg;
  string URL = "http://192.168.1.3/poe/setPOE.php?entryName=" + POEname + "&action=lock";
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
    return 1;
  }else{
    return 0;
  }
}

int unlockPOE(string POEname){
  string *errorMsg;
  string URL = "http://192.168.1.3/poe/setPOE.php?entryName=" + POEname + "&action=unlock";
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
    return 1;
  }else{
    return 0;
  }
}

int openPOE(string POEname){
  string *errorMsg;
  string URL = "http://192.168.1.3/poe/setPOE.php?entryName=" + POEname + "&action=open";
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
    return 1;
  }else{
    return 0;
  }
}

int closePOE(string POEname){
  string *errorMsg;
  string URL = "http://192.168.1.3/poe/setPOE.php?entryName=" + POEname + "&action=close";
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
    returnVal = 1;
  }else{
    returnVal = 0;
  }
}

int checkSystemStatus(string systemName, int* armed, int* alarmActive){
  string rawData;
  json_t *root, *theData;
  json_error_t error;
  int returnVal = 0;
  string *errorMsg;
  string URL = "http://192.168.1.3/system/getSystemStatus.php?systemName=" + systemName;
  string rawData = http_GET(URL);
  rawData = http_GET(URL);
  root = json_loads(rawData.c_str(), 0, &error);

  if(isSuccess(rawData,errorMsg)){
    theData = json_array_get(root, 0);
    if(json_is_object(theData)){
      *armed = !strcmp(json_string_value(json_object_get(theData,"Armed")),"1");
      *alarmActive = !strcmp(json_string_value(json_object_get(theData,"AlarmActive")),"1");
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
  string URL = "http://192.168.1.3/motion/setMotionStatus.php?sensorName=" + sensorName + "&motionFound=" + motionStatus + "&triggerStatus=1";
  string rawData = http_GET(URL);
  if(isSuccess(rawData,errorMsg)){
    return 1;
  }else{
    return 0;
  }
}
