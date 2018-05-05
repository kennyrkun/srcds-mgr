#include <SFML/Graphics.hpp>

int main(int argc, char* argv[])
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SRCDS-Manager");

	sf::Font arial;
	arial.loadFromFile("C:/Windows/Fonts/Arial.ttf");

	sf::Text helloWorldText;
	helloWorldText.setFont(arial);
	helloWorldText.setString("Hello, world!");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				window.close();
			}
		}

		window.clear();

		window.draw(helloWorldText);

		window.display();
	}
}