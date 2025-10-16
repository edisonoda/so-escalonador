#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "tcb.hpp"
using namespace std;

namespace Core
{
    class ConfigReader
    {
    public:
        ConfigReader(const string &filename);
        ~ConfigReader();

        bool openFile();
        void closeFile();
        void readPattern();
        string getAlgorithm() { return algorithm; }
        int getQuantum() { return quantum; }
        list<TCB*> readTasks();

    private:
        string filename;
        ifstream file;
        string algorithm;
        int quantum;

        string readLine();
    };
} // namespace Core
