// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _INPUT_EVENT_DISTRIBUTOR
#define _INPUT_EVENT_DISTRIBUTOR

#include <vector>
#include "InputListener.h"
#include <SFML/Window/Window.hpp>
#include "KeyListener.h"
#include "MouseListener.h"

namespace pe {
    namespace intern {
        class InputEventDistributor {
        public:
            void handleEvent(sf::Event& event);

            void addListener(InputListener* listener);

            void setUIMouseOffset(sf::Vector2f offset);
            sf::Vector2f getUIMouseOffset() const;
        private:
            std::vector<KeyListener*> _keyListeners;
            std::vector<MouseListener*> _mouseListeners;

            sf::Vector2f _uiMouseOffset;
        };
    }
}

#endif