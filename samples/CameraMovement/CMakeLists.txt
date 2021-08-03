set(SAMPLE_NAME CameraMovement)

add_executable(${SAMPLE_NAME} main.cpp CameraMovement.cpp)
target_link_libraries(${SAMPLE_NAME} Core ${CONAN_LIBS})
