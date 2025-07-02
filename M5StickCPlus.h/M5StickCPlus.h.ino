#include "globals.h"
#include "main_menu.h"
#include "wifi_menu.h"
#include "fake_ap.h"

// --- Definição das Variáveis Globais (aqui elas são criadas de facto) ---
Screen currentScreen = MAIN_MENU;

String mainMenuOptions[] = { "Menu de Wifi", "Menu de Bluetooth", "Configuracoes", "Sobre" };
int mainMenuItemCount = sizeof(mainMenuOptions) / sizeof(mainMenuOptions[0]);
int mainSelectedItem = 0;
int mainTopItemIndex = 0;
int itemsOnScreen = 3;

String wifiMenuOptions[] = { "Criar AP Falso", "Escanear Redes", "Voltar" };
int wifiMenuItemCount = sizeof(wifiMenuOptions) / sizeof(wifiMenuOptions[0]);
int wifiSelectedMenuItem = 0;
int wifiTopItemIndex = 0;


void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  drawMainMenu(); // A primeira tela a ser desenhada
}

void loop() {
  M5.update();

  // O grande roteador de telas
  switch (currentScreen) {
    case MAIN_MENU:
      handleMainMenu();
      break;
    case WIFI_MENU:
      handleWifiMenu();
      break;
    case FAKE_AP_SCREEN:
      loopFakeAP();
      break;
  }
}