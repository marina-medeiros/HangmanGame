#ifndef _HANGMAN_GM_H_
#define _HANGMAN_GM_H_

#include <set>
#include <vector>
#include <string> // std::string

#include "hm_word.h"
#include "player.h"
#include "data_manager.h"
#include "scoreboard.h"

/*!
 * This class represents the Game Controller which keeps track of player,
 * scores, and match total values, as well as determining when a match ends.
 */
class GameController {
private:
  //=== Structs
  //!< The game states.
  enum class game_state_e : short {
    STARTING = 0,       //!< Beginning the game.
    WELCOME,            //!< Opening messasges.
    MAIN_MENU,          //!< Enter main menu mode, reading option from the player.
    CATEGORY_OPTIONS,   //!< Choosing the game options
    DIFFICULTY_OPTIONS, //!< Choosing the game options
    PLAYING,            //!< Playing the game.
    FINISH_GAME,        //!< After the player wins or loses
    SHOW_SCORE,         //!< Show top scores.
    SHOW_RULES,         //!< Asks user if s/he wants to read the rules.
    QUITTING,           //!< User might want to quit (before game finishes), need to
                        //!< confirm.
    ENDING,             //!< Closing the game (final message).
  };

  //!< The menu options.
  enum class menu_e : short {
    PLAY = 1,  //!< Begin new game.
    RULES,     //!< Show rules of the game.
    SCORE,     //!< Show top scores.
    EXIT,      //!< Exit the game.
    UNDEFINED, //!< No option chosen.
  };

  //!< The match status.
  enum class match_e : short {
    ON = 1,      //!< Match still open and running.
    PLAYER_WON,  //!< Match ended and player WON.
    PLAYER_LOST, //!< Match ended and player LOST.
    //UNDEFINED,   //!< Undefined match status.
  };

  //=== Data members
  game_state_e m_game_state = game_state_e::STARTING; //!< Current game state.
  menu_e m_menu_option;      //!< Current menu option.
  mutable std::string m_system_msg; //!< Current system message displayed to user.
  bool m_asked_to_quit = false; //!< Flag that indicates whether the user wants to end
                        //!< an ongoing game.
  bool m_asked_to_menu = false; //!< Flag that indicates whether the user wants to go back
                        //!< to menu options
  bool m_match_ended = false;   //!< Flag that indicates whether the current match has
                        //!< ended or not.
  bool m_reveal_word = false;   //!< Flag that is active when user looses and we need to
                        //!< show the answer.

  //=== Game related members
  Player m_curr_player; //!<https://www.mindmeister.com/app/map/3275960895 Reference to the current player.
  HangmanWord m_curr_word; //!< Keeps track of the masked word, wrong guesses, etc.
  DataManager m_data; //!< Organizes the database information and draws the current word
  std::vector<Player> allPlayers;
  char m_ch_guess = 0; //!< Latest player guessed letter.
  char m_difficulty; //!< Keeps track of the difficulty of the game.
  int m_max_mistakes = 0; //!< Max number of mistakes allowed in a match.
  std::string m_user_name; //!< Stores the user name provided in the Welcome state.
  std::vector<std::string> m_words_list; //!< List of words to play.

public:
  //=== Public interface
  GameController() = default;
  GameController(const GameController &) = delete;
  GameController(GameController &&) = delete;
  GameController &operator=(const GameController &) = delete;
  GameController &operator=(GameController &&) = delete;
  ~GameController() = default;

  void set_difficulty(char c){
    m_difficulty = c;
    if(c == 'E'){ m_max_mistakes = 10;}
    else if(c == 'M'){ m_max_mistakes = 8;}
    else if(c == 'H'){ m_max_mistakes = 6;}
    }
  char get_difficulty() const {return m_difficulty;}

  //=== Common methods for the Game Loop design pattern.
   /// Process user input events, depending on the current game state.
  void process_events();
  /// Update the game based on the current game state.
  void update();
  /// Renders the game to the user.
  void render() const;
  /// Returns true when the user wants to quit the game.
  bool game_over() const;

private:
  // === These read_xxx() methods are called in process_events()
  /// Read the user name at the beginning of the game.
  std::string read_user_name();
  /// Reads the user confirmation, enter.
  bool read_user_enter() const;
  /// Reads the user confirmation (y/n).
  bool read_user_confirmation()const;
  /// Reads user guess letter.
  char read_user_guess();
  /// Reads user menu choice.
  menu_e read_menu_option();
  char read_difficulty_option();
  std::set<std::string> read_category_option(); 
  game_state_e menu_chosen(menu_e menu_options);

  // === These display_xxx() methods are called in render()
  /// Show the welcome mesage.
  void display_welcome() const;
  /// Show the main menu.
  void display_main_menu()const;
  /// Show category options
  void display_category_options() const;
  /// Show level difficulty options
  void display_difficulty_options() const;
  /// Show main play screen (w/ the hagman)
  void display_play_screen()const;
  /// Show screen confirming user quitting a challenge.
  void display_quitting() const;
  /// Show the game rules.
  void display_rules() const;
  /// Show farewell message displayed at the end of the game.
  void display_endgame() const;
  /// Show the top (5?) score board.
  void display_scoreboard() const;

  // === These show_xxx() methods display common elements to every screen.
  /* All screens may have up to 4 components:
   *  (1) title,                  -> must have
   *  (2) main content,           -> optional
   *  (3) a system message,       -> optional
   *  (4) an interaction message. -> optional
   */
  /// Show screen title.
  void show_screen_title(const std::string & title) const;
  /// Show system message to player.
  void show_main_content_msg() const;
  /// Show interactive message to player.
  void show_interaction_msg() const;
  /// Evaluates the match situation, whether it's over or if it still on.
  match_e match_status() const;
  /// Show the gallows with the hangman, whose body displayed depends on the # of mistakes made (argument).
  void display_gallows(short wrongGuesses, char difficulty) const;
  void display_gallows2(short wrongGuesses, char difficulty) const;
  void display_gallows_helper(short wrongGuesses) const;
  /// Changes the state of the game.
  void change_state(game_state_e new_state);
};
#endif
