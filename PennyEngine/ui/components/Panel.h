// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _PANEL_H
#define _PANEL_H

#include "MenuComponent.h"
#include "../../core/Defines.h"

namespace pe {
    /*
        If a Panel is to be added to a Menu, it should
        be the last MenuComponent added. Otherwise, 
        any other components in the Menu will not be 
        visible.

        A Component should not be attached to a Panel
        until the Menu containing that component
        has been added to the UI via pe::UI::addMenu()
    */
    class Panel : public MenuComponent {
    public:
        Panel(std::string identifier, float x, float y, float width, float height, std::string titleText = "", bool draggable = false, bool autoCenter = true);

        void setTitle(std::string text);

        virtual void move(sf::Vector2f delta);

        void attach(s_p<MenuComponent> component);
        void attach(std::string identifier);
    protected:
        void update();
        void draw(sf::RenderTexture& surface);

        virtual void mouseButtonPressed(const int mx, const int my, const int button);
        virtual void mouseButtonReleased(const int mx, const int my, const int button);
        virtual void mouseMoved(const int mx, const int my);

        bool _mouseDown = false;
    private:
        std::vector<s_p<MenuComponent>> _attachedComponents;

        sf::Vector2i _lastMousePos;

        bool _isDraggable = false;
    };
}

#endif