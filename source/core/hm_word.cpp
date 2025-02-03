/*!
 * \class HangmanWord
 * \brief Implementation of the HangmanWord class.
 *
 * This file contains the implementation of the HangmanWord class,
 * which is responsible for managing the word to be guessed in the 
 * Hangman game. It includes methods for revealing random letters,
 * making guesses, revealing guessed letters, and resetting the word.
 *
 */

#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>

#include "hm_word.h"

/*!
 * \brief Reveals a certain number of random letters based on the difficulty level.
 * 
 * This function takes the difficulty level as a parameter and reveals a 
 * corresponding number of random letters in the secret word. The difficulty 
 * levels and the number of letters revealed are as follows:
 * - 'E' (Easy): 3 letters
 * - 'M' (Medium): 2 letters
 * - 'H' (Hard): 0 letters
 * 
 * \param difficulty The difficulty level ('E', 'M', or 'H').
 */
void HangmanWord::reveal_random_letters(char difficulty) {
  int quant;
  switch (difficulty) {
    case 'E': quant = 3;
    case 'M': quant = 2;
    case 'H': quant = 0;
  }
  int size = static_cast<int>(m_secret_word.length());
  int totalLetterCount = 0;
  std::vector<int> positions; 
  std::string showWord(size, '_');
  std::set<int> revealedPositions;

  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  while (totalLetterCount < quant) {
      int letterIndex = std::rand() % size;
      
      if (m_secret_word[letterIndex] != ' ' && revealedPositions.find(letterIndex) == revealedPositions.end()){
          char letter = m_secret_word[letterIndex];

          for (int ii = 0; ii < size; ii++) {
              if (m_secret_word[ii] == letter && revealedPositions.find(ii) == revealedPositions.end()) {
                  showWord[ii] = m_secret_word[ii];
                  revealedPositions.insert(ii);
                  totalLetterCount++;
                  
                  if (totalLetterCount >= quant) {
                      break;
                  }
              }
          }
      }
      if (revealedPositions.size() >= static_cast<size_t>(size)) {
          break;
      }
  }

  m_masked_word = showWord;
}

/*!
 * \brief Makes a guess in the Hangman game.
 * 
 * This function takes a character as a guess and checks if it is present 
 * in the secret word. It returns the result of the guess, which can be 
 * CORRECT, REPEATED, or WRONG.
 * 
 * \param g The guessed character.
 * \return The result of the guess.
 */
HangmanWord::guess_e HangmanWord::guess(char g){
  for(size_t ii = 0; ii < m_secret_word.length(); ii++){
      if(m_secret_word[ii] == g){
          return guess_e::CORRECT;
      }
  }
  for(size_t ii = 0; ii < m_masked_word.length(); ii++){
      if(m_masked_word[ii] == g){
          return guess_e::REPEATED;
      }
  }
  for(size_t ii = 0; ii < m_wrong_guesses.size(); ii++){
      if(m_wrong_guesses[ii] == g){
          return guess_e::REPEATED;
      }
  }
  return guess_e::WRONG;
}

/*!
 * \brief Reveals the guessed letters in the masked word.
 * 
 * This function updates the masked word to reveal the positions of the guessed 
 * letter if the guess is correct. If the guess is wrong, it adds the guessed 
 * letter to the list of wrong guesses.
 * 
 * \param g The guessed character.
 * \return The updated masked word.
 */
std::string HangmanWord::reveal_letters(char g) {
  if(guess(g) == guess_e::CORRECT){
    for(size_t ii = 0; ii < m_secret_word.length(); ii++){
      if(m_secret_word[ii] == g){
        m_masked_word[ii] = g;
      }
    }
  }else if(guess(g) == guess_e::WRONG){
    m_wrong_guesses.push_back(g);
  }


  return m_masked_word;
}

/*!
 * \brief Resets the HangmanWord object.
 * 
 * This function resets the number of correct guesses, clears the list of wrong 
 * guesses, and resets the masked word to its initial state with all characters 
 * masked.
 */
void HangmanWord::reset() {
    m_n_correct_guesses = 0;
    m_wrong_guesses.clear();
    m_masked_word = std::string(m_secret_word.length(), m_mask_char);
}
