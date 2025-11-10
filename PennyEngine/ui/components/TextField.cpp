// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "TextField.h"
#include "../UI.h"
#include "../../core/Logger.h"

pe::TextField::TextField(std::string id, float x, float y, float width, float height, std::string label, std::string defaultText,
    bool autoCenter) : MenuComponent(id, x, y, width, height, autoCenter, TEXTFIELD_CONFIG) {

    float fontSize = UI::percentToScreenWidth(1.5f);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(sf::Color::White);
    _text.setString(label);

    _fieldText.setFont(PennyEngine::getFont());
    _fieldText.setCharacterSize(fontSize);
    _fieldText.setFillColor(sf::Color::White);
    _fieldText.setString(defaultText);

    _minWidth = UI::percentToScreenWidth(width);
    _minHeight = UI::percentToScreenHeight(height);

    _autoCenter = autoCenter;

    _padding = UI::percentToScreenWidth(1.f);
    _origin = { _pos.x + _size.x / 2.f + _padding, _pos.y + _size.y / 2.f };

    setTextPosition(50, -28);
}

void pe::TextField::update() {
    if (!_isSelected && _lastSelected) _isArmed = false;
    _lastSelected = _isSelected;

    if (((getBounds().contains(_mPos.x, _mPos.y) && !_mouseDown) || _isSelected) && !_isArmed) {
        setAppearance(TEXTFIELD_HOVER_CONFIG);
    } else if ((!_mouseDown || !getBounds().contains(_mPos.x, _mPos.y)) && !_isArmed) {
        setAppearance(TEXTFIELD_CONFIG);
    } else if (_isArmed) {
        setAppearance(TEXTFIELD_ARMED_CONFIG);
    }

    _size.x = std::max(_minWidth, _fieldText.getGlobalBounds().width + _padding * 2);
    _size.y = std::max(_minHeight, _fieldText.getGlobalBounds().height);

    if (_autoCenter) {
        _pos.x = _origin.x - _size.x / 2.f - _padding;
        _pos.y = _origin.y - _size.y / 2.f;
    }
}

void pe::TextField::draw(sf::RenderTexture& surface) {
    _fieldText.setOrigin(_fieldText.getLocalBounds().width / 2.f + _fieldText.getLocalBounds().left, _fieldText.getLocalBounds().height / 2.f + _fieldText.getLocalBounds().top);

    const sf::FloatRect bounds = getBounds();
    const float width = bounds.width;
    const float height = bounds.height;
    _fieldText.setPosition(
        bounds.left + (width / 2.f),
        bounds.top + (height / 2.f)
    );

    surface.draw(_fieldText);

    if (_isArmed) {
        sf::Text cursor;
        cursor.setString(" |");
        cursor.setFont(PennyEngine::getFont());
        cursor.setCharacterSize(_fieldText.getCharacterSize() + UI::percentToScreenWidth(0.5f));
        cursor.setFillColor(_fieldText.getFillColor());
        cursor.setOrigin(cursor.getLocalBounds().width / 2.f + cursor.getLocalBounds().left, cursor.getLocalBounds().height / 2.f + cursor.getLocalBounds().top);
        cursor.setPosition(_fieldText.getPosition().x + _fieldText.getGlobalBounds().width / 2.f, _fieldText.getPosition().y);
        constexpr unsigned int blinkRate = 24;
        if ((_cursorBlinkTimer / blinkRate) % 2) surface.draw(cursor);

        _cursorBlinkTimer++;
    }
}

void pe::TextField::move(sf::Vector2f delta) {
    _pos += delta;
    _origin += delta;
    _fieldText.move(delta);
}

void pe::TextField::controllerButtonPressed(GAMEPAD_BUTTON button) {
    if (_isSelected && button == GAMEPAD_BUTTON::A) _mouseDown = true;
}

void pe::TextField::controllerButtonReleased(GAMEPAD_BUTTON button) {
    if (_isSelected && button == GAMEPAD_BUTTON::A) {
        _mouseDown = false;
        _isArmed = true;
        gamepadArm();
    }
}

void pe::TextField::mouseButtonPressed(const int mx, const int my, const int button) {
    _mouseDown = true;
}

void pe::TextField::mouseButtonReleased(const int mx, const int my, const int button) {
    _isArmed = getBounds().contains(mx, my);
    _mouseDown = false;
}

void pe::TextField::mouseMoved(const int mx, const int my) {
    _mPos.x = mx;
    _mPos.y = my;
}

void pe::TextField::textEntered(const sf::Uint32 character) {
    if (_isArmed) {
        sf::String userInput = _fieldText.getString();
        if (character == '\b' && userInput.getSize()) {
            userInput.erase(userInput.getSize() - 1, 1);
        } else if (character != '\b') {
            userInput += character;
        }
        _fieldText.setString(userInput);
    }
}

void pe::TextField::gamepadArm() {
    for (const auto& menu : _parentMenus) {
        if (menu->isActive()) {
            menu->disableGamepadInput();
            for (const auto& component : menu->getComponents()) {
                component->blockGamepadInput = true;
            }
            break;
        }
    }

    UI::toggleVirtualKeyboard();
}

void pe::TextField::gamepadDisarm() {
    for (const auto& menu : _parentMenus) {
        if (menu->isActive()) {
            menu->enableGamepadInput();
            for (const auto& component : menu->getComponents()) {
                component->blockGamepadInput = false;
            }
            break;
        }
    }
}

void pe::TextField::virtualKeyboardClosed() {
    gamepadDisarm();
    _isArmed = false;
}

sf::Text& pe::TextField::getText() {
    return _fieldText;
}

bool pe::TextField::hasMousePriority() const {
    return getBounds().contains(_mPos.x, _mPos.y);
}