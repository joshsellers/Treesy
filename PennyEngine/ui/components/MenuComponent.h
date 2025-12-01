// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _MENU_COMPONENT_H
#define _MENU_COMPONENT_H

#include "SFML/Graphics/Font.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include "../../input/gamepad/GamepadButtons.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "ComponentAppearanceConfig.h"
#include "AppearanceConfigs.h"

namespace pe {
    class MenuComponent {
    public:
        MenuComponent(const std::string id, float x, float y, float width, float height, bool autoCenter = true, ComponentAppearanceConfig appearance = BASE_COMPONENT_CONFIG, bool square = false);

        virtual void show();
        virtual void hide();
        bool isActive() const;

        void setGamepadSelectionId(int selectionId);
        int getGamepadSelectionId() const;

        bool blockGamepadInput = false;

        virtual void setCharacterSize(float size);
        virtual void setFontColor(sf::Color color);
        virtual void setTextPosition(sf::Vector2f pos);
        void setTextPosition(float x, float y);
        virtual sf::Text& getText();

        std::string getIdentifier() const;

        virtual void setAppearance(ComponentAppearanceConfig appearance);

        void move(float x, float y);
        virtual void move(sf::Vector2f delta);

        virtual void moveForward();
        virtual void moveBack();
        virtual void moveToFront();

        virtual bool hasMousePriority() const;

        virtual sf::FloatRect getBounds() const;

        friend class Menu;
    protected:
        virtual void update() = 0;
        virtual void render(sf::RenderTexture& surface, const sf::RenderStates& states = sf::RenderStates::Default);
        virtual void constructShapes();
        virtual void drawShapes(sf::RenderTexture& surface, const sf::RenderStates& states = sf::RenderStates::Default);
        virtual void draw(sf::RenderTexture& surface) = 0;

        virtual void controllerButtonReleased(GAMEPAD_BUTTON button);
        virtual void controllerButtonPressed(GAMEPAD_BUTTON button);

        virtual void keyPressed(sf::Keyboard::Key& key);
        virtual void keyReleased(sf::Keyboard::Key& key);
        virtual void mouseButtonPressed(const int mx, const int my, const int button);
        virtual void mouseButtonReleased(const int mx, const int my, const int button);
        virtual void mouseMoved(const int mx, const int my);
        virtual void mouseWheelScrolled(sf::Event::MouseWheelScrollEvent mouseWheelScroll);
        virtual void textEntered(const sf::Uint32 character);

        virtual void virtualKeyboardClosed() {};

        std::vector<Menu*> _parentMenus;

        sf::Vector2f _pos;
        sf::Vector2f _size;

        sf::RectangleShape _shape;

        sf::Text _text;
        sf::Vector2f _textPos;

        void alignText();
        bool _disableAutomaticTextAlignment = false;

        bool _isActive = false;

        bool _isSelected = false;
        int _selectionId = 0;
        bool _disableMouseMovementDeselection = false;

        bool _hasMousePriority = false;

        ComponentAppearanceConfig _appearance;

        sf::RectangleShape _leftEdge;
        sf::RectangleShape _leftTopCorner;
        sf::RectangleShape _leftBottomCorner;
        sf::RectangleShape _center;
        sf::RectangleShape _centerTop;
        sf::RectangleShape _centerBottom;
        sf::RectangleShape _rightEdge;
        sf::RectangleShape _rightTopCorner;
        sf::RectangleShape _rightBottomCorner;
    private:
        const std::string _id;
        
        std::map<std::string, unsigned int> _zPositions;
    };
}

#endif