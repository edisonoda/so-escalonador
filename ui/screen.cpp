#include "screen.hpp"

static int convertRGB(int color) { return ((float)color / 255) * 1000; }

using namespace UI;

Screen *Screen::instance(nullptr);

// RefreshSubject definition

RefreshSubject::RefreshSubject() {}

RefreshSubject::~RefreshSubject() { observers.clear(); }

void RefreshSubject::attach(RefreshObserver *obs) { observers.insert(obs); }

void RefreshSubject::detach(RefreshObserver *obs) { observers.erase(obs); }

void RefreshSubject::update() {
  for (RefreshObserver *obs : observers)
    obs->update();
}

// Screen definition

Screen::Screen() {
  color_pair_count = 0;

  initscr(); // Inicia uma tela do NCURSES
  noecho(); // Não mostra caracteres digitados pelo usuário
  start_color();
  init_color(GRAY_INDEX, 250, 250, 250); // Inicia o cinza fora da faixa de tarefas

  // Inicializa as cores com um tom mais visível
  init_color(COLOR_BLACK, 100, 100, 125);
  init_color(COLOR_BLUE, convertRGB(66), convertRGB(135), convertRGB(245));
  init_color(COLOR_YELLOW, convertRGB(245), convertRGB(233), convertRGB(66));
  init_color(COLOR_CYAN, convertRGB(66), convertRGB(245), convertRGB(197));
  init_color(COLOR_RED, convertRGB(245), convertRGB(66), convertRGB(66));
  init_color(COLOR_MAGENTA, convertRGB(245), convertRGB(66), convertRGB(135));

  initColor(7, 0);           // branco no preto
  initColor(0, 0);           // preto no preto
  initColor(0, GRAY_INDEX);  // preto no cinza
  initColor(COLOR_GREEN, 0); // verde no preto

  bkgd(COLOR_PAIR(1)); // Define a cor de fundo para preto
}

Screen::~Screen() {
  instance = nullptr;
  endwin();
}

Screen *Screen::getInstance() {
  if (instance == nullptr)
    instance = new Screen();

  return instance;
}

void Screen::refresh() { ::refresh(); }

void Screen::erase() {
  ::erase();
  update();
}

// Por padrão o NCURSES inicia as cores em pares, texto e fundo
void Screen::initColor(int color, int bg_color) {
  init_pair(++color_pair_count, color, bg_color);
}

// Window definition

Window::Window() : window(newwin(0, 0, 0, 0)), screen(Screen::getInstance()) {
  inverted = false;
  keypad(window, true); // Inicia as macros KEY_ do NCURSES
  screen->refresh();
}

Window::~Window() {
  clear();
  refresh();
  delwin(window);
  screen->erase();
  screen->refresh();

  screen = nullptr;
  window = nullptr;
}

void Window::setWindowDimensions(int h, int w, int _x, int _y) {
  // Adiciona uma margem às janelas
  height = h + (Y_PAD * 2);
  width = w + (X_PAD * 2);
  x = _x;
  y = _y;

  wresize(window, height, width);
  mvwin(window, y, x);

  screen->refresh();
}

void Window::getPos(int *x, int *y) { getyx(window, *y, *x); }

int Window::getPosX() { return getcurx(window); }

int Window::getPosY() { return getcury(window); }

// As funções de janela do NCURSES começam com w
void Window::moveWindow(int x, int y) {
  mvwin(window, y, x), screen->refresh();
}

void Window::move(int x, int y) { wmove(window, y, x); }

void Window::print(int ch) { wprintw(window, "%c", ch); }

void Window::print(std::string str) { wprintw(window, "%s", str.c_str()); }

// Move o cursor e depois imprime na janela
void Window::print(int x, int y, int ch) {
  mvwprintw(window, y + Y_PAD, x + X_PAD, "%c", ch);
}

void Window::print(int x, int y, std::string str) {
  mvwprintw(window, y + Y_PAD, x + X_PAD, "%s", str.c_str());
}

void Window::del(int x, int y) {
  wmove(window, y, x);
  wdelch(window);
}

void Window::refresh() {
  box(window, 0, 0);
  wrefresh(window);
}

void Window::clear() { wclear(window); }

void Window::erase() { werase(window); }

int Window::getCh() { return wgetch(window); }

// Define a cor com base nas cores padrão
int Window::setColor(DefaultColor color) {
  // Função do NCURSES para atribuir cor ao terminal
  wattron(window, COLOR_PAIR(static_cast<int>(color)));
  return static_cast<int>(color);
}

// Define a cor com base no index acima das cores padrão
int Window::setColor(int color_index) {
  wattron(window, COLOR_PAIR(color_index + INITIAL_COLORS + 1));
  return color_index + INITIAL_COLORS + 1;
}

// Inverte as cores do texto e fundo
void Window::invertColor() {
  if (inverted)
    wattroff(window, A_REVERSE);
  else
    wattron(window, A_REVERSE);

  inverted = !inverted;
}

void Window::invertColor(bool inv) {
  inverted = !inv;
  invertColor();
}