#pragma once

#include <string>
#include <list>
#include "../core/tcb.hpp"

using namespace std;
using namespace Core;

namespace Scheduler
{
    // enum class AlgorithmID {
    //     FCFS,
    //     SRTF,
    //     PRIOp
    // };

    class SchedulingAlgorithm
    {
    protected:
        const static string name;
        list<TCB*>* task_list;
    
    public:
        SchedulingAlgorithm(list<TCB*>* task_list):task_list(task_list) { };
        virtual ~SchedulingAlgorithm() { };

        string getName() const { return name; }

        virtual string chooseTask() = 0;
    };
} // namespace Scheduler