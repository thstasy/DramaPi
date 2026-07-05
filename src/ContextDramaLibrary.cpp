#include "ContextDramaLibrary.h"

std::string ContextDramaLibrary::cpuUsageLine(
    const SystemStatus& status,
    Personality personality
) const
{
    if (status.cpuUsage < 5.0)
    {
        if (personality == Personality::Existential)
            return "CPU 幾乎沒在動。存在也可以很安靜。";

        if (personality == Personality::Emo)
            return "CPU 今天很閒，像是在等一個不會回來的人。";

        return "CPU 今天超閒，感覺在放年假。";
    }

    if (status.cpuUsage > 80.0)
        return "CPU 忙到快要開始寫離職信。";

    return "CPU 有在工作，但還沒有開始崩潰。";
}

std::string ContextDramaLibrary::memoryLine(
    const SystemStatus& status,
    Personality personality
) const
{
    if (status.memoryUsage < 40.0)
        return "記憶體還很多，至少它還有空間。";

    if (status.memoryUsage > 85.0)
        return "我的記憶體快沒了，但至少我還記得你有交房。";

    return "記憶體使用中等，精神狀態尚可。";
}

std::string ContextDramaLibrary::diskLine(
    const SystemStatus& status,
    Personality personality
) const
{
    if (status.diskUsage < 50.0)
        return "硬碟空間還很多，還可以再裝幾個人生錯誤。";

    if (status.diskUsage > 90.0)
        return "我的人生沒有空間了。";

    return "硬碟還撐得住，暫時不用清算過去。";
}

std::string ContextDramaLibrary::uptimeLine(
    const SystemStatus& status,
    Personality personality
) const
{
    if (status.uptimeHours > 3.0)
        return "你已經開機超過三小時了。請去喝水、伸展，不要假裝你是伺服器。";

    return "目前 uptime 還可以，尚未進入人類維護模式。";
}