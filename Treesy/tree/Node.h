// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _NODE_H
#define _NODE_H

#include "../../PennyEngine/core/Defines.h"
#include <memory>
#include <string>
#include <vector>

class Node {
public:
    Node();

    void addChild(s_p<Node> child);
    std::vector<s_p<Node>> getChildren();

    std::string getUID() const;

    bool isActive() const;
    void deactivate();

    std::string text = "";
    std::string subString = "";
private:
    std::string _uid;

    bool _active = true;

    std::vector<s_p<Node>> _children;
};

#endif