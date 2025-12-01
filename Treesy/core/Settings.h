// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <SFML/Graphics/Color.hpp>
#include "../../PennyEngine/core/Logger.h"

class Settings {
public:
    static inline bool showTermLines = false;
    static inline bool center = false;

    static inline sf::Color bgColor = sf::Color(0xFFFFFFFF);
    static inline sf::Color lineColor = sf::Color::Black;
    static inline sf::Color nonTermColor = sf::Color::Blue;
    static inline sf::Color termColor = sf::Color(0x008000FF);

    static inline float nontermVerticalDistance = 8.f;
    static inline float termVerticalDistance = 6.f;

    static float getTermDistance() {
        return showTermLines ? nontermVerticalDistance : termVerticalDistance;
    };

    static inline bool enableTriangles = true;

    static inline float horzSpacing = 0.f;

    static void save() {
        std::ofstream out("settings.ini");
        try {
            out << "bgColor=" << std::to_string(bgColor.toInteger()) << std::endl;
            out << "lineColor=" << std::to_string(lineColor.toInteger()) << std::endl;
            out << "nonTermColor=" << std::to_string(nonTermColor.toInteger()) << std::endl;
            out << "termColor=" << std::to_string(termColor.toInteger()) << std::endl;
            out << "showTermLines=" << std::to_string(showTermLines) << std::endl;
            out << "horzSpacing=" << std::to_string(horzSpacing) << std::endl;
        } catch (std::exception ex) {
            pe::Logger::log(ex.what());
        }
        out.close();
    }

    static void load() {
        std::ifstream in("settings.ini");
        if (in.good()) {
            std::string line;
            while (getline(in, line)) {
                std::vector<std::string> parsedLine = pe::splitString(line, "=");
                if (parsedLine[0] == "bgColor") bgColor = sf::Color(std::stoul(parsedLine[1]));
                else if (parsedLine[0] == "lineColor") lineColor = sf::Color(std::stoul(parsedLine[1]));
                else if (parsedLine[0] == "nonTermColor") nonTermColor = sf::Color(std::stoul(parsedLine[1]));
                else if (parsedLine[0] == "termColor") termColor = sf::Color(std::stoul(parsedLine[1]));
                else if (parsedLine[0] == "showTermLines") showTermLines = parsedLine[1] == "1";
                else if (parsedLine[0] == "horzSpacing") horzSpacing = std::stof(parsedLine[1]);
            }
        } else {
            pe::Logger::log("Did not find settings.ini");
        }

        in.close();
    }
private:
};

#endif