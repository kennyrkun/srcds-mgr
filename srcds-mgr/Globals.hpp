#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <string>

#define SUCCESS 1
#define FAILURE 0

namespace GBL
{
	namespace PATHS
	{
		const std::string BASE = "./";
		const std::string RESOURCES = BASE + "resources/";

		const std::string INTERFACE = RESOURCES + "interface/";
		const std::string STEAMCMD = RESOURCES + "steamcmd/";
	}
}

#endif // !GLOBALS_HPP
