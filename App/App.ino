#include <SoftwareSerial.h>
#include "DualVNH5019MotorShield.h"
#include "HC_SR04.hpp"

SoftwareSerial bluetooth_serial(19, 18);
DualVNH5019MotorShield md;

HC_SR04 frontDistanceSensor(17, 5);
HC_SR04 backDistanceSensor(16, 11);

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
int turn = 0;
float turnFloat = 0;
bool runing = false;

void stopIfFault()
{
    if (md.getM1Fault())
    {
        Serial.println("M1 fault");
        while (1)
            ;
    }
    if (md.getM2Fault())
    {
        Serial.println("M2 fault");
        while (1)
            ;
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
        command = s[0];

        switch (command)
        {
        case 'p':
            speedR = parseIntFromCmd(s) - 400;
            break;
        case 'l':
            speedL = parseIntFromCmd(s) - 400;
            break;
        case 'g':
            runing = !runing;

            Serial.print("GO!");
            Serial.print(", r: ");
            Serial.print(speedR);
            Serial.print(", l: ");
            Serial.print(speedL);
            Serial.print(", v: ");
            Serial.println(velocity);

            break;
        case 's':
            runing = !runing;
            break;
        case 'v':
            velocity = parseIntFromCmd(s) - 400;
            speedL = velocity;
            speedR = velocity;
            Serial.print("Velocity:");
            Serial.print(", r: ");
            Serial.print(speedR);
            Serial.print(", l: ");
            Serial.print(speedL);
            Serial.print(", v: ");
            Serial.println(velocity);
            break;
        case 't':
            turn = parseIntFromCmd(s) - 400;

            if (turn > 0)
            {
                speedL = velocity;
                speedR = velocity;

                turnFloat = (400 - abs(turn)) / 400;
                speedL = speedL * turnFloat;
                Serial.print("t > 0 | turn: ");
                Serial.print(turn);
                Serial.print(", r: ");
                Serial.print(speedR);
                Serial.print(", turnFloat: ");
                Serial.print(turnFloat);
                Serial.print(", l: ");
                Serial.print(speedL);
                Serial.print(", v: ");
                Serial.println(velocity);
            }
            else if (turn < 0)
            {

                speedR = velocity;
                speedL = velocity;

                turnFloat = (400 - abs(turn)) / 400;
                speedR = speedR * turnFloat;

                Serial.print("t > 0 | turn: ");
                Serial.print(turn);
                Serial.print(", r: ");
                Serial.print(speedR);
                Serial.print(", turnFloat: ");
                Serial.print(turnFloat);
                Serial.print(", l: ");
                Serial.print(speedL);
                Serial.print(", v: ");
                Serial.println(velocity);
            }
            else if (turn == 0)
            {
                speedR = velocity;
                speedL = velocity;

                Serial.print("t == 0");
                Serial.print(", r: ");
                Serial.print(speedR);
                Serial.print(", l: ");
                Serial.print(speedL);
                Serial.print(", v: ");
                Serial.println(velocity);
            }

            break;
        }
    }

    if (runing)
    {
        uint8_t fontDistance = frontDistanceSensor.readDistance();
        uint8_t backDistance = backDistanceSensor.readDistance();

        if (speedR > 0 && speedL > 0)
        {
            if (fontDistance > 10)
            {
                md.setM2Speed(speedR);
                md.setM1Speed(speedL * -1);
            }else{
                md.setM2Speed(0);
                md.setM1Speed(0);
            }
        }
        else if (speedR < 0 && speedL < 0)
        {
            if (backDistance > 10)
            {
                md.setM2Speed(speedR);
                md.setM1Speed(speedL * -1);
            }else{
                md.setM2Speed(0);
                md.setM1Speed(0);
            }
        }
        else
        {
            md.setM2Speed(speedR);
            md.setM1Speed(speedL * -1);
        }

        Serial.print("F:");
        Serial.print(fontDistance);
        Serial.print("cm, B: ");
        Serial.print(backDistance);
        Serial.print("cm, R: ");
        Serial.print(speedR);
        Serial.print(" L: ");
        Serial.println(speedL);

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
