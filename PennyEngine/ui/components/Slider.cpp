// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "Slider.h"
#include "../UI.h"
#include "../../input/gamepad/Gamepad.h"
#include "../../core/Logger.h"

pe::Slider::Slider(std::string id, float x, float y, sf::Vector2f railSize, sf::Vector2f handleSize, std::string label, SliderListener* listener, bool autoCenter, float gamepadStepRate) 
: MenuComponent(id, x, y, railSize.x, railSize.y, autoCenter, SLIDER_RAIL_CONFIG) {
    _gamepadStepRate = gamepadStepRate;

    float fontSize = UI::percentToScreenWidth(1.5f);
    _text.setCharacterSize(fontSize);
    _text.setFillColor(sf::Color::White);
    _text.setString(label);

    _handle = new_s_p(pe::SliderHandle, (id + "_handle", x, y, handleSize.x, handleSize.y, autoCenter));
    if (!autoCenter) _handle->move(0, -_size.y / 2.f);

    _handle->constructShapes();
    constructShapes();
    setValue(0.5f);
    _lastValue = _value;

    _listener = listener;

    setTextPosition(50, 200); // default should probably be above
}

void pe::Slider::setValue(float value) {
    _value = value;
    const float newHandleX = _pos.x + (getBounds().width - _handle->getBounds().width) * value;
    _handle->move(newHandleX - _handle->getBounds().left, 0);
}

float pe::Slider::getValue() const {
    return _value;
}

void pe::Slider::move(sf::Vector2f delta) {
    _pos += delta;
    _handle->move(delta);
}

void pe::Slider::setLabelText(std::string labelText) {
    _text.setString(labelText);
}

void pe::Slider::update() {
    if (_isSelected && Gamepad::isConnected()) {
        if (Gamepad::isButtonPressed(GAMEPAD_BUTTON::DPAD_LEFT)) {
            setValue(_value - _gamepadStepRate);
            if (_value < 0) setValue(0);
        } else if (Gamepad::isButtonPressed(GAMEPAD_BUTTON::DPAD_RIGHT)) {
            setValue(_value + _gamepadStepRate);
            if (_value > 1) setValue(1);
        }
    }

    const bool mouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (((_handle->getBounds().contains(_mousePos.x, _mousePos.y) && !_mouseDown))) {
        _handle->setAppearance(BUTTON_HOVER_CONFIG);
    } else if ((!mouseDown && !_handle->getBounds().contains(_mousePos.x, _mousePos.y) && !_isSelected)) {
        _handle->setAppearance(BASE_COMPONENT_CONFIG);
    } else if ((_mouseSelected) || _isSelected) {
        _handle->setAppearance(BUTTON_CLICKED_CONFIG);
    }

    if (_mouseSelected) {
        const float handleX = _handle->getBounds().left;
        float newHandleX = _mousePos.x - _handle->getBounds().width / 2.f;
        if (newHandleX < _pos.x) newHandleX = _pos.x;
        else if (newHandleX + _handle->getBounds().width > _pos.x + getBounds().width) newHandleX = _pos.x + getBounds().width - _handle->getBounds().width;
        _handle->move(newHandleX - handleX, 0);

        _value = std::min((_handle->getBounds().left + _handle->getBounds().width / 2.f - (_pos.x + _handle->getBounds().width / 2.f)) / (getBounds().width - _handle->getBounds().width), 1.f);
    }

    if (_value != _lastValue) {
        _listener->sliderMoved(getIdentifier(), _value);
        _lastValue = _value;
    }
}

void pe::Slider::draw(sf::RenderTexture& surface) {
    _handle->render(surface);
}

sf::Vector2i pe::Slider::getMousePos() const {
    return _mousePos;
}

void pe::Slider::mouseButtonPressed(const int mx, const int my, const int button) {
    _mouseDown = true;
    if (_handle->getBounds().contains(_mousePos.x, _mousePos.y)) {
        _mouseSelected = true;
    }
}

void pe::Slider::mouseButtonReleased(const int mx, const int my, const int button) {
    _mouseDown = false;
    _mouseSelected = false;
}

void pe::Slider::mouseMoved(const int mx, const int my) {
    _mousePos.x = mx;
    _mousePos.y = my;
}

bool pe::Slider::hasMousePriority() const {
    return _handle->getBounds().contains(_mousePos.x, _mousePos.y) || _mouseSelected;
}