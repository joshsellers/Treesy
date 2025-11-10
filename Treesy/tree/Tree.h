// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _TREE_H
#define _TREE_H

#include "Node.h"

class TreeImpl {
public:
    void manageNodes();

    s_p<Node> getNode(std::string uid);

    s_p<Node> getRoot();
private:
    s_p<Node> _root = new_s_p(Node, ());

    void manageNode(s_p<Node> node);

    s_p<Node> searchNodes(s_p<Node> node, std::string uid);
};

class Tree {
public:
    static void manageNodes() {
        _instance.manageNodes();
    }

    static s_p<Node> getNode(std::string uid) {
        return _instance.getNode(uid);
    }

    static s_p<Node> getRoot() {
        return _instance.getRoot();
    }
private:
    static inline TreeImpl _instance;
};

#endif