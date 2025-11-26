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
    RenderingVolume
    RenderingVolumeOpenGL2
    RenderingAnnotation
)

# Set source files
set(SOURCES
    src/main.cpp
    mainwindow.cpp
    examples/Example.cpp
    examples/ExampleRegistry.cpp
    examples/occ/OccBoxExample.cpp
    examples/vtk/VtkDataLoaderExample.cpp
    examples/vtk/VtkPrimitivesExample.cpp
    examples/vtk/VtkLightingExample.cpp
    examples/vtk/VtkViewportExample.cpp
    examples/vtk/VtkVolumeRenderingExample.cpp
    examples/vtk/VtkPointsExample.cpp
    examples/vtk/VtkScalarBarExample.cpp
    examples/vtk/VtkLookupTableExample.cpp
)

set(HEADERS
    global_headers.h
    mainwindow.h
    examples/Example.h
    examples/ExampleRegistry.h
    examples/occ/OccBoxExample.h
    examples/vtk/VtkDataLoaderExample.h
    examples/vtk/VtkPrimitivesExample.h
    examples/vtk/VtkLightingExample.h
    examples/vtk/VtkViewportExample.h
    examples/vtk/VtkVolumeRenderingExample.h
    examples/vtk/VtkPointsExample.h
    examples/vtk/VtkScalarBarExample.h
    examples/vtk/VtkLookupTableExample.h
)


# Add executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    mainwindow.ui
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
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
