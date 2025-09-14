#pragma once

#include <csignal>
#include <string>
#include <vector>

enum WPM {
  TooHigh = 5000,
  Ungodly = 1000,
  ReallyHigh = 500,
  Higher = 300,
  Normal = 240,
  Lower = 200,
  ReallyLow = 160,
  UngodlySlow = 140,
  TooSlow = 80,
};

namespace menuFunc {

int getRand(int min, int max);

void computer(const std::string &word);

void fancylines(char line = '=');

void loadingAnimation(const std::string &text);

void defColourScheme();

void startup(const std::string &title);

void enterScreen();

void onclose(int sig);

void clearLineAbove();

void typeLineWPM(const std::string &line, int wpm);

void VectorStringOut(const std::vector<std::string> &lines, int wpm,
                     int delayBetweenLines);

} // namespace menuFunc
