// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _ENGINE_INSTANCE_H
#define _ENGINE_INSTANCE_H

#include "GameManager.h"
#include "Resolution.h"
#include "../input/InputEventDistributor.h"

namespace pe {
    namespace intern {
        struct GfxResources {
            GfxResources(sf::RenderTexture& mainSurface, sf::Sprite& mainSurfaceSprite, sf::RenderTexture& uiSurface, sf::Sprite& uiSurfaceSprite) :
                mainSurface(mainSurface), mainSurfaceSprite(mainSurfaceSprite), uiSurface(uiSurface), uiSurfaceSprite(uiSurfaceSprite)
            {}

            sf::RenderTexture& mainSurface;
            sf::Sprite& mainSurfaceSprite;
            sf::RenderTexture& uiSurface;
            sf::Sprite& uiSurfaceSprite;
        };

        class EngineInstance {
        public:
            void start(GameManager* gameManager);
            GameManager* gameManager = nullptr;

            sf::RenderWindow window;
            int framerateLimit = 0;
            Resolution renderRes;
            Resolution displayRes;
            bool autoScaleRenderRes = true;
            bool useDisplayResForUI = true;

            std::string appIconPath = "NONE";
            std::string cursorImagePath = "NONE";

            bool fullscreen = false;
            std::string windowTitle = "";

            sf::View camera;

            bool isStarted() const;

            InputEventDistributor& getInputManager();

            std::string fontPath = "NONE";
            sf::Font& getFont();
        private:
            void createWindow(GfxResources& gfxResources);
            void mainLoop(GfxResources& gfxResources);
            void shutdown();

            void handleEvent(sf::Event& event);

            void connectGamepad();

            bool _started = false;

            InputEventDistributor _inputManager;

            sf::Font _font;
        };
    }
}

#endif