// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "VisualNode.h"
#include "../tree/Node.h"
#include "../tree/Tree.h"
#include "../../PennyEngine/PennyEngine.h"
#include "../../PennyEngine/ui/UI.h"
#include "VisualTree.h"
#include "../../PennyEngine/core/Logger.h"
#include "Line.h"
#include "../core/Settings.h"


VisualNode::VisualNode(VisualNode* parent, float x, float y) : TextField(pe::generateUID(), x, y, 3, 5, "", "XP") {
    _parent = parent;
    show();
    _fieldText.setFillColor(Settings::nonTermColor);

    constructShapes();

    const float buttonSize = pe::UI::percentToScreenWidth(1.25f);
    _plusButton.setSize({ buttonSize, buttonSize });
    _plusButton.setPosition(_pos.x + getBounds().width - _plusButton.getSize().x, _pos.y + getBounds().height / 2.f - _plusButton.getSize().y);
    _plusButton.setTexture(pe::UI::getSpriteSheet().get());
    _plusButton.setTextureRect({
        0, 0, 16, 16    
    });

    _minusButton.setSize({ buttonSize, buttonSize });
    _minusButton.setPosition(_pos.x + _minusButton.getSize().x, _pos.y + getBounds().height / 2.f - _minusButton.getSize().y);
    _minusButton.setTexture(pe::UI::getSpriteSheet().get()); 
    _minusButton.setTextureRect({
        16, 0, 16, 16
    });
}

void VisualNode::tick() {
    update();
}

void VisualNode::visualize(sf::RenderTexture& surface) {
    if (!_hideInterface) {
        render(surface);
    } else {
        constructShapes();

        alignText();
        surface.draw(_text);

        draw(surface);
    }
}

sf::Vector2f VisualNode::getPosition() const {
    return { getBounds().left, getBounds().top };
}


void VisualNode::addChild() {
    if (!hasChildren() && hasParent() && getParent()->getChildren().size() == 1) move({ 0, pe::UI::percentToScreenHeight(4.f) });
    const auto& child = VisualTree::addChild(this);
    _children.push_back(child);
}

void VisualNode::connectToParent(sf::RenderTexture& surface) {
    if (_parent != nullptr && (Settings::showTermLines || hasChildren() || getParent()->getChildren().size() > 1)) {
        const Line line(
            {_parent->getPosition().x + _parent->getBounds().width / 2.f, _parent->getPosition().y + _parent->getBounds().height}, 
            {getPosition().x + getBounds().width / 2.f, getPosition().y }
        );
        surface.draw(line);
    }
}

void VisualNode::update() {
    if (hasParent() && !_parent->isActive()) hide();

    if (!_isSelected && _lastSelected) _isArmed = false;
    _lastSelected = _isSelected;

    if (((getBounds().contains(_mPos.x, _mPos.y) && !_mouseDown) || _isSelected) && !_isArmed) {
        setAppearance(pe::TEXTFIELD_HOVER_CONFIG);
        _hideInterface = false;
    } else if ((!_mouseDown || !getBounds().contains(_mPos.x, _mPos.y)) && !_isArmed) {
        setAppearance(pe::TEXTFIELD_CONFIG);
        _hideInterface = true;
    } else if (_isArmed) {
        setAppearance(pe::TEXTFIELD_ARMED_CONFIG);
        _hideInterface = false;
    }

    _size.x = std::max(_minWidth, _fieldText.getGlobalBounds().width + _padding * 2);
    _size.y = std::max(_minHeight, _fieldText.getGlobalBounds().height);

    if (_autoCenter) {
        _pos.x = _origin.x - _size.x / 2.f - _padding;
        _pos.y = _origin.y - _size.y / 2.f;
    }

    _children.erase(std::remove_if(_children.begin(), _children.end(), [](s_p<VisualNode> node) {return !node->isActive(); }), _children.end());

    if (hasParent()) {
        const float dist = getPosition().y - _parent->getPosition().y;
        if (dist <= pe::UI::percentToScreenHeight(6.f)) {
            if (hasChildren() || getParent()->getChildren().size() > 1) {
                move({ 0, pe::UI::percentToScreenHeight(4.f) });
            }
        } else if (dist >= pe::UI::percentToScreenHeight(6.f)) {
            if (!hasChildren() && getParent()->getChildren().size() == 1) {
                move({ 0, -pe::UI::percentToScreenHeight(4.f) });
            }
        }
    }
}

void VisualNode::draw(sf::RenderTexture& surface) {
    if (!hasChildren()) _fieldText.setFillColor(Settings::termColor);
    else _fieldText.setFillColor(Settings::nonTermColor);

    _fieldText.setOrigin(_fieldText.getLocalBounds().width / 2.f + _fieldText.getLocalBounds().left, _fieldText.getLocalBounds().height / 2.f + _fieldText.getLocalBounds().top);

    const sf::FloatRect bounds = getBounds();
    const float width = bounds.width;
    const float height = bounds.height;
    _fieldText.setPosition(
        bounds.left + (width / 2.f),
        bounds.top + (height / 2.f)
    );

    surface.draw(_fieldText);

    if (_isArmed) {
        sf::Text cursor;
        cursor.setString(" |");
        cursor.setFont(PennyEngine::getFont());
        cursor.setCharacterSize(_fieldText.getCharacterSize() + pe::UI::percentToScreenWidth(0.5f));
        cursor.setFillColor(_fieldText.getFillColor());
        cursor.setOrigin(cursor.getLocalBounds().width / 2.f + cursor.getLocalBounds().left, cursor.getLocalBounds().height / 2.f + cursor.getLocalBounds().top);
        cursor.setPosition(_fieldText.getPosition().x + _fieldText.getGlobalBounds().width / 2.f, _fieldText.getPosition().y);
        constexpr unsigned int blinkRate = 24;
        if ((_cursorBlinkTimer / blinkRate) % 2) surface.draw(cursor);

        _cursorBlinkTimer++;
    }

    connectToParent(surface);

    if (!_hideInterface && (!_isArmed || getBounds().contains(_mPos.x, _mPos.y))) {
        _plusButton.setPosition(_pos.x + getBounds().width - _plusButton.getSize().x, _pos.y + getBounds().height - _plusButton.getSize().y);
        _minusButton.setPosition(_pos.x, _pos.y + getBounds().height - _minusButton.getSize().y);

        surface.draw(_plusButton);
        if (hasParent()) surface.draw(_minusButton);
    }
}

void VisualNode::mouseButtonPressed(const int mx, const int my, const int button) {
    for (const auto& node : VisualTree::getNodes()) {
        if (node->isActive() && node->getIdentifier() != getIdentifier() && node->hasMousePriority()) return;
    }

    _mouseDown = true;

    if (_plusButton.getGlobalBounds().contains(mx, my) || (hasParent() && _minusButton.getGlobalBounds().contains(mx, my))) {
        _clickingButtons = true;
    }
}

void VisualNode::mouseButtonReleased(const int mx, const int my, const int button) {
    if (_isArmed && !getBounds().contains(mx, my)) _isArmed = false;

    for (const auto& node : VisualTree::getNodes()) {
        if (node->isActive() && node->getIdentifier() != getIdentifier() && node->hasMousePriority()) return;
    }

    _isArmed = !_clickingButtons && getBounds().contains(mx, my);
    _mouseDown = false;

    if (_plusButton.getGlobalBounds().contains(mx, my) && getBounds().contains(_mPos.x, _mPos.y)) {
        addChild();
    } else if (hasParent() && _minusButton.getGlobalBounds().contains(mx, my) && getBounds().contains(_mPos.x, _mPos.y)) {
        hide();
    }

    _clickingButtons = false;
}

void VisualNode::mouseMoved(const int mx, const int my) {
    for (const auto& node : VisualTree::getNodes()) {
        if (node->isActive() && node->getIdentifier() != getIdentifier() && node->hasMousePriority()) return;
    }

    _mPos.x = mx;
    _mPos.y = my;
}

bool VisualNode::hasMousePriority() const {
    return getBounds().contains(_mPos.x, _mPos.y);
}

void VisualNode::textEntered(const sf::Uint32 character) {
    if (_isArmed) {
        sf::String userInput = _fieldText.getString();
        if (character == '\b' && userInput.getSize()) {
            userInput.erase(userInput.getSize() - 1, 1);
        } else if (character != '\b') {
            userInput += character;
        }
        _fieldText.setString(userInput);
    }
}

std::vector<s_p<VisualNode>>& VisualNode::getChildren() {
    return _children;
}

VisualNode* VisualNode::getParent() {
    return _parent;
}

bool VisualNode::hasChildren() {
    return getChildren().size() > 0;
}

bool VisualNode::hasParent() const {
    return _parent != nullptr;
}

bool VisualNode::isHovered() const {
    return !_hideInterface;
}
