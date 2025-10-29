#include "setup_ui.hpp"
#include "../core/setup_manager.hpp"
#include "screen.hpp"
#include <ncurses.h>
#include <string>

#define MSG_Y_OFFSET 15
#define INFO_X_OFFSET 60

using namespace UI;

SetupUI::SetupUI(SimulationConfig* config)
    : screen(UI::Screen::getInstance())
    , config(config)
{
    selected = 0;
}

SetupUI::~SetupUI()
{
    screen = nullptr;
    config = nullptr;
}

void SetupUI::updateInfo()
{
    int y = 0;

    screen->print(INFO_X_OFFSET, y++, "--- Configuração Atual ---");
    string alg_str;
    switch(config->alg_id)
    {
        case Scheduler::AlgorithmID::FCFS: alg_str = "FCFS"; break;
        case Scheduler::AlgorithmID::PRIOp: alg_str = "PRIOp"; break;
        case Scheduler::AlgorithmID::SRTF: alg_str = "SRTF"; break;
    }
    screen->print(INFO_X_OFFSET, y++, "Algoritmo: " + alg_str);
    screen->print(INFO_X_OFFSET, y++, "Quantum:   " + to_string(config->quantum));
    screen->print(INFO_X_OFFSET, y++, "Tasks (" + to_string(config->tasks.size()) + "):");

    if (!config->tasks.empty())
        for (size_t i = 0; i < config->tasks.size(); i++)
        {
            TCB *task = config->tasks[i];
            screen->setColor(i); // Cor da tarefa
            screen->invertColor(true);
            screen->print(INFO_X_OFFSET, y, task->getId());
            screen->print(INFO_X_OFFSET + 4, y, "COLOR: " + to_string(task->getColor()) + " ");
            
            screen->setColor(DefaultColor::WHITE);
            screen->invertColor(false);
            
            string start_str = "| START: " + to_string(task->getStart()) + " ";
            string duration_str = "| DURATION: " + to_string(task->getDuration()) + " ";
            string prio_str = "| PRIORITY: " + to_string(task->getPriority()) + " ";
            
            screen->print(INFO_X_OFFSET + 14, y, start_str);
            screen->print(INFO_X_OFFSET + 26, y, duration_str);
            screen->print(INFO_X_OFFSET + 41, y, prio_str);
            
            y++;
        }

    screen->refresh();
}

char SetupUI::navigateMainMenu()
{
    showMainMenu();

    char ch = getch();

    // Flechas (3 ch): \033 + [ + (A, B, C ou D)
    while (ch == '\033')
    {
        getch();
        navigate(getch(), 5);
        showMainMenu();
        ch = getch();
    }

    return ch;
}

void SetupUI::showMainMenu()
{
    int i = 0;

    screen->clear();
    screen->print(0, 0, "--- SETUP DA SIMULAÇÃO ---");

    printOption(i++, "Iniciar (Modo: " + string(1, config->mode) + ")");
    printOption(i++, "Carregar");
    printOption(i++, "Editar");
    printOption(i++, "Restaurar PADRÃO");
    printOption(i++, "Sair do programa");

    screen->print(0, i + 2, "*Pressione -Espaço- para alterar o modo de execução");

    screen->refresh();
    updateInfo();
}

void SetupUI::showEditor()
{
}

void SetupUI::showTaskEditor()
{
}

void SetupUI::navigate(char dir, int max_options)
{
    // Move para cima ou para baixo, dependendo do código da seta
    switch (dir)
    {
        case 'A': selected--; break;
        case 'B': selected++; break;
    }

    // Mantém a seleção dentre as opções
    selected = (selected + max_options) % max_options;
}

void SetupUI::printOption(int index, string text)
{
    // Se a opção for a selecionada, formata ela
    if (index == selected)
    {
        screen->setColor(DefaultColor::GREEN);
        text = "> " + text;
    }

    index++;
    screen->print(0, index, "[" + to_string(index) + "] " + text);
    screen->setColor(DefaultColor::WHITE);
}

void SetupUI::showError(const string& message)
{
    screen->print(0, MSG_Y_OFFSET, "ERRO: " + message);
    screen->refresh();
}

void SetupUI::showMessage(const string& message)
{
    screen->print(0, MSG_Y_OFFSET, message);
    screen->refresh();
}

string SetupUI::promptForFilename()
{
    screen->clear();
    screen->print(0, 0, "--- Carregar Arquivo de Configuração ---");
    screen->print(0, 2, "Digite o caminho do arquivo (ex: configs/meu_arquivo.txt):");
    screen->print(0, 4, "> ");
    screen->refresh();

    updateInfo();
    return readString();
}

string SetupUI::readString()
{
    echo();

    char ch = getch();
    string str = "";

    while (ch != '\n')
    {
        str += ch;
        ch = getch();
    }

    return str;
}