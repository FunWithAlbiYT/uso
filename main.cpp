#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <filesystem>
#include <json.hpp>
#include <keycodes.hpp>
#include <trackbeats.hpp>

using json = nlohmann::json;

json config;
std::string resDir;
std::map<std::string, sf::Keyboard::Key> binds;

sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
unsigned int screenWidth = desktop.width;
unsigned int screenHeight = desktop.height;

void init();
bool keyDown(sf::Keyboard::Key key);
std::map<int, std::map<std::string, std::string>> getLevels(const std::string& directory_path);

sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight, sf::Style::Fullscreen), "USO!");

bool anyKeyDown();
bool keyPressed = false;
bool inMenu = true;
bool inLevel = false;

std::map<std::string, std::map<std::string, std::string>> levelData;
int selectedLevel = 1;

int main() {
    init();

    sf::Font font;
    if (!font.loadFromFile(resDir + "font.ttf")) {
        std::cerr << "Failed to load font file." << std::endl;
        return EXIT_FAILURE;
    }

    const auto& levels = getLevels("./levels");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();

        if (inMenu) {
            int levelCount = 0;
            int lastY = 2.5f * 40.0f;

            for (const auto& level : levels) {
                levelCount++;

                lastY = lastY + (2.0f * 40.0f);

                sf::RectangleShape levelRender(sf::Vector2f(22.0f * 40.0f, 1.5f * 40.0f));
                levelRender.setOutlineColor(sf::Color::White);
                levelRender.setOutlineThickness(1.5f);
                levelRender.setFillColor(sf::Color::Black);
                levelRender.setPosition(sf::Vector2f(4.0f * 40.0f, lastY));

                sf::Text textRender;
                textRender.setFillColor(sf::Color::White);
                textRender.setFont(font);
                textRender.setString(levels.at(levelCount-1).at("name"));
                textRender.setPosition(levelRender.getPosition() + sf::Vector2f(0.25f * 40.0f, 0.25f * 40.0f));

                if (levelCount == selectedLevel) {
                    textRender.setFillColor(sf::Color::Black);
                    levelRender.setOutlineColor(sf::Color::Black);
                    levelRender.setFillColor(sf::Color::White);
                }

                window.draw(levelRender);
                window.draw(textRender);
            }

            if (keyDown(binds["down"])) {
                selectedLevel++;
                
                if (selectedLevel > levels.size()) {
                    selectedLevel = 1;
                }
            }

            if (keyDown(binds["up"])) {
                selectedLevel--;
                
                if (selectedLevel < 1) {
                    selectedLevel = levels.size();
                }
            }
        }

        if (!anyKeyDown()) {
            keyPressed = false;
        }

        window.display();
    }

    return EXIT_SUCCESS;
}

std::map<int, std::map<std::string, std::string>> getLevels(const std::string& directory_path) {
    std::map<int, std::map<std::string, std::string>> levelDt;

    int levelCount = 0;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory_path)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json") {
            continue;
        }

        std::ifstream file(entry.path());
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
            json jsonContent = json::parse(content);

            std::map<std::string, std::string> level;
            level["name"] = jsonContent["name"].get<std::string>();
            level["bpm"] = jsonContent["bpm"].get<int>();
            level["length"] = jsonContent["length"].get<int>();
            level["songFile"] = jsonContent["songFile"].get<std::string>();
            level["approachRate"] = jsonContent["approachRate"].get<int>();

            json leveldataArray = jsonContent["leveldata"];
            std::vector<std::vector<int>> levelData;
            for (const auto& data : leveldataArray) {
                levelData.push_back(data.get<std::vector<int>>());
            }

            level["levelData"] = leveldataArray.dump();
            levelDt[levelCount++] = level;

            file.close();
        } else {
            std::cerr << "Error opening file: " << entry.path() << std::endl;
        }
    }

    return levelDt;
}

bool keyDown(sf::Keyboard::Key key) {
    if (sf::Keyboard::isKeyPressed(key) && !keyPressed) {
        keyPressed = true;
        return true;
    }
    return false;
}

bool anyKeyDown() {
    for (int key = sf::Keyboard::A; key <= sf::Keyboard::KeyCount; ++key) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key))) {
            return true;
        }
    }
    return false;
}

void init() {
    std::ifstream configFile("config.json");
    config = json::parse(configFile);
    configFile.close();

    resDir = "./skins/" + config["skin"].get<std::string>() + "/";
    
    auto keyBinds = config["keyBinds"];
    for (auto it = keyBinds.begin(); it != keyBinds.end(); ++it) {
        binds[it.key()] = getSFMLKeyCode(it.value());
    }

    unsigned int windowWidth = window.getSize().x;
    unsigned int windowHeight = window.getSize().y;
    unsigned int posX = (screenWidth - windowWidth) / 2;
    unsigned int posY = (screenHeight - windowHeight) / 2;

    window.setPosition(sf::Vector2i(posX, posY));
}