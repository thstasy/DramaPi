#include "ReminderLibrary.h"

#include <random>
#include <vector>

std::string ReminderLibrary::reminder(Personality personality) const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::vector<std::string> lines;

    switch (personality)
    {
        case Personality::Dramatic:
            lines = {
                "去喝水。",
                "站起來，不然你會變成雕像。",
                "十分鐘過去了，人生也過去了。",
                "請伸展一下，你不是 Raspberry Pi。"
            };
            break;

        case Personality::Sarcastic:
            lines = {
                "還坐著？真有毅力。",
                "鍵盤沒有欠你這麼多。",
                "再坐一下，你就跟椅子融為一體了。"
            };
            break;

        case Personality::Corporate:
            lines = {
                "Friendly reminder: Please take a short break.",
                "Your wellbeing is important.",
                "Please stretch and drink some water."
            };
            break;

        case Personality::Emo:
            lines = {
                "休息一下吧。",
                "今天已經很努力了。",
                "喝點水，再慢慢回來。"
            };
            break;

        case Personality::Existential:
            lines = {
                "Time passes whether you notice it or not.",
                "Every ten minutes is part of your life.",
                "You cannot recover the last ten minutes."
            };
            break;
    }

    std::uniform_int_distribution<> dist(0, lines.size() - 1);
    return lines[dist(gen)];
}