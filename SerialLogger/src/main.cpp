#include <Arduino.h>
#include "config.h"

File LogFileName;

String strReadSerial;
const char * strNameFile = "LogFile.txt";
int checkSD;
unsigned long time = 0;
bool flagInitSD = false;
bool flagSDRemoved = false;

String removedText = "SD removido.";
String readyUseText = "SD Card pronto para uso.";
String wrongInitText = "Falha ao iniciar SD.";

void fnvWriteInfoInSD (String data);

void setup() 
{ 
   Serial.begin(9600); 
   pinMode(CHIP_SELECT_PIN, OUTPUT); 
   pinMode(CHECK_SD_PIN, INPUT_PULLUP);

   if (SD.begin()) 
   { 
      #ifdef DEBUG_ENABLE
      Serial.println(readyUseText); 
      #endif
   }
   else 
   {
      fnvWriteInfoInSD(wrongInitText);
      return;
   }
}

void loop() 
{
   checkSD = digitalRead(CHECK_SD_PIN);
   if(!checkSD)
   {
      if((millis() - time) > 500)
      {
         time = millis();
         if (flagInitSD == false) 
         { 
            SD.begin();

            fnvWriteInfoInSD(readyUseText);
            flagInitSD = true; 
            flagSDRemoved = false;
         }
      }
   }
   else
   {
      if(flagSDRemoved == false)
      {
         fnvWriteInfoInSD(removedText);
      }
      flagInitSD = false;
      flagSDRemoved = true;
   }

   if(Serial.available() > 0)
   {
      strReadSerial = Serial.readString();
      strReadSerial.trim();
      fnvWriteInfoInSD(strReadSerial);
   }
}

void fnvWriteInfoInSD (String data)
{
   LogFileName = SD.open(strNameFile, FILE_WRITE); 
    
   if (LogFileName)
   {
      LogFileName.println(data);

      #ifdef DEBUG_ENABLE
      Serial.println(data);
      #endif

      LogFileName.close();
   }
}