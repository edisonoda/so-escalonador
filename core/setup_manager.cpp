#include "setup_manager.hpp"

#define CONFIG_FILE "configs/default.txt"

SetupManager::SetupManager()
    : ui(&config)
    , screen(Screen::getInstance())
{
    loadFromFile(CONFIG_FILE);

    for (TCB *task : config.tasks)
        screen->initColor(0, task->getColor());
}

SetupManager::~SetupManager()
{
    screen = nullptr;
}

SimulationConfig SetupManager::run()
{
    timeout(-1);

    bool in_setup = true;
    char ch = '1';

    vector<char> valid_entries = {'1', '2', '3', '4', '5', ' ', '\n'};

    while (in_setup)
    {
        ch = ui.navigateMainMenu();
        while (find(valid_entries.begin(), valid_entries.end(), ch) == valid_entries.end())
        {
            ui.showError("digite uma opção válida!");
            ch = screen->getCh();
        }

        if (ch == '\n')
            ch = '0' + ui.getSelected() + 1;

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
                config.simulation_should_run = false;
                return config;

            case ' ':
                if (config.mode == 'A')
                    config.mode = 'P';
                else
                    config.mode = 'A';
                break;
        }
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

    return true;
}

void SetupManager::runEditor()
{
    bool in_editor = true;
    while (in_editor)
    {
        ui.showEditor();
    }
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
}

bool SetupManager::isNumber(const string &s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}