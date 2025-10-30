#pragma once

#include "../utils/std_libraries.hpp"

#include "screen.hpp"
#include "window.hpp"
#include "menu.hpp"

// Forward declaration
namespace Core { class SimulationConfig; }
using namespace Core;

namespace UI
{
    class SetupUI
    {
    private:
        Screen* screen;

        Menu menu;
        Window taskInfo;
        Window mensagem;
        Window input;

        SimulationConfig* config;

    public:
        SetupUI(SimulationConfig* config);
        ~SetupUI();
    
        // --- Funções de Desenho de Tela ---
        void updateInfo();
    
        int showMainMenu();
        int showEditor();
        int showTaskEditor(string id = "");
        
        void showError(const string& message);
        void showMessage(const string& message);
        
        // --- Funções de UI Utility ---
        string promptForField(string field);
        string promptForFilename();
        string readString();
    };
} // namespace UI