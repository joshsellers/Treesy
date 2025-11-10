// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _GAMEPAD_LISTENER_H
#define _GAMEPAD_LISTENER_H

#include "GamePadButtons.h"
#include "../InputListener.h"

namespace pe {
    class GamepadListener : virtual public pe::intern::InputListener {
    public:
        GamepadListener() { _isGamepadListener = true; };

        friend class Gamepad;
    protected:
        virtual void controllerButtonReleased(GAMEPAD_BUTTON button) = 0;
        virtual void controllerButtonPressed(GAMEPAD_BUTTON button) = 0;

        virtual void gamepadDisconnected() = 0;
        virtual void gamepadConnected() = 0;
    };
}

#endif