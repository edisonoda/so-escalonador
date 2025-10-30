#include "setup_ui.hpp"

#include "../core/setup_manager.hpp"
#include <ncurses.h>
#include <string>

#define INFO_X_OFFSET 60

using namespace UI;

SetupUI::SetupUI(SimulationConfig* config)
    : screen(Screen::getInstance())
    , config(config)
{
    menu.setWindowDimensions(0, INFO_X_OFFSET, 0, 0);

    taskInfo.setWindowDimensions(30, 60, INFO_X_OFFSET, 0);
    mensagem.setWindowDimensions(2, INFO_X_OFFSET, 0, menu.getHeight());
    input.setWindowDimensions(5, INFO_X_OFFSET, 0, 0);

    showMainMenu();
}

SetupUI::~SetupUI()
{
    screen = nullptr;
    config = nullptr;
}

void SetupUI::updateInfo()
{
    int y = 0;

    taskInfo.clear();

    taskInfo.print(0, y++, "--- Configuração Atual ---");
    string alg_str;
    switch(config->alg_id)
    {
        case Scheduler::AlgorithmID::FCFS: alg_str = "FCFS"; break;
        case Scheduler::AlgorithmID::PRIOp: alg_str = "PRIOp"; break;
        case Scheduler::AlgorithmID::SRTF: alg_str = "SRTF"; break;
    }
    
    taskInfo.print(0, y++, "Modo: " + string(1, config->mode));
    taskInfo.print(0, y++, "Algoritmo: " + alg_str);
    taskInfo.print(0, y++, "Quantum:   " + to_string(config->quantum));
    taskInfo.print(0, y++, "Tasks (" + to_string(config->tasks.size()) + "):");

    if (!config->tasks.empty())
        for (size_t i = 0; i < config->tasks.size(); i++)
        {
            TCB *task = config->tasks[i];
            taskInfo.setColor(i); // Cor da tarefa
            taskInfo.invertColor(true);
            taskInfo.print(0, y, task->getId());
            taskInfo.print(4, y, "COLOR: " + to_string(task->getColor()) + " ");
            
            taskInfo.setColor(DefaultColor::WHITE);
            taskInfo.invertColor(false);
            
            string start_str = "| START: " + to_string(task->getStart()) + " ";
            string duration_str = "| DURATION: " + to_string(task->getDuration()) + " ";
            string prio_str = "| PRIORITY: " + to_string(task->getPriority()) + " ";
            
            taskInfo.print(14, y, start_str);
            taskInfo.print(26, y, duration_str);
            taskInfo.print(41, y, prio_str);
            
            y++;
        }

    taskInfo.refresh();
}

int SetupUI::showMainMenu()
{
    menu.clear();

    menu.setupMenu("--- SETUP DA SIMULAÇÃO ---", {
        "Iniciar",
        "Carregar",
        "Editar",
        "Restaurar PADRÃO",
        "Sair do programa"
    });

    return menu.showMenu();
}

int SetupUI::showEditor()
{
    menu.clear();

    vector<string> options = {"Algoritmo", "Quantum"};
    
    for (TCB* task : config->tasks)
        options.push_back("[" + task->getId() + "]");

    menu.setupMenu("--- EDITAR PARÂMETROS ---", options);
    return menu.showMenu();
}

int SetupUI::showTaskEditor(string id)
{
    menu.clear();
    
    string title = "--- ADICIONAR TAREFA ---";

    if (!id.empty())
        title = "--- EDITAR TAREFA " + id + " ---";

    menu.setupMenu(title, {
        ""
    });

    return menu.showMenu();
}

void SetupUI::showError(const string& message)
{
    mensagem.moveWindow(0, menu.getHeight());
    mensagem.print(0, 0, "ERRO: " + message);
    mensagem.refresh();
}

void SetupUI::showMessage(const string& message)
{
    mensagem.moveWindow(0, menu.getHeight());
    mensagem.print(0, 0, message);
    mensagem.refresh();
}

string SetupUI::promptForFilename()
{
    menu.clear();
    input.clear();

    input.print(0, 0, "--- Carregar Arquivo de Configuração ---");
    input.print(0, 1, "Digite o nome do arquivo (ex: meu_arquivo.txt):");
    input.print(0, 2, "> ");
    input.refresh();

    return readString();
}

string SetupUI::readString()
{
    int ch = input.getCh();
    string str = "";

    while (ch != '\n')
    {
        if (ch == KEY_BACKSPACE || ch == '\b' || ch == 127)
        {
            if (!str.empty())
            {
                input.del(input.getPosX() - 1, input.getPosY());
                str.pop_back();
            }
        }
        else
        {
            input.print(ch);
            str += ch;
        }
        
        input.refresh();
        ch = input.getCh();
    }

    return str;
}