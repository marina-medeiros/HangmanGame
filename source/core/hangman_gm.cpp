/**
 * @file game_controller.cpp
 * @brief Implementation of the GameController class for Hangman game.
 */
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <set>
#include <vector>
#include <cstdlib> // for exit()

#include "hangman_gm.h"
#include "scoreboard.h"

/**
 * @brief Clears the console screen.
 */
void clear_screen() {
#ifdef _WIN32
    system("CLS");
#else
    // Assume POSIX
    std::cout << "\033[2J\033[1;1H";
#endif
}

/**
 * @brief Processes events based on the current game state.
 */
void GameController::process_events(){
  switch (m_game_state) {
    case game_state_e::STARTING:
      m_curr_word.reset();
      m_data.assign_categories("../data/word.cvs");
      break;
    case game_state_e::WELCOME:
      m_curr_player.name(read_user_name());
      break;
     case game_state_e::MAIN_MENU:
       m_menu_option = read_menu_option();
       break;
    case game_state_e::CATEGORY_OPTIONS:
      m_data.set_chosen_categories(read_category_option());
      break;
     case game_state_e::DIFFICULTY_OPTIONS :
      set_difficulty(read_difficulty_option());
      break;
    case game_state_e::PLAYING:
      if(!m_match_ended){
        m_ch_guess = read_user_guess();
      }
      break;
    case game_state_e::FINISH_GAME:
      m_asked_to_menu = read_user_enter();
      break; 
    case game_state_e::SHOW_RULES:
      m_asked_to_menu = read_user_enter();
      break;
    case game_state_e::SHOW_SCORE:
      m_asked_to_menu = read_user_enter();
      break;
     case game_state_e::QUITTING:
       m_asked_to_quit = read_user_confirmation();
       break;
     case game_state_e::ENDING:
       break;
  }
}

/**
 * @brief Updates the game state.
 */
void GameController::update(){
  switch (m_game_state) {
    case game_state_e::STARTING:
      change_state(game_state_e::WELCOME);
      break;
    case game_state_e::WELCOME:
      allPlayers = read_players("scoreboard.hgm");
      m_curr_player.set_index(find_player_index(m_curr_player,allPlayers));
      if(m_curr_player.get_index() != -1){
        m_curr_player = allPlayers[m_curr_player.get_index()];
      }
      change_state(game_state_e::MAIN_MENU);
      break;
    case game_state_e::MAIN_MENU:
      if (m_menu_option == menu_e::UNDEFINED) {
          // Opção inválida, solicite novamente
          std::cout << "Invalid option. Please enter a valid option." << std::endl;
          clear_screen();
      }
      else {
          change_state(menu_chosen(m_menu_option));
      }
      break;
    case game_state_e::CATEGORY_OPTIONS:
      change_state(game_state_e::DIFFICULTY_OPTIONS);
      break;
    case game_state_e::DIFFICULTY_OPTIONS:
      if(m_data.draw_word(get_difficulty(), m_curr_player) != "-"){
        m_curr_word.set_secret_word(m_data.draw_word(get_difficulty(), m_curr_player));
        m_curr_word.reveal_random_letters(get_difficulty());
        change_state(game_state_e::PLAYING);
      }else{
        change_state(game_state_e::CATEGORY_OPTIONS);
      }
      break;
    case game_state_e::PLAYING:
      m_curr_word.reveal_letters(m_ch_guess);
      switch (match_status()){
      case match_e::ON: 
          clear_screen();
          break;
      case match_e::PLAYER_WON:
          if (m_match_ended==true){
            change_state(game_state_e::FINISH_GAME); 
          }else{
            clear_screen();
            m_match_ended = true;
          }
          break;
      case match_e::PLAYER_LOST:
          if (m_match_ended==true){
            change_state(game_state_e::FINISH_GAME); 
          }else{
            clear_screen();
            m_match_ended = true;
          }
          break;
      }
      break;

    case game_state_e::FINISH_GAME:
      switch (get_difficulty()){
        case 'E': 
          m_curr_player.set_E_played((m_curr_player.get_E_played())+1);
          if(match_status()==match_e::PLAYER_WON){
            m_curr_player.set_E_won((m_curr_player.get_E_won())+1);
            m_curr_player.add_score(1);
          }
          break;
        case 'M':
          m_curr_player.set_M_played((m_curr_player.get_M_played())+1);
          if(match_status()==match_e::PLAYER_WON){
            m_curr_player.set_M_won((m_curr_player.get_M_won())+1);
            m_curr_player.add_score(2);
          }
          break;
        case 'H':
          m_curr_player.set_H_played((m_curr_player.get_H_played())+1);
          if(match_status()==match_e::PLAYER_WON){
            m_curr_player.set_H_won((m_curr_player.get_H_won())+1);
            m_curr_player.add_score(3);
          }
          break;
      }
      m_curr_player.insert_played_word(m_curr_word.secret_word());
      m_curr_player.set_index(find_player_index(m_curr_player,allPlayers));
      if(m_curr_player.get_index() != -1){
        allPlayers[m_curr_player.get_index()] = m_curr_player;
      }else{
        allPlayers.push_back(m_curr_player);
      }
      cleanFile("scoreboard.hgm");
      for (Player& person : allPlayers) {
        appendPlayerToFile(person,"scoreboard.hgm");
      }
      m_curr_word.reset();
      m_match_ended = false;
      if (m_asked_to_menu) {
        change_state(game_state_e::MAIN_MENU);
      }
      break;
    case game_state_e::SHOW_RULES:
      if (m_asked_to_menu) {
          change_state(game_state_e::MAIN_MENU);
      }
      break;
    case game_state_e::SHOW_SCORE:
      if (m_asked_to_menu) {
        change_state(game_state_e::MAIN_MENU);
      }
      break;
    case game_state_e::QUITTING:
      if (m_asked_to_quit) {
        change_state(game_state_e::ENDING);
      }else {
        change_state(game_state_e::MAIN_MENU);
      }
      break;
    case game_state_e::ENDING:
      exit(0);
      break;
  }
}

/**
 * @brief Renders the current game state to the console.
 */
void GameController::render() const{
    switch (m_game_state) {
    case game_state_e::WELCOME:
      display_welcome();
      break;
    case game_state_e::MAIN_MENU:
      display_main_menu();
      break;
    case game_state_e::CATEGORY_OPTIONS:
      display_category_options();
      break;
    case game_state_e::DIFFICULTY_OPTIONS:
      display_difficulty_options();
      break;
    case game_state_e::PLAYING:
      display_play_screen();
      break;

    case game_state_e::FINISH_GAME:
      if(match_status()==match_e::PLAYER_WON){
        std::cout<<std::endl<<"CONGRATULATIONS! YOU WON!"<<std::endl;
        show_interaction_msg();
        break;
      }else{
        std::cout<<std::endl<<"Sorry, you lost :( "<<std::endl;
        std::cout<<"The word was : "<< m_curr_word.secret_word() <<std::endl;
        show_interaction_msg();
        break;
      }
      break;
    case game_state_e::SHOW_RULES:
      display_rules();
      break;
    case game_state_e::SHOW_SCORE:
      display_scoreboard();
      break;
    case game_state_e::QUITTING:
      display_quitting();
      break;
    case game_state_e::ENDING:
      display_endgame();
      break;
  }
}

/**
 * @brief Checks if the game is over.
 * @return True if the game state is ENDING, otherwise false.
 */
bool GameController::game_over() const{
    return m_game_state==game_state_e::ENDING;
}

/**
 * @brief Reads the user's name from the input.
 * @return The entered user name.
 */
std::string GameController::read_user_name() {
  std::string userName;
  std::getline(std::cin, userName);
  return userName;
}

/**
 * @brief Reads a confirmation (yes/no) from the user.
 * @return True if the user confirmed with enter, otherwise false.
 */
bool GameController::read_user_enter() const {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    char c = std::cin.get();
    return c == '\n';
}

/**
 * @brief Reads a confirmation (yes/no) from the user.
 * @return True if the user confirmed with 'y' or 'Y', otherwise false.
 */
bool GameController::read_user_confirmation() const {
    std::string command;
    bool confirmation_result = true;
    
    while (true) {
        std::cin >> command;
        if (command == "y" or command == "Y") {
            confirmation_result = true;
            break;
        }
        if (command == "n" or command == "N") {
            confirmation_result = false;
            break;
        } else {
            std::cout << "Please enter 'y' or 'n': ";
        }
    }
    
    return confirmation_result;
}


/**
 * @brief Reads a guess (letter) from the user.
 * @return The guessed letter.
 */

char GameController::read_user_guess() {
  char guess;
  std::cin >> guess;
  if(guess == '#'){
    change_state(game_state_e::QUITTING);
    m_curr_word.reset();
  }else{
    if ((guess<65) || (90<guess&&guess<97) ||(guess>122)){
      std::cout << "Digits, punctuation, and spaces are not allowed. Please enter a letter >";
      return read_user_guess();
    }

    return guess;
  }
}


/**
 * @brief Reads the user's menu option.
 * @return The chosen menu option.
 */
GameController::menu_e GameController::read_menu_option() {
  int option;
  std::cin >> option;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  switch (option) {
    case 1:return menu_e::PLAY;
    case 2:return menu_e::RULES;
    case 3:return menu_e::SCORE;
    case 4:return menu_e::EXIT;
    default: std::cout << "Please enter a valid option" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    return menu_e::UNDEFINED;
  }
}

/**
 * @brief Reads the difficulty option chosen by the user.
 * @return The chosen difficulty level.
 */
char GameController::read_difficulty_option() {
  int option;
  std::cin >> option;
  switch (option) {
    case 0:return 'E';
    case 1:return 'M';
    // Reset the count of correct guesses
    case 2:return 'H';
    default: std::cout << "Please enter a valid option" << std::endl; return read_difficulty_option();
  }
}

/**
 * @brief Reads the category options chosen by the user.
 * @return A set of chosen categories.
 */
std::set<std::string> GameController::read_category_option() {
    std::set<int> numbers;
    std::string line;

    std::cin.ignore(); // Clear input buffer
    std::cout<< "Enter your options number , divided by spaces, and hit 'Enter' (ex: '1' or '1 2')>";

    std::getline(std::cin, line);

    if (!std::getline(std::cin, line)) {
      std::cerr << "Error reading input" << std::endl;
  }
    
    std::stringstream ss(line);
    int number;
    
    while (ss >> number) {
        numbers.insert(number);
    }

    std::set<std::string> finalCategories;

    try {
        auto categories = m_data.get_categories();
        std::vector<std::string> categories_vec(categories.begin(), categories.end());

        int size = static_cast<int>(categories_vec.size());
        for (const int& num : numbers) {
            if (num < size) {
                finalCategories.insert(categories_vec[num]);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return finalCategories;
}

/**
 * @brief Determines the game state based on the chosen menu option.
 * @param option The chosen menu option.
 * @return The corresponding game state.
 */
GameController::game_state_e GameController::menu_chosen(menu_e menu_option) {
  switch (menu_option) {
    case menu_e::PLAY:
      return game_state_e::CATEGORY_OPTIONS;
      break;
    case menu_e::RULES:
      return game_state_e::SHOW_RULES;
      break;
    case menu_e::SCORE:
      return game_state_e::SHOW_SCORE;
      break;
    case menu_e::EXIT:
      return game_state_e::QUITTING;
      break;
    case menu_e::UNDEFINED:
      return game_state_e::MAIN_MENU;
      break;
    default: game_state_e::MAIN_MENU;
  }
}

/**
 * @brief Determines the current match status of the game.
 * 
 * This function checks the number of wrong guesses and the current state of the masked word 
 * to determine if the player has won, lost, or if the match is still ongoing.
 * 
 * @return GameController::match_e The current status of the match:
 *         - match_e::PLAYER_LOST: The player has made the maximum number of wrong guesses.
 *         - match_e::PLAYER_WON: The player has successfully guessed the entire secret word.
 *         - match_e::ON: The match is still ongoing.
 */
GameController::match_e GameController::match_status() const {
  if(m_curr_word.get_wrong_guesses().size() ==m_max_mistakes){
      return match_e::PLAYER_LOST;
    }
    if(m_curr_word.get_masked_word()==m_curr_word.secret_word()){
      return match_e::PLAYER_WON;
    }
    return match_e::ON;
}
/**
 * @brief Changes the current game state.
 * @param new_state The new game state to switch to.
 */
void GameController::change_state(game_state_e new_state) {
  if(!(m_game_state == game_state_e::PLAYING)){
  clear_screen();
  }
  m_game_state = new_state;
}

/**
 * @brief Displays various game screens.
 * 
 * These functions handle the output of different game screens, including the welcome screen,
 * main menu, category options, difficulty options, and the play screen. Each function is responsible
 * for showing relevant information and interaction messages to the player based on the current game state.
 */
void GameController::display_welcome() const {
    std::cout<< "---> Welcome to Hangman, v 1.0 <---" << std::endl;
    std::cout<< "       -copyright UFRN 2024-       " << std::endl;
    show_interaction_msg();
}

void GameController::display_main_menu() const {
    m_system_msg = "MENU";
    show_screen_title(m_system_msg);
    show_main_content_msg();
    show_interaction_msg();
}

void GameController::display_category_options() const {
    m_system_msg = "NEW GAME";
    show_screen_title(m_system_msg);
    show_main_content_msg();
    show_interaction_msg();
}

void GameController::display_difficulty_options() const {
    show_main_content_msg();
    show_interaction_msg();
}

void GameController::display_play_screen() const {
    m_system_msg = "GAME";
    show_screen_title(m_system_msg);
    show_main_content_msg();
    show_interaction_msg();
}

void GameController::display_quitting() const {
    m_system_msg = "QUIT";
    show_screen_title(m_system_msg);
    show_interaction_msg();
}

void GameController::display_rules() const {
    m_system_msg = "RULES";
    show_screen_title(m_system_msg);
    show_main_content_msg();
    show_interaction_msg();
}

void GameController::display_endgame() const {
    m_system_msg = "THE END";
    show_screen_title(m_system_msg);
}

void GameController::display_scoreboard() const {
  m_system_msg = "SCORE";
  show_screen_title(m_system_msg);
  print_table();
  show_interaction_msg();
}

void GameController::show_screen_title(const std::string &title) const {
    std::cout<<"=-------["<< title <<"]-------="<<std::endl<<std::endl;
}

void GameController::show_main_content_msg() const {
    switch (m_game_state) {
    case game_state_e::MAIN_MENU:
      std::cout<< "Please choose an option:" << std::endl;
      std::cout<< " 1 - Start a new challenge." << std::endl;
      std::cout<< " 2 - Show the game rules." << std::endl;
      std::cout<< " 3 - Show scoreboard." << std::endl;
      std::cout<< " 4 - Quit the game." << std::endl<< std::endl;
      break;
    case game_state_e::CATEGORY_OPTIONS:
      std::cout<< "Hello, " << this->m_user_name<< "!" << std::endl;
      std::cout<< "Let's start a new game, shall we?" << std::endl<< std::endl;
      std::cout<< "These are the avaliable categories:" << std::endl<< std::endl;

      m_data.print_categories(true);
      
      std::cout<< "You can choose one or more categories"<< std::endl;
      std::cout<< "P.S.: If there aren't words in both categories together the game is gonna have words from the union of them" << std::endl<< std::endl;
      break;

    case game_state_e::DIFFICULTY_OPTIONS:
      std::cout<< "Now, let's choose the level of the word" << std::endl<< std::endl;
      std::cout<< "These are the avaliable levels:" << std::endl<< std::endl;
      std::cout<< "0- Easy" << std::endl << "1- Medium" << std::endl << "2- Hard"<< std::endl;
      std::cout<< "You can only choose one option "<< std::endl;
      break;
    case game_state_e::PLAYING:
      std::cout << "Difficulty: " << get_difficulty() << std::endl;

      std::cout << "Categories:\n";
      m_data.print_categories(0);

      display_gallows(m_curr_word.wrong_guesses(), get_difficulty());

      for (const auto& letter : m_curr_word.get_masked_word()){
        std::cout << char(letter) << " ";
       }
      std::cout << std::endl<<std::endl;
      std::cout<< "Wrong guesses so far: < ";
      for (const auto& letter : m_curr_word.get_wrong_guesses()) {
          std::cout << char(letter) << " ";
      }
      std::cout << ">" << std::endl;
      break;
    case game_state_e::SHOW_RULES:
      std::cout<< "Hi "<< this->m_user_name <<", here are the game rules:" << std::endl;
      std::cout<< " [1] You need to guess the secret word or phrase the game has chosen by suggesting letters." << std::endl;
      std::cout<< " [2] We will display a row of dashes, representing each letter of the secret word/phrase you're trying to guess." << std::endl;
      std::cout<< " [3] Each game you win earns you X points depending on the level of the game." << std::endl;
      std::cout<< " [4] Each wrong guess you make I draw on component of a hanged stick figure (the hangman!)" << std::endl;
      std::cout<< " [5] If you wrong guess X times (10 on easy mode, 8 on medium mode and 6 on hard mode) you loose the challenge" << std::endl;
      std::cout<< " [7] After a guessing round (challenge) is complete you may try another secret word/phrase or quit the game." << std::endl<< std::endl;
      break;
    case game_state_e::ENDING:
      std::cout<< "Thanks for playing! See you next time!";
      break;
    }
}

void GameController::show_interaction_msg() const {
    switch (m_game_state) {
    case game_state_e::WELCOME:
      std::cout<<"Please, enter your name >";
      break;
    case game_state_e::MAIN_MENU:
      std::cout<< "Enter your option number and hit 'Enter' >";
      break;
    case game_state_e::CATEGORY_OPTIONS:
      std::cout<< "Are you ready for this challenge? Write anything and hit 'Enter':";
      break;
    case game_state_e::DIFFICULTY_OPTIONS:
      std::cout<< "Enter your option number and hit 'Enter' >";
      break;
    case game_state_e::PLAYING:
      std::cout<<"Guess a letter [enter '#' to quit] >";
      break;

    case game_state_e::FINISH_GAME:
      std::cout<< "Would you like to go back to MENU? Hit 'Enter' to continue >";
      break;
    case game_state_e::SHOW_RULES:
      std::cout<< "Would you like to go back to MENU? Hit 'Enter' to continue >";
      break;
    case game_state_e::SHOW_SCORE:
      std::cout<< "Would you like to go back to MENU? Hit 'Enter' to continue >";
      break;
    case game_state_e::QUITTING:
      std::cout<< "Are you sure you wanna quit the game? (Your score is gonna be saved under your current username) (y/n) >";
      break;
    }
}

/*
  ____
  |  |
  |
  |
__|__
_ _ _ 

  ____
  |  |
  |  O
__|__
_ _ _ 

  ____
  |  |           easy
  |  O
  |_/|\_ 
  |_/ \_
__|__
u _ _ 

  ____
  |  |
  |  O          medium
  |_/|\_ 
  | / \
__|__
_ e _ _ e _ _ _ _ 

  ____
  |  |
  |  O
  | /|\ 
  | / \
__|__
_ _ _ _ _ _ h _ 


*/

void GameController::display_gallows2(short wrongGuesses, char difficulty) const{
  short bodyHeight = 0;
  if(wrongGuesses == 1){
    bodyHeight = 1;
  }else if(wrongGuesses > 4){
    bodyHeight = 3;
  }else{
    bodyHeight = 2;
  }
  std::cout << "  ____" << std::endl;
  std::cout << "  |  |" << std::endl;

  display_gallows_helper(wrongGuesses);

  for(short ii = 0; ii < (3 - bodyHeight); ii++){
    std::cout << "  |" << std::endl;
  }
  std::cout << "__|__" << std::endl;
}


void GameController::display_gallows_helper(short wrongGuesses) const{
  if(wrongGuesses >= 1){ std::cout << "  |  O" << std::endl;}

  if (wrongGuesses == 2){std::cout << "  |  |" << std::endl;}
  if (wrongGuesses == 3){std::cout << "  | /|" << std::endl;}
  if (wrongGuesses == 4){std::cout << "  | /|\\ " << std::endl;}
  
  if (wrongGuesses == 5){std::cout << "  | /" << std::endl;}
  if (wrongGuesses >= 6){std::cout << "  | / \\" << std::endl;}
  
}

void GameController::display_gallows(short wrongGuesses, char difficulty) const {
  short bodyHeight = 0;
  if(wrongGuesses > 0){
    if (wrongGuesses == 1) {
        bodyHeight = 1;
    } else if (difficulty != 'H') {
        bodyHeight = (wrongGuesses >= 7) ? 3 : 2;
    } else {
        bodyHeight = (wrongGuesses >= 5) ? 3 : 2;
    }
  }
  
  std::cout << bodyHeight << std::endl;
  std::cout << "  ____" << std::endl;
  std::cout << "  |  |" << std::endl;
  if (wrongGuesses >= 1) {
      std::cout << "  |  O" << std::endl;
          if (wrongGuesses == 2){std::cout << "  |  |" << std::endl;}
          if (wrongGuesses == 3){std::cout << "  | /|" << std::endl;}
      switch (difficulty){
        case 'E':
          if (wrongGuesses == 4){std::cout << "  | /|\\ " << std::endl;}
          if (wrongGuesses == 5){std::cout << "  |_/|\\ " << std::endl;}
          if (wrongGuesses >= 6){std::cout << "  |_/|\\_ " << std::endl;}
          if (wrongGuesses == 7){std::cout << "  | /" << std::endl;}
          if (wrongGuesses == 8){std::cout << "  | / \\" << std::endl;}
          if (wrongGuesses == 9){std::cout << "  |_/ \\" << std::endl;}
          if (wrongGuesses >= 10){std::cout << "  |_/ \\_" << std::endl;}
        break;
        case 'M':
          if (wrongGuesses == 4){std::cout << "  | /|\\ " << std::endl;}
          if (wrongGuesses == 5){std::cout << "  |_/|\\ " << std::endl;}
          if (wrongGuesses >= 6){std::cout << "  |_/|\\_ " << std::endl;}
          if (wrongGuesses == 7){std::cout << "  | /" << std::endl;}
          if (wrongGuesses == 8){std::cout << "  | / \\" << std::endl;}
        break;
        case 'H':
          if (wrongGuesses >= 4){std::cout << "  | /|\\ " << std::endl;}
          if (wrongGuesses == 5){std::cout << "  | /" << std::endl;}
          if (wrongGuesses >= 6){std::cout << "  | / \\" << std::endl;}
        break;
      }
  } 
  for(short ii = 0; ii < (3 - bodyHeight); ii++){
    std::cout << "  |" << std::endl;
  }
  std::cout << "__|__" << std::endl;

}

