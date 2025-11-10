// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _MENU_H
#define _MENU_H

#include "../input/gamepad/GamepadButtons.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include "components/MenuComponent.h"
#include "../core/Defines.h"

namespace pe {
    namespace intern {
        class UIManager;
    }
}

namespace pe {
    class Menu {
    public:
        Menu(const std::string id);

        void update();
        void draw(sf::RenderTexture& surface);

        void addComponent(s_p<MenuComponent> component);
        s_p<MenuComponent> getComponent(std::string id, bool suppressWarning = false) const;
        std::vector<s_p<MenuComponent>> getComponents() const;
        void clearComponents();

        void moveForward(MenuComponent* component);
        void moveBack(MenuComponent* component);
        void moveToFront(MenuComponent* component);

        void addChild(s_p<Menu> menu);
        s_p<Menu> getChild(std::string id) const;
        std::vector<s_p<Menu>> getChildren() const;

        Menu* getParent() const;
        bool hasParent() const;

        void open(bool closeParents = true);
        void close(bool openParent = true);

        bool isActive() const;

        void defineGamepadSelectionGrid(std::vector<std::vector<int>> grid);

        std::string getIdentifier() const;

        void enableGamepadInput();
        void disableGamepadInput();

        friend class pe::intern::UIManager;
    protected:
        void setParent(Menu* menu);

    private:
        const std::string _id;

        bool _isActive = false;

        bool _useGamepadConfig = false;

        bool _hasParent = false;
        Menu* _parent = nullptr;

        std::vector<s_p<MenuComponent>> _components;
        unsigned int _componentZTracker = 0;

        std::vector<s_p<Menu>> _children;

        int _selectedItemX = -1;
        int _selectedItemY = -1;
        std::vector<std::vector<int>> _selectionGrid;

        bool _pendingActivation = false;

        void sortComponents();

        void controllerButtonReleased(GAMEPAD_BUTTON button);
        void controllerButtonPressed(GAMEPAD_BUTTON button);

        void keyPressed(sf::Keyboard::Key& key);
        void keyReleased(sf::Keyboard::Key& key);
        void mouseButtonPressed(const int mx, const int my, const int button);
        void mouseButtonReleased(const int mx, const int my, const int button);
        void mouseMoved(const int mx, const int my);
        void mouseWheelScrolled(sf::Event::MouseWheelScrollEvent mouseWheelScroll);

        void textEntered(sf::Uint32 character);

        void virtualKeyboardClosed();
    };
}

#endif