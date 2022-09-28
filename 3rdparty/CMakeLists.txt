
option(QMARKDOWNTEXTEDIT_EXE "Build test executable" OFF)
add_subdirectory(qmarkdowntextedit)

option(CREATE_STATIC_LIBRARY "Create static library" ON)
add_subdirectory(FakeVim)

message("${CMAKE_SOURCE_DIR}/3rdparty/FakeVim/fakevim")