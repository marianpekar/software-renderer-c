cmake_minimum_required(VERSION 3.20)
project(software_renderer_c C)

set(CMAKE_C_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG main
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG main
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
)

target_link_libraries(software_renderer_c PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
)

target_include_directories(software_renderer_c PRIVATE
        ${SDL3_SOURCE_DIR}/include
        ${SDL3_image_SOURCE_DIR}/include
)

if (WIN32)
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