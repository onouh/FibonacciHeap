#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "FibonacciHeap.h"

/**
 * Main window class for Fibonacci Heap visualization
 * Handles rendering and user interaction
 */
class MainWindow {
private:
    sf::RenderWindow window;
    FibonacciHeap<int> heap;
    
    // UI Elements
    sf::Font font;
    sf::RectangleShape insertButton;
    sf::RectangleShape extractMinButton;
    sf::RectangleShape resetButton;
    sf::Text insertButtonText;
    sf::Text extractMinButtonText;
    sf::Text resetButtonText;
    sf::Text titleText;
    sf::Text instructionsText;
    sf::Text statusText;
    
    // Input state
    std::string inputValue;
    bool isTyping;
    sf::RectangleShape inputBox;
    sf::Text inputText;
    
    // Node positions for visualization
    std::map<FibonacciNode<int>*, sf::Vector2f> nodePositions;
    
    // Constants
    static constexpr float NODE_RADIUS = 25.0f;
    static constexpr float HORIZONTAL_SPACING = 100.0f;
    static constexpr float VERTICAL_SPACING = 80.0f;
    static constexpr int BUTTON_WIDTH = 120;
    static constexpr int BUTTON_HEIGHT = 40;
    static constexpr int INPUT_BOX_WIDTH = 100;
    static constexpr int INPUT_BOX_HEIGHT = 40;
    
    // Helper functions
    void initializeUI();
    void handleEvents();
    void handleMouseClick(const sf::Vector2f& mousePos);
    void handleTextInput(sf::Uint32 unicode);
    void render();
    void drawHeap();
    void drawNode(FibonacciNode<int>* node, float x, float y, bool isRoot = false);
    void calculateNodePositions();
    void positionSubtree(FibonacciNode<int>* node, float x, float y, float& maxX);
    bool isButtonClicked(const sf::RectangleShape& button, const sf::Vector2f& mousePos);
    void insertValue();
    void extractMinValue();
    void resetHeap();
    void updateStatus(const std::string& message);
    
public:
    MainWindow();
    void run();
};

#endif // MAINWINDOW_H
