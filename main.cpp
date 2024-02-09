#include <SFML/Graphics.hpp>
#include <fstream>
#include <json.hpp>
#include <iostream>
#include <map>
#include <keycodes.hpp>

using json = nlohmann::json;

json config;
std::string resDir;
std::map<std::string, sf::Keyboard::Key> binds;

void init();
std::string getJsonString(const json& value);
bool keyDown(sf::Keyboard::Key key);

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

        /*
        
        if (keyDown(binds["down"])) {
            // key is pressed
        }

        */

        window.display();
    }
 
    return EXIT_SUCCESS;
}

void init() {
    std::ifstream configFile("config.json");
    config = json::parse(configFile);
    configFile.close();

    resDir = "./" + getJsonString(config["skin"]) + "/";
    
    auto keyBinds = config["keyBinds"];
    for (auto it = keyBinds.begin(); it != keyBinds.end(); ++it) {
        binds[it.key()] = getSFMLKeyCode(it.value());
    }
}

std::string getJsonString(const json& value) {
    return value.get<std::string>();
}

bool keyDown(sf::Keyboard::Key key) {
    static bool wasKeyPressed = false; 
    bool isKeyPressed = sf::Keyboard::isKeyPressed(key);

    if (isKeyPressed && !wasKeyPressed) {
        wasKeyPressed = true;
        return true;
    } else if (!isKeyPressed) {
        wasKeyPressed = false;
    }

    return false;
}