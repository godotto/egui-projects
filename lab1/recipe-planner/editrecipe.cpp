#include "editrecipe.h"

EditRecipe::EditRecipe(EditMode mode, QWidget *parent)
    : QDialog(parent)
{
    // set window size
    setMinimumSize(300, 500);
    resize(350, 600);

    // set window title
    setWindowTitle(mode == Add ? "New recipe" : "Edit recipe");

    // call helper methods to initialize all objects
    createLabels();
    createLineAndTextEdits("", "", "", nullptr);
    createPushButtons(mode);
    createSpinbox();
    createLayouts();

    // set main layout
    setLayout(m_mainLayout);
}

void EditRecipe::createLabels()
{
    // labels definition
    m_nameLabel = new QLabel("Name:", this);
    m_descriptionLabel = new QLabel("Description:", this);
    m_ingredientNameLabel = new QLabel("Name:", this);
    m_ingredientQuantityLabel = new QLabel("Quantity:", this);
    m_ingredientUnitLabel = new QLabel("Unit:", this);
}

void EditRecipe::createLineAndTextEdits(QString previousName, QString previousIngredientName, QString previousUnit, QStringList* previousDescription)
{
    // line edits definition
    m_nameLineEdit = new QLineEdit(previousName, this);
    m_ingredientNameLineEdit = new QLineEdit(previousIngredientName, this);
    m_ingredientUnitLineEdit = new QLineEdit(previousUnit, this);

    // text edit definition
//    m_descriptionTextEdit = new QTextEdit(previousDescription.join(' '), this);
    m_descriptionTextEdit = new QTextEdit(this);
}

void EditRecipe::createPushButtons(EditMode mode)
{
    // buttons definition
    m_confirmUpdateRecipeButton = new QPushButton(mode == Add ? "&Ok" : "&Update", this); // determine whether it is new recipe or existing one
    m_cancelButton = new QPushButton("&Cancel", this);
    m_addUpdateIngredientButton = new QPushButton("&Add", this);
    m_deleteIngredientButton = new QPushButton("&Delete", this);

    // disable deleting button as default option
    // it will be enabled when ingredient is being selected from the list
    m_deleteIngredientButton->setDisabled(true);
}

void EditRecipe::createLayouts()
{
    // layouts definition
    m_mainLayout = new QVBoxLayout(this);
    m_buttonLayout = new QHBoxLayout(this);
    m_descriptionLayout = new QHBoxLayout(this);
    m_ingredientLayout = new QVBoxLayout(this);
    m_nameLayout = new QHBoxLayout(this);
    m_ingredientQuantityLayout = new QHBoxLayout(this);
    m_ingredientButtonLayout = new QHBoxLayout(this);
    m_ingredientNameLayout = new QHBoxLayout(this);

    // group box definition
    m_ingredientsBox = new QGroupBox("Ingredients", this);

    // recipe name layout
    m_nameLayout->addWidget(m_nameLabel);
    m_nameLayout->addWidget(m_nameLineEdit);

    // recipe description layout
    m_descriptionLayout->addWidget(m_descriptionLabel);
    m_descriptionLayout->addWidget(m_descriptionTextEdit);

    // ingredient name layout
    m_ingredientNameLayout->addWidget(m_ingredientNameLabel);
    m_ingredientNameLayout->addWidget(m_ingredientNameLineEdit);

    // ingredient quantity and unit layout
    m_ingredientQuantityLayout->addWidget(m_ingredientQuantityLabel);
    m_ingredientQuantityLayout->addWidget(m_ingredientQuantitySpinbox);
    m_ingredientQuantityLayout->addWidget(m_ingredientUnitLabel);
    m_ingredientQuantityLayout->addWidget(m_ingredientUnitLineEdit);

    // ingredient buttons
    m_ingredientButtonLayout->addWidget(m_addUpdateIngredientButton);
    m_ingredientButtonLayout->addWidget(m_deleteIngredientButton);

    // ingredients group box
    m_ingredientLayout->addLayout(m_ingredientNameLayout);
    m_ingredientLayout->addLayout(m_ingredientQuantityLayout);
    m_ingredientLayout->addLayout(m_ingredientButtonLayout);
    m_ingredientsBox->setLayout(m_ingredientLayout);

    // button layout
    m_buttonLayout->addWidget(m_confirmUpdateRecipeButton);
    m_buttonLayout->addWidget(m_cancelButton);

    // main layout
    m_mainLayout->addLayout(m_nameLayout);
    m_mainLayout->addLayout(m_descriptionLayout);
    m_mainLayout->addWidget(m_ingredientsBox);
    m_mainLayout->addLayout(m_buttonLayout);
}

void EditRecipe::createSpinbox()
{
    // spinbox definition
    m_ingredientQuantitySpinbox = new QDoubleSpinBox(this);

    // set value range and adaptive step
    m_ingredientQuantitySpinbox->setRange(0, 999);
    m_ingredientQuantitySpinbox->setStepType(QDoubleSpinBox::AdaptiveDecimalStepType);
}
