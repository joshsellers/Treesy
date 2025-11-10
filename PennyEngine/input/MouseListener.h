// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _MOUSE_LISTENER_H
#define _MOUSE_LISTENER_H

#include "InputListener.h"
#include <SFML/Window/Event.hpp>

namespace pe {
    class MouseListener : virtual public pe::intern::InputListener {
    public:
        MouseListener() { _isMouseListener = true; };

        friend class pe::intern::InputEventDistributor;
    protected:
        virtual void mouseButtonPressed(const int mx, const int my, const int button) = 0;
        virtual void mouseButtonReleased(const int mx, const int my, const int button) = 0;
        virtual void mouseMoved(const int mx, const int my) = 0;
        virtual void mouseWheelScrolled(sf::Event::MouseWheelScrollEvent mouseWheelScroll) = 0;
    };
}

#endif