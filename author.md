# Introduction

The Hangman Game is a version of the classic hangman game developed in C++. Players attempt to guess a hidden word by guessing letters. The project includes features such as word categorization, different difficulty levels, a scoring system, and an option to exit the game. The code is organized into classes that represent the main components of the game, such as the controller, the hidden word, the player, and the scoreboard. Unit tests are provided to ensure the correct functionality of the classes.

# Author(s)

- Nome: Bianca Maciel Medeiros e Marina Medeiros Araujo Leite
- Email: *<bianca.medeiros.701@ufrn.edu.br e marinamaleite@gmail.com>*
- Turma: DIM0176

# Problems found or limitations

We had some trouble with the wstrings in the code

# Grading

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.

| Item                                                                                      | Valor máximo | Valor esperado |
| ----------------------------------------------------------------------------------------- | :----------: | :------------: |
| Lê nome do jogador                                                                        |      5       |       5        |
| Exibe e gerencia menu de opções                                                           |      10      |       10       |
| Exibe e gerencia escolha de categoria das palavras                                        |      10      |       10       |
| Exibe regras do jogo quando solicitado                                                    |      5       |       5        |
| Exibe tabela de pontuação                                                                 |      5       |       5        |
| Contabiliza e acumula os pontos de várias partidas                                        |      10      |       10       |
| Escolhe uma palavra aletória (sem repetir) a partir das lista de palavras lida do arquivo |      10      |       10       |
| Controla o número de erros para um jogador perder uma partida                             |      5       |       5        |
| Exibe partes do boneco na forca conforme os erros se acumulam                             |      5       |       5        |
| Exibe os erros/acertos da palavra secreta em uma partida                                  |      10      |       10       |
| Exibe palavra secreta completa, quando uma partida é perdida                              |      5       |       5        |
| Exibe a lista de chutes errados                                                           |      5       |       5        |
| Permite o cancelamento de uma partida em andamento                                        |      5       |       5        |
| Apresenta as telas no formato organizacional sugerido                                     |      5       |       5        |
| Projeto contém pelo menos 2 classes                                                       |      5       |       5        |
| Programa gerencia erros corretamente                                                      |      5       |       5        |
| Lê palavras e categorias do arquivo de palavras `words.csv`                               |      10      |       10       |
| Lê informações do pontuação `score.hgm`                                                   |      10      |       10       |
| Funcionamento geral correto do program                                                    |      10      |       10       |

# Compiling and Runnig

To compile and run the game, follow these steps:

1. Ensure you have CMake installed on your system.
2. Clone the repository to your local machine.
3. Navigate to the project directory in your terminal.
4. Go into the source file: `cd source`
4. Create a build directory: `mkdir build && cd build`.
5. Generate the build files with CMake: `cmake ..`.
6. Compile the project: `cmake --build .`.
7. Run the compiled executable: `./hangman`.
