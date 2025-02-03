/*!
 * Data Manager implementation.
 *
 * 
 */
#include <cassert>
#include <cctype>
#include <array>
#include <string> // std::string
#include <unordered_map>
#include <iterator>
#include <bits/stdc++.h>
#include <sstream>
#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>
#include <codecvt>
#include <map>
#include <fstream>
#include <locale>
#include <string.h>

float get_variation(std::string word){
    std::set<char> aux;

    for(size_t ii = 0; ii < word.length(); ii++){
        aux.insert(word[ii]);
    }

    float index = static_cast<float>(aux.size()) / word.length();

    return index;
}

char get_dificulty(std::string word){
    int dificulty = 0;

    float variation = get_variation(word);
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

//reads [language] file and assings every word into its categories
void assign_categories(std::string language, std::multimap<std::string, std::string> *easyWords, std::multimap<std::string, std::string> *mediumWords, std::multimap<std::string, std::string> *hardWords, std::set<std::string> *categories){
    std::ifstream wordsFile(language);

    if (!wordsFile.is_open()) {
        std::cerr << "Unable to open file: " << language << std::endl;
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
                (*categories).insert(category);
                char dificulty = get_dificulty(word);
                if(dificulty == 'E'){
                    (*easyWords).insert({category, word});
                }else if(dificulty == 'M'){
                    (*mediumWords).insert({category, word});
                }else if(dificulty == 'H'){
                    (*hardWords).insert({category, word});
                }
            }
            categoryCount++;
        }
    }

    wordsFile.close();
}

std::multimap<std::string, std::string> wanted_words(std::vector<std::string> chosenCategories, std::multimap<std::string, std::string> words){
    std::multimap<std::string, std::string> wantedWords;

    for (const auto& category : chosenCategories) {
        auto range = words.equal_range(category);

        for(auto it = range.first; it != range.second; ++it){
            wantedWords.insert(*it);
        }
    }

    for (const auto& pair : wantedWords) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }

    return wantedWords;
}

void intersection(std::vector<std::string> chosenCategories, char dificulty, std::multimap<std::string, std::string> easyWords, std::multimap<std::string, std::string> mediumWords, std::multimap<std::string, std::string> hardWords){
    std::multimap<std::string, std::string> words;
    std::set<std::string> intersection;

    switch (dificulty){
        case 'E':  words = wanted_words(chosenCategories, easyWords); break;
        case 'M':  words = wanted_words(chosenCategories, mediumWords); break;
        case 'H':  words = wanted_words(chosenCategories, hardWords); break;
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

}

void union2(std::vector<std::string> chosenCategories, char dificulty, std::multimap<std::string, std::string> easyWords, std::multimap<std::string, std::string> mediumWords, std::multimap<std::string, std::string> hardWords){
    std::multimap<std::string, std::string> words;
    std::set<std::string> union2;

    switch (dificulty){
        case 'E':  words = wanted_words(chosenCategories, easyWords); break;
        case 'M':  words = wanted_words(chosenCategories, mediumWords); break;
        case 'H':  words = wanted_words(chosenCategories, hardWords); break;
    }

    for (const auto& pair : words) {
        union2.insert(pair.second);
    }

    std::cout << "\n \n";
    for(const auto& str : union2){
        std::cout << str << std::endl;
    }
}

int main(){
    std::multimap<std::string, std::string> easyWords;
    std::multimap<std::string, std::string> mediumWords;
    std::multimap<std::string, std::string> hardWords;
    std::set<std::string> categories;
    assign_categories("portugues.txt", &easyWords, &mediumWords, &hardWords, &categories);

    // std::cout << "faceis: \n";
    // for (const auto& pair : easyWords) {
    //     std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    // }

    // std::cout << "medias: \n";
    // for (const auto& pair : mediumWords) {
    //     std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    // }

    // std::cout << "dificeis: \n";
    // for (const auto& pair : hardWords) {
    //     std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    // }

    // for(auto& cat : categories){
    //     std::cout << cat << std::endl;
    // }

    intersection({"comida", "fruta"}, 'E', easyWords, mediumWords, hardWords);
    union2({"comida", "fruta"}, 'E', easyWords, mediumWords, hardWords);


    return 0;
}