#include "editrecipe.h"

EditRecipe::EditRecipe(EditMode mode, QStandardItemModel *recipesModel, QWidget *parent, QModelIndex *recipeIndex)
    : QDialog(parent)
{
    // set window size
    setMinimumSize(340, 500);
    resize(350, 600);

    // set window title
    setWindowTitle(mode == Add ? "New recipe" : "Edit recipe");

    // set pointers to model and recipe's item index
    m_recipesModel = recipesModel;
    m_recipeIndex = recipeIndex;

    // call helper methods to initialize all objects
    createTable(m_recipeIndex);
    createLabels();
    createLineAndTextEdits();
    createPushButtons(mode);
    createSpinbox();
    createLayouts();

    // set main layout
    setLayout(m_mainLayout);
}

EditRecipe::~EditRecipe()
{
    delete m_recipeIndex;
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

void EditRecipe::createLineAndTextEdits()
{
    // line edits definition
    m_nameLineEdit = new QLineEdit(m_recipesModel->itemFromIndex(*m_recipeIndex)->text(), this);
    m_ingredientNameLineEdit = new QLineEdit(this);
    m_ingredientUnitLineEdit = new QLineEdit(this);

    // text edit definition
//    m_descriptionTextEdit = new QTextEdit(previousDescription.join(' '), this);
    m_descriptionTextEdit = new QTextEdit(this);
}

void EditRecipe::createPushButtons(EditMode mode)
{
    // buttons definition
    m_confirmUpdateRecipeButton = new QPushButton(mode == Add ? "&Ok" : "&Update", this); // determine whether it is new recipe or existing one
    m_cancelButton = new QPushButton("&Cancel", this);
    m_addIngredientButton = new QPushButton("&Add", this);
    m_editIngredientButton = new QPushButton("&Edit", this);
    m_deleteIngredientButton = new QPushButton("&Delete", this);

    // disable delete and update button as default option
    // it will be enabled when ingredient is being selected from the list
    m_deleteIngredientButton->setDisabled(true);
    m_editIngredientButton->setDisabled(true);
}

void EditRecipe::createLayouts()
{
    // layouts definition
    m_mainLayout = new QVBoxLayout(this);
    m_buttonLayout = new QHBoxLayout(this);
    m_descriptionLayout = new QVBoxLayout(this);
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
    m_ingredientButtonLayout->addWidget(m_addIngredientButton);
    m_ingredientButtonLayout->addWidget(m_editIngredientButton);
    m_ingredientButtonLayout->addWidget(m_deleteIngredientButton);

    m_ingredientButtonLayout->setAlignment(Qt::AlignLeft);

    // ingredients group box
    m_ingredientLayout->addLayout(m_ingredientNameLayout);
    m_ingredientLayout->addWidget(m_ingredientsTableView);
    m_ingredientLayout->addLayout(m_ingredientQuantityLayout);
    m_ingredientLayout->addLayout(m_ingredientButtonLayout);

    m_ingredientsBox->setLayout(m_ingredientLayout);
    m_ingredientLayout->setContentsMargins(20, 20, 20, 20);

    // button layout
    m_buttonLayout->addWidget(m_confirmUpdateRecipeButton);
    m_buttonLayout->addWidget(m_cancelButton);

    m_buttonLayout->setAlignment(Qt::AlignRight);

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

void EditRecipe::createTable(QModelIndex *&recipeIndex)
{
    // pointer to the root of model
    auto modelRoot = m_recipesModel->invisibleRootItem();

    if (recipeIndex == nullptr) // if index is empty create new recipe
    {
        recipeIndex = new QModelIndex();
        modelRoot->appendRow(new QStandardItem("New recipe"));
        auto newRecipe = modelRoot->child(modelRoot->rowCount() - 1); // index of the added recipe

        // create description and ingredients items
        newRecipe->appendRow(new QStandardItem("recipe"));
        newRecipe->appendRow(new QStandardItem("ingredients"));

        // get new item's index
        *recipeIndex = newRecipe->index();
    }

    // table view definition
    m_ingredientsTableView = new QTableView(this);

    // set model with recipes as model for the list
    m_ingredientsTableView->setModel(m_recipesModel);

    // set root of the view to currently processed recipe
    m_ingredientsTableView->setRootIndex(m_recipesModel->index(1, 0, *recipeIndex));

    // hide headers
    m_ingredientsTableView->horizontalHeader()->setVisible(false);
    m_ingredientsTableView->verticalHeader()->setVisible(false);
}
