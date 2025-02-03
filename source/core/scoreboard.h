//funções relacionadas à impressão e ordenação da tabela
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iterator>

#include "player.h"

int find_player_index(const Player& player, std::vector<Player>& allPlayers);

void cleanFile(const std::string& filename);

void appendPlayerToFile(const Player& player, const std::string& filename);

std::vector<Player> read_players(const std::string& filename);

bool compare_points(const Player& player1, const Player& player2);

int collumn_length(std::vector<Player>& allPlayers, std::string Player :: *collumnType, int min);

void combiningEMH(std::vector<Player>& allPlayers);

void print_table ();
