#include "setup_manager.hpp"
#include <ncurses.h>
#include <string>

#define CONFIG_FILE "configs/default.txt"

using namespace Core;

bool isNumber(const string &s) {
  string::const_iterator it = s.begin();
  while (it != s.end() && isdigit(*it))
    ++it;
  return !s.empty() && it == s.end();
}

// ConfigReader definition

// O alg_map faz o mapeamento dos nomes dos algoritmos com os IDS
ConfigReader::ConfigReader(SetupUI *ui) : ui(ui), alg_map({
  {"FCFS", AlgorithmID::FIFO},
  {"RR", AlgorithmID::FIFO},
  {"FIFO", AlgorithmID::FIFO},
  {"PRIOp", AlgorithmID::PRIOp},
  {"SRTF", AlgorithmID::SRTF}
}) {}

ConfigReader::~ConfigReader() { closeFile(); }

AlgorithmID ConfigReader::getAlgorithm() const {
  return alg_map.find(algorithm)->second;
}

bool ConfigReader::openFile(const string &filename) {
  closeFile();
  file.open(filename);
  return file.is_open();
}

void ConfigReader::closeFile() {
  if (file.is_open())
    file.close();
}

string ConfigReader::readLine() {
  string line;
  if (getline(file, line))
    return line;
  return "";
}

// Busca o separador da primeira linha e a separa em algoritmo e quantum
bool ConfigReader::readPattern() {
  string pattern = readLine();
  if (!pattern.empty()) {
    size_t sep = pattern.find(';');
    if (sep != string::npos) {
      algorithm = pattern.substr(0, sep);

      // Verifica se o algoritmo lido está no mapa de algoritmos
      if (alg_map.find(algorithm) == alg_map.end())
        ui->inputError("Algoritmo inválido!");

      // Verifica se o quantum lido é um número
      string str = pattern.substr(sep + 1, pattern.length() - 1);
      if (isNumber(str))
        quantum = stoi(str);
      else
        ui->inputError("Valor de quantum inválido!");

    } else return false;
  } else return false;

  return true;
}


list<TCB *> ConfigReader::readTasks() {
  vector<string> configs = {};
  list<TCB *> tasks;
  string line;
  size_t pos = 0;

  while (!(line = readLine()).empty()) {

    // Busca todos os parâmetros separados por ponto e vírgula
    while (pos = line.find(';'), pos != string::npos) {
      configs.push_back(line.substr(0, pos));
      line.erase(0, pos + 1);
    }
    configs.push_back(line);

    // Verifica se há o número mínimo de parâmetros
    if (configs.size() >= 6) {
      bool valid = true;

      // Testa se as entradas numéricas são válidas
      for (int i = 1; i < 5; i++) {
        if (!isNumber(configs[i]) || (configs[i] == "0" && i > 2)) {
          ui->inputError("Tarefa com formato inválido: " + configs[0]);
          valid = false;
          break;
        }
      }

      if (valid) {
        TCB *tcb = new TCB(
          // id, cor, inicio, duracão e prioridade
          configs[0], 
          stoi(configs[1]), 
          stoi(configs[2]), 
          stoi(configs[3]),
          stoi(configs[4]), 
          list<string>(configs.begin() + 5, configs.end())
        );
    
        tasks.push_back(tcb);
      }
    } else {
      ui->inputError("Tarefa com informações insuficientes!");
    }

    configs.clear();
  }

  return tasks;
}

// SetupManager definition

SetupManager::SetupManager() : ui(&config), config_reader(&ui), screen(Screen::getInstance()) {
  // Carrega incialmente o arquivo padrão
  loadFromFile(CONFIG_FILE);
  timeout(-1);

  for (TCB *task : config.tasks)
    screen->initColor(0, task->getColor());

  ui.update();
}

SetupManager::~SetupManager() { screen = nullptr; }

SimulationConfig SetupManager::run() {
  bool in_editor = true;
  int ch = '1';

  do {
    ch = ui.showMainMenu();

    switch (ch) {
      case '1': // Iniciar a simulação
        in_editor = false;
        break;

      case '2': { // Configuração por arquivo
        string filename = ui.promptForFilename();
        if (!filename.empty())
          loadFromFile("configs/" + filename);
      } break;

      case '3': // Abre o editor
        runEditor();
        break;

      case '4': // Retorna à configuração padrão
        loadFromFile("configs/default.txt");
        break;

      case '5': // Troca o modo de execução
        if (config.mode == 'A')
          config.mode = 'P';
        else
          config.mode = 'A';
        break;

      case '6': // Sai do programa
        config.simulation_should_run = false;
        return config;
    }

    ui.update();

  } while (in_editor);

  config.simulation_should_run = true;
  screen->detach(&ui);
  return config;
}

// Faz a leitura de todos os parâmetros do arquivo
bool SetupManager::loadFromFile(const string &filename) {
  if (!config_reader.openFile(filename)) {
    ui.inputError("Não foi possível abrir: " + filename);
    return false;
  }

  if (!config_reader.readPattern()) {
    ui.inputError("Informações insuficientes de algoritmo ou quantum!");
    return false;
  }

  config.alg_id = config_reader.getAlgorithm();
  config.quantum = config_reader.getQuantum();

  for (TCB *task : config.tasks)
    delete task;
  config.tasks.clear();

  list<TCB *> tasks_list = config_reader.readTasks();
  config.tasks.assign(tasks_list.begin(), tasks_list.end());

  ui.update();

  return true;
}

void SetupManager::runEditor() {
  int ch;
  bool in_editor;
  string quantum;

  do {
    ch = ui.showEditor();

    switch (ch) {
      case '1': // Executa o editor de algoritmo
        runAlgorithmEditor();
        break;

      case '2': // Edita o quantum
        quantum = ui.promptForField("Quantum");
        if (validateEntry(quantum))
          config.quantum = stoi(quantum);
        break;

      case '3': // Executa o editor de tasks
        runTaskListEditor();
        break;

      default: // Volta ao menu principal
        in_editor = false;
        break;
    }
  } while (in_editor);

  ui.update();
}

void SetupManager::runTaskListEditor() {
  int ch;
  bool in_editor = true;

  do {
    ch = ui.showTaskList();

    switch (ch) {
      case '1': // Cria uma nova task
        addNewTask();
        break;

      case '2': // Deleta uma task existente
        deleteTask();
        break;

      default: // Edita a task selecionada
        ch = (ch - 3) - '0';
        if (ch < config.tasks.size())
          editTask(ch);
        else
          in_editor = false;
        break;
    }

  } while (in_editor);

  ui.update();
}

void SetupManager::runAlgorithmEditor() {
  int ch;
  bool in_editor = true;

  do {
    ch = ui.showAlgorithm();

    switch (ch) {
      case '1':
        config.alg_id = AlgorithmID::FIFO;
        break;

      case '2':
        config.alg_id = AlgorithmID::PRIOp;
        break;

      case '3':
        config.alg_id = AlgorithmID::SRTF;
        break;

      default:
        in_editor = false;
      break;
    }

  } while (in_editor);

  ui.update();
}

void SetupManager::addNewTask() {
  string id = ui.promptForField("Id");

  string color = ui.promptForField("Cor");
  if (!validateEntry(color))
    return;

  string start = ui.promptForField("Início");
  if (!validateEntry(start))
    return;

  string duration = ui.promptForField("Duração");
  if (!validateEntry(duration))
    return;

  string priority = ui.promptForField("Prioridade");
  if (!validateEntry(priority))
    return;

  TCB *task = new TCB(id, stoi(color), stoi(start), stoi(duration), stoi(priority));

  config.tasks.push_back(task);

  screen->initColor(0, stoi(color));

  ui.update();
}

void SetupManager::deleteTask() {
  bool valid = false;
  string str = ui.promptForField("Id");
  vector<TCB *>::iterator i = config.tasks.begin();

  // Percorre as tasks procurando a solicitada
  while (i != config.tasks.end()) {
    if ((*i)->getId() == str) {
      delete (*i);
      i = config.tasks.erase(i);
      valid = true;
    } else {
      i++;
    }
  }

  if (!valid)
    ui.inputError("Digite um ID válido!");

  ui.update();
}

void SetupManager::editTask(int index) {
  int ch;
  TCB *task;
  string entry;
  bool in_editor = true;

  task = config.tasks.at(index);

  do {
    ch = ui.showTaskEditor(task->getId());

    switch (ch) {
      case '1': // Edita o ID
        task->setId(ui.promptForField("Id"));
        break;

      case '2': // Edita a Cor
        entry = ui.promptForField("Cor");
        if (validateEntry(entry))
          task->setColor(stoi(entry));
        break;

      case '3': // Edita o Inicio
        entry = ui.promptForField("Início");
        if (validateEntry(entry))
          task->setStart(stoi(entry));
        break;

      case '4': // Edita a Duração
        entry = ui.promptForField("Duração");
        if (validateEntry(entry))
          task->setDuration(stoi(entry));
        break;

      case '5': // Edita a Prioridade
        entry = ui.promptForField("Prioridade");
        if (validateEntry(entry))
          task->setPriority(stoi(entry));
        break;

      default: // Volta à lista de tarefas
        in_editor = false;
        break;
    }

  } while (in_editor);

  ui.update();
}

bool SetupManager::validateEntry(string str) {
  if (!isNumber(str)) {
    ui.inputError("Digite um valor válido!");
    return false;
  }

  return true;
}