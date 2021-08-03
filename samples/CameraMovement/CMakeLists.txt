set(SAMPLE_NAME CameraMovement)

file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(${SAMPLE_NAME} ${SOURCE_FILES})
target_link_libraries(${SAMPLE_NAME} Core ${CONAN_LIBS})
