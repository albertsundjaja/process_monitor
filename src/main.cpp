#include "ncurses_display.h"
#include "system.h"
#include "processor.h"

int main() {
  Processor processor;
  System system(&processor);
  NCursesDisplay::Display(system);
}