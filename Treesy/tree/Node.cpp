// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "Node.h"
#include "../../PennyEngine/core/Util.h"

Node::Node() {
    _uid = pe::generateUID();
}

void Node::addChild(s_p<Node> child) {
    _children.push_back(child);
}

std::vector<s_p<Node>> Node::getChildren() {
    return _children;
}

std::string Node::getUID() const {
    return _uid;
}

bool Node::isActive() const {
    return _active;
}

void Node::deactivate() {
    _active = false;
}
