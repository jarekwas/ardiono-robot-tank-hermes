#include <SoftwareSerial.h>
#include "DualVNH5019MotorShield.h"

SoftwareSerial bluetooth_serial(5, 3);
DualVNH5019MotorShield md;

void setup()
{
    Serial.begin(115200);
    bluetooth_serial.begin(9600);
    md.init();
}

char command;
int speedL = 0;
int speedR = 0;
int velocity = 0;
int turn=0;
bool runing = false;

void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
}


void loop()
{
 stopIfFault();
  
    if (bluetooth_serial.available())
    {
        String s = "";
        char c;
        while ((c = bluetooth_serial.read()) != -1)
        {
            s += c;
            delay(5);
        }
        Serial.print(s);
        command = s[0];

        switch (command)
        {
        case 'p':
            speedR = parseIntFromCmd(s)-400;
            break;
        case 'l':
            speedL = parseIntFromCmd(s)-400;
            break;
        case 'g':
            runing = !runing;
            break;
        case 's':
            runing = !runing;
            break;
       case 'v':
            velocity = parseIntFromCmd(s)-400;
            speedL = velocity;
            speedR = velocity;
            Serial.println("Ustawiono prędkość"+velocity);

            break;
        case 't':
            turn = parseIntFromCmd(s) - 400;
            if(turn>0) {
              speedL = velocity;
              speedL =speedL-turn;
              speedR = velocity;
            Serial.print("Skrecam wprawo");
            Serial.print("r: "+speedL);
            Serial.print("v: "+velocity);
            Serial.println("Skręca SpeedL"+speedL);
            }
            else if(turn<0){

            speedR = velocity;
            speedR =speedR+turn;
            speedL = velocity;
            Serial.println("Skręcam wlewo");
            Serial.println("Skręca SpeedR"+speedR);
            Serial.println("Ustawiono prędkość"+velocity);
            Serial.println("Skręca SpeedL"+speedL);
      

            
            } else if(turn=0){
              Serial.print("M2 current: ");
            speedR = velocity;
            speedL = velocity;
            }

            break;

            

            
        }
    }

    if (runing)
    {
        md.setM2Speed(speedR);
        md.setM1Speed(speedL*-1);
               delay(2);
               

    /*    Serial.print("M2 current: ");
        Serial.println(md.getM2CurrentMilliamps());
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
        delay(2); */



        
    }
    else
        {
        md.setM1Speed(0);
        md.setM2Speed(0);
    }
}

int parseIntFromCmd(String cmd)
{
    String bufforForParsedNumberAsString;
    char bufferForParsedNumberAsChar[3];

    bufferForParsedNumberAsChar[0] = cmd[2];
    bufferForParsedNumberAsChar[1] = cmd[3];
    bufferForParsedNumberAsChar[2] = cmd[4];

    bufforForParsedNumberAsString = String(bufferForParsedNumberAsChar);
    return bufforForParsedNumberAsString.toInt();
}
