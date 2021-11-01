project(ship_run)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp ship.cpp obstacles.cpp)
enable_abcg(${PROJECT_NAME})