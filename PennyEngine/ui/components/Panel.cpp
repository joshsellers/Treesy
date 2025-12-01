// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "Panel.h"
#include "../UI.h"
#include "../../core/Logger.h"

pe::Panel::Panel(std::string identifier, float x, float y, float width, float height, std::string titleText, bool draggable, bool autoCenter) :
MenuComponent(identifier, x, y, width, height, autoCenter, PANEL_CONFIG) {
    float fontSize = UI::percentToScreenWidth(1.5f);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(sf::Color::White);
    _text.setString(titleText);

    _lastMousePos = UI::getMousePos();
    _isDraggable = draggable;
}

void pe::Panel::setTitle(std::string text) {
    _text.setString(text);
}

void pe::Panel::attach(s_p<MenuComponent> component) {
    _attachedComponents.push_back(component);
}

void pe::Panel::move(sf::Vector2f delta) {
    _pos += delta;

    for (const auto& component : _attachedComponents) {
        component->move(delta);
    }
}

void pe::Panel::attach(std::string identifier) {
    for (const auto& menu : UI::getMenus()) {
        const auto& component = menu->getComponent(identifier, true);
        if (component != nullptr) {
            _attachedComponents.push_back(component);
            return;
        }
    }

    Logger::log("Panel could not find component with id \"" + identifier + "\"");
}

void pe::Panel::attachAt(s_p<MenuComponent> component, sf::Vector2f pos) {
    attach(component);
    const sf::Vector2f convPos = { _pos.x + _size.x * (pos.x / 100.f), _pos.y + _size.y * (pos.y / 100.f) };
    const sf::Vector2f delta = convPos - sf::Vector2f(
        component->getBounds().left + component->getBounds().width / 2.f, component->getBounds().top + component->getBounds().height / 2.f
    );
    component->move(delta);
}

void pe::Panel::attachAt(std::string identifier, sf::Vector2f pos) {
    for (const auto& menu : UI::getMenus()) {
        const auto& component = menu->getComponent(identifier, true);
        if (component != nullptr) {
            attachAt(component, pos);
            return;
        }
    }

    Logger::log("Panel could not find component with id \"" + identifier + "\"");
}

void pe::Panel::update() {
}

void pe::Panel::draw(sf::RenderTexture& surface) {
}

void pe::Panel::mouseMoved(const int mx, const int my) {
    if (_mouseDown && _isDraggable) {
        MenuComponent::move((float)mx - (float)_lastMousePos.x, (float)my - (float)_lastMousePos.y);
    }
    _lastMousePos.x = mx;
    _lastMousePos.y = my;
}

void pe::Panel::mouseButtonPressed(const int mx, const int my, const int button) {
    if (_isDraggable && getBounds().contains(mx, my)) {
        for (const auto& component : _attachedComponents) {
            if (component->hasMousePriority() && component->isActive()) return;
        }

        _mouseDown = true;
        moveToFront();
        for (const auto& component : _attachedComponents) {
            component->moveToFront();
        }
    }
}

void pe::Panel::mouseButtonReleased(const int mx, const int my, const int button) {
    _mouseDown = false;
}
