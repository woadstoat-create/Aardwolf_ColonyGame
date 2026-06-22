option(
    COLONY_WARNINGS_AS_ERRORS
    "Treat compiler warnings as errors"
    OFF
)

option(
    COLONY_ENABLE_CLANG_TIDY
    "Run clang-tidy while compiling project targets"
    OFF
)

if(COLONY_ENABLE_CLANG_TIDY)
    find_program(
        COLONY_CLANG_TIDY_EXECUTABLE
        NAMES clang-tidy
        REQUIRED
    )
endif()

function(colony_configure_target target_name)
    target_compile_features(
        ${target_name}
        PRIVATE
            cxx_std_20
    )

    set_target_properties(
        ${target_name}
        PROPERTIES
            CXX_EXTENSIONS OFF
    )

    if(MSVC)
        target_compile_options(
            ${target_name}
            PRIVATE
                /W4
                /permissive-
                /Zc:preprocessor
                /Zc:__cplusplus
                /utf-8
        )

        if(COLONY_WARNINGS_AS_ERRORS)
            target_compile_options(
                ${target_name}
                PRIVATE
                    /WX
            )
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(
            ${target_name}
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion
                -Wsign-conversion
                -Wshadow
        )

        if(COLONY_WARNINGS_AS_ERRORS)
            target_compile_options(
                ${target_name}
                PRIVATE
                    -Werror
            )
        endif()
    endif()

    if(COLONY_ENABLE_CLANG_TIDY)
        set_target_properties(
            ${target_name}
            PROPERTIES
                CXX_CLANG_TIDY "${COLONY_CLANG_TIDY_EXECUTABLE}"
        )
    endif()
endfunction()