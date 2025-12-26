#ifndef TYPE_SELECTOR_H
#define TYPE_SELECTOR_H

#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

enum class DataType {
    INTEGER,
    CHAR,
    STRING
};

/**
 * Dialog for selecting the data type at application launch
 */
class TypeSelector : public QDialog {
    Q_OBJECT
    
private:
    QButtonGroup* buttonGroup;
    DataType selectedType;
    
public:
    explicit TypeSelector(QWidget* parent = nullptr);
    DataType getSelectedType() const { return selectedType; }
    
private slots:
    void onOkClicked();
};

#endif // TYPE_SELECTOR_H
