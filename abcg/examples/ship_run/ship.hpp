#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;
class Obstacles;

class Ship {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);
  void setRotation(float rotation) { m_rotation = rotation; }
  glm::vec2 getVelocity();
  float getRotation();

 private:
  friend Obstacles;
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  float m_rotation{};
  float m_scale{0.125f};
  float m_gravity{};
  float m_gravitySpeed{};

  glm::vec4 m_color{1};

  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec2 m_init{glm::vec2(0)};
  glm::vec2 m_velocity{glm::vec2(0)};

  abcg::ElapsedTimer m_trailBlinkTimer;
};

#endif