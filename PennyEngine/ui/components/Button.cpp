// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "Button.h"
#include "../UI.h"
#include "../../core/Logger.h"

pe::Button::Button(std::string buttonId, float x, float y, float width, float height, std::string labelText, ButtonListener* listner, bool centerOnCoords) :
MenuComponent(buttonId, x, y, width, height, centerOnCoords, BUTTON_CONFIG) {
    _listener = listner;

    float fontSize = UI::percentToScreenWidth(1.5f);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(sf::Color::White);
    _text.setString(labelText);
}

void pe::Button::setLabelText(std::string labelText) {
    _text.setString(labelText);
}

void pe::Button::show() {
    _isActive = true;
    _mouseDown = false;
    _mousePos = UI::getMousePos();
}

void pe::Button::hide() {
    _isActive = false;
    setAppearance(BASE_COMPONENT_CONFIG);
}

void pe::Button::update() {
    const sf::FloatRect bounds = getBounds();

    if (!_mouseDown && (bounds.contains(_mousePos.x, _mousePos.y) || _isSelected)) {
        setAppearance(BUTTON_HOVER_CONFIG);
    } else if (bounds.contains(_mousePos.x, _mousePos.y) || (_mouseDown && _isSelected)) {
        setAppearance(BUTTON_CLICKED_CONFIG);
    } else {
        setAppearance(BUTTON_CONFIG);
    }

    if (!PennyEngine::playerIsUsingMouse() && !_mouseDown && !_isSelected) setAppearance(BUTTON_CONFIG);

    if (pressWhenSelected && _isSelected && !_wasJustSelected) _listener->buttonPressed(getIdentifier());
    _wasJustSelected = _isSelected;
}

void pe::Button::draw(sf::RenderTexture& surface) {
}

sf::Vector2i pe::Button::getMousePos() const {
    return _mousePos;
}

void pe::Button::mouseButtonPressed(const int mx, const int my, const int button) {
    _mouseDown = true;
}

void pe::Button::mouseButtonReleased(const int mx, const int my, const int button) {
    _mouseDown = false;
    const sf::FloatRect bounds = getBounds();
    if (bounds.contains(_mousePos.x, _mousePos.y)) {
        _listener->buttonPressed(getIdentifier());
    }
}

void pe::Button::mouseMoved(const int mx, const int my) {
    _mousePos.x = mx;
    _mousePos.y = my;
}

void pe::Button::controllerButtonReleased(GAMEPAD_BUTTON button) {
    if (_isSelected && button == GAMEPAD_BUTTON::A && !pressWhenSelected) {
        _mouseDown = false;
        _listener->buttonPressed(getIdentifier());
    }
}

void pe::Button::controllerButtonPressed(GAMEPAD_BUTTON button) {
    if (_isSelected && button == GAMEPAD_BUTTON::A && !pressWhenSelected) _mouseDown = true;
}

bool pe::Button::hasMousePriority() const {
    return getBounds().contains(_mousePos.x, _mousePos.y);
}
