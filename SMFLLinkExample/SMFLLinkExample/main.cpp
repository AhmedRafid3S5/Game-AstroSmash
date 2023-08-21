/*#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(800, 600), "SFML works!");
    CircleShape shape(50.f);
    shape.setFillColor(Color::Red);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up && shape.getPosition().y > 0)
                    shape.move(0, -2);
                if (event.key.code == Keyboard::Down && shape.getPosition().y + 2*shape.getRadius() < window.getSize().y)
                    shape.move(0, 2);
                if (event.key.code == Keyboard::Left && shape.getPosition().x>0)
                    shape.move(-2, 0);
                if (event.key.code == Keyboard::Right && shape.getPosition().x + 2*shape.getRadius() < window.getSize().x)
                    shape.move(2, 0);
            }

            if (Mouse::isButtonPressed(Mouse::Left))
                shape.setFillColor(Color::Blue);
            else
                shape.setFillColor(Color::Red);
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}

*/
