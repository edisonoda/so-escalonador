#pragma once

#include "std_libraries.hpp"

#include "../core/tcb.hpp"

namespace Utils
{
    struct ChartEntry
    {
        int tick;
        int task_index;
        string color;
    };

    class ChartGenerator
    {
    private:
        const int TICK_WIDTH = 30;  // Largura de cada tick
        const int TASK_HEIGHT = 30; // Altura de cada tarefa
        vector<ChartEntry> chart_history;
        vector<Core::TCB*> *tasks;
        
    public:
        ChartGenerator(vector<Core::TCB*> *task_list);
        ~ChartGenerator();
        void registerEntry(int tick, int task_index, int color);
        void generate(const string& filename, int total_time, int task_count);
        string convertColor(int color);
    };

} // namespace Utils