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
    private:
        ifstream file;
        string algorithm;
        int quantum;

        string readLine();
    
    public:
        ConfigReader();
        ~ConfigReader();

        bool openFile(const string &filename);
        void closeFile();
        void readPattern();
        string getAlgorithm() const { return algorithm; }
        int getQuantum() const { return quantum; }
        list<TCB*> readTasks();
    };
} // namespace Core
