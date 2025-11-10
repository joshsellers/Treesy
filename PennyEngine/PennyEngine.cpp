// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "PennyEngine.h"

void PennyEngine::start(pe::GameManager* gameManager) {
    _instance.start(gameManager);
}

bool PennyEngine::isStarted() {
    return _instance.isStarted();
}

void PennyEngine::stop() {
    _instance.window.close();
}

void PennyEngine::setFramerateLimit(int framerate) {
    _instance.framerateLimit = framerate;
}

sf::RenderWindow& PennyEngine::getWindow() {
    return _instance.window;
}

sf::View& PennyEngine::getCamera() {
    return _instance.camera;
}

void PennyEngine::setRenderResolution(pe::Resolution res) {
    _instance.renderRes = res;
}

pe::Resolution PennyEngine::getRenderResolution() {
    return _instance.renderRes;
}

void PennyEngine::autoScaleRenderResolution(bool should) {
    _instance.autoScaleRenderRes = should;
}

bool PennyEngine::shouldAutoScaleRenderResultion() {
    return _instance.autoScaleRenderRes;
}

void PennyEngine::useDisplayResForUI(bool should) {
    _instance.useDisplayResForUI = should;
}

bool PennyEngine::useDisplayResForUI() {
    return _instance.useDisplayResForUI;
}

void PennyEngine::setDisplayResolution(pe::Resolution res) {
    _instance.displayRes = res;
}

pe::Resolution PennyEngine::getDisplayResolution() {
    return _instance.displayRes;
}

void PennyEngine::setFullscreen(bool fullscreen) {
    _instance.fullscreen = fullscreen;
}

bool PennyEngine::windowIsFullscreen() {
    return _instance.fullscreen;
}

void PennyEngine::setWindowTitle(std::string title) {
    _instance.windowTitle = title;
}

std::string PennyEngine::getWindowTitle() {
    return _instance.windowTitle;
}

void PennyEngine::setAppIcon(std::string path) {
    _instance.appIconPath = path;
}

void PennyEngine::setMouseCursor(std::string path) {
    _instance.cursorImagePath = path;
}

void PennyEngine::addInputListener(pe::intern::InputListener* listener) {
    _instance.getInputManager().addListener(listener);
}

void PennyEngine::setAppName(std::string name) {
    _appName = name;
}

std::string PennyEngine::getAppName() {
    return _appName;
}

void PennyEngine::setFont(std::string path) {
    _instance.fontPath = path;
}

std::string PennyEngine::getFontPath() {
    return _instance.fontPath;
}

sf::Font& PennyEngine::getFont() {
    return _instance.getFont();
}

bool PennyEngine::playerIsUsingMouse() {
    return _usingMouse;
}

sf::Vector2f PennyEngine::getUIMouseOffset() {
    return _instance.getInputManager().getUIMouseOffset();
}
