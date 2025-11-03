#include "task_visual.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

using namespace UI;

// TaskVisual Definition

TaskVisual::TaskVisual() : Window() {
  // Guarda os limites do que foi escrito em tela
  visual_edge_x = 0;
  visual_edge_y = 0;
  // Guarda a posição inicial das informações das tarefas
  x_offset = 0;
  y_offset = 0;
  ord_tasks = nullptr;
}

TaskVisual::~TaskVisual() { ord_tasks = nullptr; }

void TaskVisual::printAxis() {
  invertColor(true);
  
  // Busca o tamanho do maior ID e imprime todos os IDs
  int max_lenght = 0;
  for (size_t i = 0; i < ord_tasks->size(); i++) {
    Core::TCB *task = (*ord_tasks)[i];
    string id_str = task->getId();
    if (id_str.length() >= max_lenght)
      max_lenght = id_str.length();

    setColor(i);

    print(0, i + y_offset, id_str);
  }

  x_offset = max_lenght + 1;

  setColor(DefaultColor::WHITE);
  invertColor(false);

  for (size_t i = 0; i < ord_tasks->size(); i++)
    print(x_offset, i + y_offset, '|');
}

void TaskVisual::setTasks(vector<Core::TCB *> *tasks, int y_offset) {
  ord_tasks = tasks;
  this->y_offset = y_offset;
  printAxis();
}

void TaskVisual::print(int x, int y, string str) {
  checkEdges(x + str.length(), y);
  Window::print(x, y, str);
}

void TaskVisual::print(int x, int y, int ch) {
  checkEdges(x + 1, y);
  Window::print(x, y, ch);
}

// Se o caracter impresso estiver fora do limite aumenta o limite
void TaskVisual::checkEdges(int x, int y) {
  if (x > visual_edge_x)
    visual_edge_x = x;

  if (y > visual_edge_y)
    visual_edge_y = y;
}

// TaskInfo Definition

static string toStringWithPrecision(double val, int precision = 2) {
  stringstream stream;
  stream << fixed << setprecision(precision) << val;
  return stream.str();
}

TaskInfo::TaskInfo() : TaskVisual() {
  // Faz o mapeamento dos parâmetros de exibição das tasks
  MONITOR_LABELS = {
    {"Color",     "COLOR: "},
    {"Start",     "START: "},
    {"Duration",  "DURATION: "},
    {"Priority",  "PRIORITY: "},
    {"Remaining", "REMAINING: "}
  };

  // Faz o mapeamento dos possíveis status das tasks
  MONITOR_LABELS_STATUS = {
    {"New",         "NEW        "},
    {"Ready",       "READY      "},
    {"Running",     "RUNNING    "},
    {"Suspended",   "SUSPENDED  "},
    {"Terminated",  "TERMINATED "},
    {"Error",       "ERROR      "}
  };
}

TaskInfo::~TaskInfo() {}

void TaskInfo::setTasks(vector<Core::TCB *> *tasks, int y_offset) {
  // Chama a função original (Function overload)
  TaskVisual::setTasks(tasks, y_offset);

  // Define o tamanho da janela
  int width = MONITOR_LABELS_STATUS["Terminated"].length() + INFO_SPACE;
  for (auto label : MONITOR_LABELS)
    width += label.second.length() + INFO_SPACE;

  setWindowDimensions(
    tasks->size() + y_offset + STATS_SPACE,
    width + x_offset,
    0,
    tasks->size() + 3
  );

  printAxis();
}

void TaskInfo::drawTick(int tick) {
  int x;

  // Imprime as informações atuais de cada task
  for (size_t i = 0; i < ord_tasks->size(); i++) {
    Core::TCB *task = (*ord_tasks)[i];
    x = x_offset;

    setColor(i);
    invertColor(true);

    Core::TCBState status = task->getState();
    string status_str;
    string rem_str = MONITOR_LABELS["Remaining"] + to_string(task->getRemaining());

    switch (status) {
      case Core::TCBState::NEW:
        status_str = MONITOR_LABELS_STATUS["New"];
        break;
      case Core::TCBState::READY:
        status_str = MONITOR_LABELS_STATUS["Ready"];
        break;
      case Core::TCBState::RUNNING:
        invertColor(false);
        status_str = MONITOR_LABELS_STATUS["Running"];
        break;
      case Core::TCBState::SUSPENDED:
        status_str = MONITOR_LABELS_STATUS["Suspended"];
        break;
      case Core::TCBState::TERMINATED:
        status_str = MONITOR_LABELS_STATUS["Terminated"];
        break;
      default:
        status_str = MONITOR_LABELS_STATUS["Error"];
        break;
    }

    print(x, i + y_offset, status_str);
    invertColor(true);
    print(x = x + INFO_SPACE + MONITOR_LABELS_STATUS["Terminated"].length(), i + y_offset, rem_str);

    // Imprime as informações estáticas da tarefa
    drawStaticInfo(i, x + INFO_SPACE + MONITOR_LABELS["Remaining"].length() - x_offset);
  }

  refresh();
}

void TaskInfo::drawStaticInfo(int i, int offset) {
  if (!ord_tasks)
    return;

  int x = x_offset + offset;
  Core::TCB *task = (*ord_tasks)[i];

  string color_str = MONITOR_LABELS["Color"] + to_string(task->getColor());
  string start_str = MONITOR_LABELS["Start"] + to_string(task->getStart());
  string duration_str = MONITOR_LABELS["Duration"] + to_string(task->getDuration());
  string prio_str = MONITOR_LABELS["Priority"] + to_string(task->getPriority());

  setColor(DefaultColor::WHITE);
  invertColor(false);

  // Faz a impressão com base no final da anterior
  print(x, i + y_offset, color_str);
  print(x = x + INFO_SPACE + MONITOR_LABELS["Color"].length(), i + y_offset, start_str);
  print(x = x + INFO_SPACE + MONITOR_LABELS["Start"].length(), i + y_offset, duration_str);
  print(x = x + INFO_SPACE + MONITOR_LABELS["Duration"].length(), i + y_offset, prio_str);
}

// Faz o cálculo dos tempos médios
void TaskInfo::calcFinalStatistics(double *avg_turnaround, double *avg_wait) {
  double total_turnaround_time = 0;
  double total_wait_time = 0;

  if (ord_tasks->empty())
    return;

  for (Core::TCB *task : *ord_tasks) {
    int arrival = task->getStart();
    int duration = task->getDuration();
    int completion = task->getCompletionTime();

    int turnaround = completion - arrival;
    total_turnaround_time += turnaround;

    int wait = turnaround - duration;
    total_wait_time += wait;
  }

  (*avg_turnaround) = total_turnaround_time / ord_tasks->size();
  (*avg_wait) = total_wait_time / ord_tasks->size();
}

void TaskInfo::displayFinalStatistics() {
  // Força um último "tick" para redesenhar a janela com as estatísticas finais
  drawTick(0);

  setColor(DefaultColor::WHITE);
  invertColor(false);

  // Posição Y: 1 linha abaixo da última task
  int stats_y_pos = y_offset + ord_tasks->size() + 1;
  double avg_turnaround = 0;
  double avg_wait = 0;

  calcFinalStatistics(&avg_turnaround, &avg_wait);

  string turn_str = "Tempo de Vida Médio:   " + toStringWithPrecision(avg_turnaround);
  string wait_str = "Tempo de Espera Médio: " + toStringWithPrecision(avg_wait);

  print(1, stats_y_pos, turn_str);
  print(1, stats_y_pos + 1, wait_str);
  refresh();
}

// GanttExporter definition

GanttExporter::GanttExporter(vector<Core::TCB *> *task_list) {
  tasks = task_list;
}

GanttExporter::~GanttExporter() { tasks = nullptr; }

// Função para registrar cada tarefa em todo tick, enquanto o programa executa
void GanttExporter::registerEntry(int tick, int task_index, int color) {
  chart_history.push_back( {tick, task_index, convertColor(color)});
}

void GanttExporter::generate(const string &filename, int total_time, int task_count) {
  int max_id = TICK_WIDTH;
  for (int i = 0; i < tasks->size(); i++) {
    string id = (*tasks)[i]->getId();
    int length = id.length() * 10;

    if (length > max_id)
      max_id = length;
  }

  // Largura: +1 para o número do tick final e +max_id para o eixo vertical (tarefas)
  int img_width = ((total_time + 1) * TICK_WIDTH) + max_id;

  // Altura: +1 para o eixo horizontal (ticks)
  int img_height = (task_count + 1) * TASK_HEIGHT;

  ofstream file(filename);
  if (!file.is_open())
    return;

  // Cria o svg e um background branco
  file << "<svg width='" << img_width << "' height='" << img_height
       << "' font-family='Courier New'" << " xmlns='http://www.w3.org/2000/svg'>\n";
  file << "<rect width='" << img_width << "' height='" << img_height
       << "' fill='white' />\n";

  // Eixo vertical (tasks)
  for (int i = 0; i < tasks->size(); i++) {
    int y = i * TASK_HEIGHT;

    file << "<text x='0' y='" << y + 5 << "' width='" << max_id
         << "' height='" << TASK_HEIGHT << "' dominant-baseline='hanging'>"
         << (*tasks)[i]->getId() << "</text>";
  }

  // Cada entrada do gráfico
  for (const auto &entry : chart_history) {
    int x = (entry.tick * TICK_WIDTH) + max_id;
    int y = entry.task_index * TASK_HEIGHT;
    string color = entry.color;

    file << "  <rect x='" << x << "' y='" << y << "' width='" << TICK_WIDTH
         << "' height='" << TASK_HEIGHT << "' fill='" << color
         << "' stroke='black' stroke-width='0.5'/>\n";
  }

  // Eixo horizontal (ticks)
  for (int i = 0; i <= total_time + 1; i++) {
    int x = (i * TICK_WIDTH) + max_id;

    file << "<text x='" << x << "' y='" << img_height - TASK_HEIGHT + 5
         << "' width='" << TICK_WIDTH << "' height='" << TASK_HEIGHT
         << "' dominant-baseline='hanging'>" << i << "</text>";
  }

  file << "</svg>\n";
  file.close();
}

// Conversão de cores do formato RGB para hexadecimal
string GanttExporter::convertColor(int color) {
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

// GanttChart definition

const vector<int> GanttChart::scrollKeys({KEY_LEFT, KEY_RIGHT});

GanttChart::GanttChart(GanttExporter *chart_gen) : TaskVisual() {
  gantt_exporter = chart_gen;
  // scrollok(window, true);
}

GanttChart::~GanttChart() { gantt_exporter = nullptr; }

void GanttChart::scrollChart() {
  timeout(-1);

  int scroll = 0;

  prefresh(window, 0, scroll, 0, 0, height, max_width - 1);

  int ch = wgetch(window);
  while(find(scrollKeys.begin(), scrollKeys.end(), ch) != scrollKeys.end()) {
    switch (ch) {
      case KEY_LEFT:
        if (scroll > 0)
          prefresh(window, 0, scroll -= 3, 0, 0, height, max_width - 1);
        break;
      case KEY_RIGHT:
        if (scroll < width - max_width - 1)
          prefresh(window, 0, scroll += 3, 0, 0, height, max_width - 1);
        break;
    }

    ch = wgetch(window);
  }
}

void GanttChart::drawTick(int tick) {
  // Um tick é igual a três colunas e desenhamos por coluna
  int x = visual_edge_x;
  string unit = string(UNIT_WIDTH, ' ');

  for (size_t i = 0; i < ord_tasks->size(); i++) {
    Core::TCB *task = (*ord_tasks)[i];
    int color;

    // Muda a cor dependendo do estado
    switch (task->getState()) {
      case Core::TCBState::RUNNING:
        color = setColor(i);
        break;
      case Core::TCBState::READY:
        color = setColor(DefaultColor::GRAY);
        break;
      case Core::TCBState::SUSPENDED:
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

// Define o tamanho do gráfico dinâmicamente com base na quantidade de tasks e duração
void GanttChart::setTasks(vector<Core::TCB *> *tasks, int y_offset) {
  TaskVisual::setTasks(tasks, y_offset);

  int total_time = 0;
  int latest_start = 0;

  // Busca o inicio mais tardio dentre as tarefas
  for (Core::TCB *task : *tasks) {
    total_time += task->getDuration();
    latest_start = task->getStart() > latest_start ? task->getStart() : latest_start;
  }

  total_time += latest_start;

  // Largura com base no offset e total máximo de ticks
  setWindowDimensions(
    tasks->size() + 1,
    ((total_time + 1) * UNIT_WIDTH) + x_offset,
    0,
    0
  );

  printAxis();
}