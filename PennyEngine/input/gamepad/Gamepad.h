// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _GAMEPAD_H
#define _GAMEPAD_H

#include "GamepadButtons.h"
#include <vector>
#include "GamepadListener.h"
#include <SFML/Window/Event.hpp>

namespace pe {
    constexpr unsigned int SONY_VID = 0x054C;
    constexpr unsigned int DUALSENSE_PID = 0x0CE6;

    class Gamepad {
    public:
        static void setControllerId(unsigned int id);

        static unsigned int getControllerId();
        static unsigned int getVendorId();
        static unsigned int getProductId();

        static void vibrate(int vibrationAmount, long long time);

        static float getLeftStickXAxis();

        static float getLeftStickYAxis();

        static float getRightStickXAxis();

        static float getRightStickYAxis();

        static bool isButtonPressed(GAMEPAD_BUTTON button);

        static void setDeadZone(float deadZone);

        static float getDeadZone();

        static bool isLeftStickDeadZoned();

        static bool isRightStickDeadZoned();

        static bool isConnected();

        friend class pe::intern::InputEventDistributor;
    private:
        inline static unsigned int _id = 0;

        inline static unsigned int _vid = 0;
        inline static unsigned int _pid = 0;

        inline static float _deadZone = 10.f;

        inline static float _lastLeftTriggerValue = 0.f;
        inline static float _lastRightTriggerValue = 0.f;
        inline static float _triggerDeadZone = 1.f;
        inline static float _triggerPressThreshold = 50.f;

        inline static float _lastDPadXValue = 0.f;
        inline static float _lastDPadYValue = 0.f;

        inline static bool _triggerIsPressed[(int)GAMEPAD_BUTTON::DPAD_RIGHT - (int)GAMEPAD_BUTTON::LEFT_TRIGGER + 1];

        inline static bool _isVibrating = false;

        static void runVibration(int vibrationAmount, long long time);

        static float removeDeadZone(float axisValue);

        static void updateControllerAxisValue(GAMEPAD_AXIS axis, float value);

        inline static std::vector<GamepadListener*> _listeners;
        static void listenerButtonReleaseCallback(GAMEPAD_BUTTON button);

        static void listenerButtonPressCallback(GAMEPAD_BUTTON button);

        static GAMEPAD_BUTTON translateButton(GAMEPAD_BUTTON button);

        static void addListener(GamepadListener* listener);

        static void receiveControllerEvent(sf::Event& event);
    };
}

#endif