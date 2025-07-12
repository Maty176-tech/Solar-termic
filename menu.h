#pragma once
#include <arduino.h>
// Deklarace pro menu
void drawMenu();      // Funkce pro vykreslení menu
void menuUp();        // Pohyb nahoru v menu
void menuDown();      // Pohyb dolů v menu
void menuEnter();     // Vyber položku menu
void menuBack();      // Návrat do předchozího menu

// Definice pro různé typy položek v menu
#define MENU_VALUE 0
#define MENU_SUBMENU 1
#define MENU_ACTION 2

// Struktura pro menu položky
typedef struct{
  const char *label;
  uint8_t type;
  void (*action)();
  void *valuePtr;
  int minValue;
  int maxValue;
  struct MenuItem*children;
  int childrenCount;
};
