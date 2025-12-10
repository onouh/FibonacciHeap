#include "MainWindow.h"
#include <iostream>
#include <cmath>
#include <sstream>

MainWindow::MainWindow() : 
    window(sf::VideoMode(1200, 800), "Fibonacci Heap Visualization"),
    isTyping(false) {
    
    // Try to load font from multiple locations for cross-platform compatibility
    std::vector<std::string> fontPaths = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",           // Linux
        "/usr/share/fonts/TTF/DejaVuSans.ttf",                       // Linux alternative
        "C:\\Windows\\Fonts\\Arial.ttf",                              // Windows
        "/System/Library/Fonts/Helvetica.ttc",                       // macOS
        "/Library/Fonts/Arial.ttf",                                  // macOS alternative
        "DejaVuSans.ttf"                                             // Local bundled font
    };
    
    bool fontLoaded = false;
    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            fontLoaded = true;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cerr << "Warning: Could not load any font. Text may not display properly." << std::endl;
    }
    
    initializeUI();
}

void MainWindow::initializeUI() {
    // Title
    titleText.setFont(font);
    titleText.setString("Fibonacci Heap Visualization");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::Black);
    titleText.setPosition(20, 10);
    
    // Instructions
    instructionsText.setFont(font);
    instructionsText.setString("Enter a number and click Insert. Click Extract Min to remove minimum.");
    instructionsText.setCharacterSize(14);
    instructionsText.setFillColor(sf::Color(60, 60, 60));
    instructionsText.setPosition(20, 50);
    
    // Status text
    statusText.setFont(font);
    statusText.setString("Ready");
    statusText.setCharacterSize(14);
    statusText.setFillColor(sf::Color(0, 128, 0));
    statusText.setPosition(20, 75);
    
    // Input box
    inputBox.setSize(sf::Vector2f(INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT));
    inputBox.setPosition(20, 110);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);
    
    inputText.setFont(font);
    inputText.setString("");
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(30, 120);
    
    // Insert button
    insertButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    insertButton.setPosition(140, 110);
    insertButton.setFillColor(sf::Color(70, 130, 180));
    
    insertButtonText.setFont(font);
    insertButtonText.setString("Insert");
    insertButtonText.setCharacterSize(18);
    insertButtonText.setFillColor(sf::Color::White);
    insertButtonText.setPosition(170, 120);
    
    // Extract Min button
    extractMinButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    extractMinButton.setPosition(280, 110);
    extractMinButton.setFillColor(sf::Color(220, 20, 60));
    
    extractMinButtonText.setFont(font);
    extractMinButtonText.setString("Extract Min");
    extractMinButtonText.setCharacterSize(18);
    extractMinButtonText.setFillColor(sf::Color::White);
    extractMinButtonText.setPosition(295, 120);
    
    // Reset button
    resetButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    resetButton.setPosition(420, 110);
    resetButton.setFillColor(sf::Color(128, 128, 128));
    
    resetButtonText.setFont(font);
    resetButtonText.setString("Reset");
    resetButtonText.setCharacterSize(18);
    resetButtonText.setFillColor(sf::Color::White);
    resetButtonText.setPosition(455, 120);
}

void MainWindow::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void MainWindow::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                handleMouseClick(mousePos);
            }
        }
        else if (event.type == sf::Event::TextEntered) {
            if (isTyping) {
                handleTextInput(event.text.unicode);
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter && isTyping) {
                insertValue();
            }
        }
    }
}

void MainWindow::handleMouseClick(const sf::Vector2f& mousePos) {
    // Check if input box was clicked
    if (inputBox.getGlobalBounds().contains(mousePos)) {
        isTyping = true;
        inputBox.setOutlineColor(sf::Color::Blue);
    } else {
        isTyping = false;
        inputBox.setOutlineColor(sf::Color::Black);
    }
    
    // Check button clicks
    if (isButtonClicked(insertButton, mousePos)) {
        insertValue();
    }
    else if (isButtonClicked(extractMinButton, mousePos)) {
        extractMinValue();
    }
    else if (isButtonClicked(resetButton, mousePos)) {
        resetHeap();
    }
}

void MainWindow::handleTextInput(sf::Uint32 unicode) {
    if (unicode == 8) { // Backspace
        if (!inputValue.empty()) {
            inputValue.pop_back();
        }
    }
    else if (unicode >= 48 && unicode <= 57) { // Numbers 0-9
        if (inputValue.length() < 6) {
            inputValue += static_cast<char>(unicode);
        }
    }
    else if (unicode == 45 && inputValue.empty()) { // Minus sign (only at start)
        inputValue += '-';
    }
    
    inputText.setString(inputValue);
}

bool MainWindow::isButtonClicked(const sf::RectangleShape& button, const sf::Vector2f& mousePos) {
    return button.getGlobalBounds().contains(mousePos);
}

void MainWindow::insertValue() {
    if (inputValue.empty()) {
        updateStatus("Please enter a value");
        return;
    }
    
    try {
        int value = std::stoi(inputValue);
        heap.insert(value);
        updateStatus("Inserted: " + inputValue);
        inputValue.clear();
        inputText.setString("");
    }
    catch (const std::exception& e) {
        updateStatus("Invalid input");
    }
}

void MainWindow::extractMinValue() {
    if (heap.isEmpty()) {
        updateStatus("Heap is empty");
        return;
    }
    
    try {
        int minValue = heap.getMin();
        FibonacciNode<int>* extracted = heap.extractMin();
        delete extracted;  // Free the extracted node
        updateStatus("Extracted min: " + std::to_string(minValue));
    }
    catch (const std::exception& e) {
        updateStatus("Error extracting min");
    }
}

void MainWindow::resetHeap() {
    // Clear the heap by creating a new one
    heap = FibonacciHeap<int>();
    nodePositions.clear();
    updateStatus("Heap reset");
}

void MainWindow::updateStatus(const std::string& message) {
    statusText.setString(message);
}

void MainWindow::render() {
    window.clear(sf::Color(240, 240, 240));
    
    // Draw UI elements
    window.draw(titleText);
    window.draw(instructionsText);
    window.draw(statusText);
    
    window.draw(inputBox);
    window.draw(inputText);
    
    window.draw(insertButton);
    window.draw(insertButtonText);
    
    window.draw(extractMinButton);
    window.draw(extractMinButtonText);
    
    window.draw(resetButton);
    window.draw(resetButtonText);
    
    // Draw heap visualization
    drawHeap();
    
    window.display();
}

void MainWindow::drawHeap() {
    if (heap.isEmpty()) {
        sf::Text emptyText;
        emptyText.setFont(font);
        emptyText.setString("Heap is empty");
        emptyText.setCharacterSize(20);
        emptyText.setFillColor(sf::Color(128, 128, 128));
        emptyText.setPosition(500, 400);
        window.draw(emptyText);
        return;
    }
    
    calculateNodePositions();
    
    // Draw connections first (so they appear behind nodes)
    for (const auto& [node, pos] : nodePositions) {
        if (node->child) {
            // Draw lines to children
            FibonacciNode<int>* child = node->child;
            FibonacciNode<int>* start = child;
            do {
                if (nodePositions.find(child) != nodePositions.end()) {
                    sf::Vector2f childPos = nodePositions[child];
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Black),
                        sf::Vertex(sf::Vector2f(childPos.x, childPos.y), sf::Color::Black)
                    };
                    window.draw(line, 2, sf::Lines);
                }
                child = child->right;
            } while (child != start);
        }
    }
    
    // Draw nodes
    for (const auto& [node, pos] : nodePositions) {
        drawNode(node, pos.x, pos.y, node->parent == nullptr);
    }
}

void MainWindow::drawNode(FibonacciNode<int>* node, float x, float y, bool isRoot) {
    // Draw circle
    sf::CircleShape circle(NODE_RADIUS);
    circle.setPosition(x - NODE_RADIUS, y - NODE_RADIUS);
    
    // Color based on node properties
    if (node == heap.getMinNode()) {
        circle.setFillColor(sf::Color(255, 215, 0)); // Gold for minimum
    } else if (node->marked) {
        circle.setFillColor(sf::Color(255, 140, 0)); // Orange for marked
    } else if (isRoot) {
        circle.setFillColor(sf::Color(144, 238, 144)); // Light green for roots
    } else {
        circle.setFillColor(sf::Color(173, 216, 230)); // Light blue for other nodes
    }
    
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2);
    window.draw(circle);
    
    // Draw key value
    sf::Text keyText;
    keyText.setFont(font);
    keyText.setString(std::to_string(node->key));
    keyText.setCharacterSize(16);
    keyText.setFillColor(sf::Color::Black);
    
    // Center text in circle
    sf::FloatRect textBounds = keyText.getLocalBounds();
    keyText.setPosition(x - textBounds.width / 2, y - textBounds.height / 2 - 5);
    window.draw(keyText);
}

void MainWindow::calculateNodePositions() {
    nodePositions.clear();
    
    if (heap.isEmpty()) return;
    
    std::vector<FibonacciNode<int>*> roots = heap.getRootList();
    
    float startX = 100;
    float startY = 250;
    float currentX = startX;
    
    for (auto root : roots) {
        float maxX = currentX;
        positionSubtree(root, currentX, startY, maxX);
        currentX = maxX + HORIZONTAL_SPACING * 1.5f;
    }
}

void MainWindow::positionSubtree(FibonacciNode<int>* node, float x, float y, float& maxX) {
    if (!node) return;
    
    nodePositions[node] = sf::Vector2f(x, y);
    maxX = std::max(maxX, x);
    
    // Position children
    if (node->child) {
        FibonacciNode<int>* child = node->child;
        FibonacciNode<int>* start = child;
        float childX = x - (node->degree - 1) * HORIZONTAL_SPACING / 2.0f;
        
        do {
            positionSubtree(child, childX, y + VERTICAL_SPACING, maxX);
            childX += HORIZONTAL_SPACING;
            child = child->right;
        } while (child != start);
    }
}
