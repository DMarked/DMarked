
if (UES_VENDORED_QMARKDOWNTEXTEDIT)
    option(QMARKDOWNTEXTEDIT_EXE "Build test executable" OFF)
    add_subdirectory(qmarkdowntextedit)
endif()

if (UES_VENDORED_FAKEVIM)
    option(CREATE_STATIC_LIBRARY "Create static library" ON)
    add_subdirectory(FakeVim)
endif()
