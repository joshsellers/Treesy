// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#define _USE_MATH_DEFINES

#include "Util.h"
#include <random>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <regex>
#include "Logger.h"


int pe::randomInt(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

bool pe::randomChance(float probability) {
    if (probability >= 1.0f) return true;
    const int range = (int)(1.f / probability);
    const int min = 0;
    const int max = range - 1;
    return pe::randomInt(min, max) == 0;
}

long long pe::currentTimeMillis() {
    auto time = std::chrono::system_clock::now();

    auto since_epoch = time.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>
        (since_epoch);

    return millis.count();
}

long long pe::currentTimeNano() {
    auto time = std::chrono::system_clock::now();

    auto since_epoch = time.time_since_epoch();
    auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>
        (since_epoch);
    return nano.count();
}

float pe::norm_0_1(float x, float min, float max) {
    return (x - min) / (max - min);
}

std::string pe::trimString(std::string str) {
    if (str.find(".") != std::string::npos) {
        for (std::string::size_type s = str.length() - 1; s > 0; --s) {
            if (str[s - 1] == '.') break;
            else if (str[s] == '0') str.erase(s, 1);
            else break;
        }
    }
    return str;
}

std::vector<std::string> pe::splitString(std::string str, std::string delimiter) {
    size_t pos = 0;
    std::vector<std::string> parsedString;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        parsedString.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    parsedString.push_back(str);

    return parsedString;
}

void pe::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

bool pe::stringStartsWith(std::string str, std::string start) {
    return str.rfind(start, 0) == 0;
}

bool pe::stringEndsWith(std::string const& fullString, std::string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

bool pe::stringContains(std::string str, std::string contained) {
    return str.find(contained) != std::string::npos;
}

bool pe::isNumber(std::string s) {
    return !s.empty() && s.find_first_not_of("0123456789.") == std::string::npos;
}

void pe::rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

float pe::degToRads(float angle) {
    return angle * (M_PI / 180.f);
}

float pe::radsToDeg(float angle) {
    return angle * 180.f / M_PI;
}

bool pe::linesIntersect(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y) {
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    return s >= 0 && s <= 1 && t >= 0 && t <= 1;
}

// fast pow for int, credit to https://stackoverflow.com/a/101613/13188071
int pe::ipow(int base, int exp) {
    int result = 1;
    while (true) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp == 0)
            break;
        base *= base;
    }

    return result;
}

double pe::round_prec(double n, int prec) {
    return std::round(n * ipow(10, prec)) / ipow(10, prec);
}

std::string pe::intToHex(int i) {
    std::stringstream stream;
    stream << "0x"
        << std::setfill('0') << std::setw(sizeof(int) * 2)
        << std::hex << i;
    return stream.str();
}

std::string pe::generateUID() {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    return ss.str();
}

std::string pe::hash(std::string text) {
    unsigned int hash = 0x811c9dc5u;
    for (int i = 0; i < text.length(); i++) {
        hash = (hash ^ (unsigned int)text.at(i)) * 0x1000193u;
    }
    hash = (hash ^ (hash >> 16)) * 0x7feb352du;
    hash = (hash ^ (hash >> 15)) * 0x846ca68bu;

    return std::to_string(hash ^ (hash >> 16));
}

std::string pe::getLocalLowPath() {
    std::string pathStr = "err";
    char* buf = nullptr;
    size_t sz = 0;
    if (_dupenv_s(&buf, &sz, "APPDATA") == 0 && buf != nullptr) {
        std::string temp(buf);
        pathStr =
            std::regex_replace(
                temp, std::regex("Roaming"),
                "LocalLow"
            );
        free(buf);
    } else {
        pe::Logger::log("Failed path retrieval");
    }
    return pathStr;
}