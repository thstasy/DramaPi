#include "DramaEngine.h"

#include <random>
#include <vector>

namespace
{
    std::string pickRandom(const std::vector<std::string>& lines)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_int_distribution<> dist(0, lines.size() - 1);
        return lines[dist(gen)];
    }
}

std::string DramaEngine::comment(
    DramaEvent event,
    Personality personality
) const
{
    return pickRandom(library.quotesFor(event, personality));
}