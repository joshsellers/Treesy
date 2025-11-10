// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H
#include "SFML/Graphics.hpp"

namespace pe {
    /*
        Class that manages this game.
        One of these should be created in main(), and needs to be passed to
        the engine's start() function.
        A GameManager should be created right before the start() function
        is called, but after resolution and other settings have been configured.
    */
    class GameManager {
    public:
        /*
            This function is called right before the main loop starts,
            and after the window is finalized.
            This means that anything that depends on the size of the window
            (e.g. UI components) should be configured in this function,
            not in the GameManager's constructor.
        */
        virtual void init() = 0;

        virtual void update() = 0;
        virtual void draw(sf::RenderTexture& surface) = 0;
        virtual void drawUI(sf::RenderTexture& surface) = 0;

        virtual void renderUI(sf::RenderTexture& surface);

        virtual void onShutdown() = 0;
    protected:

    private:

    };
}

#endif