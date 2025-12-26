#include "MainWindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);
        
        // Create main window for integer type
        MainWindow window;
        window.show();
        
        return app.exec();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

