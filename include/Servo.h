// #pragma once

// #include <atomic>
// #include <thread>


// class Servo
// {
// public:
//     explicit Servo(
//         int gpio = 18
//     );

//     ~Servo();


//     Servo(
//         const Servo&
//     ) = delete;

//     Servo& operator=(
//         const Servo&
//     ) = delete;


//     /*
//      * 開啟 GPIO 並啟動 Servo 控制執行緒。
//      * 初始化完成後，Servo 會回到中央。
//      */
//     void init();


//     /*
//      * 切換到待機模式並平滑回到中央。
//      */
//     void home();


//     /*
//      * 切換到一般左右掃描模式。
//      */
//     void startScanning();


//     /*
//      * 停止一般掃描並回到中央。
//      */
//     void stopScanning();


//     /*
//      * 切換到緊急快速甩頭模式。
//      * 這個函式不阻塞 main loop。
//      */
//     void startEmergencyShake();


//     /*
//      * Servo 控制執行緒是否正在運作。
//      */
//     [[nodiscard]]
//     bool isRunning() const;


// private:
//     enum class Mode
//     {
//         Home,
//         Scanning,
//         Emergency
//     };


//     enum class ScanPhase
//     {
//         Left,
//         MiddleAfterLeft,
//         Right,
//         MiddleAfterRight
//     };


//     void controlLoop();


//     void updateHome();


//     void updateScanning();


//     void updateEmergency();


//     void setAngle(
//         int angle
//     );


//     [[nodiscard]]
//     int angleToPulseWidth(
//         int angle
//     ) const;


//     [[nodiscard]]
//     int randomAngle(
//         int minimum,
//         int maximum
//     );


//     void chooseTargetForCurrentPhase();


//     void advanceScanPhase();


//     void moveTowardTarget(
//         int stepDegrees
//     );


// private:
//     int gpio_;

//     int chipHandle_;


//     int currentAngle_;

//     int targetAngle_;


//     ScanPhase scanPhase_;


//     std::atomic<Mode> mode_;

//     std::atomic<bool> running_;

//     std::thread controlThread_;


//     /*
//      * 程式邏輯中心。
//      * 經校正後對應 1800 µs。
//      */
//     static constexpr int HomeAngle = 90;


//     /*
//      * Servo 實際校正結果：
//      *
//      * 1600 µs → 右側
//      * 1800 µs → 中央
//      * 2000 µs → 左側
//      */
//     static constexpr int MinimumPulseWidthUs = 1600;

//     static constexpr int CenterPulseWidthUs = 1800;

//     static constexpr int MaximumPulseWidthUs = 2000;


//     static constexpr int ServoFrequencyHz = 50;


//     /*
//      * 控制執行緒固定每 15 ms 更新一次。
//      */
//     static constexpr int ControlIntervalMs = 15;


//     /*
//      * 一般模式移動較平滑。
//      */
//     static constexpr int NormalStepDegrees = 3;


//     /*
//      * Emergency 模式移動較快速。
//      */
//     static constexpr int EmergencyStepDegrees = 12;


//     /*
//      * 一般掃描範圍。
//      */
//     static constexpr int LeftMinimum = 165;

//     static constexpr int LeftMaximum = 178;


//     static constexpr int MiddleMinimum = 85;

//     static constexpr int MiddleMaximum = 95;


//     static constexpr int RightMinimum = 2;

//     static constexpr int RightMaximum = 15;


//     /*
//      * Emergency 固定在兩端之間反覆甩頭。
//      */
//     static constexpr int EmergencyLeftAngle = 175;

//     static constexpr int EmergencyRightAngle = 5;
// };

#pragma once

#include "ServoMode.h"

#include <atomic>
#include <thread>


class Servo
{
public:
    explicit Servo(
        int gpio = 18
    );

    ~Servo();


    Servo(
        const Servo&
    ) = delete;

    Servo& operator=(
        const Servo&
    ) = delete;


    void init();


    void setMode(
        ServoMode mode
    );


    void home();


    void startScanning();


    void stopScanning();


    void startEmergencyShake();


    [[nodiscard]]
    ServoMode mode() const;


    [[nodiscard]]
    bool isRunning() const;


private:
    enum class ScanPhase
    {
        Left,
        MiddleAfterLeft,
        Right,
        MiddleAfterRight
    };


    void controlLoop();


    void applyModeTransition(
        ServoMode previousMode,
        ServoMode currentMode
    );


    void updateHome();


    void updateScanning();


    void updateEmergency();


    void setAngle(
        int angle
    );


    [[nodiscard]]
    int angleToPulseWidth(
        int angle
    ) const;


    [[nodiscard]]
    int randomAngle(
        int minimum,
        int maximum
    );


    void chooseTargetForCurrentPhase();


    void advanceScanPhase();


    void moveTowardTarget(
        int stepDegrees
    );


private:
    int gpio_;

    int chipHandle_;

    int currentAngle_;

    int targetAngle_;

    ScanPhase scanPhase_;

    std::atomic<ServoMode> mode_;

    std::atomic<bool> running_;

    std::thread controlThread_;


    static constexpr int HomeAngle = 90;

    static constexpr int MinimumPulseWidthUs = 1600;

    static constexpr int CenterPulseWidthUs = 1800;

    static constexpr int MaximumPulseWidthUs = 2000;

    static constexpr int ServoFrequencyHz = 50;

    static constexpr int ControlIntervalMs = 15;

    static constexpr int NormalStepDegrees = 3;

    static constexpr int EmergencyStepDegrees = 12;

    static constexpr int LeftMinimum = 165;

    static constexpr int LeftMaximum = 178;

    static constexpr int MiddleMinimum = 85;

    static constexpr int MiddleMaximum = 95;

    static constexpr int RightMinimum = 2;

    static constexpr int RightMaximum = 15;

    static constexpr int EmergencyLeftAngle = 175;

    static constexpr int EmergencyRightAngle = 5;
};
