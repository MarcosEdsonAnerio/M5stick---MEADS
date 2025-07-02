# M5Stick-MEADS

![M5Stick-C](https://static.m5stack.com/uploads/2019/11/stickc_01.png)

Firmware customizado para o dispositivo **M5Stick-C**, focado em demonstrações de segurança Wi-Fi e estudos de redes. O nome MEADS é um acrónimo para **M**arcos **E**dson **A**nerio **d**os **S**antos.

Este projeto foi desenvolvido como uma ferramenta de aprendizado para explorar as capacidades da plataforma ESP32 no campo da segurança de redes sem fio, de forma controlada e educativa.

## ✨ Funcionalidades

O firmware M5Stick-MEADS possui uma interface de menu e dois modos de ataque distintos:

* **Navegação por Menus:**
    * Interface de utilizador controlada pelos botões do dispositivo.
    * Menus e submenus para fácil seleção de funcionalidades.
    * Lógica de rolagem (scroll) para listas com muitos itens.
    * Código organizado em módulos (`.h` e `.cpp`) para cada tela.

* **Ataque "Clone AP (Portal)" (Evil Twin):**
    * **Escaneamento e Clonagem Dinâmica:** Escaneia redes Wi-Fi próximas e clona os seus SSIDs.
    * **Rotação de SSID:** Alterna automaticamente entre os nomes das redes clonadas a cada 30 segundos para aumentar a credibilidade.
    * **Portal Cativo:** Cria um Ponto de Acesso aberto e redireciona qualquer utilizador para uma página de login falsa.
    * **Engenharia Social:** A página de login imita a da Conta Google para máxima eficácia.
    * **Captura de Dados Avançada:**
        * Recolhe **credenciais** (usuário e senha).
        * Recolhe **dados passivos** do navegador (Resolução de tela, Fuso Horário, Idioma, Plataforma/SO).
        * Captura o **endereço IP** do cliente e todos os **cabeçalhos HTTP** da requisição.
    * **Visualização de Logs:**
        * Uma rota secreta (`/rotaparaverlogins`) exibe em tempo real todos os dados capturados.
        * A página de logs apresenta os dados em blocos organizados para cada tentativa de login.

* **Ataque de Inundação (Beacon Flood):**
    * Escaneia as redes Wi-Fi no ambiente.
    * Cria uma "inundação" de pacotes de anúncio (beacons).
    * Gera centenas de redes Wi-Fi "fantasma" na lista de redes de dispositivos próximos, usando os nomes das redes reais.
    * **Objetivo:** Causar perturbação e poluição na camada 2 da rede, demonstrando um tipo de ataque de negação de serviço.

## 🛠️ Construído com

* **C++** - Linguagem principal de programação para o firmware.
* **Arduino Framework** - Abstração de hardware e bibliotecas para o ESP32.
* **M5Stick-C Hardware** - A plataforma de hardware alvo do projeto.
* **HTML5 / CSS3** - Estruturação e estilização das páginas web do Portal Cativo.
* **JavaScript** - Captura de dados passivos do lado do cliente (navegador).

## 🚀 Como Usar

Para compilar e carregar este firmware no seu próprio M5Stick-C, siga os passos abaixo.

### Pré-requisitos

1.  **Hardware:** Um dispositivo M5Stick-C.
2.  **Software:** [Arduino IDE](https://www.arduino.cc/en/software) instalada.
3.  **Configuração da IDE:**
    * Adicione o URL do gestor de placas M5Stack: `https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json`
    * Instale o pacote de placas "M5Stack" através do Gestor de Placas.
    * Instale a biblioteca "M5StickC" através do Gestor de Bibliotecas.

### Instalação

1.  Clone este repositório para a sua máquina local.
    ```sh
    git clone [https://github.com/MarcosEdsonAnerio/seu-repositorio.git](https://github.com/MarcosEdsonAnerio/seu-repositorio.git)
    ```
2.  Abra o ficheiro `.ino` principal na Arduino IDE.
3.  No menu `Ferramentas` > `Placa`, selecione **"M5Stick-C"**.
4.  Conecte o seu M5Stick-C ao computador e selecione a porta COM correta.
5.  Clique em **Carregar (Upload)**.

## ✒️ Autor

Este firmware foi criado por Marcos Edson Anerio Dos Santos.

* **Marcos Edson Anerio Dos Santos** - *Desenvolvedor* - [MarcosEdsonAnerio](https://github.com/MarcosEdsonAnerio)

---
*Este projeto foi desenvolvido para fins estritamente educativos e de demonstração. A utilização das suas funcionalidades em redes ou dispositivos sem autorização explícita é ilegal e antiética.*
