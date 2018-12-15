cmake_minimum_required(VERSION 3.4)
project(OpenLarynx)

FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE( OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer osgText)

INCLUDE_DIRECTORIES( ${OPENSCENEGRAPH_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 1)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)


IF (NOT TARGET gtest)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(libs/googletest)

ENDIF()


SET(MYSOURCE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindowform.ui
    osgwidget.h
    osgwidget.cpp
    OSGLarynx.cpp
    OSGLarynx.h
    ThyroidUpdateCallback.h
    ArytenoidUpdateCallback.h
    CricoidUpdateCallback.h
    AxisUpdateCallback.h
    )

set(MYRESOURCES icons/icons.qrc)

set(TEST_NAME OpenLarynx_UnitTest)

add_executable(${PROJECT_NAME}
    ${MYSOURCE}
    ${MYRESOURCES}
    )

add_executable(Testing
    unitTest.cpp
    OSGLarynx.cpp
    OSGLarynx.h
    )

target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}	
    Qt5::Widgets
    Qt5::Gui
    )


target_link_libraries(Testing
    ${OPENSCENEGRAPH_LIBRARIES}
    Qt5::Widgets
    Qt5::Gui
    gtest
    gtest_main
    )

