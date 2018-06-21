
#include "Globals.hpp"
#include "Zip.hpp"

#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/Theme.hpp>
#include <SFUI/SFUI.hpp>
#include <SFML\Network\Http.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::experimental::filesystem;

int veriifySteamCMD()
{
	std::cout << "verifying SteamCMD" << std::endl;

	if (!fs::exists(GBL::PATHS::STEAMCMD))
	{
		fs::create_directories(GBL::PATHS::STEAMCMD);
		std::cout << "creating steamcmd directory" << std::endl;
	}

	if (!fs::exists(GBL::PATHS::STEAMCMD + "steamcmd.exe"))
	{
		std::cout << "getting steamcmd" << std::endl;

		// if steamcmd.exe is missing, clear all files from this folder and try again
		for (auto& p : fs::directory_iterator(GBL::PATHS::STEAMCMD))
			fs::remove(p);

		sf::Http http("http://steamcdn-a.akamaihd.net");
		sf::Http::Request request("client/installer/steamcmd.zip");

		std::cout << "connecting to http://steamcdn-a.akamaihd.net" << std::endl;
		sf::Http::Response response = http.sendRequest(request);

		if (response.getStatus() == sf::Http::Response::Status::Ok)
		{
			std::cout << "saving steamcmd.zip" << std::endl;

			std::ofstream saveSteamCMD(GBL::PATHS::STEAMCMD + "steamcmd.zip", std::ios::binary);

			if (saveSteamCMD.is_open())
			{
				saveSteamCMD << response.getBody();

				saveSteamCMD.close();

				if (!saveSteamCMD.good())
					std::cerr << "failed to write to steamcmd.zip" << std::endl;
				else
				{
					Zip zip;
					zip.archivePath = GBL::PATHS::STEAMCMD;
					zip.archiveName = "steamcmd.zip";
					zip.extractFileFromArchive("steamcmd.exe");

					fs::remove(GBL::PATHS::STEAMCMD + "steamcmd.zip");
				}
			}
			else
			{
				std::cerr << "failed to open write stream" << std::endl;
			}
		}
	}
}

int initialise()
{
	std::cout << "initliasing" << std::endl;

	veriifySteamCMD();

	std::cout << "ready" << std::endl;

	return 0;
}

int main(int argc, char* argv[])
{
	for (size_t i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-nogui")
		{
			std::cerr << "No command line support yet, however it is planned\nhttps://github.com/kennyrkun/srcds-mgr/projects/1#card-10731990" << std::endl;
		}
	}
	std::cout << std::endl;

	initialise();

	// TODO: srcdsmgr class

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
	sf::RenderWindow window(sf::VideoMode(400, 400), "SRCDS Manager");

	SFUI::Menu menu(window);
	menu.setPosition(sf::Vector2f(10, 10));

	SFUI::Theme::loadFont(GBL::PATHS::INTERFACE + "tahoma.ttf");
	SFUI::Theme::loadTexture(GBL::PATHS::INTERFACE + "texture_square.png");
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
	form->addButton("Quit", Quit);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			int id = menu.onEvent(event);
			switch (id)
			{
			case Submit:
				break;
			case Quit:
				window.close();
			}

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(SFUI::Theme::windowBgColor);

		window.draw(menu);

		window.display();
	}

	return EXIT_SUCCESS;
}
