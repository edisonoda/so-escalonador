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
        string filename;
        ifstream file;
        string algorithm;
        int quantum;

        string readLine();
    
    public:
        ConfigReader(const string &filename);
        ~ConfigReader();

        bool openFile();
        void closeFile();
        void readPattern();
        string getAlgorithm() const { return algorithm; }
        int getQuantum() const { return quantum; }
        list<TCB*> readTasks();
    };
} // namespace Core
