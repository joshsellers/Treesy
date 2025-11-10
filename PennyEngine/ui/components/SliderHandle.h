// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _SLIDER_HANDLE_H
#define _SLIDER_HANDLE_H

#include "MenuComponent.h"

namespace pe {
    class SliderHandle : public MenuComponent {
    public:
        SliderHandle(std::string id, float x, float y, float width, float height, bool autoCenter = true);

        friend class Slider;
    protected:
        void update();
        void draw(sf::RenderTexture& surface);
    private:
    };
}

#endif