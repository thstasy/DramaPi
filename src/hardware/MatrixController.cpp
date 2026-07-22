#include "MatrixController.h"

#include <iostream>


MatrixController::MatrixController(
    Button& redButton,
    Button& blueButton,
    RGBLed& rgbLed
)
:
redButton_(redButton),
blueButton_(blueButton),
rgbLed_(rgbLed),
mode_(MatrixMode::Neutral),
triggeredMode_(MatrixMode::Neutral),
buttonTriggered_(false),
redWasDown_(false),
blueWasDown_(false)
{
}


void MatrixController::init()
{
    redButton_.init();
    blueButton_.init();
    rgbLed_.init();

    redButton_.update();
    blueButton_.update();

    redWasDown_ =
        redButton_.isPressed();

    blueWasDown_ =
        blueButton_.isPressed();

    mode_ =
        MatrixMode::Neutral;

    triggeredMode_ =
        MatrixMode::Neutral;

    buttonTriggered_ =
        false;

    showModeColor();
}


void MatrixController::updateButtons()
{
    redButton_.update();
    blueButton_.update();

    buttonTriggered_ =
        false;

    const bool redDown =
        redButton_.isPressed();

    const bool blueDown =
        blueButton_.isPressed();


    const bool redJustPressed =
        redDown
        && !redWasDown_;

    const bool blueJustPressed =
        blueDown
        && !blueWasDown_;


    /*
     * 兩顆同時按住，而且至少有一顆是這一輪才按下。
     */
    if(
        redDown
        && blueDown
        && (
            redJustPressed
            || blueJustPressed
        )
    )
    {
        trigger(
            MatrixMode::Neutral
        );
    }
    else if(redJustPressed)
    {
        trigger(
            MatrixMode::RedPill
        );
    }
    else if(blueJustPressed)
    {
        trigger(
            MatrixMode::BluePill
        );
    }


    redWasDown_ =
        redDown;

    blueWasDown_ =
        blueDown;
}


HumanState MatrixController::applyMode(
    HumanState detectedState
) const
{
    if(
        mode_ == MatrixMode::BluePill
        && detectedState != HumanState::Alone
    )
    {
        return HumanState::Nearby;
    }

    return detectedState;
}


void MatrixController::updateColor(
    HumanState humanState
)
{
    if(humanState == HumanState::Emergency)
    {
        rgbLed_.white();

        return;
    }

    if(
        humanState == HumanState::Approaching
        || humanState == HumanState::Close
        || humanState == HumanState::PersonalSpace
    )
    {
        rgbLed_.yellow();

        return;
    }

    showModeColor();
}


void MatrixController::showModeColor()
{
    switch(mode_)
    {
        case MatrixMode::Neutral:

            rgbLed_.green();

            break;


        case MatrixMode::BluePill:

            rgbLed_.blue();

            break;


        case MatrixMode::RedPill:

            rgbLed_.red();

            break;
    }
}


MatrixMode MatrixController::mode() const
{
    return mode_;
}


bool MatrixController::buttonTriggered() const
{
    return buttonTriggered_;
}


MatrixMode MatrixController::triggeredMode() const
{
    return triggeredMode_;
}


void MatrixController::trigger(
    MatrixMode selectedMode
)
{
    /*
     * 即使 selectedMode 和目前 mode_ 相同，
     * 仍然要產生一次新的按鈕事件。
     */
    mode_ =
        selectedMode;

    triggeredMode_ =
        selectedMode;

    buttonTriggered_ =
        true;

    showModeColor();


    switch(selectedMode)
    {
        case MatrixMode::Neutral:

            std::cout
                << "Matrix button: Neutral - Reality reset\n";

            break;


        case MatrixMode::BluePill:

            std::cout
                << "Matrix button: Blue Pill - Asleep\n";

            break;


        case MatrixMode::RedPill:

            std::cout
                << "Matrix button: Red Pill - Truth hurts\n";

            break;
    }
}
