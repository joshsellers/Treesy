// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <SFML/Graphics/Color.hpp>

class Settings {
public:
    static inline bool showTermLines = false;

    static inline sf::Color bgColor = sf::Color(0xDDDDDDFF);
    static inline sf::Color lineColor = sf::Color::Black;
    static inline sf::Color nonTermColor = sf::Color::Blue;
    static inline sf::Color termColor = sf::Color(0x008000FF);
private:

};

#endif