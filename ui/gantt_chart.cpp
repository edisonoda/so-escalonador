#include "gantt_chart.hpp"

using namespace UI;

GanttChart::GanttChart(Utils::ChartGenerator *chart_gen) : TaskVisual()
{
    chart_generator = chart_gen;
}

GanttChart::~GanttChart()
{
    chart_generator = nullptr;
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
        chart_generator->registerEntry(tick, i, color);
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
    int latest_start = 0;
    for (TCB *task : *tasks)
    {
        total_time += task->getDuration();
        latest_start = task->getStart() > latest_start ? task->getStart() : latest_start;
    }
    total_time += latest_start;

    setWindowDimensions(
        tasks->size() + 1,
        ((total_time) * UNIT_WIDTH) + x_offset,
        0,
        0
    );
}