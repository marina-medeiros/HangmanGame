#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <set>
#include <string>
#include <map>

#include "player.h"

class DataManager{
private:
    std::multimap<std::string, std::string> easyWords; //category, word
    std::multimap<std::string, std::string> mediumWords;
    std::multimap<std::string, std::string> hardWords;
    std::set<std::string> categories;
    std::set<std::string> chosenCategories;

public:
    DataManager() = default;

    //Getters:
    std::set<std::string> get_categories()const{return categories;};
    std::set<std::string> get_chosen_categories()const{return chosenCategories;}

    //Setters:
    void set_chosen_categories(std::set<std::string> c){ chosenCategories = c;}

    float calculate_variation(std::string word);
    void assign_categories(std::string language);
    char assign_dificulty(std::string word);
    std::multimap<std::string, std::string> words_from_categories(std::multimap<std::string, std::string> words);
    std::set<std::string> intersection(char difficulty);
    std::set<std::string> union2(char difficulty);
    std::string draw_word(char difficulty, Player player);
    void print_categories(bool all_cat) const;
};

#endif