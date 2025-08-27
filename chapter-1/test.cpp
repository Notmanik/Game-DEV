#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Texture Example");

    // Load a texture from a file and check for success
    sf::Texture texture;
    if (!texture.loadFromFile("./images.png"))
        return -1; // Handle error appropriately

    // Create a sprite using the texture
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(400.f, 300.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
