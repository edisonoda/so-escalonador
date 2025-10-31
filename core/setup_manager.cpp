#include "setup_manager.hpp"
#include "../scheduler/scheduling_algorithm.hpp"
#include <ncurses.h>
#include <string>

#define CONFIG_FILE "configs/default.txt"

using namespace Scheduler;

SetupManager::SetupManager()
    : ui(&config), screen(Screen::getInstance())
{
    loadFromFile(CONFIG_FILE);
    timeout(-1);

    for (TCB *task : config.tasks)
        screen->initColor(0, task->getColor());

    ui.update();
}

SetupManager::~SetupManager()
{
    screen = nullptr;
}

SimulationConfig SetupManager::run()
{
    bool in_editor = true;
    int ch = '1';

    do
    {
        ch = ui.showMainMenu();

        switch (ch)
        {
        case '1': // LÓGICA: Iniciar Simulação
            in_editor = false;
            break;

        case '2':
        {
            string filename = ui.promptForFilename();
            if (!filename.empty())
                loadFromFile("configs/" + filename);
        }
        break;

        case '3':
            runEditor();
            break;

        case '4':
            loadFromFile("configs/default.txt");
            break;

        case '5':
            if (config.mode == 'A')
                config.mode = 'P';
            else
                config.mode = 'A';
            break;

        case '6':
            config.simulation_should_run = false;
            return config;
        }

        ui.update();

    } while (in_editor);

    config.simulation_should_run = true;
    screen->detach(&ui);
    return config;
}

bool SetupManager::loadFromFile(const string &filename)
{
    if (!config_reader.openFile(filename))
    {
        ui.inputError("Não foi possível abrir o arquivo: " + filename);
        return false;
    }

    config_reader.readPattern();
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

void SetupManager::runEditor()
{
    int ch;
    bool in_editor;
    string quantum;

    do
    {
        ch = ui.showEditor();

        switch (ch)
        {
        case '1':
            runAlgorithmEditor();
            break;

        case '2':
            quantum = ui.promptForField("Quantum");
            if (validateEntry(quantum))
                config.quantum = stoi(quantum);
            break;

        case '3':
            runTaskListEditor();
            break;

        default:
            in_editor = false;
            break;
        }
    } while (in_editor);

    ui.update();
}

void SetupManager::runTaskListEditor()
{
    int ch;
    bool in_editor = true;

    do
    {
        ch = ui.showTaskList();

        switch (ch)
        {
        case '1':
            addNewTask();
            break;

        case '2':
            deleteTask();
            break;

        default:
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

void SetupManager::runAlgorithmEditor()
{
    int ch;
    bool in_editor = true;

    do
    {
        ch = ui.showAlgorithm();

        switch (ch)
        {
        case '1':
            config.alg_id = AlgorithmID::FCFS;
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

void SetupManager::addNewTask()
{
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

    TCB *task = new TCB(
        id,
        stoi(color),
        stoi(start),
        stoi(duration),
        stoi(priority));

    config.tasks.push_back(task);

    screen->initColor(0, stoi(color));

    ui.update();
}

void SetupManager::deleteTask()
{
    bool valid = false;
    string str = ui.promptForField("Id");
    vector<TCB *>::iterator i = config.tasks.begin();

    while (i != config.tasks.end())
    {
        if ((*i)->getId() == str)
        {
            delete (*i);
            config.tasks.erase(i++);
            valid = true;
        }
        else
        {
            i++;
        }
    }

    if (!valid)
    {
        ui.inputError("digite um ID válido!");
    }
    

    ui.update();
}

void SetupManager::editTask(int index)
{
    int ch;
    TCB *task;
    string entry;
    bool in_editor = true;

    task = config.tasks.at(index);

    do
    {
        ch = ui.showTaskEditor(task->getId());

        switch (ch)
        {
        case '1':
            task->setId(ui.promptForField("Id"));
            break;

        case '2':
            entry = ui.promptForField("Cor");
            if (validateEntry(entry))
                task->setColor(stoi(entry));
            break;

        case '3':
            entry = ui.promptForField("Início");
            if (validateEntry(entry))
                task->setStart(stoi(entry));
            break;

        case '4':
            entry = ui.promptForField("Duração");
            if (validateEntry(entry))
                task->setDuration(stoi(entry));
            break;

        case '5':
            entry = ui.promptForField("Prioridade");
            if (validateEntry(entry))
                task->setPriority(stoi(entry));
            break;

        default:
            in_editor = false;
            break;
        }

    } while (in_editor);

    ui.update();
}

bool SetupManager::isNumber(const string &s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

bool SetupManager::validateEntry(string str)
{
    if (!isNumber(str))
    {
        ui.inputError("Digite um valor válido!");
        return false;
    }

    return true;
}