#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <filesystem>
namespace fs = std::filesystem;

static void HandleExit()
{
	std::cout << "Press ENTER to exit...";
	std::getchar();
}

namespace UT {
	constexpr const char* utConfigExtension = ".ini";
	constexpr std::size_t utConfigExtensionLength = std::char_traits<const char>::length(utConfigExtension);

	constexpr const char utDefaultCSContents[] =
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

	static inline const std::string GetTime() {
		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);
		std::ostringstream oss;

		oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");

		return oss.str();
	}

	static bool ParseConfig(const std::string& configName, bool backup) {
		std::ifstream cfgFile(configName, std::ifstream::in);
		if (!cfgFile.is_open()) {
			std::cout << "Couldn't open provided config file\n";
			return false;
		}

		if (!fs::exists("./default_cs.txt")) {
			std::cout << "Creating default_cs.txt because it doesn't exist... ";

			std::ofstream defaultFile("./default_cs.txt", std::ofstream::out);
			defaultFile.write(utDefaultCSContents, sizeof(utDefaultCSContents) - 1);
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

			if (backup && !fs::copy_file(finalPath, finalPath + ".backup" + GetTime()))
			{
				std::cout << "Unable to backup file\n";
				return false;
			}

			std::cout << "Destination for .cs.txt file: " << finalPath << '\n';
			if (!fs::copy_file("default_cs.txt", finalPath, fs::copy_options::overwrite_existing)) {
				std::cout << "Couldn't copy file\n";
				return false;
			}
		}

		return true;
	}
} // namespace UT

int main(int argc, char** argv) {
	std::atexit(HandleExit);

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
		if (argLength <= UT::utConfigExtensionLength) {
			continue;
		}

		const std::string extension = arg.substr(argLength - UT::utConfigExtensionLength, UT::utConfigExtensionLength);
		if (extension != UT::utConfigExtension) {
			continue;
		}

		std::cout << "Found a config file in arguments\n";
		if (!UT::ParseConfig(arg, createBackup))
		{
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	std::cout << "Didn't find cfg file in arguments, make sure file ends with .ini\n";
	return EXIT_FAILURE;
}
