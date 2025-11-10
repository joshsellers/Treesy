// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "SliderHandle.h"

pe::SliderHandle::SliderHandle(std::string id, float x, float y, float width, float height, bool autoCenter) :
MenuComponent(id, x, y, width, height, autoCenter, SLIDER_HANDLE_CONFIG) {
}

void pe::SliderHandle::update() {
}

void pe::SliderHandle::draw(sf::RenderTexture& surface) {
}