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

    public:
        SetupUI(SimulationConfig* config);
        ~SetupUI();
    
        // --- Funções de Desenho de Tela ---
        void updateInfo();
    
        char showMainMenu();
        void showEditor();
        void showTaskEditor();
        
        void showError(const string& message);
        void showMessage(const string& message);
        
        // --- Funções de UI Utility ---
        string promptForFilename();
        string readString();
    };
} // namespace UI