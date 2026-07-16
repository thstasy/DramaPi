#pragma once

#include <string>


class Servo
{
public:

    Servo();


    void init();


    // 立即以平滑方式回到絕對 90°。
    void home();


    // 開始掃描：
    // 左側 → 隨機中間 → 右側 → 隨機中間。
    void startScanning();


    // 停止掃描。
    // 目標會被設成絕對 90°，
    // 之後由 update() 每次移動一小步回去。
    void stopScanning();


    // 每個主迴圈呼叫一次。
    // 每次只移動一小步，不阻塞主程式。
    void update();


    bool isScanning() const;


    /*
     * 保留舊介面。
     * 其他 target 若仍有使用這些函式，
     * 也可以繼續編譯。
     */
    void shakeHead();

    void stopShaking();

    void lookLeft();

    void lookCenter();

    void lookRight();


private:

    enum class ScanPhase
    {
        Left,
        MiddleAfterLeft,
        Right,
        MiddleAfterRight
    };


    bool exists(
        const std::string& path
    ) const;


    void writeFile(
        const std::string& path,
        int value
    );


    void setDutyCycle(
        int dutyNs
    );


    int angleToDuty(
        int angle
    ) const;


    int randomAngle(
        int minimum,
        int maximum
    );


    void chooseTargetForCurrentPhase();


    void advanceScanPhase();


    void moveOneStepTowardTarget();


    void moveSmooth(
        int targetAngle,
        int delayMs = 20
    );


private:

    int currentAngle_;

    int targetAngle_;

    bool scanning_;

    ScanPhase scanPhase_;


    /*
     * GPIO18 / Physical Pin 12
     * 對應 pwmchip0/pwm2。
     */
    static constexpr int Channel = 2;


    /*
     * Servo PWM period：20 ms = 50 Hz。
     */
    static constexpr int PeriodNs = 20000000;


    /*
     * SG90 較安全的 pulse 範圍。
     * 不使用 0.5 ms 與 2.5 ms，
     * 避免撞到機械極限。
     */
    static constexpr int MinimumDutyNs = 900000;

    static constexpr int MaximumDutyNs = 2100000;


    /*
     * 開機、待機、人離開時的絕對中心。
     */
    static constexpr int HomeAngle = 90;


    /*
     * 每次 update() 移動的角度。
     * 你的主迴圈約每 300 ms 呼叫一次。
     */
    static constexpr int StepDegrees = 3;


    /*
     * 掃描左側範圍。
     */
    static constexpr int LeftMinimum = 50;

    static constexpr int LeftMaximum = 70;


    /*
     * 掃描過程中的隨機中間範圍。
     * 這和 HomeAngle = 90 是兩個不同概念。
     */
    static constexpr int ScanMiddleMinimum = 75;

    static constexpr int ScanMiddleMaximum = 105;


    /*
     * 掃描右側範圍。
     */
    static constexpr int RightMinimum = 110;

    static constexpr int RightMaximum = 130;


    const std::string PwmChipPath =
        "/sys/class/pwm/pwmchip0";


    const std::string PwmPath =
        "/sys/class/pwm/pwmchip0/pwm2";
};