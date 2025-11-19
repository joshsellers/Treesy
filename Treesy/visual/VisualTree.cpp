// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "VisualTree.h"
#include "../../PennyEngine/PennyEngine.h"
#include "../../PennyEngine/core/Logger.h"
#include "../core/Settings.h"

VisualTreeImpl::VisualTreeImpl() {
    PennyEngine::addInputListener(this);
}

void VisualTreeImpl::update() {
    if (!_nodeBuffer.empty()) {
        for (auto& node : _nodeBuffer) {
            _nodes.push_back(node);
            _renderNodes.push_back(node);
        }
    }
    _nodeBuffer.clear();

    int numNodesWithPriority = 0;
    for (const auto& node : _nodes) {
        if (node->isActive()) {
            node->tick();
            if (node->hasMousePriority()) {
                if (numNodesWithPriority == 1) node->releasePriority();
                else numNodesWithPriority++;
            }
        }
    }

    _nodes.erase(std::remove_if(_nodes.begin(), _nodes.end(), [](s_p<VisualNode> node) { return !node->isActive(); }), _nodes.end());
    _renderNodes.erase(std::remove_if(_renderNodes.begin(), _renderNodes.end(), [](s_p<VisualNode> node) { return !node->isActive(); }), _renderNodes.end());

    if (_nodes.size() != 0) {
        alignNode(_nodes.at(0));
        if (Settings::center) centerNodes(_nodes.at(0));
    }
}

void VisualTreeImpl::centerNodes(s_p<VisualNode> node) {
    float width = 0;
    float offset = 0;
    if (node->getChildren().size() > 0) {
        const auto& rightNode = node->getChildren().at(node->getChildren().size() - 1);
        const auto& leftNode = node->getChildren().at(0);
        width = rightNode->getPosition().x + rightNode->getBounds().width - leftNode->getPosition().x;
        float left = node->getPosition().x + node->getBounds().width / 2.f - width / 2.f;
        offset = left - leftNode->getPosition().x;
    }

    for (auto& child : node->getChildren()) {
        child->move({ offset, 0 });
        centerNodes(child);
    }
}

SubtreeWidth VisualTreeImpl::alignNode(s_p<VisualNode> node) {
    auto& children = node->getChildren();
    float nodeWidth = node->getBounds().width;

    if (children.empty()) {
        float half = nodeWidth * 0.5f;
        return { half, half };
    }

    const float horzSpace = Settings::horzSpacing;

    std::vector<SubtreeWidth> cw(children.size());
    for (size_t i = 0; i < children.size(); ++i) {
        cw[i] = alignNode(children[i]);
    }

    float step = 0.f;
    for (size_t i = 0; i + 1 < children.size(); ++i) {
        float required =
            cw[i].right + horzSpace + cw[i + 1].left;
        step = std::max(step, required);
    }

    size_t n = children.size();
    float halfSpread = ((float)(n - 1) * 0.5f) * step;

    float leftWidth = std::max(halfSpread + cw.front().left, nodeWidth * 0.5f);
    float rightWidth = std::max(halfSpread + cw.back().right, nodeWidth * 0.5f);

    float parentCenter = node->getPosition().x + nodeWidth * 0.5f;

    float leftStart = parentCenter - halfSpread;

    for (size_t i = 0; i < n; ++i) {
        float childCenter = leftStart + i * step;
        auto& child = children[i];
        float childWidth = child->getBounds().width;
        float currentCenter = child->getPosition().x + childWidth * 0.5f;

        child->move({ childCenter - currentCenter, 0 });
    }

    return { leftWidth, rightWidth };
}


void VisualTreeImpl::draw(sf::RenderTexture& surface) {
    std::sort(_renderNodes.begin(), _renderNodes.end(),
        [](s_p<VisualNode> node0, s_p<VisualNode> node1) {
            return node1->isHovered();
        }
    );

    for (const auto& node : _renderNodes) {
        if (node->isActive()) {
            node->visualize(surface);
        }
    }
}

s_p<VisualNode> VisualTreeImpl::addChild(VisualNode* parent) {
    const auto& res = PennyEngine::getRenderResolution();
    const sf::Vector2f pos = parent == nullptr ? sf::Vector2f(50, 50) : sf::Vector2f(
        parent->getPosition().x / res.width * 100.f + (parent->getBounds().width / 2.f / res.width * 100.f), 
        parent->getPosition().y / res.height * 100.f + Settings::nontermVerticalDistance
    );

    const auto& newNode = new_s_p(VisualNode, (parent, pos.x, pos.y));

    _nodeBuffer.push_back(newNode);
    return newNode;
}

static sf::Vector2f mapMouseCoordinates(const int mx, const int my) {
    const sf::Vector2i mouseCoords(mx, my);

    return PennyEngine::getWindow().mapPixelToCoords(mouseCoords, PennyEngine::getCamera());
}

void VisualTreeImpl::mouseButtonPressed(const int mx, const int my, const int button) {
    const auto mousePos = mapMouseCoordinates(mx, my);

    for (const auto& node : _nodes) {
        if (node->isActive()) {
            node->mouseButtonPressed(mousePos.x, mousePos.y, button);
        }
    }
}

void VisualTreeImpl::mouseButtonReleased(const int mx, const int my, const int button) {
    const auto mousePos = mapMouseCoordinates(mx, my);

    for (const auto& node : _nodes) {
        if (node->isActive()) {
            node->mouseButtonReleased(mousePos.x, mousePos.y, button);
        }
    }
}

void VisualTreeImpl::mouseMoved(const int mx, const int my) {
    const auto mousePos = mapMouseCoordinates(mx, my);

    for (const auto& node : _nodes) {
        if (node->isActive()) {
            node->mouseMoved(mousePos.x, mousePos.y);
        }
    }
}

void VisualTreeImpl::mouseWheelScrolled(sf::Event::MouseWheelScrollEvent mouseWheelScroll) {
    for (const auto& node : _nodes) {
        if (node->isActive()) {
            node->mouseWheelScrolled(mouseWheelScroll);
        }
    }
}

void VisualTreeImpl::textEntered(sf::Uint32 character) {
    for (const auto& node : _nodes) {
        if (node->isActive()) {
            node->textEntered(character);
        }
    }
}

std::vector<s_p<VisualNode>> VisualTreeImpl::getNodes() {
    return _nodes;
}

void VisualTreeImpl::reset() {
    _nodes.clear();
    _renderNodes.clear();
}