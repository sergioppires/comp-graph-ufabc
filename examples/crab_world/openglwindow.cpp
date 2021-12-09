#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "imfilebrowser.h"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (event.type == SDL_MOUSEMOTION) {
    m_trackBallModel.mouseMove(mousePosition);
    m_trackBallLight.mouseMove(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      m_trackBallModel.mousePress(mousePosition);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mousePress(mousePosition);
    }
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      m_trackBallModel.mouseRelease(mousePosition);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mouseRelease(mousePosition);
    }
  }
  if (event.type == SDL_MOUSEWHEEL) {
    m_zoom += (event.wheel.y > 0 ? 1.0f : -1.0f) / 5.0f;
    m_zoom = glm::clamp(m_zoom, -1.5f, 1.0f);
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  // Create programs
  for (const auto &name : m_shaderNames) {
    const auto path{getAssetsPath() + "shaders/" + name};
    const auto program{createProgramFromFile(path + ".vert", path + ".frag")};
    m_programs.push_back(program);
  }

  // Load default model
  loadModel(getAssetsPath() + "crab.obj");

  // Load cubemap
  m_model.loadCubeTexture(getAssetsPath() + "maps/cube/");

  // Initial trackball spin
  m_trackBallModel.setAxis(glm::normalize(glm::vec3(1, 1, 1)));
  m_trackBallModel.setVelocity(0.0001f);

  initializeSkybox();

  for (const auto index : iter::range(m_numCrabs)) {
    auto &position{m_crabsPositions.at(index)};
    auto &rotation{m_crabsRotations.at(index)};

    randomizeCrab(position, rotation);
  }
}

void OpenGLWindow::randomizeCrab(glm::vec3 &position, glm::vec3 &rotation) {
  // Get random position
  // x and y coordinates in the range [-20, 20]
  // z coordinates in the range [-100, 0]
  std::uniform_real_distribution<float> distPosXY(-2.0f, 2.0f);
  std::uniform_real_distribution<float> distPosZ(-50.0f, 0.0f);

  position = glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                       distPosZ(m_randomEngine));

  //  Get random rotation axis
  std::uniform_real_distribution<float> distRotAxis(-1.0f, 1.0f);

  rotation = glm::normalize(glm::vec3(distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine)));
}

void OpenGLWindow::initializeSkybox() {
  // Create skybox program
  const auto path{getAssetsPath() + "shaders/" + m_skyShaderName};
  m_skyProgram = createProgramFromFile(path + ".vert", path + ".frag");

  // Generate VBO
  abcg::glGenBuffers(1, &m_skyVBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyPositions),
                     m_skyPositions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(m_skyProgram, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_skyVAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_skyVAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void OpenGLWindow::loadModel(std::string_view path) {
  m_model.terminateGL();

  m_model.loadDiffuseTexture(getAssetsPath() + "maps/pattern.png");
  m_model.loadNormalTexture(getAssetsPath() + "maps/pattern_normal.png");
  m_model.loadObj(path);
  m_model.setupVAO(m_programs.at(m_currentProgramIndex));
  m_trianglesToDraw = m_model.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Use currently selected program
  const auto program{m_programs.at(m_currentProgramIndex)};
  abcg::glUseProgram(program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(program, "normalTex")};
  const GLint cubeTexLoc{abcg::glGetUniformLocation(program, "cubeTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(program, "mappingMode")};
  const GLint texMatrixLoc{abcg::glGetUniformLocation(program, "texMatrix")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(cubeTexLoc, 2);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  const glm::mat3 texMatrix{m_trackBallLight.getRotation()};
  abcg::glUniformMatrix3fv(texMatrixLoc, 1, GL_TRUE, &texMatrix[0][0]);

  const auto lightDirRotated{m_trackBallLight.getRotation() * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables of the current object
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

  const auto modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
  const glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform1f(shininessLoc, m_shininess);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_model.render(m_trianglesToDraw);

  // Render each crab
  for (const auto index : iter::range(m_numCrabs)) {
    const auto &position{m_crabsPositions.at(index)};
    const auto &rotation{m_crabsRotations.at(index)};

    // Compute model matrix of the current crab
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, rotation);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    m_model.render(m_trianglesToDraw);
  }

  abcg::glUseProgram(0);

  if (m_currentProgramIndex == 0 || m_currentProgramIndex == 1) {
    renderSkybox();
  }
}

void OpenGLWindow::renderSkybox() {
  abcg::glUseProgram(m_skyProgram);

  // Get location of uniform variables
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_skyProgram, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_skyProgram, "projMatrix")};
  const GLint skyTexLoc{abcg::glGetUniformLocation(m_skyProgram, "skyTex")};

  // Set uniform variables
  const auto viewMatrix{m_trackBallLight.getRotation()};
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(skyTexLoc, 0);

  abcg::glBindVertexArray(m_skyVAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, m_model.getCubeTexture());

  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);
  abcg::glDrawArrays(GL_TRIANGLES, 0, m_skyPositions.size());
  abcg::glDepthFunc(GL_LESS);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  // File browser for models
  static ImGui::FileBrowser fileDialogModel;
  fileDialogModel.SetTitle("Load 3D Model");
  fileDialogModel.SetTypeFilters({".obj"});
  fileDialogModel.SetWindowSize(m_viewportWidth * 0.8f,
                                m_viewportHeight * 0.8f);

  // File browser for textures
  static ImGui::FileBrowser fileDialogDiffuseMap;
  fileDialogDiffuseMap.SetTitle("Load Diffuse Map");
  fileDialogDiffuseMap.SetTypeFilters({".jpg", ".png"});
  fileDialogDiffuseMap.SetWindowSize(m_viewportWidth * 0.8f,
                                     m_viewportHeight * 0.8f);

  // File browser for normal maps
  static ImGui::FileBrowser fileDialogNormalMap;
  fileDialogNormalMap.SetTitle("Load Normal Map");
  fileDialogNormalMap.SetTypeFilters({".jpg", ".png"});
  fileDialogNormalMap.SetWindowSize(m_viewportWidth * 0.8f,
                                    m_viewportHeight * 0.8f);

// Only in WebGL
#if defined(__EMSCRIPTEN__)
  fileDialogModel.SetPwd(getAssetsPath());
  fileDialogDiffuseMap.SetPwd(getAssetsPath() + "/maps");
  fileDialogNormalMap.SetPwd(getAssetsPath() + "/maps");
#endif

  // Create main window widget
  {
    auto widgetSize{ImVec2(222, 80)};

    if (!m_model.isUVMapped()) {
      // Add extra space for static text
      widgetSize.y += 26;
    }

    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    auto flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration};
    ImGui::Begin("Widget window", nullptr, flags);

    // Slider will be stretched horizontally
    abcg::glEnable(GL_CULL_FACE);

    // CW/CCW combo box
    { abcg::glFrontFace(GL_CCW); }

    // Projection combo box
    {
      const auto aspect{static_cast<float>(m_viewportWidth) /
                        static_cast<float>(m_viewportHeight)};

      m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 5.0f);
    }

    // Shader combo box
    {
      static std::size_t currentIndex{};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Shader", m_shaderNames.at(currentIndex))) {
        for (auto index : iter::range(m_shaderNames.size())) {
          const bool isSelected{currentIndex == index};
          if (ImGui::Selectable(m_shaderNames.at(index), isSelected))
            currentIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      // Set up VAO if shader program has changed
      if (static_cast<int>(currentIndex) != m_currentProgramIndex) {
        m_currentProgramIndex = currentIndex;
        m_model.setupVAO(m_programs.at(m_currentProgramIndex));
      }
    }

    if (!m_model.isUVMapped()) {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Mesh has no UV coords.");
    }

    ImGui::End();
  }

  fileDialogModel.Display();
  if (fileDialogModel.HasSelected()) {
    loadModel(fileDialogModel.GetSelected().string());
    fileDialogModel.ClearSelected();

    if (m_model.isUVMapped()) {
      // Use mesh texture coordinates if available...
      m_mappingMode = 3;
    } else {
      // ...or triplanar mapping otherwise
      m_mappingMode = 0;
    }
  }

  fileDialogDiffuseMap.Display();
  if (fileDialogDiffuseMap.HasSelected()) {
    m_model.loadDiffuseTexture(fileDialogDiffuseMap.GetSelected().string());
    fileDialogDiffuseMap.ClearSelected();
  }

  fileDialogNormalMap.Display();
  if (fileDialogNormalMap.HasSelected()) {
    m_model.loadNormalTexture(fileDialogNormalMap.GetSelected().string());
    fileDialogNormalMap.ClearSelected();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_trackBallModel.resizeViewport(width, height);
  m_trackBallLight.resizeViewport(width, height);
}

void OpenGLWindow::terminateGL() {
  m_model.terminateGL();
  for (const auto &program : m_programs) {
    abcg::glDeleteProgram(program);
  }
  terminateSkybox();
}

void OpenGLWindow::terminateSkybox() {
  abcg::glDeleteProgram(m_skyProgram);
  abcg::glDeleteBuffers(1, &m_skyVBO);
  abcg::glDeleteVertexArrays(1, &m_skyVAO);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  m_modelMatrix = m_trackBallModel.getRotation();

  m_viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f + m_zoom),
                  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  // Update Crabs
  for (const auto index : iter::range(m_numCrabs)) {
    auto &position{m_crabsPositions.at(index)};
    auto &rotation{m_crabsRotations.at(index)};

    // Z coordinate increases by 10 units per second
    position.z += deltaTime * 1.0f;

    // If this crab is behind the camera, select a new random position and
    // orientation, and move it back to -100
    if (position.z > 0.1f) {
      randomizeCrab(position, rotation);
      position.z = -10.0f;  // Back to -100
    }
  }
}