cmake_minimum_required(VERSION 3.16)
project(occvtkex VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(VTK 9.3 REQUIRED COMPONENTS
    FiltersCore
    CommonDataModel
    CommonExecutionModel
    CommonColor
    CommonCore
    FiltersSources
    InteractionStyle
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
    GUISupportQt
    IOLegacy
)

# Set source files
set(SOURCES
    src/main.cpp
)

set(HEADERS
    global_headers.h
)


# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}
    mainwindow.h mainwindow.cpp mainwindow.ui
    )

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    /opt/opencascade-7.9.1/include/opencascade
    /usr/local/include/vtk-9.3
)

# Link libraries
target_link_directories(${PROJECT_NAME} PRIVATE /opt/opencascade-7.9.1/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${VTK_LIBRARIES}
    # OpenCASCADE Core libraries
    TKernel
    TKMath
    TKG2d
    TKG3d
    TKGeomBase
    TKBRep
    TKGeomAlgo
    TKTopAlgo
    TKPrim
    TKBO
    TKBool
    TKFillet
    TKOffset
    # OpenCASCADE Visualization
    TKV3d
    TKService
    # OpenCASCADE-VTK Integration
    TKIVtk
)

# VTK autoinit
vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)

# Set install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
