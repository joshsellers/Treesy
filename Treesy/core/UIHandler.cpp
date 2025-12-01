// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#include "UIHandler.h"
#include "../../PennyEngine/ui/UI.h"
#include "../../PennyEngine/ui/components/TextField.h"
#include "../../PennyEngine/ui/components/Panel.h"
#include "../../PennyEngine/ui/components/Button.h"
#include "../../PennyEngine/ui/components/Slider.h"
#include "../../PennyEngine/ui/components/ToggleButton.h"
#include "../../PennyEngine/core/Util.h"
#include <Windows.h>
#include "../../PennyEngine/core/Logger.h"
#include "../visual/VisualTree.h"
#include "Settings.h"
#include "Persistence.h"

void UIHandlerImpl::init() {
    // Subscripts
    auto subscriptMenu = pe::UI::addMenu("subscriptMenu");
    subscriptMenu->addComponent(new_s_p(pe::TextField, ("subscriptField", 50, 38, 9, 4, "")));
    subscriptMenu->addComponent(new_s_p(pe::Button, ("close_subscriptMenu", 50, 44, 6, 3, "Done", this)));
    auto subscriptPanel = new_s_p(pe::Panel, ("subscriptPanel", 50, 40, 12, 16, "Add subscript:", true));
    subscriptPanel->setTextPosition({50.f, 11.f});
    subscriptMenu->addComponent(subscriptPanel);
    subscriptPanel->attach(subscriptMenu->getComponent("subscriptField"));
    subscriptPanel->attach(subscriptMenu->getComponent("close_subscriptMenu"));
    //

    // Main
    auto mainMenu = pe::UI::addMenu("main");
    const sf::Vector2f posOffset = { -3, 0 };
    mainMenu->addComponent(new_s_p(pe::Button, ("load", 10 + posOffset.x, 13 + posOffset.y, 8, 3, "Open tree", this)));
    mainMenu->addComponent(new_s_p(pe::Button, ("save", 10 + posOffset.x, 18 + posOffset.y, 8, 3, "Save tree", this)));
    mainMenu->addComponent(new_s_p(pe::Button, ("export", 10 + posOffset.x, 23 + posOffset.y, 8, 3, "Export tree", this)));
    mainMenu->addComponent(new_s_p(pe::Button, ("open_settings", 10 + posOffset.x, 28 + posOffset.y, 7, 3, "Settings", this)));
    mainMenu->addComponent(new_s_p(pe::Button, ("exit", 10 + posOffset.x, 33 + posOffset.y, 6, 3, "Quit", this)));

    auto mainPanel = new_s_p(pe::Panel, ("mainPanel", 10 + posOffset.x, 20 + posOffset.y, 10, 32, "Treesy", true));
    mainPanel->setTextPosition({ 50.f, 14.f });
    mainMenu->addComponent(mainPanel);
    mainPanel->attach(mainMenu->getComponent("load"));
    mainPanel->attach(mainMenu->getComponent("save"));
    mainPanel->attach(mainMenu->getComponent("export"));
    mainPanel->attach(mainMenu->getComponent("open_settings"));
    mainPanel->attach(mainMenu->getComponent("exit"));
    mainMenu->open();
    //

    // Settings
    auto settingsMenu = pe::UI::addMenu("settings");
    settingsMenu->addComponent(new_s_p(pe::Slider, ("widthSlider", 7, 53.f, { 8, 1.f }, { 1.f, 2.f }, "Horizontal Spacing", this)));
    pe::Slider* horzslider = dynamic_cast<pe::Slider*>(settingsMenu->getComponent("widthSlider").get());
    horzslider->setValue(Settings::horzSpacing / 500.f + 0.5f);
    horzslider->getText().setCharacterSize(pe::UI::percentToScreenWidth(1.f));

    //settingsMenu->addComponent(new_s_p(pe::Slider, ("heightSlider", 0, 0, { 8.f, 1.f }, { 1.f, 2.f }, "Vertical Spacing", this))); 
    //pe::Slider* vertslider = dynamic_cast<pe::Slider*>(settingsMenu->getComponent("heightSlider").get());
    ////vertslider->setValue(0.f);
    //vertslider->getText().setCharacterSize(pe::UI::percentToScreenWidth(1.f));

    settingsMenu->addComponent(new_s_p(pe::ToggleButton, ("termLines", 0, 0, 0.6f, 0.35f, "Terminal Lines: ", this)));
    pe::ToggleButton* termLinesButton = dynamic_cast<pe::ToggleButton*>(settingsMenu->getComponent("termLines").get());
    termLinesButton->setValue(Settings::showTermLines);

    settingsMenu->addComponent(new_s_p(pe::Button, ("open_colors", 0, 0, 8, 3, "Colors", this)));
    settingsMenu->addComponent(new_s_p(pe::Button, ("close_settings", 0, 0, 8, 3, "Close", this)));

    auto settingsPanel = new_s_p(pe::Panel, ("settingsPanel", 7, 60, 13, 30, "Settings", true));
    settingsPanel->setTextPosition(50.f, 12.f);
    settingsMenu->addComponent(settingsPanel);
    settingsPanel->attachAt("widthSlider", { 50, 22 });
    settingsPanel->attachAt("heightSlider", { 50, 37 });
    settingsPanel->attachAt("termLines", { 82, 50 });
    settingsPanel->attachAt("open_colors", { 50, 71 });
    settingsPanel->attachAt("close_settings", { 50, 84 });
    //

    // Colors
    auto colorsMenu = pe::UI::addMenu("colors");
    settingsMenu->addChild(colorsMenu);

    colorsMenu->addComponent(new_s_p(pe::Button, ("bgColor", 0, 0, 9, 3, "Background", this)));
    colorsMenu->addComponent(new_s_p(pe::Button, ("lineColor", 0, 0, 9, 3, "Lines", this)));
    colorsMenu->addComponent(new_s_p(pe::Button, ("termColor", 0, 0, 9, 3, "Terminals", this)));
    colorsMenu->addComponent(new_s_p(pe::Button, ("nonTermColor", 0, 0, 9, 3, "Non-terminals", this)));

    colorsMenu->addComponent(new_s_p(pe::Button, ("close_colors", 7, 72, 6, 3, "Back", this)));

    colorsMenu->addComponent(settingsPanel);
    settingsPanel->attachAt("bgColor", { 50, 25 });
    settingsPanel->attachAt("lineColor", { 50, 38 });
    settingsPanel->attachAt("termColor", { 50, 51 });
    settingsPanel->attachAt("nonTermColor", { 50, 64 });
    settingsPanel->attachAt("close_colors", { 50, 80 });
    //

    // Color
    auto colorMenu = pe::UI::addMenu("color");
    colorMenu->addComponent(new_s_p(pe::Slider, ("c_red_slider", 0, 0, { 8, 1.f }, { 1.f, 2.f }, "Red", this)));
    colorMenu->addComponent(new_s_p(pe::Slider, ("c_green_slider", 0, 0, { 8, 1.f }, { 1.f, 2.f }, "Green", this)));
    colorMenu->addComponent(new_s_p(pe::Slider, ("c_blue_slider", 0, 0, { 8, 1.f }, { 1.f, 2.f }, "Blue", this)));
    colorMenu->addComponent(new_s_p(pe::Slider, ("c_alpha_slider", 0, 0, { 8, 1.f }, { 1.f, 2.f }, "Alpha", this)));
    colorMenu->addComponent(new_s_p(pe::Button, ("close_color", 0, 0, 8, 3, "Close", this)));

    auto colorPanel = new_s_p(pe::Panel, ("colorPanel", 80, 50, 13, 40, "", true));
    colorPanel->setTextPosition(50.f, 8.f);
    colorPanel->attachAt("c_red_slider", { 50, 15 });
    colorPanel->attachAt("c_green_slider", { 50, 35 });
    colorPanel->attachAt("c_blue_slider", { 50, 55 });
    colorPanel->attachAt("c_alpha_slider", { 50, 75 });
    colorPanel->attachAt("close_color", { 50, 95 });

    colorMenu->addComponent(colorPanel);
    //
}

void UIHandler::init() {
    _instance.init();
}

void UIHandlerImpl::buttonPressed(std::string buttonId) {
    if (pe::stringStartsWith(buttonId, "close_")) {
        const auto& menu = pe::UI::getMenu(pe::splitString(buttonId, "close_")[1]);
        if (menu != nullptr) menu->close();
    } else if (pe::stringStartsWith(buttonId, "open_")) {
        const auto& menu = pe::UI::getMenu(pe::splitString(buttonId, "open_")[1]);
        if (menu != nullptr) menu->open();
    } else if (buttonId == "export") {
        const std::string path = UIHandler::getExportPath();
        saveImage(path);
    } else if (buttonId == "exit") {
        PennyEngine::stop();
    } else if (buttonId == "save") {
        const std::string path = UIHandler::getSavePath();
        Persistence::save(path);
    } else if (buttonId == "load") {
        const std::string path = UIHandler::getLoadPath();
        VisualTree::reset();
        Persistence::load(path);
    } else if (buttonId == "bgColor") {
        pe::UI::getMenu("color")->open();
        _selectedColor = &Settings::bgColor;
        setColorSliders();
    } else if (buttonId == "lineColor") {
        pe::UI::getMenu("color")->open();
        _selectedColor = &Settings::lineColor;
        setColorSliders();
    } else if (buttonId == "termColor") {
        pe::UI::getMenu("color")->open();
        _selectedColor = &Settings::termColor;
        setColorSliders();
    } else if (buttonId == "nonTermColor") {
        pe::UI::getMenu("color")->open();
        _selectedColor = &Settings::nonTermColor;
        setColorSliders();
    }
}

void UIHandlerImpl::sliderMoved(std::string sliderId, float value) {
    if (sliderId == "widthSlider") {
        Settings::horzSpacing = (value - 0.5f) * 500.f;
    } else if (sliderId == "heightSlider") {
        Settings::nontermVerticalDistance = (value) * 8.f;
    } else if (pe::stringStartsWith(sliderId, "c_")) {
        sf::Uint8* channel = nullptr;
        std::string channelStr = pe::splitString(sliderId, "_")[1];
        if (channelStr == "red") channel = &_selectedColor->r;
        else if (channelStr == "green") channel = &_selectedColor->g;
        else if (channelStr == "blue") channel = &_selectedColor->b;
        else if (channelStr == "alpha") channel = &_selectedColor->a;

        if (channel != nullptr) {
            *channel = (sf::Uint8)(value * 255.f);
        }
    }
}

void UIHandlerImpl::toggleButtonPressed(std::string buttonid, bool newValue) {
    if (buttonid == "termLines") Settings::showTermLines = newValue;
}

void UIHandlerImpl::setColorSliders() {
    pe::Slider* slider = dynamic_cast<pe::Slider*>(pe::UI::getMenu("color")->getComponent("c_red_slider").get());
    slider->setValue(_selectedColor->r / 255.f);

    slider = dynamic_cast<pe::Slider*>(pe::UI::getMenu("color")->getComponent("c_green_slider").get());
    slider->setValue(_selectedColor->g / 255.f);

    slider = dynamic_cast<pe::Slider*>(pe::UI::getMenu("color")->getComponent("c_blue_slider").get());
    slider->setValue(_selectedColor->b / 255.f);

    slider = dynamic_cast<pe::Slider*>(pe::UI::getMenu("color")->getComponent("c_alpha_slider").get());
    slider->setValue(_selectedColor->a / 255.f);
}

void UIHandlerImpl::saveImage(std::string path) {
    float lowestX = 9999999;
    float lowestY = 9999999;
    float highestX = 0;
    float highestY = 0;
    for (const auto& node : VisualTree::getNodes()) {
        const sf::Vector2f pos = node->getPosition();
        const sf::Vector2f size = { node->getBounds().width, node->getBounds().height };

        lowestX = std::min(pos.x, lowestX);
        lowestY = std::min(pos.y, lowestY);
        highestX = std::max(pos.x + size.x, highestX);
        highestY = std::max(std::max(pos.y + size.y, node->getMovementLineVertex()), highestY);
    }
    const sf::Vector2f size = { highestX - lowestX, highestY - lowestY };
    const sf::View view({ lowestX + size.x / 2.f, lowestY + size.y / 2.f }, size);

    sf::RenderTexture outputSurface;
    outputSurface.create(size.x, size.y);
    const sf::Texture& outputSurfaceTexture = outputSurface.getTexture();

    sf::RectangleShape bg;
    bg.setFillColor(Settings::bgColor);
    bg.setPosition(lowestX, lowestY);
    bg.setSize({(float)view.getSize().x, (float)view.getSize().y});

    outputSurface.setView(view);
    outputSurface.draw(bg);
    VisualTree::draw(outputSurface);
    outputSurface.display();

    const bool result = outputSurfaceTexture.copyToImage().saveToFile(path);

    if (!result) {
        pe::Logger::log("Failed to save: " + path);
    }
}

static std::string WcharToUtf8(const WCHAR* wideString, size_t length = 0) {
    if (length == 0)
        length = wcslen(wideString);

    if (length == 0)
        return std::string();

    std::string convertedString(WideCharToMultiByte(CP_UTF8, 0, wideString, (int)length, NULL, 0, NULL, NULL), 0);

    WideCharToMultiByte(
        CP_UTF8, 0, wideString, (int)length, &convertedString[0], (int)convertedString.size(), NULL, NULL);

    return convertedString;
}

std::string UIHandler::getExportPath() {
    OPENFILENAME ofn;

    WCHAR szFileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = (LPCWSTR)L"PNG Files (*.png)\0*.png\0JPG Files (*.jpg)\0*.jpg\0JPEG files (*.jpeg)\0*.jpeg\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName; 
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"png";

    GetSaveFileName(&ofn);
    return WcharToUtf8(ofn.lpstrFile);
}

std::string UIHandler::getSavePath() {
    OPENFILENAME ofn;

    WCHAR szFileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = (LPCWSTR)L"Treesy Files (*.treesy)\0*.treesy\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"treesy";

    GetSaveFileName(&ofn);
    return WcharToUtf8(ofn.lpstrFile);
}

std::string UIHandler::getLoadPath() {
    OPENFILENAME ofn;

    WCHAR szFileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = (LPCWSTR)L"Treesy Files (*.treesy)\0*.treesy\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"treesy";

    GetOpenFileName(&ofn);
    return WcharToUtf8(ofn.lpstrFile);
}
