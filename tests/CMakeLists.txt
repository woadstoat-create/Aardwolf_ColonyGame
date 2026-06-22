add_executable(
    AardwolfTests
    core/BuildInfoTests.cpp
    core/world/ChunkTests.cpp
    core/world/CoordinatesTests.cpp
    core/world/TileTests.cpp
    core/world/WorldTests.cpp
)

target_link_libraries(
    AardwolfTests
    PRIVATE
        Aardwolf::Core
        Catch2::Catch2WithMain
)

aardwolf_configure_target(AardwolfTests)

include(Catch)

catch_discover_tests(AardwolfTests)