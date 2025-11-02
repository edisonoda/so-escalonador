#include "menu.hpp"
#include "../core/scheduler.hpp"
#include "../core/setup_manager.hpp"

#include <algorithm>
#include <ncurses.h>
#include <string>

using namespace UI;

// Menu declaration

const vector<int> Menu::navigation_keys({KEY_UP, KEY_DOWN});
const vector<int> Menu::confirm_keys({KEY_ENTER, KEY_RIGHT, '\n', ' '});

Menu::Menu(SetupUI *ui) : Window(), ui(ui) { selected = 0; }

Menu::~Menu() { ui = nullptr; }

void Menu::setupMenu(string title, vector<string> options) {
  this->title = title;
  this->options = options;
  height = options.size() + 3;
  screen->erase();
  wresize(window, height, width);
  screen->refresh();

  valid_entries.clear();
  valid_entries.push_back(KEY_ENTER);
  valid_entries.push_back('\n');
  for (int i = 0; i < options.size(); i++)
    valid_entries.push_back('0' + i + 1);

  selected = 0;
}

int Menu::showMenu() {
  printMenu();

  int ch = wgetch(window);

  while (find(valid_entries.begin(), valid_entries.end(), ch) ==
         valid_entries.end()) {
    if (find(navigation_keys.begin(), navigation_keys.end(), ch) !=
        navigation_keys.end())
      navigate(ch);
    else {
      ui->showError("Digite uma opção válida!");
      ui->clearMessage();
    }

    printMenu();
    ch = wgetch(window);
  }

  if (find(confirm_keys.begin(), confirm_keys.end(), ch) != confirm_keys.end())
    return '0' + selected + 1;

  return ch;
}

void Menu::printMenu() {
  int i = 0;

  clear();
  print(0, 0, title);

  for (int it = 0; it < options.size(); it++)
    printOption(it, options.at(it));

  move(0, getPosY() + 1);
  refresh();
}

void Menu::navigate(int dir) {
  int max_options = options.size();

  // Move para cima ou para baixo, dependendo do código da seta
  switch (dir) {
  case KEY_UP:
    selected--;
    break;
  case KEY_DOWN:
    selected++;
    break;
  }

  // Mantém a seleção dentre as opções
  selected = (selected + max_options) % max_options;
}

void Menu::printOption(int index, string option) {
  // Se a opção for a selecionada, formata ela
  if (index == selected) {
    setColor(DefaultColor::GREEN);
    option = "> " + option;
  }

  print(0, index + 1, "[" + to_string(index + 1) + "] " + option);
  setColor(DefaultColor::WHITE);
}

// SetupUI declaration

SetupUI::SetupUI(Core::SimulationConfig *config)
    : screen(Screen::getInstance()), config(config), menu(this) {
  menu.setWindowDimensions(0, MAIN_WIDTH, 0, 0);
  task_info.setWindowDimensions(0, 0, INFO_X_OFFSET, 0);
  mensagem.setWindowDimensions(3, MAIN_WIDTH, 0, 0);
  input.setWindowDimensions(5, MAIN_WIDTH, 0, 0);

  screen->attach(this);
}

SetupUI::~SetupUI() {
  screen->detach(this);
  screen = nullptr;
  config = nullptr;
}

void SetupUI::update() {
  int y = 0;

  task_info.clear();
  task_info.print(0, y++, "--- Configuração Atual ---");

  string alg_str;
  switch (config->alg_id) {
  case Core::AlgorithmID::FIFO:
    alg_str = "FIFO";
    break;
  case Core::AlgorithmID::PRIOp:
    alg_str = "PRIOp";
    break;
  case Core::AlgorithmID::SRTF:
    alg_str = "SRTF";
    break;
  }

  task_info.print(0, y++, "Modo: " + string(1, config->mode));
  task_info.print(0, y++, "Algoritmo: " + alg_str);
  task_info.print(0, y++, "Quantum:   " + to_string(config->quantum));
  task_info.print(0, y++, "Tasks (" + to_string(config->tasks.size()) + "):");

  task_info.setTasks(&(config->tasks), y);

  if (!config->tasks.empty())
    for (int i = 0; i < config->tasks.size(); i++)
      task_info.drawStaticInfo(i);

  task_info.moveWindow(INFO_X_OFFSET, 0);
  task_info.refresh();
}

int SetupUI::showMainMenu() {
  menu.setupMenu("--- SETUP DA SIMULAÇÃO ---",
                 {"Iniciar", "Carregar", "Editar", "Restaurar PADRÃO",
                  "Trocar modo", "Sair do programa"});

  return menu.showMenu();
}

int SetupUI::showEditor() {
  menu.setupMenu(
      "--- EDITAR CONFIGURAÇÕES ---",
      {"Algoritmo", "Quantum", "Tarefas", "Voltar ao Menu Principal"});

  return menu.showMenu();
}

int SetupUI::showTaskList() {
  vector<string> options = {"Adicionar", "Remover"};

  for (Core::TCB *task : config->tasks)
    options.push_back("[" + task->getId() + "]");

  options.push_back("Voltar");

  menu.setupMenu("--- EDITAR LISTA DE TAREFAS ---", options);
  return menu.showMenu();
}

int SetupUI::showAlgorithm() {
  menu.setupMenu("--- ESCOLHER ALGORITMO ---",
                 {"FIFO", "PRIOp", "SRTF", "Voltar"});
  return menu.showMenu();
}

int SetupUI::showTaskEditor(string id) {
  string title = "--- ADICIONAR TAREFA ---";

  if (!id.empty())
    title = "--- EDITAR TAREFA " + id + " ---";

  menu.setupMenu(title,
                 {"Id", "Cor", "Início", "Duração", "Prioridade", "Sair"});

  return menu.showMenu();
}

void SetupUI::showError(const string &message, Window *window) {
  if (window)
    mensagem.moveWindow(0, window->getHeight());
  else
    mensagem.moveWindow(0, menu.getHeight());

  mensagem.print(0, 0, "--- ERRO ---");
  mensagem.print(0, 1, message);
  mensagem.print(0, 2, "Pressione qualquer tecla para seguir.");
  mensagem.refresh();
  getch();
}

void SetupUI::inputError(const string &message) { 
  showError(message, &input); 
  clearMessage();
}

void SetupUI::clearMessage() {
  mensagem.clear();
  mensagem.refresh();

  screen->erase();
  screen->refresh();
}

string SetupUI::promptForField(string field) {
  screen->erase();
  screen->refresh();

  input.clear();
  input.print(0, 0, "--- Inserir " + field + " ---");
  input.print(0, 1, "> ");
  input.refresh();

  return readString();
}

string SetupUI::promptForFilename() {
  screen->erase();
  screen->refresh();

  input.clear();
  input.print(0, 0, "--- Carregar Arquivo de Configuração ---");
  input.print(0, 1, "Digite o nome do arquivo (ex: meu_arquivo.txt):");
  input.print(0, 2, "> ");
  input.refresh();

  return readString();
}

string SetupUI::readString() {
  int ch = input.getCh();
  string str = "";
  vector<int> valid_entries = {'_', ' ', '-', '.'};

  while (ch != '\n') {
    if (ch == KEY_BACKSPACE || ch == '\b' || ch == 127) {
      if (!str.empty()) {
        input.move(input.getPosX() - 1, input.getPosY());
        input.print(' ');
        input.move(input.getPosX() - 1, input.getPosY());
        str.pop_back();
      }
    } else if (isalnum(ch) || find(valid_entries.begin(), valid_entries.end(),
                                   ch) != valid_entries.end()) {
      input.print(ch);
      str += ch;
    }

    input.refresh();
    ch = input.getCh();
  }

  return str;
}