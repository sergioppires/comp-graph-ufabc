#include "obstacles.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Obstacles::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_obstacles.clear();

  // Create regular polygon
  auto sides{3};

  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  auto step{M_PI * 2 / sides};
  for (auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Obstacles::paintGL() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);
  abcg::glUniform4f(m_colorLoc, 1, 1, 1, 1);
  abcg::glUniform1f(m_rotationLoc, 0);
  abcg::glUniform1f(m_scaleLoc, m_scale);

  for (auto &obstacle : m_obstacles) {
    abcg::glUniform2f(m_translationLoc, obstacle.m_translation.x,
                      obstacle.m_translation.y);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 62);
  }

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);

  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);
}

void Obstacles::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Obstacles::update(Ship &ship, const GameData &gameData, float deltaTime,
                       int level) {
  switch (level) {
    case 1:
      m_delay = 1500;
      break;
    case 2:
      m_delay = 1000;
      break;
    case 3:
      m_delay = 800;
      break;
    case 4:
      m_delay = 500;
      break;
    default:
      m_delay = 1500;
      break;
  }

  if (gameData.m_state == State::Playing) {
    if (m_obstaclesTimer.elapsed() > m_delay / 1000.0) {
      m_obstaclesTimer.restart();

      glm::vec2 forward{glm::rotate(glm::vec2{0.0f, 1.0f}, 4.712f)};
      auto Speed{2.0f};

      std::uniform_real_distribution<float> rd1(-1.0f, 1.0f);

      Obstacle obstacle{.m_dead = false,
                        .m_translation = glm::vec2(1.0f, rd1(m_randomEngine)),
                        .m_velocity = ship.getVelocity() + forward * Speed,
                        .m_rotation = ship.getRotation()};

      m_obstacles.push_back(obstacle);
    }
  }

  for (auto &obstacle : m_obstacles) {
    obstacle.m_translation.x -= 2.0f * deltaTime;

    // Kill bullet if it goes off screen
    if (obstacle.m_translation.x < -1.1f) {
      obstacle.m_dead = true;
    }
    if (obstacle.m_translation.x > +1.1f) {
      obstacle.m_dead = true;
    }
    if (obstacle.m_translation.y < -1.1f) {
      obstacle.m_dead = true;
    }
    if (obstacle.m_translation.y > +1.1f) {
      obstacle.m_dead = true;
    }
  }

  // Remove dead bullets
  m_obstacles.remove_if([&](const Obstacle &obs) { return obs.m_dead; });
}