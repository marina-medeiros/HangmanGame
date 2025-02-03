#ifndef SCORE_H
#define SCORE_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "scoreboard.h"
#include "player.h"

/*!
 * \file score.h
 * \brief Contains functions for managing player scores and related data.
 *
 * This file provides functions for managing player scores, including reading and writing to files,
 * finding player indices, and cleaning files.
 */

/*!
 * \brief Finds the index of a player in a vector of players.
 * \param player The player to search for.
 * \param allPlayers The vector containing all players.
 * \return The index of the player if found, otherwise -1.
 */
int find_player_index(const Player& player, std::vector<Player>& allPlayers){
    int size = static_cast<int>((allPlayers).size());
    for(int ii = 0; ii < size; ii++){
        if((allPlayers)[ii].name() == player.name()){
            return ii;
        }
    }
    return -1;
}

/*!
 * \brief Cleans a file by truncating its content.
 * \param filename The name of the file to clean.
 */
void cleanFile(const std::string& filename) {
    std::ofstream file(filename, std::ofstream::out | std::ofstream::trunc);
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
    }
}

/*!
 * \brief Appends player data to a file.
 * \param player The player whose data is to be appended.
 * \param filename The name of the file to append to.
 */
void appendPlayerToFile(const Player& player, const std::string& filename) {
    std::ofstream file(filename, std::ios_base::app);
    if (!file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }
    file << player.name() << ",";
    file << player.score() << ",";
    file << player.get_E_played() << ",";
    file << player.get_M_played() << ",";
    file << player.get_H_played() << ",";
    file << player.get_E_won() << ",";
    file << player.get_M_won() << ",";
    file << player.get_H_won() << ",";
    for (const auto& word : player.get_played_words()) {
        file << word << ",";
    }
    file << std::endl;
    
    file.close();
}

/*!
 * \brief Reads player data from a file and returns a vector of players.
 * \param filename The name of the file to read from.
 * \return A vector containing player data read from the file.
 */
std::vector<Player> read_players(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<Player> players;

    if (!file.is_open()) {
        std::cout << "Error opening file " << filename << std::endl;
        return players;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        int count = 0;
        std::vector<std::string> words;
        Player player;
        while (std::getline(ss, word, ',')) {
            switch(count){
                case 0: player.set_name(word); break;
                case 1: player.add_score(std::stoi(word)); break;
                case 2: player.set_E_played(std::stoi(word)); break;
                case 3: player.set_M_played(std::stoi(word)); break;
                case 4: player.set_H_played(std::stoi(word)); break;
                case 5: player.set_E_won(std::stoi(word)); break;
                case 6: player.set_M_won(std::stoi(word)); break;
                case 7: player.set_H_won(std::stoi(word)); break;
                default: player.insert_played_word(word); break;
            }
            count++;
        }
        players.push_back(player);
    }

    file.close();
    return players;
}

/*!
 * \brief Compares players based on their scores.
 * \param player1 The first player.
 * \param player2 The second player.
 * \return True if the score of player1 is greater than player2, otherwise false.
 */
bool compare_points(const Player& player1, const Player& player2) {
    return player1.score() > player2.score();
}

/*!
 * \brief Calculates the maximum length of a column in the player data table.
 * \param allPlayers The vector containing all players.
 * \param collumnType Pointer to the member variable indicating the column type.
 * \param min The minimum length of the column.
 * \return The maximum length of the specified column.
 */
int collumn_length(std::vector<Player>& allPlayers, std::string Player :: *collumnType, int min) {
    int BiggestLength = min + 2;
    for (Player& person : allPlayers) {
        std::string& columnValue = person.*collumnType;
        if(int(columnValue.length()) > BiggestLength) {
            BiggestLength = columnValue.length();
        }
    }
    return BiggestLength;
}

/*!
 * \brief Combines played games and win statistics of players for different difficulty levels.
 * \param allPlayers The vector containing all players.
 */
void combiningEMH(std::vector<Player>& allPlayers){
    for (Player& person : allPlayers) {
        std:: stringstream GamesPlayed, GamesWon, GamesLost;
        GamesPlayed << person.get_E_played() << "/" << person.get_M_played() << "/" << person.get_H_played();
        GamesWon << person.get_E_won() << "/" << person.get_M_won() << "/" << person.get_H_won();
        GamesLost << person.get_E_played() - person.get_E_won() << "/" << person.get_M_played() - person.get_M_won() << "/" << person.get_H_played() - person.get_H_won();
        person.stringGamesPlayed = GamesPlayed.str();
        person.stringGamesWon = GamesWon.str();
        person.stringGamesLost = GamesLost.str();
        person.stringWinPercentage = setWinPercentage(person);
    }
}

std::string setWinPercentage(Player& player){
    std:: stringstream WinPercentage;
    float easyPercentage = 0;
    float mediumPercentage = 0;
    float hardPercentage = 0;
    if(player.get_E_played() > 0){
        easyPercentage = float(player.get_E_won())/float(player.get_E_played()) *100;
    }
    if(player.get_M_played() > 0){
        mediumPercentage = float(player.get_M_won())/float(player.get_M_played()) *100;
    }
    if(player.get_H_played() > 0){
        hardPercentage = float(player.get_H_won())/float(player.get_H_played()) *100;
    }
    WinPercentage << std::fixed << std::setprecision(1) << easyPercentage << "%/"<< std::fixed << std::setprecision(1) << mediumPercentage << "%/" << std::fixed << std::setprecision(1) << hardPercentage << "%";

    return WinPercentage.str();
}

/*!
 * \brief Prints the player data table.
 */
void print_table (){
    std::ifstream content_entrada("scoreboard.hgm", std::ios::binary);
    if (!content_entrada) {
        std::cerr << "Erro ao abrir o content para leitura." << std::endl;
        //exit?
    }

    // Ler todos os allPlayers do content e armazená-los em um vetor
    std::vector<Player> allPlayers = read_players("scoreboard.hgm");

    // Ordenar os allPlayers por pontuação (da maior para a menor)
    std::sort(allPlayers.begin(), allPlayers.end(), compare_points);

    combiningEMH(allPlayers);

    int collumn1, collumn2, collumn3, collumn4, collumn5, collumn6;
    collumn1 = 20;
    collumn2 = collumn_length(allPlayers, &Player::stringGamesPlayed , 14);
    collumn3 = collumn_length(allPlayers, &Player::stringGamesWon , 17);
    collumn4 = collumn_length(allPlayers, &Player::stringGamesLost , 18);
    collumn5 = collumn_length(allPlayers, &Player::stringWinPercentage , 24);
    collumn6 = collumn_length(allPlayers, &Player::stringScore , 5);

    std::vector<std::string> header {"Player", "Games (E/M/H)", "Games Won (E/M/H)", "Games Lost (E/M/H)", "Win Percentage (E/M/H)", "Score"};
    std::vector<int> collumnsWidth {collumn1, collumn2, collumn3, collumn4, collumn5, collumn6};

    std::cout << std::left;
    for(int i = 0; i < int(header.size()); i++){
        std::cout << std::setw(collumnsWidth[i]) << header[i];
        if(i==0){std::cout << "  ";}
    }
    std::cout << std::endl;
    for (const Player& players : allPlayers) {
        std::cout << std::left << std::setw(collumn1) << players.name() << "  " << std::setw(collumn2) << players.stringGamesPlayed << std::setw(collumn3) << players.stringGamesWon  << std::setw(collumn4) << players.stringGamesLost  << std::setw(collumn5) << players.stringWinPercentage  << std::setw(collumn6) << players.score() << std::endl;
    }
    std::cout << std::setfill('-') << std::setw(collumn1+collumn2+collumn3+collumn4+collumn5+collumn6) << "" << std::setfill(' ') << std::endl;
}

#endif