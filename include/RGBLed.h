#pragma once


class RGBLed
{
public:
    RGBLed(
        int redGpio,
        int greenGpio,
        int blueGpio
    );

    ~RGBLed();


    RGBLed(
        const RGBLed&
    ) = delete;

    RGBLed& operator=(
        const RGBLed&
    ) = delete;


    void init();


    void off();

    void red();

    void green();

    void blue();

    void yellow();

    void purple();

    void cyan();

    void white();


    /*
     * 亮度範圍為 0 到 100。
     */
    void setBrightness(
        int redPercent,
        int greenPercent,
        int bluePercent
    );


    void set(
        bool redOn,
        bool greenOn,
        bool blueOn
    );


private:
    void setChannelBrightness(
        int gpio,
        int percent
    );


private:
    int redGpio_;

    int greenGpio_;

    int blueGpio_;

    int chipHandle_;

    bool initialized_;


    static constexpr int PwmFrequencyHz = 500;
};
