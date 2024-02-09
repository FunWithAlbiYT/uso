#include <SFML/Graphics.hpp>
#include <fstream>
#include <json.hpp>
#include <iostream>
#include <keycodes.hpp>
#include <filesystem>
#include <map>
#include <trackbeats.hpp>

using json = nlohmann::json;

json config;
std::string resDir;
std::map<std::string, sf::Keyboard::Key> binds;

void init();
std::string getJsonString(const json& value);
bool keyDown(sf::Keyboard::Key key);
std::map<std::string, std::string> getLevels(const std::string& directory_path);

bool inMenu = true;

sf::RenderWindow window(sf::VideoMode(800, 600), "USO!");

int main() {
    init();

    BeatTracker beatTracker(182);
    int beats = 0;
 
    std::map<int, int> levelData =
        {
            {1, 0},
            {2, 1},
            {3, 1},
            {4, 0},
        };

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

        if (beatTracker.isBeat()) {
            beats++;

            for (const auto& pair : levelData) {
                if (pair.first == beats) {
                    if (pair.second == 0) {
                        std::cout << "left" << std::endl;
                    }

                    if (pair.second == 1) {
                        std::cout << "right" << std::endl;
                    }
                }
            }
        }

        window.display();
    }
 
    return EXIT_SUCCESS;
}

std::map<std::string, std::string> getLevels(const std::string& directory_path) {
    std::map<std::string, std::string> json_contents;
    
    for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
        if (entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
                json_contents[entry.path().filename().string()] = content;
                file.close();
            } else {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
            }
        }
    }
    
    return json_contents;
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

void init() {
    std::ifstream configFile("config.json");
    config = json::parse(configFile);
    configFile.close();

    resDir = "./" + getJsonString(config["skin"]) + "/";
    
    auto keyBinds = config["keyBinds"];
    for (auto it = keyBinds.begin(); it != keyBinds.end(); ++it) {
        binds[it.key()] = getSFMLKeyCode(it.value());
    }

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktop.width;
    unsigned int screenHeight = desktop.height;

    unsigned int windowWidth = window.getSize().x;
    unsigned int windowHeight = window.getSize().y;
    unsigned int posX = (screenWidth - windowWidth) / 2;
    unsigned int posY = (screenHeight - windowHeight) / 2;

    window.setPosition(sf::Vector2i(posX, posY));
}