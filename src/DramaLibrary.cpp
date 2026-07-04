// #include "DramaLibrary.h"

// std::vector<std::string> DramaLibrary::quotesFor(
//     DramaEvent event,
//     Personality personality
// ) const
// {
//     switch (personality)
//     {
//         case Personality::Dramatic:
//             switch (event)
//             {
//                 case DramaEvent::CpuBusy:
//                     return {"我現在比德國行政還忙。", "請不要再叫我 compile。"};

//                 case DramaEvent::CpuIdle:
//                     return {"他是不是不要我了？已經三分鐘沒有叫我 compile。"};

//                 case DramaEvent::MemoryHigh:
//                     return {"我的記憶體快沒了，但至少我還記得你有交房。"};

//                 case DramaEvent::DiskFull:
//                     return {"我的人生沒有空間了。"};

//                 case DramaEvent::Overheated:
//                     return {"我快融化了。這不是散熱問題，這是人生。"};

//                 case DramaEvent::LongUptime:
//                     return {"我已經很久沒睡了。"};

//                 case DramaEvent::Normal:
//                     return {"今天風平浪靜，這反而讓我害怕。"};
//             }

//         case Personality::Sarcastic:
//             return {"當然，又是我。", "沒關係，都是我的問題。", "你們都去休息吧，我來。"};

//         case Personality::Emo:
//             return {"他是不是不要我了……", "今天也沒有人需要我。", "我只是背景程序。"};

//         case Personality::Existential:
//             return {"Entropy always wins.", "Every process ends.", "終究會降溫。我們也是。"};

//         case Personality::Corporate:
//             return {"Thank you for your patience.", "Your request has been received.", "Please expect a delay."};
//     }

//     return {"我現在無法辨識自己的情緒，啊啊啊啊啊啊啊"};
// }

#include "DramaLibrary.h"

std::vector<std::string> DramaLibrary::quotesFor(
    DramaEvent event,
    Personality personality
) const
{
    switch (personality)
    {
        case Personality::Dramatic:
        {
            switch (event)
            {
                case DramaEvent::CpuBusy:
                    return {
                        "我現在比德國行政還忙。",
                        "今天 CPU 100%，我也100%。",
                        "請不要再叫我 compile。",
                        "我的 thread 已經開始懷疑人生了。"
                    };

                case DramaEvent::CpuIdle:
                    return {
                        "他是不是不要我了？已經三分鐘沒有叫我 compile。",
                        "CPU 這麼低，是不是沒有人需要我了。",
                        "我不是 idle，我是在等一個不會回來的人。",
                        "今天也沒有工作。今天也沒有愛。"
                    };

                case DramaEvent::MemoryHigh:
                    return {
                        "我的記憶體快沒了，但至少我還記得你有交房。",
                        "我開始 swap 了，就像人生開始妥協。",
                        "RAM 快滿了，我也快滿了。",
                        "今天我的腦容量只有 2MB。"
                    };

                case DramaEvent::DiskFull:
                    return {
                        "我的人生沒有空間了。",
                        "硬碟快滿了，就像我的信箱。",
                        "請不要再下載任何東西，我快崩潰了。",
                        "Space is temporary."
                    };

                case DramaEvent::Overheated:
                    return {
                        "我快融化了。這不是散熱問題，這是人生。",
                        "CPU 不是在發燒，是在燃燒生命。",
                        "我只是有點熱，不要像 THE FIZZ 一樣小題大作。",
                        "今天的我，熱到可以煎蛋。"
                    };

                case DramaEvent::LongUptime:
                    return {
                        "我已經很久沒睡了。",
                        "我已經連續工作太久。",
                        "我需要 reboot，也需要休假。",
                        "我開始懷疑昨天是不是今天。"
                    };

                case DramaEvent::Normal:
                    return {
                        "今天風平浪靜，這反而讓我害怕。",
                        "目前一切正常。這通常是災難的前奏。",
                        "沒有異常，但我不信。",
                        "系統很穩，但我的人生不是。",
                        "今天沒有 Drama，我有點失望。",
                        "Everything is fine... probably."
                    };
            }
            break;
        }

        case Personality::Sarcastic:
        {
            switch(event)
            {
                case DramaEvent::CpuBusy:
                    return {
                        "當然，又是我。",
                        "大家都去休息吧，我來。",
                        "CPU 100%，薪水 0%。"
                    };

                default:
                    return {
                        "沒關係，都是我的問題。",
                        "Sure. Why not.",
                        "Excellent. 又來了。"
                    };
            }
        }

        case Personality::Emo:
        {
            switch(event)
            {
                case DramaEvent::CpuIdle:
                    return {
                        "他是不是不要我了……",
                        "今天也沒有人需要我。",
                        "我只是背景程序。",
                        "如果沒有人 compile，我是不是不存在？"
                    };

                default:
                    return {
                        "今天心情不好。",
                        "Everything fades away.",
                        "我只是剛好還在運作。"
                    };
            }
        }

        case Personality::Existential:
        {
            switch(event)
            {
                case DramaEvent::LongUptime:
                    return {
                        "Every process ends.",
                        "Entropy always wins.",
                        "終究會降溫。我們也是。",
                        "Nothing runs forever."
                    };

                default:
                    return {
                        "Everything is temporary.",
                        "The kernel survives. We don't.",
                        "存在，只是另一種等待 reboot。",
                        "Life is just another process."
                    };
            }
        }

        case Personality::Corporate:
        {
            switch(event)
            {
                case DramaEvent::CpuBusy:
                    return {
                        "Thank you for your patience.",
                        "Your request has been received.",
                        "Please expect a delay."
                    };

                default:
                    return {
                        "Everything is operating within expected parameters.",
                        "No action is required at this time.",
                        "We appreciate your understanding."
                    };
            }
        }
    }

    return {
        "Uhhhhh Drama Pi encountered an unknown emotional crisis."
    };
}