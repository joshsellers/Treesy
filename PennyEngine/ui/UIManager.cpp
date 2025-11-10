// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "UIManager.h"
#include "../PennyEngine.h"
#include "../core/Logger.h"
#include "UI.h"
#include "components/Button.h"
#include "components/Panel.h"

pe::intern::UIManager::UIManager() {
    PennyEngine::addInputListener(this);

    if (!_spriteSheet->loadFromFile("res/ui_sprite_sheet.png")) {
        Logger::log("Failed to load UI sprite sheet");
    }
}

void pe::intern::UIManager::update() {
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->update();
    }
}

void pe::intern::UIManager::draw() {
    if (!PennyEngine::isStarted()) return;

    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->draw(*getSurface());
    }
}

std::vector<s_p<pe::Menu>>& pe::intern::UIManager::getMenus() {
    return _menus;
}

void pe::intern::UIManager::setSurface(sf::RenderTexture* surface) {
    _surface = surface;
}

sf::RenderTexture* pe::intern::UIManager::getSurface() {
    return _surface;
}

s_p<sf::Texture> pe::intern::UIManager::getSpriteSheet() const {
    return _spriteSheet;
}

void pe::intern::UIManager::textEntered(sf::Uint32 character) {
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->textEntered(character);
    }
}

void pe::intern::UIManager::keyPressed(sf::Keyboard::Key& key) {
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->keyPressed(key);
    }
}

void pe::intern::UIManager::keyReleased(sf::Keyboard::Key& key) {
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->keyReleased(key);
    }
}

void pe::intern::UIManager::mouseButtonPressed(const int mx, const int my, const int button) {
    const sf::Vector2f mouseOffset = PennyEngine::getUIMouseOffset();
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->mouseButtonPressed(mx + mouseOffset.x, my + mouseOffset.y, button);
    }
}

void pe::intern::UIManager::mouseButtonReleased(const int mx, const int my, const int button) {
    const sf::Vector2f mouseOffset = PennyEngine::getUIMouseOffset();
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->mouseButtonReleased(mx + mouseOffset.x, my + mouseOffset.y, button);
    }
}

void pe::intern::UIManager::mouseMoved(const int mx, const int my) {
    const sf::Vector2f mouseOffset = PennyEngine::getUIMouseOffset();
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->mouseMoved(mx + mouseOffset.x, my + mouseOffset.y);
    }
}

void pe::intern::UIManager::mouseWheelScrolled(sf::Event::MouseWheelScrollEvent mouseWheelScroll) {
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->mouseWheelScrolled(mouseWheelScroll);
    }
}

void pe::intern::UIManager::controllerButtonReleased(GAMEPAD_BUTTON button) {
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->controllerButtonReleased(button);
    }

    switch (button) {
        case GAMEPAD_BUTTON::B:
            buttonPressed("virtkey:back");
            break;
        case GAMEPAD_BUTTON::Y:
            buttonPressed("virtkey:space");
            break;
        case GAMEPAD_BUTTON::START:
            buttonPressed("virtkey:done");
            break;
        case GAMEPAD_BUTTON::LEFT_STICK:
            buttonPressed("virtkey:caps");
            break;
    }
}

void pe::intern::UIManager::controllerButtonPressed(GAMEPAD_BUTTON button) {
    for (const auto& menu : getMenus()) {
        if (menu->isActive()) menu->controllerButtonPressed(button);
    }
}

void pe::intern::UIManager::gamepadDisconnected() {

}

void pe::intern::UIManager::gamepadConnected() {
}

void pe::intern::UIManager::createVirtualKeyboard() {
    constexpr int rowWidth = 13;
    constexpr float keyWidth = 3.f;
    constexpr float keyHeight = 3.f;
    constexpr float keyPadding = 0.5f;

    const std::string row0_lower = "1234567890-=\b";
    const std::string row0_upper = "!@#$%^&*()_+\b";
    const std::string row1_lower = " qwertyuiop[]";
    const std::string row1_upper = " QWERTYUIOP{}";
    const std::string row2_lower = " asdfghjkl;'";
    const std::string row2_upper = " ASDFGHJKL:\"";
    const std::string row3_lower = "§ zxcvbnm,./ ";
    const std::string row3_upper = "§ ZXCVBNM<>? ";

    const std::vector<std::string> lowerRows = { row0_lower, row1_lower, row2_lower, row3_lower };
    const std::vector<std::string> upperRows = { row0_upper, row1_upper, row2_upper, row3_upper };

    constexpr float keyboardX = 50.f - (rowWidth * (keyWidth + keyPadding)) / 2.f;
    constexpr float keyboardY = 60.f;
    constexpr float rowPadding = keyHeight + 1.f;
    const std::vector<sf::Vector2f> rowCoords = { {
            keyboardX, keyboardY}, {keyboardX, keyboardY + rowPadding}, {keyboardX, keyboardY + rowPadding * 2}, {keyboardX, keyboardY + rowPadding * 3}
    };

    const auto& spaceBar = new_s_p(Button, ("virtkey:space", 50, keyboardY + rowPadding * 4.f + 2.5f, 10, 3, "space", this, true));
    spaceBar->setGamepadSelectionId(4 * rowWidth);

    const auto& menuLower = UI::addMenu("virtKeyboard_lower");
    menuLower->addComponent(spaceBar);
    const auto& menuUpper = UI::addMenu("virtKeyboard_upper");
    menuUpper->addComponent(spaceBar);

    const float panelPadding = 1.f;
    const auto& panel = new_s_p(Panel, ("virtKeyboardPanel", 
        keyboardX - panelPadding, keyboardY - panelPadding, (keyWidth + keyPadding) * rowWidth + panelPadding * 1.5f, rowPadding * 5.f + panelPadding * 2, "", true, false
    ));

    auto initKeyboard = [&](s_p<Menu> keyboard, const std::vector<std::string> rows) {
        std::vector<std::vector<int>> grid;
        for (int row = 0; row < rows.size(); row++) {
            grid.push_back({});

            const float x = rowCoords.at(row).x;
            const float y = rowCoords.at(row).y;
            for (int i = 0; i < rows.at(row).length(); i++) {
                const std::string rowString = rows.at(row);
                std::string character(1, rowString.at(i));

                if (character == "\b") character = "back";
                else if (character == "") character = "done";
                else if (character == "§") character = "caps";

                const auto& keyButton = new_s_p(Button, ("virtkey:" + character, x + (keyWidth + keyPadding) * i, y, keyWidth, keyHeight, character, this, false));
                keyButton->setGamepadSelectionId(i + row * rowWidth);
                grid.at(row).push_back(keyButton->getGamepadSelectionId());

                keyboard->addComponent(keyButton);
                panel->attach(keyButton);
            }
        }
        grid.push_back({ {spaceBar->getGamepadSelectionId()} });

        keyboard->defineGamepadSelectionGrid(grid);
        keyboard->addComponent(panel);
    };

    initKeyboard(menuLower, lowerRows);
    initKeyboard(menuUpper, upperRows);
    panel->attach("virtkey:space");
}

void pe::intern::UIManager::buttonPressed(std::string id) {
    if (stringContains(id, "virtkey:")) {
        std::string key = splitString(id, ":")[1];

        if (key == "done") {
            toggleVirtualKeyboard();
            return;
        } else if (key == "caps") {
            const auto& lowerMenu = UI::getMenu("virtKeyboard_lower");
            const auto& upperMenu = UI::getMenu("virtKeyboard_upper");

            if (lowerMenu->isActive()) {
                upperMenu->_selectedItemX = lowerMenu->_selectedItemX;
                upperMenu->_selectedItemY = lowerMenu->_selectedItemY;

                lowerMenu->close();
                upperMenu->open();
            } else if (upperMenu->isActive()) {
                lowerMenu->_selectedItemX = upperMenu->_selectedItemX;
                lowerMenu->_selectedItemY = upperMenu->_selectedItemY;

                upperMenu->close();
                lowerMenu->open();
            }

            return;
        } else if (key == "back") key = "\b";
        else if (key == "space") key = " ";

        textEntered(key.at(0));
    }
}

void pe::intern::UIManager::toggleVirtualKeyboard() {
    const auto& lowerMenu = UI::getMenu("virtKeyboard_lower");
    const auto& upperMenu = UI::getMenu("virtKeyboard_upper");

    if (lowerMenu->isActive() || upperMenu->isActive()) {
        lowerMenu->close();
        upperMenu->close();

        for (const auto& menu : _menus) {
            if (menu->isActive()) {
                menu->virtualKeyboardClosed();
            }
        }
    } else {
        lowerMenu->open();
    }
}