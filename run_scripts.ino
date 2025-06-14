/*  run_scripts.ino
/*
/*  All code relating to actual parsing and execution of the loaded script
/*  Commands supported are duckyscript v1, also mouse commands
/*  TODO later loops etc. 
/*
/* because of how arduino works (there is no linker), this file will be added to the bottom of the main file
/* at compile time, by the compiler. So we have access to any globals and functions declared in the main .ino file
/*
*/
#include "hashes.h"

#ifdef CALCULATE_HASHES
#include "commands.h"
//function to output codes for the hashes.h file
// enable it in the #define at the top of main file
// and it will output the file to the serial monitor
void printHashes(){
  Serial.println("// hashes.h");
  Serial.println("// contains hashes values of keywords and tokens");
  Serial.println("// for comparison functions in parser");
  Serial.println("//");
  Serial.println("//Command words:");
  for( auto &command : commandsToHash){
    Serial.print("#define CMD_");
    Serial.print( command );
    Serial.print(" ");
    Serial.println( hashString(command));
  }
  Serial.println("//");
  Serial.println("//Modifier Keys");
  for( auto &modifier : modifier_keys){
    Serial.print("#define MOD_");
    Serial.print( modifier );
    Serial.print(" ");
    Serial.println( hashString(modifier));
  }
  Serial.println("//");
  Serial.println("//Single Keys");
  for( auto &single : single_keys){
    Serial.print("#define SKEY_");
    Serial.print( single );
    Serial.print(" ");
    Serial.println( hashString(single));
  }
  Serial.println("//");
  Serial.println("//Mouse Functions");
  for( auto &mousecomm : mouse_commands){
    Serial.print("#define MOU_");
    Serial.print( mousecomm );
    Serial.print(" ");
    Serial.println( hashString(mousecomm));
  }
}
#endif

//return hash of string for comparisons
unsigned long hashString(char *str){
  unsigned long hash = 5381;
  int c;
  while (c = *str++){
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }  
  return hash;
}


void runScript(){
  int len = loadedScript.length();
  if( len ==0) {return;} //no script loaded
  bool eof = false;
  int index= 0, endex=-1, repindex=0;
  //Parse script line by line until EOF (denoted by \n) 
  while(!eof){
    endex = loadedScript.indexOf('\r',index);
    if (endex == -1){
      //no more newlines found
      eof = true;
      endex = len;
    }
    String nextLine = loadedScript.substring(index,endex);
    nextLine.trim();
    if(nextLine.length()>0){
      //line is not empty- send it to parser
      int rep = parseLine( nextLine );
      if(rep>0){
        //Do <rep> repeats of previous line 
        String repLine = loadedScript.substring(repindex,index-1);//get previous line
        repLine.trim();
        for( ;rep>0;rep--){
          parseLine(repLine);
        }
      }
      repindex=index; //save old index for REPEATs
    }
    index = endex +1; //start next search after last newline
    delay(defaultDelay);
  }
}


//int parseLine(line) ---- takes a trimmed line of input and parses it
//will call needed functions
//returns 0, or number of times to repeat the previous line on a REPEAT instruction
int parseLine( String line){
  delay(defaultDelay); //configurable delay between script lines
  char command[21];
  //get command part of line,
  int pos= line.indexOf(' ');
  if(pos==-1){pos=line.length();}//no more spaces - list command - use whole string
  strcpy(command,line.substring(0,pos).c_str());
  //hash command for comparison with defined keywords
  unsigned long cmd_hash=hashString(command);
  //DBG("command: ", command);
  if(cmd_hash==CMD_REPEAT){return(line.substring(pos,line.length()).toInt());} //TODO error check repeat number
  //check for string/rem block ends
  switch(cmd_hash){
    case CMD_ID : break; //TODO set USB IDs
    
    case CMD_DELAY :
      delay(line.substring(pos,line.length()).toInt());
      break;

    case CMD_STRING :
      //TODO check for string block
      typeOutString(line.substring(pos+1,line.length()).c_str());
      stringDelay= defaultStringDelay; //in case it's been set for single line
      break;

    case  CMD_STRINGLN :
      //TODO check for string block
      typeOutString(line.substring(pos+1,line.length()).c_str());
      Keyboard.press(KEY_RETURN);
      Keyboard.release(KEY_RETURN);//send rest of line to keyboard out followed by newline (enter) key
      stringDelay= defaultStringDelay; //in case it's been set for single line
      break;

    //set delay between each line
    case CMD_DEFAULT_DELAY: case CMD_DEFAULTDELAY:
    defaultDelay=(line.substring(pos,line.length()).toInt());
      break;

    //set delay between each keypress for next string command only
    case CMD_STRINGDELAY: case CMD_STRING_DELAY:
      stringDelay=(line.substring(pos,line.length()).toInt());
      break;

    //set delay between each keypress
    case CMD_DEFAULT_STRING_DELAY: case CMD_DEFAULTSTRINGDELAY:
      defaultStringDelay=(line.substring(pos,line.length()).toInt());
      stringDelay= defaultStringDelay;
      break;

    //hold single key
    case CMD_HOLD:
      holdKey = true;
      checkKey(line.substring(pos,line.length()));
      holdKey = false;
    break;

    //release single key
    case CMD_RELEASE:
      releaseKey = true;
      checkKey(line.substring(pos,line.length()));
      releaseKey = false;
    break;

    //pause until side button is pressed
    case CMD_WAIT_FOR_BUTTON_PRESS:
      while(!digitalRead(0)){ }
      break;

    case CMD_REM:
      break;

  // TODO REM_BLOC 
  // MOUSE COMMANDS - including random
    default:
      checkKey(line); //no command word found, check for modifiers and single keys:
  }

  return(0);//no repeats
}

void checkKey(String line){
  line.trim();//remove leading spaces
  //check for single alphanumeric key
  if(line.length()==0){return;} //no keys (shouldnt happen)
  if(line.length()==1){
    //single char to input TODO should fail if no modifiers preceeding?
    keyOut(line[0]);
    return;
  }
  //get 
  char command[11];
  bool lastKey=false;
  //get command part of line,
  int pos= line.indexOf(' ');
  if(pos==-1){
    pos=line.length();
    lastKey=true;
  }//no more spaces - last command - use whole string
  strcpy(command,line.substring(0,pos).c_str());
  //compare against all defined keys
  switch(hashString(command)){
    //check for modifiers
    case MOD_CTRL: case MOD_CONTROL:
      Keyboard.press(KEY_LEFT_CTRL);
      break;
    case MOD_SHIFT:
      Keyboard.press(KEY_LEFT_SHIFT);
      break;
    case MOD_ALT:
      Keyboard.press(KEY_LEFT_ALT);
      break;
    case MOD_GUI: case MOD_WINDOWS:
      Keyboard.press(KEY_LEFT_GUI);
      break;
    //check for single function keys
    case SKEY_UPARROW: case SKEY_UP:
      keyOut(KEY_UP_ARROW);
      break;     
    case SKEY_DOWNARROW: case SKEY_DOWN:
      keyOut(KEY_DOWN_ARROW);
      break; 
    case SKEY_LEFTARROW: case SKEY_LEFT:
      keyOut(KEY_LEFT_ARROW);
      break; 
    case SKEY_RIGHTARROW: case SKEY_RIGHT:
      keyOut(KEY_RIGHT_ARROW);
      break;
    case SKEY_MENU : case SKEY_APP:
      keyOut(KEY_MENU);
      break;
    case SKEY_SPACE        :
      keyOut(KEY_SPACE);
      break;
    case SKEY_BACKSPACE    :
      keyOut(KEY_BACKSPACE);
      break;
    case SKEY_TAB          :
      keyOut(KEY_TAB);
      break;
    case SKEY_ENTER       :
      keyOut(KEY_RETURN);
      break;
    case SKEY_ESC: case SKEY_ESCAPE:
      keyOut(KEY_ESC);
      break;
    case SKEY_INSERT       :
      keyOut(KEY_INSERT);
      break;
    case SKEY_DELETE       :
      keyOut(KEY_DELETE);
      break;
    case SKEY_PAGEUP      :
      keyOut(KEY_PAGE_UP);
      break;
    case SKEY_PAGEDOWN    :
      keyOut(KEY_PAGE_DOWN);
      break;
    case SKEY_HOME         :
      keyOut(KEY_HOME);
      break;
    case SKEY_END          :
      keyOut(KEY_DOWN_ARROW);
      break;
    case SKEY_NUMLOCK     :
      keyOut(KEY_NUM_LOCK);
      break;
    case SKEY_CAPSLOCK    :
      keyOut(KEY_CAPS_LOCK);
      break;
    case SKEY_F1           :
      keyOut(KEY_F1);
      break;
    case SKEY_F2           :
      keyOut(KEY_F2);
      break;
    case SKEY_F3           :
      keyOut(KEY_F3);
      break;
    case SKEY_F4           :
      keyOut(KEY_F4);
      break;
    case SKEY_F5           :
      keyOut(KEY_F5);
      break;
    case SKEY_F6           :
      keyOut(KEY_F6);
      break;
    case SKEY_F7           :
      keyOut(KEY_F7);
      break;
    case SKEY_F8           :
      keyOut(KEY_F8);
      break;
    case SKEY_F9           :
      keyOut(KEY_F9);
      break;
    case SKEY_F10          :
      keyOut(KEY_F10);
      break;
    case SKEY_F11          :
      keyOut(KEY_F11);
      break;
    case SKEY_F12          :
      keyOut(KEY_F12);
      break;
    case SKEY_F13          :
      keyOut(KEY_F13);
      break;
    case SKEY_F14          :
      keyOut(KEY_F14);
      break;
    case SKEY_F15          :
      keyOut(KEY_F15);
      break;
    case SKEY_F16          :
      keyOut(KEY_F16);
      break;
    case SKEY_F17          :
      keyOut(KEY_F17);
      break;
    case SKEY_F18          :
      keyOut(KEY_F18);
      break;
    case SKEY_F19          :
      keyOut(KEY_F19);
      break;
    case SKEY_F20          :
      keyOut(KEY_F20);
      break;
    case SKEY_F21          :
      keyOut(KEY_F21);
      break;
    case SKEY_F22          :
      keyOut(KEY_F22);
      break;
    case SKEY_F23          :
      keyOut(KEY_F23);
      break;
    case SKEY_F24          :
      keyOut(KEY_F24);
      break;
    case SKEY_PRINTSCREEN :
      keyOut(KEY_PRINT_SCREEN);
      break;
    case SKEY_SCROLLLOCK  :
      keyOut(KEY_SCROLL_LOCK);
      break;
    case SKEY_PAUSE        :
      keyOut(KEY_PAUSE);
      break;
  }
  if(lastKey==false){
    checkKey(line.substring(pos,line.length()));
  }else{//last key sent
    if(!holdKey){ Keyboard.releaseAll();}
  }
  return;
}

void keyOut(char k){
    if(!releaseKey) {Keyboard.press(k);}
    if(!holdKey) {Keyboard.release(k);}
    delay(stringDelay);
}

void typeOutString(const char *line){
  int i=0;
  char k=line[i];
  while(k != '\0'){
    #ifdef DEBUG
      Serial.print(k);
    #endif
    keyOut(k);
    i++;
    k=line[i];
  }
}