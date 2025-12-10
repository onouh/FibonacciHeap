#include "MainWindow.h"
#include <iostream>

int main() {
    try {
        MainWindow window;
        window.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
