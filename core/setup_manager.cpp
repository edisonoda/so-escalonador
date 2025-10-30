#include "setup_manager.hpp"
#include "../scheduler/scheduling_algorithm.hpp"
#include <ncurses.h>
#include <string>

#define CONFIG_FILE "configs/default.txt"

using namespace Scheduler;

SetupManager::SetupManager()
    : ui(&config)
    , screen(Screen::getInstance())
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
    bool in_setup = true;
    int ch = '1';

    while (in_setup)
    {
        ch = ui.showMainMenu();

        switch (ch)
        {
            case '1': // LÓGICA: Iniciar Simulação
                in_setup = false;
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
    }

    config.simulation_should_run = true;
    return config;
}

bool SetupManager::loadFromFile(const string &filename)
{
    if (!config_reader.openFile(filename))
    {
        ui.showError("Não foi possível abrir o arquivo: " + filename);
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

    do
    {
        ch = ui.showEditor();
        
        switch (ch)
        {
        case '1':
            runAlgorithmEditor();
            break;
        
        case '2':
            config.quantum = stoi(ui.promptForField("Quantum"));
            break;

        case '3':
            runTaskListEditor();
            break;
        }
    } while (ch != '0' + config.tasks.size() - 1);

    ui.update();
}

void SetupManager::runTaskListEditor()
{
    int ch;

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
            // edit task
            break;
        }

    } while (ch != '0' + config.tasks.size() - 1);

    ui.update();
}

void SetupManager::runAlgorithmEditor()
{
    int ch;

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
        }

    } while (ch != '0' + config.tasks.size() - 1);

    ui.update();
}

void SetupManager::addNewTask()
{
    ui.showTaskEditor();
    string id = ui.readString();
    string color = ui.readString();
    string start = ui.readString();
    string duration = ui.readString();
    string priority = ui.readString();
    config.tasks.push_back(new TCB(
        id,
        stoi(color),
        stoi(start),
        stoi(duration),
        stoi(priority)
    ));

    screen->initColor(0, stoi(color));

    ui.update();
}

void SetupManager::deleteTask()
{
    string str = ui.readString();

    while (!isNumber(str) || stoi(str) >= config.tasks.size() - 1)
    {
        ui.showError("digite uma task válida!");
        str = ui.readString();
    }

    delete config.tasks.at(stoi(str));
    config.tasks.erase(config.tasks.begin() + stoi(str));

    ui.update();
}

void SetupManager::editTask(int index)
{
    
}

bool SetupManager::isNumber(const string &s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}