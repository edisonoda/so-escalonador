#include "setup_ui.hpp"

#include "../core/setup_manager.hpp"
#include <ncurses.h>
#include <string>

#define MAIN_WIDTH 50
#define INFO_X_OFFSET MAIN_WIDTH + 4

using namespace UI;

SetupUI::SetupUI(SimulationConfig* config)
    : screen(Screen::getInstance())
    , config(config)
    , menu(this)
{
    menu.setWindowDimensions(0, MAIN_WIDTH, 0, 0);
    task_info.setWindowDimensions(0, 0, INFO_X_OFFSET, 0);
    mensagem.setWindowDimensions(3, MAIN_WIDTH, 0, 0);
    input.setWindowDimensions(5, MAIN_WIDTH, 0, 0);

    screen->attach(this);
}

SetupUI::~SetupUI()
{
    screen->detach(this);
    screen = nullptr;
    config = nullptr;
}

void SetupUI::update()
{
    int y = 0;

    task_info.clear();
    task_info.print(0, y++, "--- Configuração Atual ---");

    string alg_str;
    switch(config->alg_id)
    {
        case AlgorithmID::FIFO: alg_str = "FIFO"; break;
        case AlgorithmID::PRIOp: alg_str = "PRIOp"; break;
        case AlgorithmID::SRTF: alg_str = "SRTF"; break;
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

int SetupUI::showMainMenu()
{
    menu.setupMenu("--- SETUP DA SIMULAÇÃO ---", {
        "Iniciar",
        "Carregar",
        "Editar",
        "Restaurar PADRÃO",
        "Trocar modo",
        "Sair do programa"
    });

    return menu.showMenu();
}

int SetupUI::showEditor()
{
    menu.setupMenu("--- EDITAR CONFIGURAÇÕES ---", {
        "Algoritmo", 
        "Quantum", 
        "Tarefas", 
        "Voltar ao Menu Principal"
    });

    return menu.showMenu();
}

int SetupUI::showTaskList()
{
    vector<string> options = {"Adicionar", "Remover"};
    
    for (TCB* task : config->tasks)
        options.push_back("[" + task->getId() + "]");

    options.push_back("Voltar");

    menu.setupMenu("--- EDITAR LISTA DE TAREFAS ---", options);
    return menu.showMenu();
}

int SetupUI::showAlgorithm()
{
    menu.setupMenu("--- ESCOLHER ALGORITMO ---", {
        "FIFO",
        "PRIOp",
        "SRTF",
        "Voltar"
    });
    return menu.showMenu();
}

int SetupUI::showTaskEditor(string id)
{
    string title = "--- ADICIONAR TAREFA ---";

    if (!id.empty())
        title = "--- EDITAR TAREFA " + id + " ---";

    menu.setupMenu(title, {
        "Id",
        "Cor",
        "Início",
        "Duração",
        "Prioridade",
        "Sair"
    });

    return menu.showMenu();
}

void SetupUI::showError(const string& message, Window* window)
{
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

void SetupUI::inputError(const string& message)
{
    showError(message, &input);
}

void SetupUI::clearMessage()
{
    mensagem.clear();
    mensagem.refresh();

    screen->erase();
    screen->refresh();
}

string SetupUI::promptForField(string field)
{
    screen->erase();
    screen->refresh();
    
    input.clear();
    input.print(0, 0, "--- Inserir " + field + " ---");
    input.print(0, 1, "> ");
    input.refresh();

    return readString();
}

string SetupUI::promptForFilename()
{
    screen->erase();
    screen->refresh();

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
    vector<int> valid_entries = {'_', ' ', '-', '.'};

    while (ch != '\n')
    {
        if (ch == KEY_BACKSPACE || ch == '\b' || ch == 127)
        {
            if (!str.empty())
            {
                input.move(input.getPosX() - 1, input.getPosY());
                input.print(' ');
                input.move(input.getPosX() - 1, input.getPosY());
                str.pop_back();
            }
        }
        else if (isalnum(ch) || find(valid_entries.begin(), valid_entries.end(), ch) != valid_entries.end())
        {
            input.print(ch);
            str += ch;
        }
        
        input.refresh();
        ch = input.getCh();
    }

    return str;
}