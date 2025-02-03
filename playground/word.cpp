#include <iostream>
#include <vector>
#include <unordered_set>
#include <cstdlib>
#include <ctime>

std::string reveal_letters(int quant, std::string word) {
    int size = static_cast<int>(word.length());
    int totalLetterCount = 0;
    std::vector<int> positions; 
    std::string showWord(size, '_');
    std::unordered_set<int> revealedPositions;

    // Seed for randomness
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    while (totalLetterCount < quant) {
        int letterIndex = std::rand() % size;
        
        // Avoid spaces and already revealed positions
        if (word[letterIndex] != ' ' && revealedPositions.find(letterIndex) == revealedPositions.end()) {
            char letter = word[letterIndex];

            // Reveal all instances of this letter
            for (int ii = 0; ii < size; ii++) {
                if (word[ii] == letter && revealedPositions.find(ii) == revealedPositions.end()) {
                    showWord[ii] = word[ii];
                    revealedPositions.insert(ii);
                    totalLetterCount++;
                    
                    if (totalLetterCount >= quant) {
                        break;
                    }
                }
            }
        }
        
        // Break if all positions are revealed
        if (revealedPositions.size() >= static_cast<size_t>(size)) {
            break;
        }
    }

    return showWord;
}

int main() {
    std::string word = "banana";
    int quant = 3;
    std::string revealed = reveal_letters(quant, word);
    std::cout << "Revealed word: " << revealed << std::endl;
    return 0;
}
