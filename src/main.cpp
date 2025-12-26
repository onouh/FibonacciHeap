#include "MainWindow.h"
#include "TypeSelector.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);
        
        // Show type selector dialog
        TypeSelector typeSelector;
        if (typeSelector.exec() == QDialog::Rejected) {
            return 0;  // User cancelled
        }
        
        DataType selectedType = typeSelector.getSelectedType();
        
        // Create main window based on selected type
        QMainWindow* window = nullptr;
        
        switch (selectedType) {
            case DataType::INTEGER:
                window = new MainWindow<int>(selectedType);
                break;
            case DataType::CHAR:
                window = new MainWindow<char>(selectedType);
                break;
            case DataType::STRING:
                window = new MainWindow<std::string>(selectedType);
                break;
        }
        
        if (window) {
            window->show();
            int result = app.exec();
            delete window;
            return result;
        }
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

