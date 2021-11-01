#ifndef OBSTACLES_HPP_
#define OBSTACLES_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "ship.hpp"

class OpenGLWindow;

class Obstacles {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(Ship &ship, const GameData &gameData, float deltaTime, int level);
  
 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  struct Obstacle {
    bool m_dead{false};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
    float m_rotation{};
    float m_scale{0.18f};
  };

  float m_scale{0.18f};
  int m_delay{1500};
  int timer;

  std::list<Obstacle> m_obstacles;
  std::default_random_engine m_randomEngine;
  abcg::ElapsedTimer m_obstaclesTimer;
};

#endif