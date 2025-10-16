#include "config_reader.hpp"
#include <vector>

namespace Core
{
    ConfigReader::ConfigReader(const string &filename) : filename(filename) {}

    ConfigReader::~ConfigReader()
    {
        closeFile();
    }

    bool ConfigReader::openFile()
    {
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
                printf("Config: %s\n", configs.back().c_str());
                line.erase(0, pos + 1);
            }
            configs.push_back(line);
            printf("Config: %s\n", configs.back().c_str());

            TCB *tcb = new TCB(configs[0], configs[1], stoi(configs[2]), stoi(configs[3]), stoi(configs[4]));
            tasks.push_back(tcb);
            configs.clear();
        }
        return tasks;
    }
} // namespace Core