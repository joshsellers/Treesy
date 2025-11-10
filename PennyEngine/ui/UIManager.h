// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _UI_MANAGER_H
#define _UI_MANAGER_H

#include <SFML/Graphics/Texture.hpp>
#include "../input/Gamepad/GamepadListener.h"
#include "../input/KeyListener.h"
#include "../input/MouseListener.h"
#include "Menu.h"
#include "components/ButtonListener.h"
#include "../core/Defines.h"

namespace pe {
    namespace intern {
        class UIManager : public KeyListener, public MouseListener, public GamepadListener, public ButtonListener {
        public:
            UIManager();

            void update();
            void draw();

            void setSurface(sf::RenderTexture* surface);
            sf::RenderTexture* getSurface();

            std::vector<s_p<Menu>>& getMenus();

            s_p<sf::Texture> getSpriteSheet() const;

            void createVirtualKeyboard();
            void toggleVirtualKeyboard();

        protected:
            virtual void keyPressed(sf::Keyboard::Key& key);
            virtual void keyReleased(sf::Keyboard::Key& key);
            virtual void textEntered(sf::Uint32 character);

            virtual void mouseButtonPressed(const int mx, const int my, const int button);
            virtual void mouseButtonReleased(const int mx, const int my, const int button);
            virtual void mouseMoved(const int mx, const int my);
            virtual void mouseWheelScrolled(sf::Event::MouseWheelScrollEvent mouseWheelScroll);

            virtual void controllerButtonReleased(GAMEPAD_BUTTON button);
            virtual void controllerButtonPressed(GAMEPAD_BUTTON button);

            virtual void gamepadDisconnected();
            virtual void gamepadConnected();

            virtual void buttonPressed(std::string id);
        private:
            sf::RenderTexture* _surface = nullptr;

            std::vector<s_p<Menu>> _menus;

            s_p<sf::Texture> _spriteSheet = new_s_p(sf::Texture, ());
        };
    }
}

#endif