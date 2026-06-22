add_executable(
    AardwolfGame
    main.cpp
)

target_link_libraries(
    AardwolfGame
    PRIVATE
        Aardwolf::Core
        SFML::Graphics
)

set_target_properties(
    AardwolfGame
    PROPERTIES
        OUTPUT_NAME "Aardwolf"
)

aardwolf_configure_target(AardwolfGame)