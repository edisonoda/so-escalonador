#pragma once

#include <string>
#include "screen.hpp"

using namespace std;

// Forward declaration
namespace Core { class SimulationConfig; }
using namespace Core;

namespace UI
{
    class SetupUI
    {
    private:
        UI::Screen* screen;
        SimulationConfig* config;

        int selected;

        void navigate(char dir, int max_options);
        void printOption(int index, string text);
        
        void showMainMenu();

    public:
        SetupUI(SimulationConfig* config);
        ~SetupUI();

        int getSelected() const { return selected; }
    
        // --- Funções de Desenho de Tela ---
        void updateInfo();
    
        char navigateMainMenu();
        void showEditor();
        void showTaskEditor();
        
        void showError(const string& message);
        void showMessage(const string& message);
        
        // --- Funções de UI Utility ---
        string promptForFilename();
        string readString();
    };
} // namespace UI