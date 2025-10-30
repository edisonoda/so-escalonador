#include "menu.hpp"
#include "setup_ui.hpp"
#include <ncurses.h>
#include <string>

using namespace UI;

const vector<int> Menu::navigation_keys({KEY_UP, KEY_DOWN});

Menu::Menu(SetupUI* ui) : Window(), ui(ui)
{
    selected = 0;
}

Menu::~Menu()
{
    ui = nullptr;
}

void Menu::setupMenu(string title, vector<string> options)
{
    this->title = title;
    this->options = options;
    height = options.size() + 4;
    screen->erase();
    wresize(window, height, width);
    screen->refresh();
    ui->updateInfo();

    valid_entries.clear();
    valid_entries.push_back(KEY_ENTER);
    valid_entries.push_back('\n');
    for (int i = 0; i < options.size(); i++)
        valid_entries.push_back('0' + i + 1);

    selected = 0;
}

int Menu::showMenu()
{
    printMenu();

    int ch = wgetch(window);

    while(find(valid_entries.begin(), valid_entries.end(), ch) == valid_entries.end())
    {
        if (find(navigation_keys.begin(), navigation_keys.end(), ch) != navigation_keys.end())
            navigate(ch);
        else
        {
            ui->clearMessage();
            ui->showError("digite uma opção válida!");
        }

        printMenu();
        ch = wgetch(window);
    }

    ui->clearMessage();

    if (ch == KEY_ENTER || ch == '\n')
        return '0' + selected + 1;

    return ch;
}

void Menu::printMenu()
{
    int i = 0;

    clear();
    print(0, 0, title);

    for (int it = 0; it < options.size(); it++)
        printOption(it, options.at(it));

    move(0, getPosY() + 1);
    refresh();
}

void Menu::navigate(int dir)
{
    int max_options = options.size();

    // Move para cima ou para baixo, dependendo do código da seta
    switch (dir)
    {
        case KEY_UP: selected--; break;
        case KEY_DOWN: selected++; break;
    }

    // Mantém a seleção dentre as opções
    selected = (selected + max_options) % max_options;
}

void Menu::printOption(int index, string option)
{
    // Se a opção for a selecionada, formata ela
    if (index == selected)
    {
        setColor(DefaultColor::GREEN);
        option = "> " + option;
    }

    print(0, index + 1, "[" + to_string(index + 1) + "] " + option);
    setColor(DefaultColor::WHITE);
}