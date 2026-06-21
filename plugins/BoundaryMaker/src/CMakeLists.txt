INCLUDE_DIRECTORIES(
     .
     ${CMAKE_BINARY_DIR}/plugins/BoundaryMaker/src
)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

SET(BoundaryMaker_SRCS
     BoundaryMaker.hpp
     BoundaryMaker.cpp
     BoundaryMakerDialog.hpp
     BoundaryMakerDialog.cpp
)

ADD_LIBRARY(BoundaryMaker-static STATIC ${BoundaryMaker_SRCS})
SET_TARGET_PROPERTIES(BoundaryMaker-static PROPERTIES OUTPUT_NAME "BoundaryMaker")

TARGET_LINK_LIBRARIES(BoundaryMaker-static Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
SET_TARGET_PROPERTIES(BoundaryMaker-static PROPERTIES COMPILE_FLAGS "-DQT_STATICPLUGIN")
ADD_DEPENDENCIES(AllStaticPlugins BoundaryMaker-static)

SET_TARGET_PROPERTIES(BoundaryMaker-static PROPERTIES FOLDER "plugins/BoundaryMaker")
