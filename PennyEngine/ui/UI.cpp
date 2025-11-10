// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "UI.h"
#include "../core/Resolution.h"
#include "../core/Logger.h"

sf::Vector2f pe::UI::percentToScreenPos(sf::Vector2f pos) {
    const Resolution res = PennyEngine::useDisplayResForUI() ? PennyEngine::getDisplayResolution() : PennyEngine::getRenderResolution();
    return sf::Vector2f((float)res.width * (pos.x / 100.f), (float)res.height * (pos.y / 100.f));
}

sf::Vector2f pe::UI::percentToScreenPos(float x, float y) {
    return percentToScreenPos({ x, y });
}

sf::Vector2f pe::UI::percentToScreenDimensions(sf::Vector2f dimensions) {
    return percentToScreenPos(dimensions);
}

sf::Vector2f pe::UI::percentToScreenDimensions(float x, float y) {
    return percentToScreenPos(x, y);
}

float pe::UI::percentToScreenWidth(float width) {
    const Resolution res = PennyEngine::useDisplayResForUI() ? PennyEngine::getDisplayResolution() : PennyEngine::getRenderResolution();
    return (float)res.width * (width / 100.f);
}

float pe::UI::percentToScreenHeight(float height) {
    const Resolution res = PennyEngine::useDisplayResForUI() ? PennyEngine::getDisplayResolution() : PennyEngine::getRenderResolution();
    return (float)res.height * (height / 100.f);
}

sf::Vector2i pe::UI::getMousePos() {
    const sf::Vector2i mPos(sf::Mouse::getPosition().x + PennyEngine::getUIMouseOffset().x, sf::Mouse::getPosition().y + PennyEngine::getUIMouseOffset().y);
    return mPos;
}

void pe::UI::toggleVirtualKeyboard() {
    _instance.toggleVirtualKeyboard();
}

s_p<pe::Menu> pe::UI::addMenu(std::string id) {
    _instance.getMenus().push_back(new_s_p(Menu, (id)));
    return getMenu(id);
}

s_p<pe::Menu> pe::UI::getMenu(std::string id) {
    for (const auto& menu : _instance.getMenus()) {
        if (menu->getIdentifier() == id) return menu;
    }

    Logger::log("Did not find menu with id \"" + id + "\"");
    return nullptr;
}

std::vector<s_p<pe::Menu>> pe::UI::getMenus() {
    return _instance.getMenus();
}

s_p<sf::Texture> pe::UI::getSpriteSheet() {
    return _instance.getSpriteSheet();
}