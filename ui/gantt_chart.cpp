#include "gantt_chart.hpp"
#include <climits>

using namespace UI;

GanttChart::GanttChart(Utils::GanttExporter *chart_gen) : TaskVisual()
{
    gantt_exporter = chart_gen;
}

GanttChart::~GanttChart()
{
    gantt_exporter = nullptr;
}

void GanttChart::drawTick(int tick)
{
    // Um tick é igual a três colunas e desenhamos por coluna
    int x = visual_edge_x;
    string unit = string(UNIT_WIDTH, ' ');

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB *task = (*ord_tasks)[i];
        int color;

        switch (task->getState())
        {
        case TCBState::RUNNING:
            color = setColor(i);
            break;
        case TCBState::READY:
            color = setColor(DefaultColor::GRAY);
            break;
        case TCBState::SUSPENDED:
            color = setColor(DefaultColor::GRAY);
            break;
        default:
            color = setColor(DefaultColor::BLACK);
            break;
        }

        // Registra como está a task no ponto do tick para criação da imagem final
        gantt_exporter->registerEntry(tick, i, color);
        print(x, i + y_offset, unit);
    }

    setColor(DefaultColor::WHITE); // branco no preto
    print(x, ord_tasks->size() + y_offset, to_string(tick));
    refresh();
}

// Seta o tamanho do gráfico dinâmicamente com base na quantidade de tasks e duração
void GanttChart::setTasks(vector<TCB *> *tasks, int y_offset)
{
    TaskVisual::setTasks(tasks, y_offset);

    int total_time = 0;
    int earliest_start = INT_MAX;
    for (TCB *task : *tasks)
    {
        total_time += task->getDuration();
        earliest_start = task->getStart() < earliest_start ? task->getStart() : earliest_start;
    }
    total_time += earliest_start;

    setWindowDimensions(
        tasks->size() + 1,
        ((total_time + 1) * UNIT_WIDTH) + x_offset,
        0,
        0
    );
}