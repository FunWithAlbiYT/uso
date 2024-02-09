#include <keycodes.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <string>

sf::Keyboard::Key getSFMLKeyCode(const std::string& keyName) {
    static const std::map<std::string, sf::Keyboard::Key> keyMap = {
        {"A", sf::Keyboard::A},
        {"B", sf::Keyboard::B},
        {"C", sf::Keyboard::C},
        {"D", sf::Keyboard::D},
        {"E", sf::Keyboard::E},
        {"F", sf::Keyboard::F},
        {"G", sf::Keyboard::G},
        {"H", sf::Keyboard::H},
        {"I", sf::Keyboard::I},
        {"J", sf::Keyboard::J},
        {"K", sf::Keyboard::K},
        {"L", sf::Keyboard::L},
        {"M", sf::Keyboard::M},
        {"N", sf::Keyboard::N},
        {"O", sf::Keyboard::O},
        {"P", sf::Keyboard::P},
        {"Q", sf::Keyboard::Q},
        {"R", sf::Keyboard::R},
        {"S", sf::Keyboard::S},
        {"T", sf::Keyboard::T},
        {"U", sf::Keyboard::U},
        {"V", sf::Keyboard::V},
        {"W", sf::Keyboard::W},
        {"X", sf::Keyboard::X},
        {"Y", sf::Keyboard::Y},
        {"Z", sf::Keyboard::Z},
        {"NUM0", sf::Keyboard::Num0},
        {"NUM1", sf::Keyboard::Num1},
        {"NUM2", sf::Keyboard::Num2},
        {"NUM3", sf::Keyboard::Num3},
        {"NUM4", sf::Keyboard::Num4},
        {"NUM5", sf::Keyboard::Num5},
        {"NUM6", sf::Keyboard::Num6},
        {"NUM7", sf::Keyboard::Num7},
        {"NUM8", sf::Keyboard::Num8},
        {"NUM9", sf::Keyboard::Num9},
        {"ESCAPE", sf::Keyboard::Escape},
        {"LCONTROL", sf::Keyboard::LControl},
        {"LSHIFT", sf::Keyboard::LShift},
        {"LALT", sf::Keyboard::LAlt},
        {"LSYSTEM", sf::Keyboard::LSystem},
        {"RCONTROL", sf::Keyboard::RControl},
        {"RSHIFT", sf::Keyboard::RShift},
        {"RALT", sf::Keyboard::RAlt},
        {"RSYSTEM", sf::Keyboard::RSystem},
        {"MENU", sf::Keyboard::Menu},
        {"LBRACKET", sf::Keyboard::LBracket},
        {"RBRACKET", sf::Keyboard::RBracket},
        {"SEMICOLON", sf::Keyboard::Semicolon},
        {"COMMA", sf::Keyboard::Comma},
        {"PERIOD", sf::Keyboard::Period},
        {"QUOTE", sf::Keyboard::Quote},
        {"SLASH", sf::Keyboard::Slash},
        {"BACKSLASH", sf::Keyboard::Backslash},
        {"TILDE", sf::Keyboard::Tilde},
        {"EQUAL", sf::Keyboard::Equal},
        {"DASH", sf::Keyboard::Dash},
        {"SPACE", sf::Keyboard::Space},
        {"RETURN", sf::Keyboard::Return},
        {"BACKSPACE", sf::Keyboard::Backspace},
        {"TAB", sf::Keyboard::Tab},
        {"PAGEUP", sf::Keyboard::PageUp},
        {"PAGEDOWN", sf::Keyboard::PageDown},
        {"END", sf::Keyboard::End},
        {"HOME", sf::Keyboard::Home},
        {"INSERT", sf::Keyboard::Insert},
        {"DELETE", sf::Keyboard::Delete},
        {"ADD", sf::Keyboard::Add},
        {"SUBTRACT", sf::Keyboard::Subtract},
        {"MULTIPLY", sf::Keyboard::Multiply},
        {"DIVIDE", sf::Keyboard::Divide},
        {"LEFT", sf::Keyboard::Left},
        {"RIGHT", sf::Keyboard::Right},
        {"UP", sf::Keyboard::Up},
        {"DOWN", sf::Keyboard::Down},
        {"NUMPAD0", sf::Keyboard::Numpad0},
        {"NUMPAD1", sf::Keyboard::Numpad1},
        {"NUMPAD2", sf::Keyboard::Numpad2},
        {"NUMPAD3", sf::Keyboard::Numpad3},
        {"NUMPAD4", sf::Keyboard::Numpad4},
        {"NUMPAD5", sf::Keyboard::Numpad5},
        {"NUMPAD6", sf::Keyboard::Numpad6},
        {"NUMPAD7", sf::Keyboard::Numpad7},
        {"NUMPAD8", sf::Keyboard::Numpad8},
        {"NUMPAD9", sf::Keyboard::Numpad9},
        {"F1", sf::Keyboard::F1},
        {"F2", sf::Keyboard::F2},
        {"F3", sf::Keyboard::F3},
        {"F4", sf::Keyboard::F4},
        {"F5", sf::Keyboard::F5},
        {"F6", sf::Keyboard::F6},
        {"F7", sf::Keyboard::F7},
        {"F8", sf::Keyboard::F8},
        {"F9", sf::Keyboard::F9},
        {"F10", sf::Keyboard::F10},
        {"F11", sf::Keyboard::F11},
        {"F12", sf::Keyboard::F12},
        {"F13", sf::Keyboard::F13},
        {"F14", sf::Keyboard::F14},
        {"F15", sf::Keyboard::F15},
        {"PAUSE", sf::Keyboard::Pause}
    };

    auto it = keyMap.find(keyName);
    if (it != keyMap.end()) {
        return it->second;
    } else {
        return sf::Keyboard::Unknown;
    }
}