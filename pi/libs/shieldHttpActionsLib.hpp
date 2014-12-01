#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

using namespace std;
 
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
      fprintf(stderr, "curl_easy_perform() failed: %s\n",errorBuffer);
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



struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}



string http_GET(string URL){

  CURL *curl;
  CURLcode res;
  string result;

  struct MemoryStruct chunk;
 
  chunk.memory = malloc(1);  /* will be grown as needed by the realloc in the WriteMemoryCallback Function */ 
  chunk.size = 0;    /* no data at this point */ 
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, URL.cstr());
    /* example.com is redirected, so we tell libcurl to follow redirection */ 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

     /* send all data to the callback function  */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
    /* we pass our 'chunk' struct to the callback function */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK){
      result = format("curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
      COUT << result << endl;
    }else{
    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */ 

    result = (string)chunk;
    printf("%s\n", result.c_str());

  }
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }

  if(chunk.memory)
    free(chunk.memory);

  /* we're done with libcurl, so clean it up */ 
  curl_global_cleanup();

  return result;
}

int janssonTest(string URL){

  string data;

  data = http_GET(URL);

  json_t *root;
  json_error_t error;

  root = json_loads( data, 0, &error );
  if(!root){
      COUT << format("error: on line %d: %s\n", error.line, error.text) >> endl;
      return 0;
  }

  const char *key;
  json_t *value;

  void *iter = json_object_iter(root);
  while(iter){
      key = json_object_iter_key(iter);
      value = json_object_iter_value(iter);

      COUT << format("Key: %s, Value: %f\n", key, json_real_value(value)) << endl;

      iter = json_object_iter_next(root, iter);
  }

  json_decref(root);

  return 1;
}

