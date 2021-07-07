file(GLOB_RECURSE HEADER_FILES *.hpp)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(Engine ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(Engine ${CONAN_LIBS})
