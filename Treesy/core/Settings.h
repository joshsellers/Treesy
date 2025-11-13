// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <SFML/Graphics/Color.hpp>

class Settings {
public:
    static inline bool showTermLines = false;
    static inline bool center = false;

    static inline sf::Color bgColor = sf::Color(0xFFFFFFFF);
    static inline sf::Color lineColor = sf::Color::Black;
    static inline sf::Color nonTermColor = sf::Color::Blue;
    static inline sf::Color termColor = sf::Color(0x008000FF);

    static inline float nontermVerticalDistance = 8.f;
    static inline float termVerticalDistance = showTermLines ? nontermVerticalDistance : 6.f;

    static inline float horzSpacing = 0.f;
private:

};

#endif