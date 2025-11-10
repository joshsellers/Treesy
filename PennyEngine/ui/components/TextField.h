// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _TEXT_FIELD_H
#define _TEXT_FIELD_H

#include "MenuComponent.h"

class VisualNode;

namespace pe {
    class TextField : public MenuComponent {
    public:
        TextField(std::string id, float x, float y, float width, float height, std::string label, std::string defaultText = "", 
            bool autoCenter = true);

        virtual void move(sf::Vector2f delta);
        virtual sf::Text& getText();

        virtual bool hasMousePriority() const;

        friend class VisualTree;
        friend class VisualNode;
    protected:
        virtual void update();
        virtual void draw(sf::RenderTexture& surface);

        virtual void controllerButtonReleased(GAMEPAD_BUTTON button);
        virtual void controllerButtonPressed(GAMEPAD_BUTTON button);
        virtual void mouseButtonPressed(const int mx, const int my, const int button);
        virtual void mouseButtonReleased(const int mx, const int my, const int button);
        virtual void mouseMoved(const int mx, const int my);
        virtual void textEntered(const sf::Uint32 character);

        virtual void virtualKeyboardClosed();
    private:
        sf::Text _fieldText;
        sf::Vector2f _fieldTextPos;
        
        sf::Vector2i _mPos;
        bool _mouseDown = false;

        bool _isArmed = false;

        bool _lastSelected = false;

        float _minWidth, _minHeight;

        bool _autoCenter = true;

        sf::Vector2f _origin;

        float _padding = 0.f;

        void gamepadArm();
        void gamepadDisarm();

        unsigned int _cursorBlinkTimer = 0;
    };
}

#endif