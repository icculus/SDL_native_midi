# YOU PROBABLY DON'T HAVE TO USE THIS CMAKE PROJECT, IT'S JUST A FEW SOURCE
#  FILES WITH NO CONFIGURATION. COPY IT INTO YOUR PROJECT INSTEAD.

cmake_minimum_required(VERSION 3.12.0)
project(SDL_native_midi)

find_package(SDL3 REQUIRED)

# Don't add C++ files if not forced to.
if(HAIKU)
   set(SDL_NATIVE_MIDI_EXTRA_SOURCES src/SDL_native_midi_haiku.cpp)
endif()

add_library(SDL_native_midi SHARED
    src/SDL_native_midi_common.c
    src/SDL_native_midi_alsa.c
    src/SDL_native_midi_win32.c
    src/SDL_native_midi_dummy.c
    src/SDL_native_midi_macos.c
    ${SDL_NATIVE_MIDI_EXTRA_SOURCES}
)
target_include_directories(SDL_native_midi PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(SDL_native_midi PRIVATE ${SDL3_LIBRARIES})
set_property(TARGET SDL_native_midi PROPERTY DEFINE_SYMBOL "DLL_EXPORT")
if(WIN32)
    target_link_libraries(SDL_native_midi PRIVATE winmm)
endif()
if(APPLE)
    target_link_libraries(SDL_native_midi PRIVATE -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,CoreServices)
endif()

add_executable(test_sdl_native_midi test/test_sdl_native_midi.c)
target_link_libraries(test_sdl_native_midi PRIVATE SDL_native_midi)
target_link_libraries(test_sdl_native_midi PRIVATE ${SDL3_LIBRARIES})
target_include_directories(test_sdl_native_midi PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(test_sdl_native_midi PRIVATE ${SDL3_INCLUDE_DIRS})

