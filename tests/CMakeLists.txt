add_executable(
    AardwolfTests
    core/BuildInfoTests.cpp
)

target_link_libraries(
    AardwolfTests
    PRIVATE
        Aardwolf::Core
        Catch2::Catch2WithMain
)

colony_configure_target(AardwolfTests)

include(Catch)

catch_discover_tests(AardwolfTests)