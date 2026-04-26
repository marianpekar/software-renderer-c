cmake_minimum_required(VERSION 3.23)
project(software_renderer_c
    VERSION 1.0.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)

set(SDLIMAGE_VENDORED OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(SDL3_image)

add_executable(software_renderer_c
    main.c
    sdl_gfx.c
    vectors.c
    matrix.c
    camera.c
    mesh.c
    draw.c
    inputs.c
    z_buffer.c
    sort.c
    light.c
    texture.c
    model.c
)

target_link_libraries(software_renderer_c PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

if(WIN32)
    add_custom_command(TARGET software_renderer_c POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:software_renderer_c>
    )
    add_custom_command(TARGET software_renderer_c POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_image::SDL3_image>
            $<TARGET_FILE_DIR:software_renderer_c>
    )
endif()

add_custom_command(TARGET software_renderer_c POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:software_renderer_c>/assets"
)
