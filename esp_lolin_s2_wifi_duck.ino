/*
  Wifi enabled keyboard injection
  with on board editing and storage

*/

//#define WIFICLIENT  //connect WiFi in client mode instead of AP mode
//#define DEBUG       //enable serial.println outputs
//#define CALCULATE_HASHES //this generates the hashes.h file - use if we changed keywords or hashing algo
#ifdef DEBUG
#define DBG(fixedText,variableName) \
        Serial.print( F(#fixedText " "  #variableName"=") ); \
        Serial.println(variableName); \
        delay(500);
#else
#define DBG(fixedText,variableName)
#endif


#include "index_html.h"
#include "edit_html.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include <LittleFS.h>

//#include <fs.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;

#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

/******************************/
/*        WiFi setup          */
/******************************/
const char* ssid = "EVIL_ESP32";
const char* password = "1234567890";


/*******************/
/*     GLOBALS     */
/*******************/
String loadedScript = "";
String filesList = "<option value=\"file1.txt\">file1.txt</option>";
String fileName = "default.txt";
String oldFileName = "new_file.txt";
bool scriptRunning=false, holdKey=false, releaseKey=false;
int stringDelay=0, defaultDelay=0, defaultStringDelay=0;


//404 Page
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

//Substitutions for html files
String processor(const String& var){
  if(var == "SCRIPT_TXT")  {
    return (loadedScript); //return current scripts contents;
  }
  if(var == "SCRIPT_FILENAME")  {
    return (fileName); //return current scripts filename;
  }
  if(var == "SELECT_LIST"){
    return(filesList);
  }
  return String();
}


void loadScript(fs::FS &fs, const char * path){
  loadedScript = "";
  File file = fs.open(path, "r");
  #ifdef DEBUG
  if(!file || file.isDirectory())
  {
    Serial.println("no file");
    return;
  }
  #endif
  while(file.available())
  {
    loadedScript+=String((char)file.read());
  }
  file.close();
  return;
}

//get directory contents for showing in the file list
void populateFileList(fs::FS &fs){ 

    File root = fs.open("/");

    filesList = "";
    File file = root.openNextFile();
    while(file){
      filesList += "<option value=\"";
      filesList += file.name();
      filesList += "\">";
      filesList += file.name();
      filesList += "</option>";
      file = root.openNextFile();
    }
}

//save edited file (prepend / before calling)
void writeFile(fs::FS &fs, const char * path, const char * message){
    #ifdef DEBUG
    Serial.printf("Writing file: %s\r\n", path);
    #endif

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        #ifdef DEBUG
        Serial.println("- failed to open file for writing");
        #endif
        return;
    }
    #ifdef DEBUG
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    #else
    file.print(message);
    #endif
    file.close();
}

  
void setup() {
  // TODO move these to only where needed
  // TODO also add USBMSC for SD card storage for bin payloads and exfil
  //initialize control over the keyboard:
  Keyboard.begin();
  USB.begin();

  //initialise LittleFS
  #ifdef DEBUG
  if(!LittleFS.begin(false)){
    Serial.println("LittleFS Mount Failed");
  }
  #else
  LittleFS.begin(false);
  #endif

  //connect to WIFI TODO WIFI AP mode
  #ifdef WIFICLIENT //WIFI CLIENT MODE
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed!");
      return;
    }
    Serial.println( WiFi.localIP());
  #else //WiFi AP mode
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
  #endif

  //Load default script if it exists
  loadScript(LittleFS, "/default.txt");

  // set up web server endpoints
  //main page 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    populateFileList(LittleFS);
    request->send_P(200, "text/html", index_html, processor);
  });

  //404 Page
  server.onNotFound(notFound);

  //User clicked "run" button run currently loaded script if there is one
  server.on("/run", HTTP_GET, [](AsyncWebServerRequest *request) {
    scriptRunning=true;
    request->redirect("/");
  });

  //file functions - load/delete
  server.on("/file", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage = request->getParam("action")->value();
    String actionFileName = "/";
    if(inputMessage =="Load"){
      fileName = request->getParam("script_select")->value();
      actionFileName += fileName;
      loadScript(LittleFS, actionFileName.c_str());
      request->redirect("/");
    }
    if(inputMessage =="Delete"){
      //delete file selected in list
      actionFileName += request->getParam("script_select")->value();
      LittleFS.remove(actionFileName.c_str());
      request->redirect("/");
    }
    if(inputMessage =="Cancel"){
      //file edit cancelled from edit window
      fileName = oldFileName;
      request->redirect("/");
    }
    if(inputMessage =="Save") {
      loadedScript = request->getParam("script_data")->value();
      fileName = request->getParam("script_filename")->value(); // get filename from textarea - it may have been changed
      actionFileName += fileName; 
      writeFile(LittleFS, actionFileName.c_str(), loadedScript.c_str());
      request->redirect("/");
    }
  });

  server.on("/edit", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage = request->getParam("action")->value();
    if(inputMessage =="New Script"){
      loadedScript = "";
      oldFileName = fileName;
      fileName = "new_file.txt";
      request->send_P(200, "text/html", edit_html, processor);
    }
    if(inputMessage =="Edit") {
      //edit current loaded script
      oldFileName = fileName;
      request->send_P(200, "text/html", edit_html, processor);
    }
  });
  server.begin();
}

void loop() {
  #ifdef CALCULATE_HASHES
  printHashes();
  // wait a long time
  delay(100000);
  #endif

  if(scriptRunning){
    // String actionFileName = "/";
    // actionFileName += fileName;
    // runScript(actionFileName.c_str());
    runScript();
    scriptRunning=false;
  }
}