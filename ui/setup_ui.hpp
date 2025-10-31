#pragma once

#include "../utils/std_libraries.hpp"
#include "refresh_observer.hpp"

#include "screen.hpp"
#include "window.hpp"
#include "menu.hpp"

// Forward declaration
namespace Core { class SimulationConfig; }
using namespace Core;

namespace UI
{
    class SetupUI : public RefreshObserver
    {
    private:
        Screen* screen;

        Menu menu;
        Window task_info;
        Window mensagem;
        Window input;

        SimulationConfig* config;

    public:
        SetupUI(SimulationConfig* config);
        ~SetupUI();

        // --- Funções de Desenho de Tela ---
        virtual void update();
    
        int showMainMenu();
        int showEditor();
        int showTaskEditor(string id = "");
        int showTaskList();
        int showAlgorithm();
        
        void showError(const string& message, Window* window = nullptr);
        void inputError(const string& message);
        void clearMessage();
        
        // --- Funções de UI Utility ---
        string promptForField(string field);
        string promptForFilename();
        string readString();
    };
} // namespace UI