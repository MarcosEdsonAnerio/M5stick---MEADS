#include "globals.h"
#include "wifi_menu.h"
#include "main_menu.h"
#include "fake_ap.h"   // <<-- IMPORTANTE: Incluir o header do nosso novo módulo

void drawWifiMenu() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(45, 10);
  M5.Lcd.println("Menu de Wifi");
  M5.Lcd.drawFastHLine(0, 22, 160, DARKGREY);

  for (int i = 0; i < itemsOnScreen; i++) {
    int itemIndex = wifiTopItemIndex + i;
    if (itemIndex < wifiMenuItemCount) {
      int y_pos = 35 + (i * 15);
      M5.Lcd.setCursor(10, y_pos);
      if (itemIndex == wifiSelectedMenuItem) {
        M5.Lcd.setTextColor(BLACK);
        M5.Lcd.fillRect(0, y_pos - 2, 160, 12, ORANGE);
        M5.Lcd.print("> "); M5.Lcd.print(wifiMenuOptions[itemIndex]);
      } else {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.print("  "); M5.Lcd.print(wifiMenuOptions[itemIndex]);
      }
    }
  }
}

void handleWifiMenu() {
  if (M5.BtnB.wasPressed()) {
    wifiSelectedMenuItem = (wifiSelectedMenuItem + 1) % wifiMenuItemCount;
    if (wifiSelectedMenuItem >= wifiTopItemIndex + itemsOnScreen) {
      wifiTopItemIndex = wifiSelectedMenuItem - itemsOnScreen + 1;
    }
    if (wifiSelectedMenuItem < wifiTopItemIndex) {
      wifiTopItemIndex = wifiSelectedMenuItem;
    }
    drawWifiMenu();
  }

  if (M5.BtnA.wasPressed()) {
    switch (wifiSelectedMenuItem) {
      // --- MODIFICAÇÃO PRINCIPAL ABAIXO ---
      case 0: // Ação para "Criar AP Falso"
        currentScreen = FAKE_AP_SCREEN; // Muda para a nova tela
        setupFakeAP();                // Chama a função de setup do módulo de AP Falso
        break;
        
      case 1: // Ação para "Escanear Redes" (continua como placeholder)
        M5.Lcd.fillScreen(DARKCYAN); M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(30, 35); M5.Lcd.println("Funcao a implementar...");
        delay(1500);
        drawWifiMenu();
        break;
        
      case 2: // Ação para "Voltar"
        currentScreen = MAIN_MENU;
        drawMainMenu();
        break;
    }
  }
}