#include "gantt_exporter.hpp"

using namespace Utils;

GanttExporter::GanttExporter(vector<Core::TCB *> *task_list)
{
    tasks = task_list;
}

GanttExporter::~GanttExporter()
{
    tasks = nullptr;
}

// Função para registrar cada tarefa em todo tick, enquanto o programa executa
void GanttExporter::registerEntry(int tick, int task_index, int color)
{
    chart_history.push_back({
        tick: tick,
        task_index: task_index,
        color: convertColor(color)
    });
}

void GanttExporter::generate(const string &filename, int total_time, int task_count)
{
    // Tamanho da imagem: +1 para os eixos
    int img_width = (total_time + 1) * TICK_WIDTH;
    int img_height = (task_count + 1) * TASK_HEIGHT;

    ofstream file(filename);
    if (!file.is_open())
        return;

    // Cria o svg e um background branco
    file << "<svg width='" << img_width << "' height='" << img_height << "' xmlns='http://www.w3.org/2000/svg'>\n";
    file << "<rect width='" << img_width << "' height='" << img_height << "' fill='white' />\n";

    // Eixo vertical (tasks)
    for (int i = 0; i < tasks->size(); i++)
    {
        int y = i * TASK_HEIGHT;
        file << "<text x='0' y='" << y + 5
             << "' width='" << TICK_WIDTH << "' height='" << TASK_HEIGHT
             << "' dominant-baseline='hanging'>" << (*tasks)[i]->getId() << "</text>";
    }

    // Cada entrada do gráfico
    for (const auto &entry : chart_history)
    {
        int x = (entry.tick + 1) * TICK_WIDTH;
        int y = entry.task_index * TASK_HEIGHT;
        string color = entry.color;

        file << "  <rect x='" << x << "' y='" << y
             << "' width='" << TICK_WIDTH << "' height='" << TASK_HEIGHT
             << "' fill='" << color << "' stroke='black' stroke-width='0.5'/>\n";
    }

    // Eixo horizontal (ticks)
    for (int i = 0; i < total_time + 1; i++)
    {
        int x = (i + 1) * TICK_WIDTH;
        file << "<text x='" << x << "' y='" << img_height - TASK_HEIGHT + 5
             << "' width='" << TICK_WIDTH << "' height='" << TASK_HEIGHT
             << "' dominant-baseline='hanging'>" << i << "</text>";
    }
    
    file << "</svg>\n";
    file.close();
}

// Conversão de cores do formato RGB para hexadecimal
string GanttExporter::convertColor(int color)
{
    short r, g, b;
    short fgcolor, bgcolor;
    pair_content(color, &fgcolor, &bgcolor);
    color_content(bgcolor, &r, &g, &b);

    // Converte o RGB (0-1000) do ncurses para 8-bit RGB (0-255)
    int red = (r * 255) / 1000;
    int green = (g * 255) / 1000;
    int blue = (b * 255) / 1000;

    // Realiza os deslocamentos de 8 bits (2^8-1 = 255)
    unsigned long hexa = ((red) << 16) | ((green) << 8) | ((blue));
    stringstream stream;
    // Formata o valor em hexa (tamanho 6 com '0' à esquerda)
    stream << setfill('0') << setw(6) << hex << hexa;

    return "#" + stream.str();
}