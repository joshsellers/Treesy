// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _UI_H
#define _UI_H

#include "UIManager.h"
#include <SFML/System/Vector2.hpp>
#include "../core/EngineInstance.h"

#include "../PennyEngine.h"
#include "../core/Defines.h"

namespace pe {
    class UI {
    public:
        static sf::Vector2f percentToScreenPos(sf::Vector2f pos);
        static sf::Vector2f percentToScreenPos(float x, float y);
        static sf::Vector2f percentToScreenDimensions(sf::Vector2f dimensions);
        static sf::Vector2f percentToScreenDimensions(float x, float y);
        static float percentToScreenWidth(float width);
        static float percentToScreenHeight(float height);

        template <typename T>
        static void draw(T& graphic, bool convertPos = true) {
            if (!PennyEngine::isStarted()) return;
            static_assert(std::is_base_of<sf::Drawable, T>::value, "pe::UI::draw parameter must be a subclass of sf::Drawable");
            static_assert(std::is_base_of<sf::Transformable, T>::value, "pe::UI::draw parameter must be a subclass of sf::Transformable");

            if (convertPos) graphic.setPosition(percentToScreenPos(graphic.getPosition()));
            _instance.getSurface()->draw(graphic);
        }

        static s_p<Menu> addMenu(std::string id);
        static s_p<Menu> getMenu(std::string id); 
        static std::vector<s_p<Menu>> getMenus();

        static s_p<sf::Texture> getSpriteSheet();

        static sf::Vector2i getMousePos();

        static void toggleVirtualKeyboard();

        friend class pe::intern::EngineInstance;
    private:
        static inline intern::UIManager _instance;
    };
}

#endif