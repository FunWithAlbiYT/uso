#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <filesystem>
#include <json.hpp>
#include <keycodes.hpp>
#include <metronome.hpp>
#include <thread>
#include <chrono>

using json = nlohmann::json;

json config;
std::string resDir;
std::map<std::string, sf::Keyboard::Key> binds;

sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
sf::Music music;
unsigned int screenWidth = desktop.width;
unsigned int screenHeight = desktop.height;

void init();
bool keyDown(sf::Keyboard::Key key);
std::map<int, std::map<std::string, std::string>> getLevels(const std::string& directory_path);

sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight, sf::Style::Fullscreen), "USO!");
BeatTracker beatTracker(1);

bool anyKeyDown();
bool keyPressed = false;
bool inGame = false;
bool inMenu = true;
bool inStart = true;

int selectedLevel = 1;
int score = 0;
double accuracy = 100.0;

int main() {
    init();

    sf::Font font;
    if (!font.loadFromFile(resDir + "font.ttf")) {
        std::cerr << "Failed to load font file." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Text usoText;
    usoText.setString("USO!");
    usoText.setPosition(sf::Vector2f(screenWidth / 1.8f - usoText.getLocalBounds().width / 2.0f, 2.0f * 40.0f));
    usoText.setFont(font);
    usoText.setFillColor(sf::Color::White);

    sf::Text scoreText;
    scoreText.setString("Score: 0");
    scoreText.setPosition(sf::Vector2f(screenWidth / 1.8f - scoreText.getLocalBounds().width / 2.0f, screenHeight - (0.25f * 40.0f)));
    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::White);

    sf::RectangleShape beaterLeft;
    beaterLeft.setPosition(sf::Vector2(screenWidth / 2.0f, screenHeight / 2.5f));
    beaterLeft.setFillColor(sf::Color::Blue);
    beaterLeft.setOutlineColor(sf::Color::Red);
    beaterLeft.setOutlineThickness(3.0f);
    beaterLeft.setSize(sf::Vector2(3.0f * 40.0f, 6.5f * 40.0f));

    sf::RectangleShape beaterRight;
    beaterRight.setPosition(sf::Vector2(screenWidth / 2.3f, screenHeight / 2.5f));
    beaterRight.setFillColor(sf::Color::Red);
    beaterRight.setOutlineColor(sf::Color::Blue);
    beaterRight.setOutlineThickness(3.0f);
    beaterRight.setSize(sf::Vector2(3.0f * 40.0f, 6.5f * 40.0f));

    const auto& levels = getLevels("./levels");
    int beats = 0;

    auto lastTimeTextChanged = std::chrono::high_resolution_clock::now();
    bool musicPlayed = false;

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

            if (keyDown(binds["confirm"])) {
                inMenu = false;
                inGame = true;
                inStart = true;
            }

            if (keyDown(binds["exit"])) {
                window.close();
            }
        }

        if (inGame) {
            if (keyDown(binds["exit"]) || (music.getStatus() != sf::Music::Playing && !inStart && musicPlayed)) {
                inGame = false;
                inMenu = true;
                musicPlayed = false;
                score = 0;
                usoText.setString("USO!");
                usoText.setFillColor(sf::Color::White);
                music.stop();
            }

            if (inStart) {
                beats = 0;
                inStart = false;

                if (
                        !music.openFromFile(
                            "./levels/" +
                            levels.at(selectedLevel-1).at("directory") +
                            "/" +
                            levels.at(selectedLevel-1).at("songFile")
                        )
                    )
                {
                    return EXIT_FAILURE;
                }

                music.setVolume(80);
                beatTracker = BeatTracker(std::stoi(levels.at(selectedLevel-1).at("bpm")));
            }

            if (beatTracker.isBeat()) {
                beats++;

                for (const auto& note : levels.at(selectedLevel-1).at("levelData")) {
                    int beatNumber = note - '0'; 
                    int direction = (note == '0') ? 0 : 1;

                    int scoreAdder = 0;
                    sf::Color scoreColor = sf::Color::Red;

                    if ((beatNumber >= beats && beatNumber < beats + 2) || (beatNumber < beats && beatNumber > (beats - 2))) {
                        if ((direction == 0 && keyDown(binds["left"])) || (direction == 1 && keyDown(binds["right"]))) {
                            int accuracy = abs(beatNumber - beats);
                            if (accuracy == 0) {
                                scoreAdder = 100;
                                scoreColor = sf::Color::Green;
                            } else if (accuracy == 1) {
                                scoreAdder = 50;
                                scoreColor = sf::Color::Yellow;
                            }
                        }
                    }

                    score += scoreAdder;
                    accuracy = static_cast<double>(score) / beats * 100;
                    
                    lastTimeTextChanged = std::chrono::high_resolution_clock::now();
                    usoText.setString(std::to_string(scoreAdder));
                    usoText.setFillColor(scoreColor);
                }
            }

            scoreText.setString("Score: "+std::to_string(score));

            window.draw(scoreText);
            window.draw(beaterLeft);
            window.draw(beaterRight);

            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTimeTextChanged).count();

            if (elapsedTime >= 400) {
                usoText.setString("USO!");
                usoText.setFillColor(sf::Color::White);
                lastTimeTextChanged = currentTime;
            }
        }

        if (!anyKeyDown()) {
            keyPressed = false;
        }

        window.draw(usoText);
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
            level["bpm"] = std::to_string(jsonContent["bpm"].get<int>());
            level["songFile"] = jsonContent["songFile"].get<std::string>();
            level["approachRate"] = std::to_string(jsonContent["approachRate"].get<int>());
            level["directory"] = entry.path().parent_path().filename().string();

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