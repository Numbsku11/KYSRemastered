#ifndef NOCURSES_HPP
#define NOCURSES_HPP

#include <iostream>
#include <limits>
#include <string_view>

#if defined(_WIN32)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__) ||                 \
    defined(__APPLE__) || defined(__linux__)
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

enum Bools { BTRUE = 1, BFALSE = 0 };

enum Color { BLACK = 0, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };

enum CursorShape {
  BLOCK_BLINK = 1,
  BLOCK = 2,
  UNDERLINE_BLINK = 3,
  UNDERLINE = 4,
  BAR_BLINK = 5,
  BAR = 6
};

namespace nocurses {

constexpr std::string_view ESC = "\x1b";

struct termsize {
  int cols = 0;
  int rows = 0;
};

inline int bg_color = BLACK;
inline int font_color = WHITE;
inline bool font_bold = false;

inline void wait() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline void clrscr() {
  std::cout << ESC << "[2J" << ESC << "[?6h" << std::flush;
}

inline void gotoxy(int x, int y) {
  std::cout << ESC << '[' << y << ';' << x << 'H' << std::flush;
}

inline void setfontcolor(Color color) {
  std::cout << ESC << "[3" << static_cast<int>(color) << 'm' << std::flush;
  font_color = color;
}

inline void setbgrcolor(Color color) {
  std::cout << ESC << "[4" << static_cast<int>(color) << 'm' << std::flush;
  bg_color = color;
}

inline void setfontbold(bool status) {
  std::cout << ESC << "[" << (status ? "1" : "22") << 'm' << std::flush;
  font_bold = status;
  setfontcolor(static_cast<Color>(font_color));
  setbgrcolor(static_cast<Color>(bg_color));
}

inline void setunderline(bool status) {
  std::cout << ESC << "[" << (status ? "4" : "24") << 'm' << std::flush;
  setfontcolor(static_cast<Color>(font_color));
  setbgrcolor(static_cast<Color>(bg_color));
  setfontbold(font_bold);
}

inline void setblink(bool status) {
  std::cout << ESC << "[" << (status ? "5" : "25") << 'm' << std::flush;
  setfontcolor(static_cast<Color>(font_color));
  setbgrcolor(static_cast<Color>(bg_color));
  setfontbold(font_bold);
}

inline void settitle(std::string_view title) {
  std::cout << ESC << "]0;" << title << "\x07" << std::flush;
}

inline void setcurshape(CursorShape shape) {
  std::cout << ESC << "[" << static_cast<int>(shape) << " q" << std::flush;
}

inline termsize gettermsize() {
  termsize size;
#if defined(_WIN32)
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
    size.cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  }
#elif defined(__unix__)
  struct winsize win;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &win) == 0) {
    size.cols = win.ws_col;
    size.rows = win.ws_row;
  }
#endif
  return size;
}

inline int getch() {
  int ch;
#if defined(_WIN32)
  HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
  if (input == nullptr)
    return EOF;

  DWORD oldmode;
  GetConsoleMode(input, &oldmode);
  SetConsoleMode(input, oldmode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
#elif defined(__unix__)
  struct termios oldattr, newattr;
  tcgetattr(STDIN_FILENO, &oldattr);
  newattr = oldattr;
  newattr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
#endif

  ch = getchar();

#if defined(_WIN32)
  SetConsoleMode(input, oldmode);
#elif defined(__unix__)
  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
#endif

  return ch;
}

inline int getche() {
  int ch;
#if defined(_WIN32)
  HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
  if (input == nullptr)
    return EOF;

  DWORD oldmode;
  GetConsoleMode(input, &oldmode);
  SetConsoleMode(input, oldmode & ~ENABLE_LINE_INPUT);
#elif defined(__unix__)
  struct termios oldattr, newattr;
  tcgetattr(STDIN_FILENO, &oldattr);
  newattr = oldattr;
  newattr.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
#endif

  ch = getchar();

#if defined(_WIN32)
  SetConsoleMode(input, oldmode);
#elif defined(__unix__)
  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
#endif

  return ch;
}

inline void clrline() { std::cout << ESC << "[2K" << ESC << "E" << std::flush; }

inline void resetcolors() { std::cout << ESC << "[0m" << std::flush; }

} // namespace nocurses

#endif // NOCURSES_HPP
