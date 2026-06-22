add_library(
    AardwolfCore
    STATIC
        src/BuildInfo.cpp
        src/world/Chunk.cpp
        src/world/World.cpp
)

add_library(Aardwolf::Core ALIAS AardwolfCore)

target_include_directories(
    AardwolfCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(
    AardwolfCore
    PUBLIC
        cxx_std_20
)

target_compile_definitions(
    AardwolfCore
    PRIVATE
        AARDWOLF_PROJECT_NAME="${PROJECT_NAME}"
        AARDWOLF_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        AARDWOLF_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        AARDWOLF_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

aardwolf_configure_target(AardwolfCore)