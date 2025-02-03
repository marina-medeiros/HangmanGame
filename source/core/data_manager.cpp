#include <string>
#include <iterator>
#include <bits/stdc++.h>
#include <map>
#include <set>
#include <fstream> 
#include <string.h>
#include <time.h> 


#include "data_manager.h"
#include "player.h"

/*!
 * \brief Calculates the variation index of a given word.
 *
 * This method calculates the variation index by determining the ratio of unique characters
 * to the total length of the word.
 *
 * \param word The word for which to calculate the variation index.
 * \return The variation index of the word.
 */
float DataManager::calculate_variation(std::string word){
    std::set<char> aux;

    for(size_t ii = 0; ii < word.length(); ii++){
        aux.insert(word[ii]);
    }

    float index = static_cast<float>(aux.size()) / word.length();

    return index;
}

/*!
 * \brief Assigns a difficulty level to a given word.
 *
 * This method assigns a difficulty level (Easy, Medium, or Hard) based on the variation index
 * and the length of the word.
 *
 * \param word The word for which to assign a difficulty level.
 * \return A character representing the difficulty level ('E' for Easy, 'M' for Medium, 'H' for Hard).
 */
char DataManager::assign_dificulty(std::string word){
    int dificulty = 0;

    float variation = calculate_variation(word);
    float size = static_cast<float>(word.length());

    if(variation > 0 && variation <= 0.66){
        dificulty++;
    }else if(variation > 0.66 && variation <= 0.83){
        dificulty = dificulty + 2;
    }else if(variation > 0.83 && variation <= 1){
        dificulty = dificulty + 3;
    }

    if(size > 3 && size <= 7){
        dificulty++;
    }else if(size > 7 && size <= 13){
        dificulty = dificulty + 2;
    }else if(size > 13){
        dificulty = dificulty + 3;
    }

    if(dificulty < 4){
        return 'E';
    }else if(dificulty == 4){
        return 'M';
    }else if(dificulty > 4){
        return 'H';
    }

    return 'M';
}

/*!
 * \brief Assigns categories and dificulty levels to words based on a language file.
 *
 * This method reads a language file containing words and their categories,
 * then categorizes the words based on their difficulty level.
 *
 * \param language The path to the language file.
 */
void DataManager::assign_categories(std::string language){
    std::ifstream wordsFile(language);

    if (!wordsFile.is_open()) {
        std::cout << "Problem opening the file:" + language << std::endl;
        return;
    }

    std::string line;
    while(std::getline(wordsFile, line)){
        std::stringstream auxLine(line);
        std::string auxWord;
        std::string word;
        std::string category;
        int categoryCount = 0;

        while(!auxLine.eof()){
            std::getline(auxLine, auxWord, ',');
            if(categoryCount == 0){
                word = auxWord;
            }else{
                category = auxWord;
                (categories).insert(category);
                char dificulty = assign_dificulty(word);
                if(dificulty == 'E'){
                    (easyWords).insert({category, word});
                }else if(dificulty == 'M'){
                    (mediumWords).insert({category, word});
                }else if(dificulty == 'H'){
                    (hardWords).insert({category, word});
                }
            }
            categoryCount++;
        }
    }

    wordsFile.close();
}

/*!
 * \brief Retrieves words from specified categories.
 *
 * This method retrieves words from the specified categories and returns them
 * as a multimap.
 *
 * \param words The multimap containing words and their categories.
 * \return A multimap of words from the specified categories.
 */
std::multimap<std::string, std::string> DataManager::words_from_categories(std::multimap<std::string, std::string> words){
    std::multimap<std::string, std::string> wantedWords;

    for (const auto& category : chosenCategories) {
        auto range = words.equal_range(category);

        for(auto it = range.first; it != range.second; ++it){
            wantedWords.insert(*it);
        }
    }

    return wantedWords;
}

/*!
 * \brief Finds the intersection of words from specified categories with the given difficulty.
 *
 * This method finds the intersection of words that belong to all chosen categories with the specified difficulty.
 *
 * \param dificulty The difficulty level ('E', 'M', 'H') to filter the words.
 * \return A set of words that are common to all chosen categories with the specified difficulty.
 */
std::set<std::string> DataManager::intersection(char dificulty){
    std::multimap<std::string, std::string> words;
    std::set<std::string> intersection;

    switch (dificulty){
        case 'E':  words = words_from_categories(easyWords); break;
        case 'M':  words = words_from_categories(mediumWords); break;
        case 'H':  words = words_from_categories(hardWords); break;
    }

    std::multimap<std::string, std::string> swappedMap;

    for(const auto& pair : words){
        swappedMap.emplace(pair.second, pair.first);
    }

    for (const auto& pair : swappedMap) {
        auto range = swappedMap.equal_range(pair.first);
        size_t count = std::distance(range.first, range.second);
        if(count == chosenCategories.size()){
            intersection.insert(pair.first);
        }
    }

    for(const auto& str : intersection){
        std::cout << str << std::endl;
    }

    return intersection;
}

/*!
 * \brief Finds the union of words from specified categories with the given difficulty.
 *
 * This method finds the union of words that belong to any of the chosen categories with the specified difficulty.
 *
 * \param dificulty The difficulty level ('E', 'M', 'H') to filter the words.
 * \return A set of words that belong to any of the chosen categories with the specified difficulty.
 */
std::set<std::string> DataManager::union2(char dificulty){
    std::multimap<std::string, std::string> words;
    std::set<std::string> union2;

    switch (dificulty){
        case 'E': words = words_from_categories(easyWords); break;
        case 'M': words = words_from_categories(mediumWords); break;
        case 'H': words = words_from_categories(hardWords); break;
    }

    for (const auto& pair : words) {
        union2.insert(pair.second);
    }
    
    return union2;
}

/*!
 * \brief Draws a word for the player based on difficulty and played words.
 *
 * This method draws a word for the player considering the specified difficulty level
 * and the words the player has already played.
 *
 * \param difficulty The difficulty level ('E', 'M', 'H') to filter the words.
 * \param player The player object to check played words.
 * \return A randomly selected word that the player has not played yet.
 */
std::string DataManager::draw_word(char difficulty, Player player){
    std::set<std::string> available_words;
    if(intersection(difficulty).empty()){
        available_words = union2(difficulty);
    }else{
        available_words = intersection(difficulty);
    }

    for(const auto& word : player.get_played_words()){
        available_words.erase(word);
    }

    if(available_words.empty()) {
        return "-"; // or handle the case when there are no available words
    }

    srand(time(0));

    int index = rand() % static_cast<int>(available_words.size());

    std::vector<std::string> available_words_vec(available_words.begin(), available_words.end());

    return (available_words_vec[index]);
}

/*!
 * \brief Prints the categories.
 *
 * This method prints either all categories or only the chosen categories based on the parameter.
 *
 * \param all_cat A boolean indicating whether to print all categories (true) or only chosen categories (false).
 */
void DataManager::print_categories(bool all_cat) const {
    if (all_cat) {
        int count = 0;
        for (auto& element : get_categories()) {
            if(count > 0){
                std::cout << count << " - " << element << std::endl;
            }
            count++;
        }
    } else {
        for (auto& element : get_chosen_categories()) {
            std::cout << " - " << element << std::endl;
        }
    }
}