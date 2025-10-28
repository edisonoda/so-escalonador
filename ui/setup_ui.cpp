#include "setup_ui.hpp"
#include <ncurses.h> // Para toupper()
#include <stdio.h>   // Para sprintf

SetupUI::SetupUI(UI::Screen* scr) : screen(scr) 
{
    // Construtor
}

SetupUI::~SetupUI()
{
    // Destrutor (screen é gerenciado pelo System)
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

int SetupUI::showMainMenu(Scheduler::AlgorithmID alg, int quantum, int task_count)
{
    screen->clear();
    screen->print(0, 0, "--- SETUP DA SIMULAÇÃO ---");
    screen->print(2, 2, "[1] Carregar configuração PADRÃO (configs/default.txt)");
    screen->print(2, 3, "[2] Carregar configuração de ARQUIVO");
    screen->print(2, 4, "[3] Editar configuração PADRÃO");
    screen->print(2, 6, "[4] INICIAR Simulação com a configuração atual");
    screen->print(2, 8, "[Q] Sair do programa");
    
    screen->print(0, 10, "--- Configuração Atual ---");
    string alg_str;
    switch(alg)
    {
        case Scheduler::AlgorithmID::FCFS: alg_str = "FCFS"; break;
        case Scheduler::AlgorithmID::PRIOp: alg_str = "PRIOp"; break;
        case Scheduler::AlgorithmID::SRTF: alg_str = "SRTF"; break;
    }
    screen->print(2, 11, "Algoritmo: " + alg_str);
    screen->print(2, 12, "Quantum:   " + to_string(quantum));
    screen->print(2, 13, "Nº de Tasks: " + to_string(task_count));
    screen->refresh();

    return screen->getCh();
}

string SetupUI::promptForFilename()
{
    screen->clear();
    screen->print(0, 0, "--- Carregar Arquivo de Configuração ---");
    screen->print(0, 2, "Digite o caminho do arquivo (ex: configs/meu_arquivo.txt):");
    screen->print(0, 4, "> ");
    screen->refresh();
    return readString();
}

void SetupUI::showEditor(Scheduler::AlgorithmID alg, int quantum, const vector<TCB*>& tasks)
{
}

void SetupUI::showTaskEditor()
{
}

string SetupUI::readString()
{
    int ch = screen->getCh();
    string str = "";

    while (ch != '\n')
    {
        str += ch;
        ch = screen->getCh();
    }

    return str;
}