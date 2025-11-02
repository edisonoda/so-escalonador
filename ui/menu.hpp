#pragma once

#include "screen.hpp"
#include "task_visual.hpp"
#include <vector>

#define MAIN_WIDTH 50
#define INFO_X_OFFSET MAIN_WIDTH + 4

namespace UI { class SetupUI; }
namespace Core { class SimulationConfig; }

namespace UI {
  class Menu : public Window {
    private:
      const static vector<int> navigation_keys;
      const static vector<int> confirm_keys;
      vector<int> valid_entries;
      vector<string> options;

      SetupUI* ui;

      string title;
      int selected;

      void printMenu();
      void printOption(int index, string option);

  public:
    Menu(SetupUI* ui);
    ~Menu();

    void setupMenu(string title, vector<string> options);

    int getSelected() const { return selected; }
    int showMenu();

    void navigate(int dir);
  };

  class SetupUI : public RefreshObserver {
    private:
      Screen* screen;

      Menu menu;
      TaskInfo task_info;
      Window mensagem;
      Window input;

      Core::SimulationConfig* config;

    public:
      SetupUI(Core::SimulationConfig* config);
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