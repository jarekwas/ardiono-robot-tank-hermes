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
bool runing = false;

void loop()
{
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
        }
    }

    if (runing)
    {
        md.setM2Speed(speedR);
        md.setM1Speed(speedL*-1);
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