#include "setup_manager.hpp"
#include <algorithm>

// Construtor: Cria a UI e carrega o default
SetupManager::SetupManager(UI::Screen *screen)
    : setup_alg_id(Scheduler::AlgorithmID::FCFS), setup_quantum(0)
{
    this->ui = new SetupUI(screen);

    loadFromFile("configs/default.txt");
}

SetupManager::~SetupManager()
{
    delete ui;

    setup_tasks.clear();
}

SimulationConfig SetupManager::run()
{
    SimulationConfig final_config;

    bool in_setup = true;
    vector<int> valid_entries = {'1', '2', '3', '4', 'Q'};

    while (in_setup)
    {
        int ch;
        while (find(valid_entries.begin(), valid_entries.end(), ch) != valid_entries.end())
            ch = ui->showMainMenu(setup_alg_id, setup_quantum, setup_tasks.size());

        switch (ch)
        {
        case '1':
            loadFromFile("configs/default.txt");
            break;

        case '2':
        {
            string filename = ui->promptForFilename();
            if (!filename.empty())
                loadFromFile("configs/" + filename);
        }
        break;

        case '3':
            runEditor();
            break;

        case '4': // LÓGICA: Iniciar Simulação
            in_setup = false;
            break;

        case 'Q':
            final_config.simulation_should_run = false;
            return final_config;
        }
    }

    final_config.alg_id = setup_alg_id;
    final_config.quantum = setup_quantum;
    final_config.tasks = setup_tasks;
    final_config.simulation_should_run = true;

    setup_tasks.clear();

    return final_config;
}

bool SetupManager::loadFromFile(const string &filename)
{
    if (!config_reader.openFile(filename))
    {
        ui->showError("Não foi possível abrir o arquivo: " + filename);
        return false;
    }

    config_reader.readPattern();
    setup_alg_id = config_reader.getAlgorithm();
    setup_quantum = config_reader.getQuantum();

    for (TCB *task : setup_tasks)
        delete task;
    setup_tasks.clear();

    list<TCB *> tasks_list = config_reader.readTasks();
    setup_tasks.assign(tasks_list.begin(), tasks_list.end());

    return true;
}

void SetupManager::runEditor()
{
    bool in_editor = true;
    while (in_editor)
    {
        ui->showEditor(setup_alg_id, setup_quantum, setup_tasks);
    }
}

void SetupManager::addNewTask()
{
    ui->showTaskEditor();
    string id = ui->readString();
    string color = ui->readString();
    string start = ui->readString();
    string duration = ui->readString();
    string priority = ui->readString();
    setup_tasks.push_back(new TCB(
        id,
        stoi(color),
        stoi(start),
        stoi(duration),
        stoi(priority)));
}

void SetupManager::deleteTask()
{
    string str = ui->readString();

    while (!isNumber(str) || stoi(str) >= setup_tasks.size() - 1)
    {
        ui->showError("digite uma task válida!");
        str = ui->readString();
    }

    delete setup_tasks.at(stoi(str));
    setup_tasks.erase(setup_tasks.begin() + stoi(str));
}

bool SetupManager::isNumber(const string &s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}