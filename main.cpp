#include <SFML/Graphics.hpp>
#include <fstream>
#include <json.hpp>
#include <iostream>

nlohmann::json config;
nlohmann::json skinConfig;
std::string resDir;
std::map<char, std::string> binds;

void init();
std::string getJsonString(nlohmann::json value);

int main() {
    init();

    sf::RenderWindow window(sf::VideoMode(1000, 800), "USO!");
 
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
 
        window.clear();

        // make some level selection shit

        /*
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(binds["up"]))) {
            // handle keypress
        }

        */

        window.display();
    }
 
    return EXIT_SUCCESS;
}

void init() {
    std::cout << "test";

    std::ifstream configFile("config.json");
    configFile >> config;
    configFile.close();

    resDir = "./" + getJsonString(config["skin"]) + "/";
    std::cout << resDir;

    std::ifstream skinFile(resDir + "skin.json");
    skinFile >> skinConfig;
    skinFile.close();

    binds = config["keyBinds"];
}

std::string getJsonString(nlohmann::json value) {
    return value.get<std::string>();
}