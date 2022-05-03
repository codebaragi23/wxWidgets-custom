CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
PROJECT(wxWidgets-custom)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET(TOOLKIT GTK)


# link opencv.
FIND_PACKAGE(OpenCV 4 REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

# link gtk-3
find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})

ADD_DEFINITIONS(-D__WX${TOOLKIT}__)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib/include)

AUX_SOURCE_DIRECTORY(. SRCS)
ADD_EXECUTABLE(${PROJECT_NAME}
  ${SRCS}
)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -L${CMAKE_SOURCE_DIR}/lib 
-lwx_gtk3_aui-3.1 -lwx_gtk3_core-3.1 -lwx_base-3.1
-lgthread-2.0 -pthread -lX11 -lXxf86vm -lSM -lgtk-3 -lgdk-3 -lpangocairo-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lXtst -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lwxregex-3.1 -lwxscintilla-3.1 
-lexpat -lpng -ljpeg -ltiff -ljbig 
-lz -ldl -lm 
)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBRARIES})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin)