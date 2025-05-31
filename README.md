# LCOM - Jogo da Forca (Hangman)

Este projeto foi desenvolvido no âmbito da unidade curricular de **Laboratório de Computadores (LCOM)** do 2º ano da Licenciatura em Engenharia Informática e de Computação (LEIC) na FEUP, no ano letivo 2024/2025.

## Grupo 2LEIC01_4

- Ana Sousa (up202306419@up.pt)
- José Vizcaíno (up202305009@up.pt)
- Matilde Sousa (up202305502@up.pt)
- Rafael Rodrigues (up202303855@up.pt)

## Descrição do Projeto

O objetivo do projeto é o desenvolvimento de uma aplicação em ambiente **Minix 3**, utilizando programação de baixo nível para interagir diretamente com os periféricos do computador. O jogo escolhido foi o clássico **Jogo da Forca**.

### Funcionalidades do Jogo:

- **Modos de Jogo:** Escolha de categorias de palavras.
- **Interface Gráfica:** Desenho dinâmico da forca e do personagem à medida que o jogador erra as letras.
- **Controlo de Tempo:** Countdown em tempo real para adivinhar a palavra.
- **Input Híbrido:** Utilização do teclado para introduzir letras e do rato para navegação nos menus.
- **Feedback Visual:** Visualização das letras já tentadas e do estado atual da palavra.

## Tecnologias e Periféricos

A aplicação foi escrita em **C11** e corre sobre a **LCOM Framework (LCF)**. Foram implementados drivers para:

- **Timer (i8254):** Gestão de interrupções para controlo do framerate e do contador de tempo de jogo.
- **Teclado (i8042):** Leitura de _make codes_ e _break codes_ para input de texto.
- **Rato (i8042):** Processamento de pacotes de dados para controlo do cursor e deteção de cliques.
- **Placa Gráfica (VBE):** Implementação de desenho de pixeis, linhas, retângulos e XPMs em modo gráfico (Linear Frame Buffer).

## Estrutura do Repositório

- `proj/src/`: Código fonte principal do projeto.
  - `peripherals/`: Drivers de baixo nível para hardware.
  - `game/`: Lógica do jogo, gestão de estados e menus.
  - `assets/`: Imagens (XPM) e definições de fontes.
- `lab2-lab5`: Exercícios laboratoriais preparatórios.

## Como Executar

Para correr o projeto, é necessário um ambiente **Minix 3**.

1.  Navegar até à pasta do código fonte:
    ```bash
    cd proj/src
    ```
2.  Compilar o projeto:
    ```bash
    make clean && make
    ```
3.  Executar a aplicação:
    ```bash
    lcom_run proj
    ```
