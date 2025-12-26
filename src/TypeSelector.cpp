#include "TypeSelector.h"
#include <QGroupBox>

TypeSelector::TypeSelector(QWidget* parent) 
    : QDialog(parent), selectedType(DataType::INTEGER) {
    
    setWindowTitle("Select Data Type");
    setModal(true);
    setMinimumSize(300, 200);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title label
    QLabel* titleLabel = new QLabel("Choose the data type for your Fibonacci Heap:", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);
    
    mainLayout->addSpacing(20);
    
    // Radio buttons group
    QGroupBox* groupBox = new QGroupBox("Data Type", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(groupBox);
    
    buttonGroup = new QButtonGroup(this);
    
    QRadioButton* intRadio = new QRadioButton("Integer", this);
    QRadioButton* charRadio = new QRadioButton("Character", this);
    QRadioButton* stringRadio = new QRadioButton("String", this);
    
    intRadio->setChecked(true);  // Default selection
    
    buttonGroup->addButton(intRadio, static_cast<int>(DataType::INTEGER));
    buttonGroup->addButton(charRadio, static_cast<int>(DataType::CHAR));
    buttonGroup->addButton(stringRadio, static_cast<int>(DataType::STRING));
    
    groupLayout->addWidget(intRadio);
    groupLayout->addWidget(charRadio);
    groupLayout->addWidget(stringRadio);
    
    mainLayout->addWidget(groupBox);
    mainLayout->addStretch();
    
    // OK button
    QPushButton* okButton = new QPushButton("OK", this);
    okButton->setStyleSheet("QPushButton { background-color: #4682B4; color: white; padding: 8px 16px; }");
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &TypeSelector::onOkClicked);
    
    mainLayout->addWidget(okButton);
}

void TypeSelector::onOkClicked() {
    int id = buttonGroup->checkedId();
    selectedType = static_cast<DataType>(id);
    accept();
}
