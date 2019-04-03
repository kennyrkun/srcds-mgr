#include "Globals.hpp"
#include "Zip.hpp"

#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/Theme.hpp>
#include <SFUI/SFUI.hpp>
#include <SFML\Network\Http.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>

#ifndef _WIN32
#error This program is not written to support non-Windows platforms! (yet)
#endif

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

		// if steamcmd.exe is missing, assume something horrible went wrong and reinstall
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
				{
					std::cerr << "failed to write to steamcmd.zip" << std::endl;

					return FAILURE;
				}
				else
				{
					std::cout << "unzipping steamcmd" << std::endl;

					Zip zip;

					zip.archivePath = GBL::PATHS::STEAMCMD;

					if (!zip.extractFileFromArchive("steamcmd.zip", "steamcmd.exe"))
					{
						std::cerr << "failed to extract steamcmd" << std::endl;
						return FAILURE;
					}
					else
					{
						// we're done with you
						fs::remove(GBL::PATHS::STEAMCMD + "steamcmd.zip");

						// use SteamCMD
					}
				}
			}
			else
			{
				std::cerr << "failed to open write stream" << std::endl;

				return FAILURE;
			}
		}
		else
		{
			std::cerr << "failed to connect to http://steamcdn-a.akamaihd.net (" << response.getStatus() << ")" << std::endl;

			return FAILURE;
		}
	}

	return SUCCESS;
}

int initialise()
{
	std::cout << "initliasing" << std::endl;

	if (veriifySteamCMD())
	{
		std::cout << "SteamCMD verified :D" << std::endl;
	}
	else
	{
		std::cerr << "Failed to verify SteamCMD :'(" << std::endl;

		// TOOD: possibly don't fail because user might already
		// have servers and such installed. in this case, we
		// should just disable features that use SteamCMD
		// and ask the user if they want to attempt to gather it.
		return FAILURE; // fail because SteamCMD is necessary
	}

	std::cout << "ready" << std::endl;

	return SUCCESS;
}

// Only windows.
// TODO: perhaps use an std::map with an std::pair of <string> and <int> for id and name
// TODO: probably at some point it'd be better to acces the streamdb and find servers
enum DedicatedServers
{
	AlienSwarm = 635,
	AlienSwarmReactiveDrop = 582400,
	BlackMesaDeathMatch = 346680,
	CounterStrike16 = 90,
	CounterStrikeGlobalOffensive = 740,
	CounterStrikeConditionZero = 90, // Is this really the same as CounterStrike16?
	CounterStrikeSource = 232330,
	DayOfDefeat = 90, // Is this really the same as CounterStrike16?
	DayOfDefeatSource = 232290,
	DeathmatchClassic = 90, // Is this really the same as CounterStrike16?
	GarrysMod = 4020,
	HalfLife2Deathmatch = 232370,
	HalfLifeDeathmatchSource = 255470,
	HalfLife = 90, // Is this really the same as CounterStrike16?
	HalfLifeOpposingForce = 90, // Is this really the same as CounterStrike16?
	Left4Dead = 222840,
	Left4Dead2 = 222860,
	NoMoreRoomInHell = 317670,
	SvenCoOp = 276060,
	Ricochet = 90, // Is this really the same as CounterStrike16?
	Source = 205,
	SourceSDKBase2013 = 244310,
	SourceSDKBase2006MP = 205, // Is this really the same as Source?
	Source2007 = 310,
	TeamFortress2 = 232250,
	TeamFortressClassic = 90, // Is this really the same as CounterStrike16?
	TheShip = 2403,
};

int m2ain(int argc, char* argv[])
{
	std::cout << "Running with " << argc << " arguments." << std::endl;

	for (size_t i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-nogui")
		{
			std::cerr << "No command line support yet, however it is planned\nhttps://github.com/kennyrkun/srcds-mgr/projects/1#card-10731990" << std::endl;
		}
	}
	std::cout << std::endl;

	if (!initialise())
	{
		std::cerr << "Failed to initalise, aborting." << std::endl;

		abort();
	}

	// TODO: srcdsmgr class

	enum Callback
	{
		C_APP_ID,
		Submit,
		Quit
	};

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(400, 400), "SRCDS Manager");

	SFUI::Menu menu(window);
	menu.setPosition(sf::Vector2f(10, 10));

	SFUI::Theme::loadFont(GBL::PATHS::INTERFACE + "tahoma.ttf");
	SFUI::Theme::loadTexture(GBL::PATHS::INTERFACE + "texture_square.png");
	SFUI::Theme::textCharacterSize = 11;
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

	form->addLabel("New Server");

	SFUI::InputBox* AppID = new SFUI::InputBox();
	form->addRow("AppID", AppID, C_APP_ID);

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
