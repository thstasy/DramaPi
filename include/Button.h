#pragma once

#include <chrono>


class Button
{
public:
    explicit Button(
        int gpio,
        bool activeLow = true
    );

    ~Button();


    Button(
        const Button&
    ) = delete;

    Button& operator=(
        const Button&
    ) = delete;


    void init();


    /*
     * 每次主迴圈呼叫一次。
     * 內含 debounce。
     */
    void update();


    [[nodiscard]]
    bool isPressed() const;


private:
    [[nodiscard]]
    bool readRawPressed() const;


private:
    int gpio_;

    int chipHandle_;

    bool activeLow_;

    bool initialized_;

    bool stablePressed_;

    bool lastRawPressed_;

    std::chrono::steady_clock::time_point lastRawChange_;


    static constexpr int DebounceMs = 40;
};
