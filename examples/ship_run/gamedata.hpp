#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Up };
enum class State { Playing, GameOver };

struct GameData {
  State m_state{State::Playing};
  std::bitset<1> m_input;
};

#endif