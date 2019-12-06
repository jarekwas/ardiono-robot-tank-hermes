
#include <Arduino.h>

class HC_SR04
{
private:
    uint8_t triggerPin;
    uint8_t echoPin;
public:
    HC_SR04(const uint8_t triggerPin, const uint8_t echoPin);
    uint8_t readDistance();
};

HC_SR04::HC_SR04(const uint8_t triggerPin, const uint8_t echoPin){
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;

    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
};

uint8_t HC_SR04::readDistance(){
    long czas, dystans;

    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    czas = pulseIn(echoPin, HIGH);
    return czas / 58;
}
