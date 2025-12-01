// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _TOGGLE_BUTTON_LISTENER_H
#define _TOGGLE_BUTTON_LISTENER_H

#include <string>

namespace pe {
    class ToggleButtonListener {
    public:
        friend class ToggleButton;
    protected:
        virtual void toggleButtonPressed(std::string buttonId, bool newValue) = 0;
    };
}
#endif