#pragma once

class DistanceSensor
{
public:
    DistanceSensor(
        int triggerPin,
        int echoPin
    );

    float distance() const;

private:
    int triggerPin;
    int echoPin;
};