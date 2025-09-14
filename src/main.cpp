//#include "menuFunc.h"
#include "nocurses.hpp"

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

const std::string_view headTitle = "kys";
const std::string msg = "KYS";


void startup() 
{ 
  nocurses::clrscr();
  nocurses::settitle(headTitle);
  nocurses::setfontbold(BFALSE);
  nocurses::setbgrcolor(BLACK);
  nocurses::setfontcolor(WHITE); 
}

int main() {
  bool doubletick = false;
  startup();

  while (true) {  
    if (doubletick == false)
    {
    nocurses::setfontbold(BFALSE);
    nocurses::setbgrcolor(BLACK);
    nocurses::setfontcolor(WHITE); 
    doubletick = true;
    }
    
    else
    {
    nocurses::setfontbold(BTRUE);
    nocurses::setbgrcolor(WHITE);
    nocurses::setfontcolor(BLACK); 
    doubletick = false;
    }
    
    std::cout << msg << "\t";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  return 0; // if it gets here, somethings gone really wrong...
}
