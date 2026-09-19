# LCOM - Jogo da Forca (Hangman)

> Jogo da Forca em C, desenvolvido em ambiente Minix 3 com programação de baixo nível sobre os periféricos do computador.

## Autores

Grupo 2LEIC01_4

| Nome             | E-Mail                   |
| ---------------- | ------------------------ |
| Ana Sousa        | up202306419@edu.fe.up.pt |
| José Vizcaíno    | up202305009@edu.fe.up.pt |
| Matilde Sousa    | up202305502@edu.fe.up.pt |
| Rafael Rodrigues | up202303855@edu.fe.up.pt |

## Contexto Académico

- **Unidade Curricular:** Laboratório de Computadores
- **Instituição:** FEUP — Faculdade de Engenharia da Universidade do Porto
- **Ano/Semestre:** 2024/2025 — 2.º semestre
- **Nota obtida:** 13.4/20

## Descrição

O objetivo do projeto é o desenvolvimento de uma aplicação em ambiente **Minix 3**, utilizando programação de baixo nível para interagir diretamente com os periféricos do computador. O jogo escolhido foi o clássico **Jogo da Forca**.

### Funcionalidades do Jogo

- **Modos de Jogo:** Escolha de categorias de palavras.
- **Interface Gráfica:** Desenho dinâmico da forca e do personagem à medida que o jogador erra as letras.
- **Controlo de Tempo:** Countdown em tempo real para adivinhar a palavra.
- **Input Híbrido:** Utilização do teclado para introduzir letras e do rato para navegação nos menus.
- **Feedback Visual:** Visualização das letras já tentadas e do estado atual da palavra.

## Tecnologias Utilizadas

A aplicação foi escrita em **C11** e corre sobre a **LCOM Framework (LCF)**. Foram implementados drivers para:

- **Timer (i8254):** Gestão de interrupções para controlo do framerate e do contador de tempo de jogo.
- **Teclado (i8042):** Leitura de _make codes_ e _break codes_ para input de texto.
- **Rato (i8042):** Processamento de pacotes de dados para controlo do cursor e deteção de cliques.
- **Placa Gráfica (VBE):** Implementação de desenho de pixeis, linhas, retângulos e XPMs em modo gráfico (Linear Frame Buffer).

## Estrutura do Projeto

```text
proj/
├── doc/
└── src/
    ├── assets/              # Imagens (XPM) e definições de fontes/pixmaps
    │   ├── font.c / font.h
    │   ├── game_pixmap.h
    │   └── mouse_pixmap.h
    ├── game/                # Lógica do jogo, estados, menus e categorias
    │   ├── categories.c / categories.h
    │   ├── game_state.c / game_state.h
    │   ├── game.c / game.h
    │   ├── hangman.c / hangman.h
    │   ├── instructions.c / instructions.h
    │   ├── menu.c / menu.h
    │   └── words.c / words.h
    ├── peripherals/         # Drivers de baixo nível para hardware
    │   ├── graphics/        # Driver da placa gráfica (VBE)
    │   ├── keyboard/        # Driver do teclado
    │   ├── mouse/           # Driver do rato
    │   ├── timer/           # Driver do timer
    │   ├── k_controller.c / k_controller.h
    │   └── utils.c
    ├── main.c
    └── Makefile
```

O repositório inclui ainda as pastas `lab0` a `lab5`, com os exercícios laboratoriais preparatórios (drivers de timer, teclado, rato e vídeo) que serviram de base para o projeto final em `proj/`.

## Requisitos

- Ambiente **Minix 3** com a LCOM Framework (LCF) instalada

## Como Compilar / Executar

Para correr o projeto, é necessário um ambiente **Minix 3**.

1. Navegar até à pasta do código fonte:
   ```bash
   cd proj/src
   ```
2. Compilar o projeto:
   ```bash
   make clean && make
   ```
3. Executar a aplicação:
   ```bash
   lcom_run proj
   ```

## Como Usar

Após executar a aplicação, escolher uma categoria de palavras no menu (navegável por rato) e usar o teclado para introduzir letras dentro do tempo limite, tentando adivinhar a palavra antes que a forca fique completa.

## Notas Adicionais

Os exercícios laboratoriais em `lab0` a `lab5` não fazem parte da aplicação final, servindo apenas de preparação incremental (timer, teclado, rato, vídeo) para o código implementado em `proj/`.
