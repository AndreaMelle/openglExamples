CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

PROJECT(openglExamples)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_SEARCH_MODULE(GLFW REQUIRED glfw3)
FIND_PACKAGE(GLEW REQUIRED)

SET(SOIL_INCLUDE_DIRS /opt/local/include/SOIL)
FIND_LIBRARY(SOIL_LIBRARY NAMES SOIL PATHS /opt/local/lib )

SET(GLM_INCLUDE_DIRS /usr/local/include/glm)

SET(UTILS ${CMAKE_SOURCE_DIR}/utils)

IF(APPLE)
	#SET(GUI_TYPE MACOSX_BUNDLE)
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(IOKIT_LIBRARY IOKit)
   FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
   FIND_LIBRARY(QUARTZ_LIBRARY QuartzCore)
   MARK_AS_ADVANCED (COCOA_LIBRARY IOKIT_LIBRARY OPENGL_LIBRARY QUARTZ_LIBRARY)
   SET(EXTRA_LIBS ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${OPENGL_LIBRARY} ${QUARTZ_LIBRARY})
ENDIF (APPLE)

ADD_SUBDIRECTORY(contextCreation)
ADD_SUBDIRECTORY(drawingPolygons)
ADD_SUBDIRECTORY(textures)
ADD_SUBDIRECTORY(transformations)
ADD_SUBDIRECTORY(depthAndStencils)
ADD_SUBDIRECTORY(framebuffers)
ADD_SUBDIRECTORY(geometryShaders)
#ADD_SUBDIRECTORY(transformFeedback)