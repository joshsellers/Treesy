// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _KEY_LISTNER_H
#define _KEY_LISTNER_H

#include "InputListener.h"
#include <SFML/Window/Keyboard.hpp>

namespace pe {
    class KeyListener : virtual public pe::intern::InputListener {
    public:
        KeyListener() { _isKeyListener = true; };

        friend class pe::intern::InputEventDistributor;
    protected:
        virtual void keyPressed(sf::Keyboard::Key& key) = 0;
        virtual void keyReleased(sf::Keyboard::Key& key) = 0;
        virtual void textEntered(sf::Uint32 character) {};
    };
}

#endif