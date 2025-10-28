#include "setup_ui.hpp"
#include "../core/setup_manager.hpp"

#define INFO_X_OFFSET 60

using namespace UI;

SetupUI::SetupUI(SimulationConfig* config)
    : screen(UI::Screen::getInstance())
    , config(config)
{
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
        for (TCB* task : config->tasks)
        {
            screen->setColor(task->getColor() ? task->getColor() : 0); // Cor da tarefa
            screen->invertColor(true);
            screen->print(INFO_X_OFFSET, y, task->getId());
            
            screen->setColor(DefaultColor::WHITE);
            screen->invertColor(false);
            
            string start_str = " START: " + to_string(task->getStart()) + " ";
            string duration_str = "| DURATION: " + to_string(task->getDuration()) + " ";
            string prio_str = "| PRIORITY: " + to_string(task->getPriority()) + " ";
            
            screen->print(INFO_X_OFFSET + 5, y, start_str);
            screen->print(INFO_X_OFFSET + 15, y, duration_str);
            screen->print(INFO_X_OFFSET + 30, y, prio_str);
            
            y++;
        }

    screen->refresh();
}

char SetupUI::showMainMenu()
{
    screen->clear();
    screen->print(0, 0, "--- SETUP DA SIMULAÇÃO ---");
    screen->print(2, 1, "[1] Iniciar (Modo: " + string(1, config->mode) + ")");
    screen->print(2, 2, "[2] Carregar");
    screen->print(2, 3, "[3] Editar");
    screen->print(2, 4, "[4] Restaurar PADRÃO");
    screen->print(2, 5, "[Q] Sair do programa");
    screen->print(2, 7, "*Pressione -Espaço- para alterar o modo de execução");
    screen->refresh();

    updateInfo();
    return screen->getCh();
}

void SetupUI::showEditor()
{
}

void SetupUI::showTaskEditor()
{
}

void SetupUI::showError(const string& message)
{
    // Mostra uma mensagem e espera o usuário
    screen->print(0, 20, "ERRO: " + message);
    screen->refresh();
}

void SetupUI::showMessage(const string& message)
{
    screen->print(0, 20, message);
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
    char ch = screen->getCh();
    string str = "";

    while (ch != '\n')
    {
        str += ch;
        ch = screen->getCh();
    }

    return str;
}