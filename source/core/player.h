#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <codecvt>
#include <locale>

class Player {
  //=== Private members.
private:
  std::string m_name;                   //!< The player's name
  int m_score = 0;                      //!< The player's total score.
  std::set<std::string> m_played_words; //!< List of words played in a game.
  int m_E_played = 0;
  int m_M_played = 0;
  int m_H_played = 0;
  int m_E_won = 0;
  int m_M_won = 0;
  int m_H_won = 0;
  int m_index;

  //=== Public interface
public:
  /// Default Ctro.
  Player(std::string n = "no_name") : m_name{std::move(n)} { /*empty*/ }
  /// Dtro
  ~Player() = default;

  Player& operator=(const Player& other) {
    if (this != &other) {
      m_name = other.m_name;
      m_score = other.m_score;
      m_played_words = other.m_played_words;
      m_E_played = other.m_E_played;
      m_M_played = other.m_M_played;
      m_H_played = other.m_H_played;
      m_E_won = other.m_E_won;
      m_M_won = other.m_M_won;
      m_H_won = other.m_H_won;
    }
    return *this;
  }

  //Getters:
  /// Returns the player's name
  std::string name() const;
  /// Returns the player's accumulated score.
  [[nodiscard]] int score() const;
  int get_E_played() const {return m_E_played;}
  int get_M_played() const {return m_M_played;}
  int get_H_played() const {return m_H_played;}
  int get_E_won() const {return m_E_won;}
  int get_M_won() const {return m_M_won;}
  int get_H_won() const {return m_H_won;}
  int get_index() const {return m_index;}
  std::set<std::string> get_played_words() const {return m_played_words;}

  //Setters:
  ///  Sets player name.
  void name(const std::string &);
  void set_E_played(int p){m_E_played = p;}
  void set_M_played(int p){m_M_played = p;}
  void set_H_played(int p){m_H_played = p;}
  void set_E_won(int p){m_E_won = p;}
  void set_M_won(int p){m_M_won = p;}
  void set_H_won(int p){m_H_won = p;}
  void set_name(std::string n){m_name = n;}
  void set_index(int p){m_index = p;}
  
  void insert_played_word(std::string w){m_played_words.insert(w);}
  void add_word(const std::string &w);
  /// Add the score from one challenge.
  void add_score(int);
  /// Check if this word has been played before.
  bool has_played(const std::string &) const;
  /// Clear the word list.
  void clear_word_list();

  std::string stringGamesPlayed;
  std::string stringGamesWon;
  std::string stringGamesLost;
  std::string stringWinPercentage;
  std::string stringScore = std::to_string(m_score);
};

#endif