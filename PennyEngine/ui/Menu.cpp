// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "Menu.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include "../PennyEngine.h"
#include "../core/Logger.h"

pe::Menu::Menu(const std::string id) : _id(id) {}

void pe::Menu::update() {
    sortComponents();

    for (auto& component : _components) {
        if (component->isActive()) component->update();
    }

    if (_pendingActivation) {
        for (auto& component : _components) {
            component->show();

            if (!PennyEngine::playerIsUsingMouse() && _useGamepadConfig) {
                if (component->isActive()
                    && component->_selectionId == _selectionGrid.at(_selectedItemY).at(_selectedItemX)) {
                    component->_isSelected = true;
                } else component->_isSelected = false;
            }
        }

        _pendingActivation = false;
    }
}

void pe::Menu::draw(sf::RenderTexture& surface) {
    for (auto& component : _components) {
        if (component->isActive()) component->render(surface);
    }
}

void pe::Menu::addComponent(s_p<MenuComponent> component) {
    _components.push_back(component);
    component->_parentMenus.push_back(this);
    component->_zPositions[getIdentifier()] = _componentZTracker;
    _componentZTracker++;
}

s_p<pe::MenuComponent> pe::Menu::getComponent(std::string id, bool suppressWarning) const {
    for (const auto& component : _components) {
        if (component->getIdentifier() == id) return component;
    }

    if (!suppressWarning) {
        Logger::log("Did not find menu component with id \"" + id + "\"");
    }
    return nullptr;
}

std::vector<s_p<pe::MenuComponent>> pe::Menu::getComponents() const {
    return _components;
}

void pe::Menu::clearComponents() {
    _components.clear();
}

void pe::Menu::moveForward(MenuComponent* component) {
    if (component->_zPositions.at(getIdentifier()) == 0) return;
    component->_zPositions.at(getIdentifier())--;

    sortComponents();
    for (int i = getComponents().size() - 1; i >= 0; i--) {
        const auto& otherComponent = _components.at(i);
        if (otherComponent->getIdentifier() != component->getIdentifier()) {
            if (otherComponent->_zPositions.at(getIdentifier()) == component->_zPositions.at(getIdentifier())) {
                otherComponent->_zPositions.at(getIdentifier())++;
                return;
            }
        }
    }
}

void pe::Menu::moveBack(MenuComponent* component) {
    component->_zPositions.at(getIdentifier())++;

    sortComponents();
    for (int i = 0; i < getComponents().size(); i++) {
        const auto& otherComponent = _components.at(i);
        if (otherComponent->getIdentifier() != component->getIdentifier()) {
            if (otherComponent->_zPositions.at(getIdentifier()) == component->_zPositions.at(getIdentifier())) {
                otherComponent->_zPositions.at(getIdentifier())--;
                return;
            }
        }
    }
}

void pe::Menu::moveToFront(MenuComponent* component) {
    if (component->_zPositions.at(getIdentifier()) == 0) return;
    sortComponents();

    for (int i = 0; i < getComponents().size(); i++) {
        const auto& otherComponent = _components.at(i);
        if (otherComponent->getIdentifier() != component->getIdentifier()) {
            if (otherComponent->_zPositions.at(getIdentifier()) > component->_zPositions.at(getIdentifier())) {
                otherComponent->_zPositions.at(getIdentifier())--;
            }
        } else break;
    }

    component->_zPositions.at(getIdentifier()) = 0;
    if (_components.at(_components.size() - 1)->_zPositions.at(getIdentifier()) == 0) {
        for (int i = getComponents().size() - 1; i >= 0; i--) {
            const auto& otherComponent = _components.at(i);
            if (otherComponent->getIdentifier() != component->getIdentifier()) {
                otherComponent->_zPositions.at(getIdentifier())++;
            }
        }
    }
}

void pe::Menu::sortComponents() {
    std::sort(_components.begin(), _components.end(),
        [this](s_p<MenuComponent> component0, s_p<MenuComponent> component1) {
            return component0->_zPositions.at(getIdentifier()) > component1->_zPositions.at(getIdentifier());
        }
    );
}

void pe::Menu::addChild(s_p<Menu> menu) {
    menu->setParent(this);
    _children.push_back(menu);
}

s_p<pe::Menu> pe::Menu::getChild(std::string id) const {
    for (const auto& child : _children) {
        if (child->getIdentifier() == id) return child;
    }

    Logger::log("Did not find child menu with id \"" + id + "\"");
    return nullptr;
}

std::vector<s_p<pe::Menu>> pe::Menu::getChildren() const {
    return _children;
}

void pe::Menu::setParent(Menu* menu) {
    _hasParent = true;
    _parent = menu;
}

pe::Menu* pe::Menu::getParent() const {
    return _parent;
}

void pe::Menu::open(bool closeParents) {
    _isActive = true;
    _pendingActivation = true;

    if (!PennyEngine::playerIsUsingMouse() && _useGamepadConfig) {
        if (_selectedItemX == -1) _selectedItemX = 0;
        if (_selectedItemY == -1) _selectedItemY = 0;
    }

    if (_hasParent && closeParents) {
        _parent->close(false);
    }
}

void pe::Menu::close(bool openParent) {
    _isActive = false;
    for (auto& component : _components) {
        component->hide();
        component->_isSelected = false;
    }

    _selectedItemX = -1;
    _selectedItemY = -1;

    if (_hasParent && openParent) _parent->open();
}

bool pe::Menu::isActive() const {
    return _isActive;
}

void pe::Menu::defineGamepadSelectionGrid(std::vector<std::vector<int>> grid) {
    _useGamepadConfig = true;
    _selectionGrid = grid;
}

std::string pe::Menu::getIdentifier() const {
    return _id;
}

void pe::Menu::controllerButtonReleased(GAMEPAD_BUTTON button) {
    for (auto& component : _components) {
        if (component->isActive() && !component->blockGamepadInput) component->controllerButtonReleased(button);
    }
}

void pe::Menu::controllerButtonPressed(GAMEPAD_BUTTON button) {
    if (_useGamepadConfig && (int)button >= (int)GAMEPAD_BUTTON::DPAD_UP) {
        if (_selectedItemX == -1) _selectedItemX = 0;
        if (_selectedItemY == -1) _selectedItemY = 0;

        switch (button) {
            case GAMEPAD_BUTTON::DPAD_UP:
                if (_selectedItemY > 0) {
                    _selectedItemY--;
                    if (_selectedItemX > (int)_selectionGrid.at(_selectedItemY).size() - 1) _selectedItemX = 0;
                }
                break;
            case GAMEPAD_BUTTON::DPAD_DOWN:
                if (_selectedItemY < (int)_selectionGrid.size() - 1) {
                    _selectedItemY++;
                }
                if (_selectedItemX > (int)_selectionGrid.at(_selectedItemY).size() - 1) _selectedItemX = 0;
                break;
            case GAMEPAD_BUTTON::DPAD_LEFT:
                if (_selectedItemX > 0) _selectedItemX--;
                break;
            case GAMEPAD_BUTTON::DPAD_RIGHT:
                if (_selectedItemX < (int)_selectionGrid.at(_selectedItemY).size() - 1) {
                    _selectedItemX++;
                }
                break;
        }
        for (auto& component : _components) {
            if (component->isActive()
                && component->_selectionId == _selectionGrid.at(_selectedItemY).at(_selectedItemX)) {
                component->_isSelected = true;
            } else component->_isSelected = false;
        }
    }

    for (auto& component : _components) {
        if (component->isActive() && !component->blockGamepadInput) component->controllerButtonPressed(button);
    }
}

void pe::Menu::keyPressed(sf::Keyboard::Key& key) {
    for (auto& component : _components) {
        if (component->isActive()) component->keyPressed(key);
    }
}

void pe::Menu::keyReleased(sf::Keyboard::Key& key) {
    for (auto& component : _components) {
        if (component->isActive()) component->keyReleased(key);
    }
}

void pe::Menu::mouseButtonPressed(const int mx, const int my, const int button) {
    for (auto& component : _components) {
        if (component->isActive()) component->mouseButtonPressed(mx, my, button);
    }
}

void pe::Menu::mouseButtonReleased(const int mx, const int my, const int button) {
    for (auto& component : _components) {
        if (component->isActive()) component->mouseButtonReleased(mx, my, button);
    }
}

void pe::Menu::mouseMoved(const int mx, const int my) {
    _selectedItemX = -1;
    _selectedItemY = -1;

    for (auto& component : _components) {
        if (component->isActive()) {
            component->mouseMoved(mx, my);
            if (!component->_disableMouseMovementDeselection) component->_isSelected = false;
        }
    }
}

void pe::Menu::mouseWheelScrolled(sf::Event::MouseWheelScrollEvent mouseWheelScroll) {
    for (auto& component : _components) {
        if (component->isActive()) component->mouseWheelScrolled(mouseWheelScroll);
    }
}

void pe::Menu::textEntered(sf::Uint32 character) {
    for (auto& component : _components) {
        if (component->isActive()) component->textEntered(character);
    }
}

void pe::Menu::enableGamepadInput() {
    _useGamepadConfig = true;
}

void pe::Menu::disableGamepadInput() {
    _useGamepadConfig = false;
}

void pe::Menu::virtualKeyboardClosed() {
    for (const auto& component : _components) {
        component->virtualKeyboardClosed();
    }
}