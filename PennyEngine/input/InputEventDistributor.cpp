// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "InputEventDistributor.h"
#include "Gamepad/Gamepad.h"
#include "../PennyEngine.h"

void pe::intern::InputEventDistributor::handleEvent(sf::Event& event) {
    switch (event.type) {
        case sf::Event::TextEntered: 
        {
            for (const auto& listener : _keyListeners) listener->textEntered(event.text.unicode);
            break;
        }
        case sf::Event::KeyPressed:
        {
            for (const auto& listener : _keyListeners) listener->keyPressed(event.key.code);
            break;
        }
        
        case sf::Event::KeyReleased:
        {
            for (const auto& listener : _keyListeners) listener->keyReleased(event.key.code);
            break;
        }

        case sf::Event::MouseButtonPressed:
        {
            for (const auto& listener : _mouseListeners) listener->mouseButtonPressed(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            for (const auto& listener : _mouseListeners) listener->mouseButtonReleased(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
            break;
        }

        case sf::Event::MouseMoved:
        {
            PennyEngine::getWindow().setMouseCursorVisible(true);
            PennyEngine::_usingMouse = true;
            for (const auto& listener : _mouseListeners) listener->mouseMoved(event.mouseMove.x, event.mouseMove.y);
            break;
        }

        case sf::Event::MouseWheelScrolled:
        {
            for (const auto& listener : _mouseListeners) listener->mouseWheelScrolled(event.mouseWheelScroll);
            break;
        }

        case sf::Event::JoystickConnected:
            Gamepad::receiveControllerEvent(event);
            break;
        case sf::Event::JoystickDisconnected:
            Gamepad::receiveControllerEvent(event);
            break;
        case sf::Event::JoystickMoved:
            PennyEngine::getWindow().setMouseCursorVisible(false);
            PennyEngine::_usingMouse = false;
            Gamepad::receiveControllerEvent(event);
            break;
        case sf::Event::JoystickButtonReleased:
            PennyEngine::getWindow().setMouseCursorVisible(false);
            PennyEngine::_usingMouse = false;
            Gamepad::receiveControllerEvent(event);
            break;
        case sf::Event::JoystickButtonPressed:
            PennyEngine::getWindow().setMouseCursorVisible(false);
            PennyEngine::_usingMouse = false;
            Gamepad::receiveControllerEvent(event);
            break;
    }
}

void pe::intern::InputEventDistributor::addListener(InputListener* listener) {
    if (listener->_isKeyListener) {
        _keyListeners.push_back(dynamic_cast<KeyListener*>(listener));
    }

    if (listener->_isMouseListener) {
        _mouseListeners.push_back(dynamic_cast<MouseListener*>(listener));
    }

    if (listener->_isGamepadListener) {
        pe::Gamepad::addListener(dynamic_cast<GamepadListener*>(listener));
    }
}

void pe::intern::InputEventDistributor::setUIMouseOffset(sf::Vector2f offset) {
    _uiMouseOffset = offset;
}

sf::Vector2f pe::intern::InputEventDistributor::getUIMouseOffset() const {
    return _uiMouseOffset;
}
