// file header to port into main
#include "menuFunc.h"

// all needed libs :P
#include "nocurses.hpp"
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>

namespace menuFunc {

// random gen
int getRand(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

// for computer looking output (like a system report or something)
void computer(const std::string &word) {
  std::istringstream stream(word);
  std::string Wword;

  while (stream >> Wword) {
    int delayMs = getRand(200, 400);
    std::cout << Wword << " " << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
  }
}

// raw escape codes, moves write head up, clear, 1st column
void clearLineAbove() {
  std::cout << "\x1b[1A"
            << "\x1b[2K"
            << "\x1b[1G" << std::flush;
}

// for a really cool header that goes across the terminal :D
void fancylines(char line) {
  struct nocurses::termsize size = nocurses::gettermsize();

  for (int i = 0; i < size.cols; i++) {
    std::cout << line;
  }
  std::cout << std::endl;
  nocurses::gotoxy(1, 3);
}

void loadingAnimation(const std::string &text) {
  const char spinner[] = {'|', '/', '-', '\\'};
  menuFunc::typeLineWPM(text, WPM::Normal);

  for (int i = 0; i < 40; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "\b" << spinner[i % 4] << std::flush;
  }

  std::cout << "\r \r" << std::flush; // erase spinner
  std::cout << "Done!" << std::endl;
}

// reset colours :P
void defColourScheme() {
  nocurses::setbgrcolor(BLUE);
  nocurses::setfontcolor(WHITE);
}

// sets all terminal info plus
void startup(const std::string &title) {
  std::signal(SIGINT, menuFunc::onclose);
  nocurses::clrscr();
  defColourScheme();
  nocurses::setblink(false);
  nocurses::settitle(title);
  nocurses::clrscr();
}

// simple wait for enter keystroke
void enterScreen() {
  std::cout << std::endl << "Enter to continue...";
  nocurses::wait();
  nocurses::clrscr();
}

// function ran when terminated sig is sent (ctrl + z)
void onclose(int sig) {
  nocurses::clrscr();
  std::exit(0);
}

void typeLineWPM(const std::string &line, int wpm) {
  int baseDelay = 12000 / wpm;

  for (char c : line) {
    std::cout << c << std::flush;

    if (c == '\n' || c == '\t' || c == '\r' || c == '\b')
      continue;

    int jitteredDelay =
        menuFunc::getRand(baseDelay - baseDelay / 4, baseDelay + baseDelay / 4);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(std::max(1, jitteredDelay)));
  }
  std::cout << '\n';
}

void VectorStringOut(const std::vector<std::string> &lines, int wpm = 240,
                     int delayBetweenLines = 100) {
  for (const auto &line : lines) {
    typeLineWPM(line, wpm);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayBetweenLines));
  }
}

} // namespace menuFunc
