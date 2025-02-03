# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.26)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_definitions(-DUNICODE -D_UNICODE)

set(SCRIBBLEMD_PLATFORM "unknown" CACHE STRING "Target platform")

if("${SCRIBBLEMD_PLATFORM}" STREQUAL "windows")
    SET(OPTIONS WIN32)
endif()

project ("MarkdownEditor")
add_executable(MarkdownEditor ${OPTIONS})

find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base html)
find_package(cppMarkdown)
find_package(wxMarkdownCtrl)

set_property(TARGET MarkdownEditor PROPERTY CXX_STANDARD 17)

target_link_libraries(MarkdownEditor PUBLIC wx::core wx::base wx::html)
target_link_libraries(MarkdownEditor PUBLIC cppMarkdown::cppMarkdown)
target_link_libraries(MarkdownEditor PUBLIC wxMarkdownCtrl::wxMarkdownCtrl)

# Copy DLLs to exe directory
add_custom_command(TARGET MarkdownEditor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:MarkdownEditor> $<TARGET_RUNTIME_DLLS:MarkdownEditor>
  COMMAND_EXPAND_LISTS
)

# Copy res directory to exe directory
add_custom_command(
        TARGET MarkdownEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/res
                ${CMAKE_CURRENT_BINARY_DIR}/res
)

add_subdirectory("src")