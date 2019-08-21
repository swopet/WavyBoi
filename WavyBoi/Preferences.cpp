#include "pch.h"
#include "Preferences.h"

sf::Vector2i getIntPairFromString(std::string pair) {
	size_t pos = pair.find(",");
	if (pos == std::string::npos) {
		throw std::invalid_argument("bad argument: no comma");
		return sf::Vector2i(0, 0);
	}
	std::string first = pair.substr(0, pos);
	std::string second = pair;
	second.erase(0, pos + 1);
	return sf::Vector2i(std::stoi(first), std::stoi(second));
}

sf::Vector2u getUnsignedIntPairFromString(std::string pair) {
	size_t pos = pair.find(",");
	if (pos == std::string::npos) {
		throw std::invalid_argument("bad argument: no comma");
		return sf::Vector2u(0, 0);
	}
	std::string first = pair.substr(0, pos);
	std::string second = pair;
	second.erase(0, pos + 1);
	return sf::Vector2u(std::stoi(first), std::stoi(second));
}

void Preferences::loadConfigFromFile() {
	std::string cwd = std::experimental::filesystem::current_path().u8string();
	std::replace(cwd.begin(), cwd.end(), '\\', '/');
	std::ifstream t("WBConfig.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string config_str = buffer.str();
	int ctr = 0;
	size_t pos = 0;
	std::string line;
	while ((pos = config_str.find("\n")) != std::string::npos) {
		line = config_str.substr(0, pos);
		size_t colon_pos = line.find(':');
		if (colon_pos != std::string::npos) {
			std::string key = line.substr(0, colon_pos);
			line.erase(0, colon_pos + 1);
			std::string value = line;
			if (value.compare("DEFAULT") != 0) {
				if (key.compare("control_resolution") == 0) {
					try {
						control_resolution = getUnsignedIntPairFromString(value);
					}
					catch (std::invalid_argument ex) {
						std::cerr << "Invalid argument on line " << ctr << ": \"" << value << "\"" << std::endl;
					}
				}
				else if (key.compare("control_position") == 0) {
					try {
						control_position = getIntPairFromString(value);
					}
					catch (std::invalid_argument ex) {
						std::cerr << "Invalid argument on line " << ctr << ": \"" << value << "\"" << std::endl;
					}
				}
				if (key.compare("display_resolution") == 0) {
					try {
						display_resolution = getUnsignedIntPairFromString(value);
					}
					catch (std::invalid_argument ex) {
						std::cerr << "Invalid argument on line " << ctr << ": \"" << value << "\"" << std::endl;
					}
				}
				if (key.compare("display_position") == 0) {
					try {
						display_position = getIntPairFromString(value);
					}
					catch (std::invalid_argument ex) {
						std::cerr << "Invalid argument on line " << ctr << ": \"" << value << "\"" << std::endl;
					}
				}
			}
		}
		else {
			std::cerr << "Invalid format on line " << ctr << " of config file!" << std::endl;
		}
		config_str.erase(0, pos + 1);
		ctr++;
	}
}

Preferences::Preferences()
{
	control_resolution = sf::Vector2u(1080, 720);
	control_position = sf::Vector2i(0, 0);
	display_resolution = sf::Vector2u(1080, 720);
	display_position = sf::Vector2i(1080, 0);
}


Preferences::~Preferences()
{
}

void Preferences::writeNewDefaults()
{
}
