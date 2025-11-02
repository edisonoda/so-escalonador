#pragma once

#include "tcb.hpp"

#include "scheduler.hpp"

#include "../ui/menu.hpp"
#include "../ui/screen.hpp"

#include <fstream>

using namespace UI;

namespace Core {
  struct SimulationConfig {
    AlgorithmID alg_id = AlgorithmID::FIFO;
    vector<TCB*> tasks;

    int quantum = 2;
    char mode = 'A';
    bool simulation_should_run = false;
  };

  class ConfigReader {
    private:
      ifstream file;
      string algorithm;
      int quantum;
      SetupUI *ui;
      const map<string, AlgorithmID> alg_map;

      string readLine();
  
    public:
      ConfigReader(SetupUI *ui);
      ~ConfigReader();

      bool openFile(const string &filename);
      void closeFile();
      bool readPattern();
      AlgorithmID getAlgorithm() const;
      int getQuantum() const { return quantum; }
      list<TCB*> readTasks();
  };

  class SetupManager {
    private:
      ConfigReader config_reader;
      Screen* screen;
      SetupUI ui;

      SimulationConfig config;

      bool loadFromFile(const string& filename);
      void runEditor();
      void runTaskListEditor();
      void runAlgorithmEditor();
      
      void addNewTask();
      void deleteTask();
      void editTask(int index);

      bool validateEntry(string str);

    public:
      SetupManager();
      ~SetupManager();

      SimulationConfig run();
  };
} // namespace Core