#ifndef HM_WORD_H
#define HM_WORD_H
/*!
 * Hangman Word class
 * @file hm_word.h
 *
 * This class represents a Hangman Word, being responsible
 * for receiving a word, a (optional) list of letter that
 * must be revealed, and creating the corresponding masked
 * string for display.
 *
 * This class also receives guesses, and keeps track of how
 * many of these guesses were correct or wrong, so that at
 * the end we may retrieve this information for score
 * (performance) calculation.
 *
 * \author Selan
 * \date April 20th, 2022
 */

#include <string>
#include <vector>

class HangmanWord {
  //=== Data members or attributes.
private:
  std::string m_secret_word;  //!< The secret word to guess.
  std::string m_masked_word = std::string(m_secret_word.length(), m_mask_char);  //!< The secret word with closed letters masked.
  long m_n_correct_guesses = 0;    //!< # of correct guesses made by the player.
  char m_mask_char= '_'; //!< Char used as mask to hide letters in the secret word. _
  std::vector<char> m_wrong_guesses; //!< List of wrong guesses made.

  //=== Public types
public:
  /// Return types of the guess(char) method.
  enum class guess_e : short {
    CORRECT = 0, //!< Correct guess.
    WRONG,       //!< Wrong guess.
    REPEATED
  }; //!< This guess (wrong/correct) has been used before.

  friend bool operator==(guess_e lhs, guess_e rhs) {
    // Simply compare the underlying integer values of the enums
    return static_cast<int>(lhs) == static_cast<int>(rhs);
  }

  //=== Public interface
  /// Default Ctro. Assumes input strings are all lower case.
  /*!
   * @param secret The secret word.
   */
  HangmanWord(std::string secret = ""){
    this->m_secret_word = secret;
  }
  /// Default Ctro.
  ~HangmanWord() = default;

  //Getters:
  /// Return a the secret word with the unguessed letters masked.
  std::string get_masked_word() const { return m_masked_word; }
  /// Return the number of **wrong** guesses so far in the match.
  [[nodiscard]] int wrong_guesses() const {return static_cast<int>(m_wrong_guesses.size());}
  /// Return the number of **correct** guesses so far in the match.
  [[nodiscard]] long correct_guesses() const{return m_n_correct_guesses;}
  /// Return the list of wrong guesses.
  std::vector<char> get_wrong_guesses() const{return m_wrong_guesses;}
  /// Return the number of masked lettes in the secret word.
  [[nodiscard]] short n_masked_ch() const{
    short n_masked_ch = 0;
    for(size_t ii = 0; ii < m_masked_word.length(); ii++){
      if(m_masked_word[ii] == mask_char()){
          n_masked_ch++;
      }
    }
    return n_masked_ch;
  }
  /// Return the secret word
  [[nodiscard]] std::string secret_word() const{return m_secret_word;}
  /// Return the mask char
  [[nodiscard]] char mask_char() const{return m_mask_char;}

  //Setters:
  void set_secret_word(std::string c){m_secret_word = c;}
  void set_masked_word(std::string c){m_masked_word = c;}

  /// Randomly reveals letters from the secret word
  void reveal_random_letters(char difficulty);
  /// Given a guess `g` this method returns three possible outcomes: CORRECT,
  /// WRONG or REPEATED.
  guess_e guess(char g);
  /// Reveals letters from the secret word based on the guesses
  std::string reveal_letters(char g);
  /// Reset the object to its original state and mask the secret word.
  void reset();

};

#endif