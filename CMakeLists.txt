# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.5)

# 定义项目名称和版本号，以及使用的编程语言
project(ComBridge VERSION 0.1 LANGUAGES CXX)

# 设置自动化的UIC（用户界面编译器）、MOC（元对象编译器）和RCC（资源编译器）选项
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置C++标准为C++17，并且为必需项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找所需的Qt组件
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialBus)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialBus)

# 设置项目源文件
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        myserialport.h
        myserialport.cpp
)

# 根据Qt的版本不同，使用不同的宏来添加可执行文件
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ComBridge
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        logo.rc  # 添加资源文件
    )
else()
    if(ANDROID)
        add_library(ComBridge SHARED
            ${PROJECT_SOURCES}
            logo.rc  # 添加资源文件
        )
    else()
        add_executable(ComBridge WIN32
            ${PROJECT_SOURCES}
            logo.rc  # 添加资源文件
        )
    endif()
endif()

# 链接所需的Qt库
target_link_libraries(ComBridge PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialBus)

# 设置macOS捆绑包标识符，仅在Qt版本小于6.1.0时设置
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ComBridge)
endif()

# 设置目标属性，包括macOS和Windows平台的配置
set_target_properties(ComBridge PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/ComBridge
)

# 如果是Windows平台，设置图标
if(WIN32)
    set(APP_ICON "${CMAKE_SOURCE_DIR}/ComBridge.ico")
    set_target_properties(ComBridge PROPERTIES WIN32_EXECUTABLE TRUE)
    add_custom_command(TARGET ComBridge POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${APP_ICON} $<TARGET_FILE_DIR:ComBridge>)
endif()

# 设置安装目标和安装目录
include(GNUInstallDirs)
install(TARGETS ComBridge
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 如果是Qt 6版本，最终确定可执行文件的设置
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ComBridge)
endif()
