// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _SLIDER_LISTENER_H
#define _SLIDER_LISTENER_H

#include <string>

namespace pe {
    class SliderListener {
    public:
        friend class Slider;
    protected:
        virtual void sliderMoved(std::string sliderId, float value) = 0;
    };
}

#endif