// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _PROGRAM_MANAGER_H
#define _PROGRAM_MANAGER_H

#include "../../PennyEngine/core/GameManager.h"
#include "../../PennyEngine/input/KeyListener.h"
#include "../../PennyEngine/input/MouseListener.h"

class ProgramManager : public pe::GameManager, public pe::MouseListener, public pe::KeyListener {
public:
    ProgramManager();

    virtual void init();

    virtual void update();
    virtual void draw(sf::RenderTexture& surface);
    virtual void drawUI(sf::RenderTexture& surface);
protected:
    virtual void buttonPressed(std::string buttonId);
    virtual void sliderMoved(std::string sliderId, float value);

    virtual void keyPressed(sf::Keyboard::Key& key);
    virtual void keyReleased(sf::Keyboard::Key& key);

    virtual void mouseButtonPressed(const int mx, const int my, const int button);
    virtual void mouseButtonReleased(const int mx, const int my, const int button);
    virtual void mouseMoved(const int mx, const int my);
    virtual void mouseWheelScrolled(sf::Event::MouseWheelScrollEvent mouseWheelScroll);

    virtual void onShutdown();
private:
    sf::Vector2i _clickPos;
    bool _clickedIntoNode = false;
};

#endif