// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _SLIDER_H
#define _SLIDER_H

#include "MenuComponent.h"
#include "SliderHandle.h"
#include "../../core/Defines.h"
#include "SliderListener.h"

namespace pe {
    class Slider : public MenuComponent {
    public:
        Slider(std::string id, float x, float y, sf::Vector2f railSize, sf::Vector2f handleSize, std::string label, SliderListener* listener, bool autoCenter = true, float gamepadStepRate = 0.01);

        void setValue(float value);
        float getValue() const;

        virtual void move(sf::Vector2f delta);

        void setLabelText(std::string labelText);

        virtual bool hasMousePriority() const;
    protected:
        void update();
        void draw(sf::RenderTexture& surface);

        sf::Vector2i getMousePos() const;

        virtual void mouseButtonPressed(const int mx, const int my, const int button);
        virtual void mouseButtonReleased(const int mx, const int my, const int button);
        virtual void mouseMoved(const int mx, const int my);
    private:
        float _lastValue = 0;
        float _value = 0;

        float _gamepadStepRate = 0.01f;

        bool _mouseDown = false;
        bool _mouseSelected = false;

        sf::Vector2i _mousePos;

        SliderListener* _listener = nullptr;

        s_p<SliderHandle> _handle = nullptr;
    };
}

#endif