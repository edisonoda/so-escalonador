#include "config_reader.hpp"
#include <list>
#include <vector>

using namespace Core;

ConfigReader::ConfigReader() : alg_map({
    {"FCFS", Scheduler::AlgorithmID::FCFS}, 
    {"PRIOp", Scheduler::AlgorithmID::PRIOp},
    {"SRTF", Scheduler::AlgorithmID::SRTF},
    {"FIFO", Scheduler::AlgorithmID::FCFS}
}) 
{
}

ConfigReader::~ConfigReader()
{
    closeFile();
}

Scheduler::AlgorithmID ConfigReader::getAlgorithm() const
{
    return alg_map.find(algorithm)->second;
}

bool ConfigReader::openFile(const string &filename)
{
    closeFile();
    file.open(filename);
    return file.is_open();
}

void ConfigReader::closeFile()
{
    if (file.is_open())
        file.close();
}

string ConfigReader::readLine()
{
    string line;
    if (getline(file, line))
        return line;
    return "";
}

void ConfigReader::readPattern()
{
    string pattern = readLine();
    if (!pattern.empty())
    {
        size_t sep = pattern.find(';');
        if (sep != string::npos)
        {
            algorithm = pattern.substr(0, sep);
            quantum = stoi(pattern.substr(sep + 1, pattern.length() - 1));
        }
    }
}

list<TCB *> ConfigReader::readTasks()
{
    vector<string> configs = {};
    list<TCB *> tasks;
    string line;
    size_t pos = 0;
    
    while (!(line = readLine()).empty())
    {
        while (pos = line.find(';'), pos != string::npos)
        {
            configs.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        configs.push_back(line);

        TCB *tcb = new TCB(
            // id, cor, inicio, duracão e prioridade
            configs[0],
            stoi(configs[1]),
            stoi(configs[2]),
            stoi(configs[3]),
            stoi(configs[4]),
            list<string>(configs.begin() + 5, configs.end())
        );

        tasks.push_back(tcb);
        configs.clear();
    }
    
    return tasks;
}