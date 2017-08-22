#pragma once
#include "ofMain.h"

typedef void (*callback)(char cmd, int cmd_idx, void * data); //user-defined callback funct for dealing with commands

class SerialCmdParser{
  public:
    ofSerial * port=NULL; //pointer to the serial port used, if not default
    int num_cmds; //number of unique, one-char commands
    char * cmds; //array containing commands, filled at runtime

    callback callback_f = NULL; //pointer to callback function
    
    //Constructors
    SerialCmdParser(){
    }
    SerialCmdParser(ofSerial * _port){
      port = _port;
    }
    SerialCmdParser(ofSerial * _port, char * _cmds, int _num_cmds){
      port = _port;
      num_cmds = _num_cmds;
      cmds = (char *)calloc(num_cmds, sizeof(char));
      memcpy(cmds, _cmds, num_cmds*sizeof(char));
    }
    SerialCmdParser(ofSerial * _port, char * _cmds, int _num_cmds, callback _function){
      port = _port;
      num_cmds = _num_cmds;
      cmds = (char *)calloc(num_cmds, sizeof(char));
      memcpy(cmds, _cmds, num_cmds*sizeof(char));
      callback_f = _function;
    }

    //Functions
    void setup(ofSerial * _port, char * _cmds, int _num_cmds, callback _function){
      port = _port;
      num_cmds = _num_cmds;
      cmds = (char *)calloc(num_cmds, sizeof(char));
      memcpy(cmds, _cmds, num_cmds*sizeof(char));
      callback_f = _function;

      ofLog()<<"commands";
      for (int i=0; i<num_cmds; i++) {
        ofLog()<< cmds[i];
      }

    }
    void setCallback(callback _function){ callback_f = _function; }
    void setPort(ofSerial * _port){ port = _port; }

    void setCmds(char * _cmds, int _num_cmds);

    void update();
};
