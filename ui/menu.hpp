#pragma once

#include "../utils/std_libraries.hpp"
#include "window.hpp"

namespace UI { class SetupUI; }

namespace UI
{
    class Menu : public Window
    {
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
} // namespace UI