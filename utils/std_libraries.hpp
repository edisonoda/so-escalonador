#pragma once

// bibliotecas de estruturas
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>

// bibliotecas de utilidade
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cctype>

// headers do C
#include <ncurses.h>
#include <locale.h>

using namespace std;

// inline void clear_log() {
//     // Limpa o log antigo
//     std::ofstream log_file("debug.log", std::ios::out | std::ios::trunc);
// }

// inline void Log(const std::string& message) {
//     // Adiciona ao log
//     std::ofstream log_file("debug.log", std::ios::out | std::ios::app);
//     if (log_file.is_open()) {
//         log_file << message << std::endl;
//     }
// }