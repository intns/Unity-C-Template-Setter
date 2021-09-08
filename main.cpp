#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <filesystem>
namespace fs = std::filesystem;

namespace UT {
	constexpr const char gDefaultCSContents[] =
		"using System.Collections;\n\
using System.Collections.Generic;\n\
using UnityEngine;\n\
\n\
#ROOTNAMESPACEBEGIN#\
public class #SCRIPTNAME# : MonoBehaviour\n\
{\n\
\tvoid Awake()\n\
\t{\n\
\t\t#NOTRIM#\n\
\t}\n\
\t\n\
\tvoid Update()\n\
\t{\n\
\t\t#NOTRIM#\n\
\t}\n\
}\n\
#ROOTNAMESPACEEND#\n";
} // namespace UT


static void handleExit()
{
	std::cout << "Press ENTER to exit...";
	((void)std::getchar());
}

int main(int argc, char** argv) {
	std::atexit(handleExit);

	if (argc == 1)
	{
		std::cout << "Unity C# Script Template Setter, by Axiot\n";
		std::cout << argv[0] << " [[-b]ackup] [INI file path]\n";

		std::cout << "Note: must have default_cs.txt in the same directory as the .exe\n\n";

		std::cout << "\t_ INI FILE STRUCTURE _\n";
		std::cout << "Every line must be the path to a Unity installation path's Editor folder (e.g. C:/Unity/2019.3.10f1/Editor)\n";

		return EXIT_FAILURE;
	}

	const std::vector<std::string> arguments(argv + 1, argv + argc);
	bool createBackup = false;

	for (const std::string& arg : arguments) {
		if (arg == "-backup" || arg == "-b") {
			createBackup = true;
		}

		// Basic check to see if the string can even contain the specific config extension
		const size_t argLength = arg.length();
		if (argLength <= 4) {
			continue;
		}

		if (arg.substr(argLength - 4, 4) != "ini") {
			continue;
		}

		std::cout << "Found a config file in arguments\n";
		std::ifstream cfgFile(arg, std::ifstream::in);
		if (!cfgFile.is_open()) {
			std::cout << "Couldn't open provided config file\n";
			return EXIT_FAILURE;
		}

		if (!fs::exists("./default_cs.txt")) {
			std::cout << "Creating default_cs.txt because it doesn't exist... ";

			std::ofstream defaultFile("./default_cs.txt", std::ofstream::out);
			defaultFile.write(UT::gDefaultCSContents, sizeof(UT::gDefaultCSContents) - 1);
			defaultFile.close();

			std::cout << "Done\n";
		}

		std::string currentLine;
		while (std::getline(cfgFile, currentLine)) {
			if (currentLine.empty()) {
				continue;
			}

			// currentLine should have a path to the "Unity\*VERSION*\Editor" folder
			// We'll assume the new script template is called "default.cs"

			const std::string finalPath = currentLine + "\\Data\\Resources\\ScriptTemplates\\81-C# Script-NewBehaviourScript.cs.txt";

			if (createBackup) {
				std::time_t t = std::time(nullptr);
				std::tm tm = *std::localtime(&t);

				std::ostringstream oss;
				oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");

				if (!fs::copy_file(finalPath, finalPath + ".backup" + oss.str()))
				{
					std::cout << "Unable to backup file\n";
					return false;
				}
			}

			std::cout << "Destination for .cs.txt file: " << finalPath << '\n';
			if (!fs::copy_file("default_cs.txt", finalPath, fs::copy_options::overwrite_existing)) {
				std::cout << "Couldn't copy file\n";
				return EXIT_FAILURE;
			}
		}

		return EXIT_SUCCESS;
	}

	std::cout << "Didn't find cfg file in arguments, make sure file ends with .ini\n";
	return EXIT_FAILURE;
}
