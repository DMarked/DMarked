set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(QT_MAJOR_VERSION 5)

set(QT_COMPONENTS Core Gui Widgets WebEngineWidgets WebChannel Svg Xml)
find_package("Qt${QT_MAJOR_VERSION}" COMPONENTS ${QT_COMPONENTS} REQUIRED)
find_package("Qt${QT_MAJOR_VERSION}LinguistTools" REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Dtk REQUIRED IMPORTED_TARGET dtkcore dtkgui dtkwidget)

set(QT_LIBRARIES "")
foreach(temp ${QT_COMPONENTS})
    list(APPEND QT_LIBRARIES "Qt${QT_MAJOR_VERSION}::${temp}")
endforeach()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.h *.ui)
set(RCC_SOURCES translations.qrc ../resources/markdowneditor.qrc)

file(GLOB TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/translations/*.ts")
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/translations)
qt_create_translation(QM_FILES ${TS_FILES})


add_executable(${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${RCC_SOURCES} ${QM_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::Dtk)

if (UES_VENDORED_FAKEVIM)
    target_link_libraries(${PROJECT_NAME} PRIVATE fakevim)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/FakeVim/fakevim
                                              ${CMAKE_BINARY_DIR}/3rdparty/FakeVim)    
else()
    pkg_check_modules(FakeVim REQUIRED IMPORTED_TARGET fakevim)
    target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::FakeVim)
endif() # UES_VENDORED_FAKEVIM

if (UES_VENDORED_QMARKDOWNTEXTEDIT)
    target_link_libraries(${PROJECT_NAME} PRIVATE qmarkdowntextedit)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/qmarkdowntextedit)
else()
    pkg_check_modules(QMarkdownTextedit REQUIRED IMPORTED_TARGET QMarkdownTextedit)
    target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::QMarkdownTextedit)
endif() #UES_VENDORED_QMARKDOWNTEXTEDIT

## install
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_SOURCE_DIR}/resources/images/dmarked.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
install(FILES ${CMAKE_SOURCE_DIR}/dmarked.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(DIRECTORY ${CMAKE_BINARY_DIR}/translations DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})
