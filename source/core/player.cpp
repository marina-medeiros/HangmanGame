/*!
 * Player class implementation.
 *
 * \date May, 2024
 * \file player.cpp
 */

#include "player.h"
// === Auxiliary functions to help user input

// === Regular methods of the Player class.

std::string Player::name() const { return m_name; }

void Player::name(const std::string &n) { m_name = n; }

void Player::add_score(int s) { m_score += s; stringScore = std::to_string(m_score); }

int Player::score() const { return m_score; }

void Player::add_word(const std::string &w) { m_played_words.insert(w); }

bool Player::has_played(const std::string &w) const {
  return m_played_words.count(w) != 0;
}

void Player::clear_word_list() { m_played_words.clear(); }