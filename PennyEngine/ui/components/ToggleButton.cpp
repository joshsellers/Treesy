// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "ToggleButton.h"
#include "../../PennyEngine.h"
#include "../UI.h"
#include "../../PennyEngine/core/Logger.h"

pe::ToggleButton::ToggleButton(std::string buttonId, float x, float y, float width, float height, std::string labelText, ToggleButtonListener* listner, bool centerOnCoords) :
    MenuComponent(buttonId, x, y, width, height, centerOnCoords, TOGGLE_OFF_CONFIG, true) {
    _listener = listner;

    float fontSize = UI::percentToScreenWidth(1.25f);
    _labelText.setFont(PennyEngine::getFont());
    _labelText.setCharacterSize(fontSize);
    _labelText.setFillColor(sf::Color::White);
    _labelText.setString(labelText);
}

void pe::ToggleButton::setLabelText(std::string labelText) {
    _labelText.setString(labelText);
}

void pe::ToggleButton::show() {
    _isActive = true;
    _mouseDown = false;
    _mousePos = UI::getMousePos();
}

void pe::ToggleButton::hide() {
    _isActive = false;
    setAppearance(_value ? TOGGLE_ON_CONFIG : TOGGLE_OFF_CONFIG);
}

void pe::ToggleButton::setValue(bool value) {
    _value = value;
}

bool pe::ToggleButton::getValue() const {
    return _value;
}

void pe::ToggleButton::update() {
    const sf::FloatRect bounds = getBounds();

    if (_value) {
        if (!_mouseDown && (bounds.contains(_mousePos.x, _mousePos.y) || _isSelected)) {
            setAppearance(TOGGLE_ON_HOVER_CONFIG);
        } else if (bounds.contains(_mousePos.x, _mousePos.y) || (_mouseDown && _isSelected)) {
            setAppearance(TOGGLE_ON_CLICKED_CONFIG);
        } else {
            setAppearance(TOGGLE_ON_CONFIG);
        }
    } else {
        if (!_mouseDown && (bounds.contains(_mousePos.x, _mousePos.y) || _isSelected)) {
            setAppearance(TOGGLE_OFF_HOVER_CONFIG);
        } else if (bounds.contains(_mousePos.x, _mousePos.y) || (_mouseDown && _isSelected)) {
            setAppearance(TOGGLE_OFF_CLICKED_CONFIG);
        } else {
            setAppearance(TOGGLE_OFF_CONFIG);
        }
    }

    if (!PennyEngine::playerIsUsingMouse() && !_mouseDown && !_isSelected) setAppearance(_value ? TOGGLE_ON_CONFIG : TOGGLE_OFF_CONFIG);

    _wasJustSelected = _isSelected;
}

void pe::ToggleButton::draw(sf::RenderTexture& surface) {
    _labelText.setOrigin(_labelText.getLocalBounds().width / 2.f + _labelText.getLocalBounds().left, _labelText.getLocalBounds().height / 2.f + _labelText.getLocalBounds().top);

    const sf::FloatRect bounds = getBounds();
    const float width = bounds.width;
    const float height = bounds.height;
    _labelText.setPosition(bounds.left - _labelText.getGlobalBounds().width / 2.f, bounds.top + bounds.height / 2.f);
    surface.draw(_labelText);
}

void pe::ToggleButton::move(sf::Vector2f delta) {
    _pos += delta;
    _labelText.move(delta);
}

sf::Vector2i pe::ToggleButton::getMousePos() const {
    return _mousePos;
}

void pe::ToggleButton::mouseButtonPressed(const int mx, const int my, const int button) {
    _mouseDown = true;
}

void pe::ToggleButton::mouseButtonReleased(const int mx, const int my, const int button) {
    _mouseDown = false;
    const sf::FloatRect bounds = getBounds();
    if (bounds.contains(_mousePos.x, _mousePos.y)) {
        _value = !_value;
        _listener->toggleButtonPressed(getIdentifier(), _value);
    }
}

void pe::ToggleButton::mouseMoved(const int mx, const int my) {
    _mousePos.x = mx;
    _mousePos.y = my;
}

void pe::ToggleButton::controllerButtonReleased(GAMEPAD_BUTTON button) {
    if (_isSelected && button == GAMEPAD_BUTTON::A) {
        _mouseDown = false;
        _value = !_value;
        _listener->toggleButtonPressed(getIdentifier(), _value);
    }
}

void pe::ToggleButton::controllerButtonPressed(GAMEPAD_BUTTON button) {
    if (_isSelected && button == GAMEPAD_BUTTON::A) _mouseDown = true;
}

bool pe::ToggleButton::hasMousePriority() const {
    return getBounds().contains(_mousePos.x, _mousePos.y);
}