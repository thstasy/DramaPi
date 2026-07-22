#pragma once

#include "Button.h"
#include "HumanState.h"
#include "MatrixMode.h"
#include "RGBLed.h"


class MatrixController
{
public:
    MatrixController(
        Button& redButton,
        Button& blueButton,
        RGBLed& rgbLed
    );


    void init();


    /*
     * 每個主迴圈呼叫一次。
     *
     * 每一次新的按下動作都會觸發事件，
     * 即使目前已經是同一個模式。
     *
     * 紅色按鈕      → Red Pill
     * 藍色按鈕      → Blue Pill
     * 兩顆同時按下  → Neutral
     */
    void updateButtons();


    [[nodiscard]]
    HumanState applyMode(
        HumanState detectedState
    ) const;


    /*
     * 依 HumanState 顯示狀態顏色：
     *
     * Approaching / Close / PersonalSpace → 黃色
     * Emergency                          → 白色
     * 其他                               → 目前模式顏色
     */
    void updateColor(
        HumanState humanState
    );


    /*
     * 立即顯示目前 Matrix 模式的基本顏色。
     */
    void showModeColor();


    [[nodiscard]]
    MatrixMode mode() const;


    /*
     * 這一輪是否有新的按鈕事件。
     * 同一顆按鈕重複按仍會回傳 true。
     */
    [[nodiscard]]
    bool buttonTriggered() const;


    /*
     * 這一輪按鈕事件所選擇的模式。
     */
    [[nodiscard]]
    MatrixMode triggeredMode() const;


private:
    void trigger(
        MatrixMode selectedMode
    );


private:
    Button& redButton_;

    Button& blueButton_;

    RGBLed& rgbLed_;


    MatrixMode mode_;

    MatrixMode triggeredMode_;

    bool buttonTriggered_;


    bool redWasDown_;

    bool blueWasDown_;
};
