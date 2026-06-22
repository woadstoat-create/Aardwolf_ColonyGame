#include <SFML/Graphics.hpp>
#include <aardwolf/core/BuildInfo.hpp>
#include <optional>
#include <string>

int main()
{
    std::string title{aardwolf::core::projectName()};
    title += " - Milestone 1";

    sf::RenderWindow window(sf::VideoMode({1280u, 720u}), title);

    window.setVerticalSyncEnabled(true);

    sf::CircleShape marker(64.0f);
    marker.setFillColor(sf::Color(90, 180, 120));
    marker.setPosition({576.0f, 296.0f});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color(28, 31, 38));
        window.draw(marker);
        window.display();
    }
}