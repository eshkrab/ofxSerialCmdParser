#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ofxSerialCmdParser.h"

void SerialCmdParser::setCmds(char * _cmds, int _num_cmds){
  num_cmds = _num_cmds;
  cmds = (char *)calloc(num_cmds, sizeof(char));
  memcpy(cmds, _cmds, num_cmds*sizeof(char));
}

void SerialCmdParser::update(){
  int num_bytes = 0;

  //get serial data
  if (port == NULL){
    ofLogError()<< "no port specified"; //if using default Serial port
    return;
  }
  else num_bytes = port->available();               //user specified another port
  if (num_bytes == 0) return;

  unsigned char bytes[num_bytes];
  port->readBytes(bytes, num_bytes);               //user specified another port
  //for (int i=0; i<num_bytes; i++) {
  //  ofLog()<< ofToString(bytes[i]);
  //  
  //}

  //split string into commands, separated by ";"
  unsigned char * cmd_ptr;
  cmd_ptr = (unsigned char*)strchr((const char *)bytes, ';');
  while (cmd_ptr != NULL){
    int cmd_size = cmd_ptr - bytes +1;
    char cmd_str[cmd_size];
    strncpy(cmd_str, (const char *)bytes, cmd_size-1);

    //parse out cmd_str into cmd, cmd_idx, and data
    //find space separated tokens, first part is the cmd+cmd index, second is data
    unsigned char hdr[255] = {0};
    unsigned char * data = NULL;
    char cmd_char = ' ';
    int cmd_idx = 0;
    int hdr_len = strcspn(cmd_str, " ");
    char * tok_ptr = strtok(cmd_str, " ");
    sprintf((char *)hdr, "%s\n", tok_ptr);

    
    //look for characters in the command list
    unsigned char * c_ptr = NULL;
    for (int i=0; i<num_cmds; i++) {
      c_ptr = (unsigned char*)strchr((const char *)hdr, cmds[i]);
      if (c_ptr != NULL) {
        cmd_char = cmds[i];
        //look for command index
        char * ptr_end;
        cmd_idx = strtol((const char *)c_ptr+1, &ptr_end, 10);
        break;
      }
    }

    //get data if there is any 
    //BUG:(everything until next space, doesn't handle spaces in data)
    tok_ptr = strtok(NULL, " ");
    if (tok_ptr != NULL) {
      int data_size = strlen(tok_ptr);
      data = (unsigned char*)calloc(data_size, sizeof(char));
      sprintf((char *)data, "%s\n", tok_ptr);
    }

    if (cmd_char != ' ') {
      //YOU HAVE COMMANDS!
      //send everything to the callback function
      //ofLog()<<"cmd: "+ofToString(cmd_char);
      //ofLog()<<"idx: "+ofToString(cmd_idx);
      if (callback_f != NULL) {
        (*callback_f)(cmd_char, cmd_idx,(void*)data);
      }
    }
    
    //parse other commands in the buffer if there are any
    int bytes_left = num_bytes-cmd_size+1;
    if (bytes_left > 0) {
      char temp[bytes_left];
      memcpy(temp, cmd_ptr+1, bytes_left);
      memset(bytes, 0, num_bytes);
      memcpy(bytes, temp, bytes_left);
      cmd_ptr = (unsigned char *)strchr((const char *)bytes, ';');
    }else {
      cmd_ptr = NULL;
    }
  }

}
