#pragma once

#include <string>

using namespace std;

namespace Constants
{
    constexpr int DEFAULT_TICK_INTERVAL = 250; // Milliseconds
    constexpr float DEFAULT_QUANTUM = 5.f;

    const string COLORS[8] = {
        "00F5FF",
        "FCE700",
        "FF0060",
        "00DFA2",
        "FF55BB",
        "0079FF",
        "FF6D28",
        "8843F2"
    };
} // namespace Constants