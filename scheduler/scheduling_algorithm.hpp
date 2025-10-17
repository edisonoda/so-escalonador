#pragma once

#include <string>
#include <list>
#include "../core/tcb.hpp"

using namespace std;

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
        list<Core::TCB*>* task_list;
    
    public:
        SchedulingAlgorithm(list<Core::TCB*>* task_list):task_list(task_list) { };
        ~SchedulingAlgorithm() { };

        string getName() const { return name; }

        virtual string chooseTask() = 0;
    };
} // namespace Scheduler