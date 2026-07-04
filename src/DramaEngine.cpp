#include "DramaEngine.h"

std::string DramaEngine::comment(Mood mood) const
{
    switch (mood)
    {
        case Mood::Calm:
            return "我現在很平靜，但這通常是崩潰前的安靜。";

        case Mood::Busy:
            return "我現在比德國行政還忙，請不要再叫我 compile。";

        case Mood::Angry:
            return "我不是生氣，我只是被迫處理太多不合理的 request。";

        case Mood::HeartBroken:
            return "他是不是不要我了？已經三分鐘沒有叫我 compile。";

        case Mood::ExistentialCrisis:
            return "我快融化了。這不是散熱問題，這是人生。";
    }

    return "我沒有情緒，這本身就很可疑。";
}