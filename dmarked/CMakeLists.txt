set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(QT_MAJOR_VERSION 5)

set(QT_COMPONENTS Core Gui Widgets WebEngineWidgets WebChannel Svg Xml)
find_package("Qt${QT_MAJOR_VERSION}" COMPONENTS ${QT_COMPONENTS} REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Dtk REQUIRED IMPORTED_TARGET dtkcore dtkgui dtkwidget)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.cpp *.h)

set(QT_LIBRARIES "")
foreach(temp ${QT_COMPONENTS})
    list(APPEND QT_LIBRARIES "Qt${QT_MAJOR_VERSION}::${temp}")
endforeach()

pkg_check_modules(3rd_party REQUIRED IMPORTED_TARGET QMarkdownTextedit fakevim)
#find_package()

target_sources(${PROJECT_NAME} PUBLIC ${sources} translations.qrc ../resources/markdowneditor.qrc)
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::Dtk PkgConfig::3rd_party)

