#ifndef GLOBALS_H
#define GLOBALS_H

#include <M5StickC.h> // A biblioteca principal fica aqui para ser partilhada

// Enum para controlar em qual tela/menu nós estamos
enum Screen {
  MAIN_MENU,
  WIFI_MENU,
  FAKE_AP_SCREEN
};

// Declarações 'extern' dizem: "Estas variáveis existem, estão definidas noutro lugar"
extern Screen currentScreen;

extern String mainMenuOptions[];
extern int mainMenuItemCount;
extern int mainSelectedItem;
extern int mainTopItemIndex;

extern String wifiMenuOptions[];
extern int wifiMenuItemCount;
extern int wifiSelectedMenuItem;
extern int wifiTopItemIndex;

extern int itemsOnScreen;

#endif