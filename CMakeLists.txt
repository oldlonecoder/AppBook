#******************************************************************************************
#*   Copyright (C) 1965/.../2023 by Serge Lussier                                        *
#*   lussier.serge@gmail.com                                                              *
#*                                                                                        *
#*                                                                                        *
#*   Unless otherwise specified, all Code IsIn this project is written                    *
#*   by the author (Serge Lussier)                                                        *
#*   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
#*   ----------------------------------------------------------------------------------   *
#*   Copyrights from authors other than Serge Lussier also apply here                     *
#*   Open source FREE licences also apply To the Code from the same author (Serge Lussier)*
#*   ----------------------------------------------------------------------------------   *
#******************************************************************************************/



cmake_minimum_required(VERSION 3.13..3.26 FATAL_ERROR)
## Project name

project(AppBook VERSION 0.1.0 LANGUAGES CXX)

message("Entering ${PROJECT_NAME} project configuration:")

include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(TargetName ${PROJECT_NAME})

SET(FRAMEWORK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(

    ${TargetName}
    SHARED
    include/${TargetName}/Exports.h                              #global framework-wide macros definitions and dll export and import macros for MSVC.
    include/${TargetName}/Util/StrAcc.h src/Util/StrAcc.cc
    include/AppBook/Util/Colors.h src/Util/Colors.cc
    include/AppBook/Utf/Glyphes.h src/Utf/Glyphes.cc
    include/AppBook/Util/StrBreak.h src/Util/StrBreak.cc
    include/AppBook/Book/BookEnums.h src/Book/BookEnums.cc
    include/AppBook/Util/Object.h src/Util/Object.cc
    include/AppBook/Util/Geometry.h src/Util/Geometry.cc
    include/AppBook/Book/StmlText.h src/Book/StmlText.cc
    include/AppBook/Book/AppBook.h src/Book/AppBook.cc src/Book/Book.Section.cc src/Book/Book.Section.Contents.cc src/Book/Book.Section.Contents.Element.cc
    include/AppBook/Util/Delegate.h src/Util/Delegate.cc
    include/AppBook/Util/CArgs.h src/Util/CArgs.cc
    include/AppBook/Book/Expect.h
    include/AppBook/Book/TScanner.h src/Book/TScanner.cxx
    include/AppBook/Book/ApplicationBase.h src/Book/ApplicationBase.cxx
    include/AppBook/Utf/Cadres.h src/Utf/Cadres.cxx
    #include/AppBook/ConsoleUI/Window.h src/ConsoleUI/Window.cxx src/ConsoleUI/CharBBB.cxx src/ConsoleUI/Pencil.cxx
    include/AppBook/ConsoleUI/UiEnums.h
    #include/AppBook/ConsoleUI/Console.h src/ConsoleUI/Console.cxx
    #include/AppBook/ConsoleUI/Screen.cxx
    #include/AppBook/ConsoleUI/Screen.h
    include/AppBook/ConsoleUI/Char.h                            src/ConsoleUI/Char.cxx
    include/AppBook/ConsoleUI/UiElement.h                       src/ConsoleUI/UiElement.cxx



)


target_compile_definitions(${TargetName} PUBLIC "${TargetName}_DEBUG=$<CONFIG:Debug>")
target_compile_features(${TargetName} PUBLIC cxx_std_20)

include(GenerateExportHeader)
generate_export_header(${TargetName} EXPORT_FILE_NAME ${${TargetName}_export})

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            )
ENDIF()

target_include_directories(${TargetName} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        $<INSTALL_INTERFACE:include/${TargetName}>
        )

target_link_libraries(${TargetName} ${CMAKE_DL_LIBS} sqlite3)


install(DIRECTORY
        include/${TargetName}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${TargetName}"
)

install(TARGETS ${TargetName}
        EXPORT ${TargetName}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)


add_executable(Main.App Main.cxx)
target_link_libraries(Main.App AppBook)



#add_subdirectory(Test)
