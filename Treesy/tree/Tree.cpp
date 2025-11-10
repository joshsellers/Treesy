// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "Tree.h"

void TreeImpl::manageNodes() {
    manageNode(_root);
}

s_p<Node> TreeImpl::getNode(std::string uid) {
    return searchNodes(_root, uid);
}

s_p<Node> TreeImpl::searchNodes(s_p<Node> node, std::string uid) {
    if (node->getUID() == uid) return node;

    for (const auto& child : node->getChildren()) {
        const auto& retChild = searchNodes(child, uid);
        if (retChild != nullptr) return retChild;
    }

    return nullptr;
}

void TreeImpl::manageNode(s_p<Node> node) {
    for (auto& child : node->getChildren()) {
        manageNode(child);
    }

    //node->getChildren().erase(std::remove_if(node->getChildren().begin(), node->getChildren().end(), [](s_p<Node> child) { return !child->isActive(); }), node->getChildren().end());
}

s_p<Node> TreeImpl::getRoot() {
    return _root;
}
