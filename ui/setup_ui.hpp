#pragma once

#include <string>
#include <vector>
#include "../ui/screen.hpp"
#include "tcb.hpp"
#include "../scheduler/scheduling_algorithm.hpp"

using namespace std;
using namespace Core;

class SetupUI
{
private:
    UI::Screen* screen;

public:
    SetupUI(UI::Screen* scr);
    ~SetupUI();

    // --- Funções de Desenho de Tela ---
    int showMainMenu(Scheduler::AlgorithmID alg, int quantum, int task_count);
    void showEditor(Scheduler::AlgorithmID alg, int quantum, const vector<TCB*>& tasks);
    string promptForFilename();

    // --- Funções de UI Utility ---
    void showTaskEditor();
    string readString();
    
    void showError(const string& message);
    void showMessage(const string& message);
};