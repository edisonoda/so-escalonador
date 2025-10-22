#pragma once

#include <vector>
#include "screen.hpp"
#include "../core/tcb.hpp"

using namespace std;
using namespace Core;

namespace UI
{
    class SystemMonitor
    {
    private:
        static SystemMonitor* instance;
        Screen* screen;
        vector<TCB*>* ord_tasks;

        SystemMonitor();

    public:
        static SystemMonitor* getInstance();

        ~SystemMonitor();
    };
} // namespace UI