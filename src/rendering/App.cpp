#include "App.hpp"
#include "gui/Button.h"
#include <chrono>
#include <iostream>
#include <SFML/Graphics/RenderStates.hpp>

int App::statementGraphDelimitation = 0;
GuiElement App::screen;
int App::upperSpace = 60;
float App::leftListScroll = 0;
LeftList* App::leftList;
bool App::stale = true;
sf::RenderWindow* App::windowRef = nullptr;

/*
* @brief Once called the function will not return until the window is closed
*/
void App::renderLoop() {

    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(videoMode.width * 0.8, videoMode.height * 0.8), "Ardezie");
    windowRef = &window;

    window.setVerticalSyncEnabled(true);

    // Calculating where the vertical separation line should be
    statementGraphDelimitation = window.getSize().x * 0.25;

    // Declaring the vertical separation line
    sf::RectangleShape delimiterLine(sf::Vector2f(statementGraphDelimitationWidth,window.getSize().y-upperSpace));
    delimiterLine.setPosition(sf::Vector2f(statementGraphDelimitation, upperSpace));
    delimiterLine.setFillColor(unselectedColor);

    // Declaring the upper solid block that hold the setting
    sf::RectangleShape upperSolidBackground(sf::Vector2f(window.getSize().x, upperSpace));
    upperSolidBackground.setPosition(sf::Vector2f(0, 0));
    upperSolidBackground.setFillColor(unselectedColor);

    // Initialising and loading the textures for the button that is used to add more expression boxes
    leftList = new LeftList(sf::Vector2f(0,upperSpace), statementGraphDelimitation + statementGraphDelimitationWidth,90, newBoxButtonHeight,addBoxButtonColor,slateInstance);
    screen.addChild(leftList);
    leftList->setParent(&screen);
    leftList->newBox += scrollToEnd;

    bool stale = true;

    // The program while true loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            stale = true;
            // Window close event
            if (event.type == sf::Event::Closed) window.close();
            // On resized event
            if (event.type == sf::Event::Resized) {
                // Normalising the view port
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                // Reclaculating where the vertical delimiter line should be
                statementGraphDelimitation = window.getSize().x * 0.25;
                // Setting the new position and size of the vertical delimiter line
                delimiterLine.setPosition(sf::Vector2f(statementGraphDelimitation, upperSpace));
                delimiterLine.setSize(sf::Vector2f(statementGraphDelimitationWidth, window.getSize().y - upperSpace));

                // Setting the new size of the uppoer solid block
                upperSolidBackground.setSize(sf::Vector2f(window.getSize().x, upperSpace));
                leftList->changeWidth(statementGraphDelimitation + statementGraphDelimitationWidth);

                snapScroll();

            }
            if (event.type == sf::Event::MouseWheelMoved) {
                if (event.mouseWheel.delta > 0) leftListScroll -= 40;
                else leftListScroll += 40;
                if (leftListScroll < 0) leftListScroll = 0;
                snapScroll();
            }
            screen.onWindowEvent(event);

        }

        screen.update();
        
        if (!stale) std::this_thread::sleep_for(std::chrono::milliseconds(10));
        

        if (leftList->getOffset().y != - leftListScroll) leftList->setOffset(sf::Vector2f(0, -leftListScroll));

        if (stale || screen.isRenderStale()) {
            // Coloring the background
            window.clear(sf::Color::White);
            // Drawing the vertical line that separates the graph area from the expression boxes
            window.draw(delimiterLine);
            // Drawing all gui elements
            screen.onWindowDraw(window);
            // Drawing the upper solid block where the setting are presetn
            window.draw(upperSolidBackground);
            // Displaying the window
            window.display();
        }
        stale = false;
    }

}

void App::setStale() {
    stale = true;
}

void App::snapScroll() {
    int listSize = leftList->getFullSize();
    int dif = listSize - (windowRef->getSize().y - upperSpace);
    if (dif <= 0) leftListScroll = 0;
    else if (leftListScroll > dif) leftListScroll = dif;
}

void App::scrollToEnd() {
    int listSize = leftList->getFullSize();
    int dif = listSize - (windowRef->getSize().y - upperSpace);
    if (dif <= 0) leftListScroll = 0;
    else leftListScroll = dif;
}
