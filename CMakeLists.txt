cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

set(MYNAME vtkQmlDynamicSplitters)

project(${MYNAME})

find_package(VTK)

if (NOT VTK_FOUND OR VTK_VERSION VERSION_LESS "9.0.0")
  message("Skipping ${MYNAME}: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

set(QT_DIR ${Qt5_DIR})

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)
#find_package(Qt5QuickCompiler QUIET)
#if(Qt5QuickCompiler_FOUND AND NOT CMAKE_BUILD_TYPE MATCHES Debug AND NOT CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
#  QTQUICK_COMPILER_ADD_RESOURCES(RESOURCES qml.qrc)
#else()
#  set(RESOURCES qml.qrc)
#endif()

get_filename_component(QT_TOP "../../.." 
  REALPATH BASE_DIR "${Qt5_DIR}" CACHE
)

message (STATUS "QT_VERSION: Qt${QT_VERSION_MAJOR}")
message (STATUS "QT_TOP: ${QT_TOP}")
message (STATUS "Qt5_DIR: ${Qt5_DIR}")
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
message (STATUS "VTK_DIR: ${VTK_DIR}")

add_executable(${MYNAME}
  QQmlListProperty_impl.h
  main.cpp
  main.qml
  DataStore.cpp
  DataStore.h
  qml.qrc
  VtkSource.h
  VtkSource.cpp
  quickVtkObject.h
  quickVtkObject.cpp
  quickVtkProp.h
  quickVtkProp.cpp
  quickVtkProp3D.h
  quickVtkProp3D.cpp
  quickVtkActor.h
  quickVtkActor.cpp  
  quickVtkAlgorithm.h
  quickVtkAlgorithm.cpp
  quickVtkAbstractMapper.h
  quickVtkAbstractMapper.cpp
  quickVtkAbstractMapper3D.h
  quickVtkAbstractMapper3D.cpp
  quickVtkMapper.h
  quickVtkMapper.cpp
  quickVtkPolyDataAlgorithm.h
  quickVtkPolyDataAlgorithm.cpp
  quickVtkPolyDataMapper.h
  quickVtkPolyDataMapper.cpp
  quickVtkDispatcher.h
  quickVtkDispatcher.cpp
  quickVtkViewer.cpp
  quickVtkViewer.h
  quickVtkCone.h
  quickVtkCone.cpp
  quickVtkConeSource.h
  quickVtkConeSource.cpp
  quickVtkImplicitFunction.h
  quickVtkImplicitFunction.cpp
  qt-qml-models/QQmlVariantListModel.cpp
  qt-qml-models/QQmlVariantListModel.h
  qt-qml-models/QQmlObjectListModel.cpp
  qt-qml-models/QQmlObjectListModel.h
  qt-qml-models/QtQmlTricksPlugin_SmartDataModels.h
  dynamicsplitview/DynamicSplitView.qml
  )

target_include_directories(${MYNAME} PRIVATE
    qt-qml-models
)

target_link_libraries(${MYNAME} PRIVATE ${VTK_LIBRARIES})
# vtk_module_autoinit is needed
vtk_module_autoinit(
TARGETS ${MYNAME}
MODULES ${VTK_LIBRARIES}
)
target_compile_definitions(${MYNAME} PRIVATE 
  $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(${MYNAME} PRIVATE 
  Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
)

if(WIN32)
    set_target_properties(${MYNAME} PROPERTIES 
      VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${VTK_DIR}/Bin/Debug;${QT_TOP}/bin;${QT_TOP}/plugins/platforms"
    )
    set_target_properties(${MYNAME} PROPERTIES 
      LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
      LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY 
      VS_STARTUP_PROJECT ${MYNAME}
    )
endif()    