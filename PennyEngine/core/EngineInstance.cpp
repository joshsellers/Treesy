// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "EngineInstance.h"
#include <iostream>
#include "Logger.h"
#include "../input/Gamepad/Gamepad.h"
#include "../audio/SoundManager.h"
#include "../ui/UI.h"

void pe::intern::EngineInstance::start(GameManager* gameManager) {
    Logger::start();
    SoundManager::loadSounds();

    this->gameManager = gameManager;

    sf::RenderTexture mainSurface;
    sf::Sprite mainSurfaceSprite;
    sf::RenderTexture uiSurface;
    sf::Sprite uiSurfaceSprite;
    GfxResources resources(mainSurface, mainSurfaceSprite, uiSurface, uiSurfaceSprite);

    createWindow(resources);

    UI::_instance.setSurface(&uiSurface);

    connectGamepad();

    sf::Image icon;
    if (appIconPath != "NONE" && icon.loadFromFile(appIconPath)) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    sf::Cursor cursor;
    sf::Image cursorImage;
    if (cursorImagePath != "NONE" && cursorImage.loadFromFile(cursorImagePath)) {
        cursor.loadFromPixels(cursorImage.getPixelsPtr(), cursorImage.getSize(), { 0, 0 });
        window.setMouseCursor(cursor);
    }

    if (fontPath != "NONE") {
        if (!_font.loadFromFile(fontPath)) {
            Logger::log("Could not load font from " + fontPath);
        }
    }

    gameManager->init();

    UI::_instance.createVirtualKeyboard();

    mainLoop(resources);

    shutdown();
}

void pe::intern::EngineInstance::createWindow(GfxResources& gfxResources) {
    sf::RenderTexture& mainSurface = gfxResources.mainSurface;
    sf::Sprite& mainSurfaceSprite = gfxResources.mainSurfaceSprite;
    sf::RenderTexture& uiSurface = gfxResources.uiSurface;
    sf::Sprite& uiSurfaceSprite = gfxResources.uiSurfaceSprite;

    const Resolution physicalRes = { (int)sf::VideoMode::getDesktopMode().width, (int)sf::VideoMode::getDesktopMode().height };

    if (renderRes == Resolution(0, 0)) {
        constexpr float windowSize = 0.7f;
        if (displayRes == Resolution(0, 0)) {
            renderRes = {
                (int)((float)sf::VideoMode::getDesktopMode().width * windowSize),
                (int)((float)sf::VideoMode::getDesktopMode().height * windowSize)
            };

            displayRes = renderRes;
        } else {
            renderRes = displayRes;
        }
    } else if (displayRes == Resolution(0, 0)) {
        displayRes = renderRes;
    } else if (displayRes != renderRes && autoScaleRenderRes) {
        renderRes.height = (float)renderRes.width / ((float)displayRes.width / (float)displayRes.height);
    }

    const sf::Vector2u windowRes = fullscreen ? 
        sf::Vector2u((unsigned int)physicalRes.width, (unsigned int)physicalRes.height) : sf::Vector2u((unsigned int)displayRes.width, (unsigned int)displayRes.height);

    window.create(sf::VideoMode(windowRes.x, windowRes.y), windowTitle, fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
    if (framerateLimit != 0) window.setFramerateLimit(framerateLimit);

    camera.setCenter((float)renderRes.width / 2, (float)renderRes.height / 2);
    camera.setSize(renderRes.width, renderRes.height);

    bool adjustForFullscreen = false;
    if (fullscreen && physicalRes != displayRes) {
        float scale = 1.0;
        if (displayRes.width > displayRes.height && displayRes.height * ((float)physicalRes.width / (float)displayRes.width) <= physicalRes.height) {
            scale = (float)physicalRes.width / (float)displayRes.width;
        } else {
            scale = (float)physicalRes.height / (float)displayRes.height;
        }

        displayRes.width *= scale;
        displayRes.height *= scale;

        adjustForFullscreen = true;
    }

    mainSurface.create(renderRes.width, renderRes.height);
    const sf::Texture& mainSurfaceTexture = mainSurface.getTexture();

    mainSurfaceSprite.setTexture(mainSurfaceTexture);
    if (renderRes != displayRes && autoScaleRenderRes) {
        mainSurfaceSprite.setScale((float)displayRes.width / (float)renderRes.width, (float)displayRes.height / (float)renderRes.height);

        if (adjustForFullscreen) {
            mainSurfaceSprite.setPosition((float)physicalRes.width / 2.f - (float)mainSurfaceSprite.getGlobalBounds().width / 2.f,
                (float)physicalRes.height / 2.f - (float)mainSurfaceSprite.getGlobalBounds().height / 2.f);
        }
    } else if (!autoScaleRenderRes && renderRes != displayRes) {
        float scale = 1.f;
        if (renderRes.width > renderRes.height) {
            scale = (float)displayRes.width / (float)renderRes.width;
        } else {
            scale = (float)displayRes.height / (float)renderRes.height;
        }

        mainSurfaceSprite.setScale(scale, scale);

        if (!adjustForFullscreen) {
            mainSurfaceSprite.setPosition((float)displayRes.width / 2.f - (float)mainSurfaceSprite.getGlobalBounds().width / 2.f,
                (float)displayRes.height / 2.f - (float)mainSurfaceSprite.getGlobalBounds().height / 2.f);
        } else {
            mainSurfaceSprite.setPosition((float)physicalRes.width / 2.f - (float)mainSurfaceSprite.getGlobalBounds().width / 2.f,
                (float)physicalRes.height / 2.f - (float)mainSurfaceSprite.getGlobalBounds().height / 2.f);
        }
    }

    uiSurface.create(useDisplayResForUI ? displayRes.width : renderRes.width, useDisplayResForUI ? displayRes.height : renderRes.height);
    const sf::Texture& uiSurfaceTexture = uiSurface.getTexture();

    uiSurfaceSprite.setTexture(uiSurfaceTexture);
    if (!useDisplayResForUI) {
        if (renderRes != displayRes && autoScaleRenderRes) {
            uiSurfaceSprite.setScale((float)displayRes.width / (float)renderRes.width, (float)displayRes.height / (float)renderRes.height);

            if (adjustForFullscreen) {
                uiSurfaceSprite.setPosition((float)physicalRes.width / 2.f - (float)uiSurfaceSprite.getGlobalBounds().width / 2.f,
                    (float)physicalRes.height / 2.f - (float)uiSurfaceSprite.getGlobalBounds().height / 2.f);
            }
        } else if (!autoScaleRenderRes && renderRes != displayRes) {
            float scale = 1.f;
            if (renderRes.width > renderRes.height) {
                scale = (float)displayRes.width / (float)renderRes.width;
            } else {
                scale = (float)displayRes.height / (float)renderRes.height;
            }

            uiSurfaceSprite.setScale(scale, scale);

            if (!adjustForFullscreen) {
                uiSurfaceSprite.setPosition((float)displayRes.width / 2.f - (float)uiSurfaceSprite.getGlobalBounds().width / 2.f,
                    (float)displayRes.height / 2.f - (float)uiSurfaceSprite.getGlobalBounds().height / 2.f);
            } else {
                uiSurfaceSprite.setPosition((float)physicalRes.width / 2.f - (float)uiSurfaceSprite.getGlobalBounds().width / 2.f,
                    (float)physicalRes.height / 2.f - (float)uiSurfaceSprite.getGlobalBounds().height / 2.f);
            }
        }
    } else if (adjustForFullscreen) {
        uiSurfaceSprite.setPosition((float)physicalRes.width / 2.f - (float)uiSurfaceSprite.getGlobalBounds().width / 2.f,
            (float)physicalRes.height / 2.f - (float)uiSurfaceSprite.getGlobalBounds().height / 2.f);
    }

    _inputManager.setUIMouseOffset(-uiSurfaceSprite.getPosition());
}

void pe::intern::EngineInstance::mainLoop(GfxResources& gfxResources) {
    sf::RenderTexture& mainSurface = gfxResources.mainSurface;
    sf::Sprite& mainSurfaceSprite = gfxResources.mainSurfaceSprite;
    sf::RenderTexture& uiSurface = gfxResources.uiSurface;
    sf::Sprite& uiSurfaceSprite = gfxResources.uiSurfaceSprite;

    _started = true;
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            handleEvent(event);
        }

        UI::_instance.update();
        gameManager->update();

        mainSurface.setView(camera);

        mainSurface.clear();
        gameManager->draw(mainSurface);

        mainSurface.display();

        uiSurface.clear(sf::Color::Transparent);
        gameManager->renderUI(uiSurface);
        UI::_instance.draw();
        gameManager->drawUI(uiSurface);
        uiSurface.display();

        window.clear();
        window.draw(mainSurfaceSprite);
        window.draw(uiSurfaceSprite);
        window.display();
    }
}

void pe::intern::EngineInstance::shutdown() {
    _started = false;

    gameManager->onShutdown();

    SoundManager::shutdown();
    //SteamAPI_Shutdown();
    Logger::log("SHUTDOWN");
    while (!Logger::queuesHaveFlushed()) {
        sf::sleep(sf::milliseconds(500));
    }
    Logger::stop();
}

void pe::intern::EngineInstance::handleEvent(sf::Event& event) {
    _inputManager.handleEvent(event);

    switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
    }
}

void pe::intern::EngineInstance::connectGamepad() {
    bool controllerConnected = false;
    int controllerId = -1;
    for (int i = 0; i < 7; i++) {
        if (sf::Joystick::isConnected(i)) {
            controllerConnected = true;
            controllerId = i;
            break;
        }
    }
    if (controllerId != -1) Gamepad::setControllerId(controllerId);
    Logger::log("Controller is " + (std::string)(controllerConnected ? "" : "not ") + "connected");
    Logger::log("Controller id: " + std::to_string(controllerId));
}

bool pe::intern::EngineInstance::isStarted() const {
    return _started;
}

pe::intern::InputEventDistributor& pe::intern::EngineInstance::getInputManager() {
    return _inputManager;
}

sf::Font& pe::intern::EngineInstance::getFont() {
    return _font;
}
