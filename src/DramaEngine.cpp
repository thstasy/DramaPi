#include "DramaEngine.h"

std::string DramaEngine::comment(const SystemStatus& status) const
{
    double temp = status.cpuTemperature;

    if (temp < 50.0) {
        return "我只是微溫，不要像 THE FIZZ 一樣小題大作。";
    } else if (temp < 65.0) {
        return "我有點熱，但至少我還沒有被德國行政逼瘋。";
    } else if (temp < 75.0) {
        return "我現在比德國行政還忙，請不要再叫我 compile。";
    } else {
        return "我快融化了。這不是散熱問題，這是生活困境。";
    }
}