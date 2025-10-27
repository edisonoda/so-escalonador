#include "scheduler.hpp"
#include "fcfs_algorithm.hpp"
#include "srtf_algorithm.hpp"
#include "priop_algorithm.hpp"

using namespace std;

namespace Scheduler
{
    Scheduler *Scheduler::instance(nullptr);

    Scheduler::Scheduler() : task_list(nullptr), algorithm(nullptr) {}

    Scheduler::~Scheduler()
    {
        delete algorithm;
        algorithm = nullptr;
        instance = nullptr;
    }

    Scheduler *Scheduler::getInstance()
    {
        if (instance == nullptr)
            instance = new Scheduler();

        return instance;
    }

    void Scheduler::setAlgorithm(AlgorithmID id)
    {
        switch (id)
        {
        case AlgorithmID::FCFS:
            algorithm = new FCFS(task_list);
            break;

        case AlgorithmID::SRTF:
            algorithm = new SRTF(task_list);
            break;

        case AlgorithmID::PRIOp:
            algorithm = new PRIOp(task_list);
            break;

        default:
            algorithm = new FCFS(task_list);
            break;
        }
    }

    void Scheduler::setTaskList(list<TCB *> *task_list)
    {
        this->task_list = task_list;
    }

    TCB *Scheduler::chooseTask(TCB *current_task)
    {
        return algorithm->chooseTask(current_task);
    }
}
