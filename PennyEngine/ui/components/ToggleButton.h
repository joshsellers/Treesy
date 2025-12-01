// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _TOGGLE_BUTTON_H
#define _TOGGLE_BUTTON_H

#include "MenuComponent.h"
#include "ToggleButtonListener.h"

namespace pe {
    class ToggleButton : public pe::MenuComponent {
    public:
        ToggleButton(std::string buttonId, float x, float y, float width, float height, std::string labelText, ToggleButtonListener* listner, bool centerOnCoords = true);

        void setLabelText(std::string labelText);

        virtual void show();
        virtual void hide();

        virtual bool hasMousePriority() const;

        void setValue(bool value);
        bool getValue() const;

        void move(sf::Vector2f detla);

    protected:
        bool _mouseDown = false;

        void update();
        void draw(sf::RenderTexture& surface);

        sf::Vector2i getMousePos() const;

        virtual void mouseButtonPressed(const int mx, const int my, const int button);
        virtual void mouseButtonReleased(const int mx, const int my, const int button);
        virtual void mouseMoved(const int mx, const int my);

        virtual void controllerButtonReleased(GAMEPAD_BUTTON button);
        virtual void controllerButtonPressed(GAMEPAD_BUTTON button);
    private:
        sf::Vector2i _mousePos;

        ToggleButtonListener* _listener = nullptr;

        bool _wasJustSelected = false;

        sf::Text _labelText;

        bool _value = false;
    };
}

#endif