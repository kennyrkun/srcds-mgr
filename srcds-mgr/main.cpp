#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/Theme.hpp>
#include <SFUI/SFUI.hpp>

#include <iostream>

int main()
{
	enum Callback
	{
		C_ITEM_NAME,
		C_ITEM_DESCRIPTION,
		C_ITEM_VERSION,
		C_ITEM_RELEASE,
		C_ITEM_FILES,
		C_ITEM_ICON,
		Submit,
		Quit
	};

	// Create the main window
	sf::RenderWindow app(sf::VideoMode(400, 400), "Item Upload v1");

	SFUI::Menu menu(app);
	menu.setPosition(sf::Vector2f(10, 10));

	SFUI::Theme::loadFont("resources/interface/tahoma.ttf");
	SFUI::Theme::loadTexture("resources/interface/texture_square.png");
	SFUI::Theme::fontSize = 11;
	SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb("#000");
	SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb("#000");
	SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb("#000");
	SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb("#000");
	SFUI::Theme::PADDING = 2.f;
	SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb("#dddbde");

	SFUI::HorizontalBoxLayout* hbox = menu.addHorizontalBoxLayout();
	SFUI::FormLayout* form = hbox->addFormLayout();

	form->addLabel("Input item info");

	SFUI::InputBox* textureName = new SFUI::InputBox();
	textureName->setText("resources/texture_");
	form->addRow("Texture", textureName, C_ITEM_NAME);

	SFUI::InputBox* itemName = new SFUI::InputBox();
	form->addRow("Item Name", itemName, C_ITEM_NAME);

	SFUI::InputBox* itemDescription = new SFUI::InputBox();
	form->addRow("Item Description", itemDescription, C_ITEM_DESCRIPTION);

	SFUI::InputBox* itemVersion = new SFUI::InputBox();
	form->addRow("Item Name", itemVersion, C_ITEM_VERSION);

	SFUI::InputBox* itemIcon = new SFUI::InputBox();
	itemIcon->setText("./files/icon.png");
	form->addRow("Item Name", itemIcon, C_ITEM_ICON);

	form->addButton("Submit", Submit);

	while (app.isOpen())
	{
		sf::Event event;
		while (app.pollEvent(event))
		{
			int id = menu.onEvent(event);
			switch (id)
			{
			case Submit:
				std::cout << "doing the do" << std::endl;

				SFUI::Theme::loadTexture(textureName->getText());

				break;
			case Quit:
				app.close();
			}

			if (event.type == sf::Event::Closed)
				app.close();
		}

		app.clear(SFUI::Theme::windowBgColor);

		app.draw(menu);

		app.display();
	}

	return EXIT_SUCCESS;
}
