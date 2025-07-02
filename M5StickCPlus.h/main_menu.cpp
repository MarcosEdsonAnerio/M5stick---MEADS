#include "globals.h"   // Inclui as variáveis globais
#include "main_menu.h" // Inclui as suas próprias declarações
#include "wifi_menu.h"   // Inclui o menu de wifi para poder navegar para ele

void drawMainMenu() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(35, 10);
  M5.Lcd.println("Menu Principal");
  M5.Lcd.drawFastHLine(0, 22, 160, DARKGREY);

  for (int i = 0; i < itemsOnScreen; i++) {
    int itemIndex = mainTopItemIndex + i;
    if (itemIndex < mainMenuItemCount) {
      int y_pos = 35 + (i * 15);
      M5.Lcd.setCursor(10, y_pos);
      if (itemIndex == mainSelectedItem) {
        M5.Lcd.setTextColor(BLACK);
        M5.Lcd.fillRect(0, y_pos - 2, 160, 12, ORANGE);
        M5.Lcd.print("> "); M5.Lcd.print(mainMenuOptions[itemIndex]);
      } else {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.print("  "); M5.Lcd.print(mainMenuOptions[itemIndex]);
      }
    }
  }
}

void handleMainMenu() {
  if (M5.BtnB.wasPressed()) {
    mainSelectedItem = (mainSelectedItem + 1) % mainMenuItemCount;
    if (mainSelectedItem >= mainTopItemIndex + itemsOnScreen) {
      mainTopItemIndex = mainSelectedItem - itemsOnScreen + 1;
    }
    if (mainSelectedItem < mainTopItemIndex) {
      mainTopItemIndex = mainSelectedItem;
    }
    drawMainMenu();
  }

  if (M5.BtnA.wasPressed()) {
    switch (mainSelectedItem) {
      case 0:
        currentScreen = WIFI_MENU;
        wifiSelectedMenuItem = 0;
        wifiTopItemIndex = 0;
        drawWifiMenu();
        break;
      case 1: case 2: case 3:
        M5.Lcd.fillScreen(BLUE); M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(30, 35); M5.Lcd.println("Funcao a implementar...");
        delay(1500);
        drawMainMenu();
        break;
    }
  }
}