#include "editrecipe.h"

EditRecipe::EditRecipe(EditMode mode, QStandardItemModel *recipesModel, QWidget *parent, QModelIndex *recipeIndex)
    : QDialog(parent)
{
    // set window size
    setMinimumSize(340, 500);
    resize(350, 600);

    // get edit mode
    m_mode = mode;

    // set window title
    setWindowTitle(this->m_mode == Add ? "New recipe" : "Edit recipe");

    // set pointers to model and recipe's item index
    m_recipesModel = recipesModel;
    m_recipeIndex = recipeIndex;

    // make a copy of item with recipe in case of discarding changes
    if (m_mode == Edit)
        m_copyOfRecipeItem = copyItem(m_recipesModel->itemFromIndex(*m_recipeIndex));

    // call helper methods to initialize all objects
    setRecipeIndex(m_recipeIndex);
    createTable();
    createLabels();
    createLineAndTextEdits();
    createPushButtons();
    createSpinbox();
    createLayouts();

    // connect rejected signal from dialog to slot which will revert changes
    connect(this, &EditRecipe::rejected, this, &EditRecipe::slotDiscardChanges);
}

void EditRecipe::slotAddIngredient()
{
    // get new ingredient's name and quantity with unit from line edit and spin box
    auto name = m_ingredientNameLineEdit->text();
    auto quantityAndUnit = QString(m_ingredientQuantitySpinbox->text() + " " + m_ingredientUnitLineEdit->text());

    // make sure that input data is not empty and that ingredient is unique
    if (name != "" && getUnit(quantityAndUnit) != "" && getQuantity(quantityAndUnit) != "" && numberOfSameIngredients(name, m_ingredientUnitLineEdit->text()) == 0)
    {
        // get item with ingredients
        auto ingredients = m_recipesModel->itemFromIndex(*m_recipeIndex)->child(ingredientsChildItem);

        // create a temporary list with row of ingredient's name, number and unit
        auto rowList = QList<QStandardItem*>();
        rowList.append(new QStandardItem(name));
        rowList.append(new QStandardItem(quantityAndUnit));
        rowList[1]->setTextAlignment(Qt::AlignCenter);

        // add row to the model
        ingredients->appendRow(rowList);

        // clear line edits and spin box
        m_ingredientNameLineEdit->clear();
        m_ingredientQuantitySpinbox->setValue(0);
        m_ingredientUnitLineEdit->clear();
    }
}

void EditRecipe::slotEditIngredient()
{
    // get new ingredient's name and quantity with unit from line edit and spin box
    auto name = m_ingredientNameLineEdit->text();
    auto quantityAndUnit = QString(m_ingredientQuantitySpinbox->text() + " " + m_ingredientUnitLineEdit->text());

    // make sure that input data is not empty and that ingredient is unique
    if (name != "" && getUnit(quantityAndUnit) != "" && getQuantity(quantityAndUnit) != "" && numberOfSameIngredients(name, m_ingredientUnitLineEdit->text()) <= 1)
    {
        // get selected indices
        auto selectedIngredientIndices = m_ingredientsTableView->selectionModel()->selectedIndexes();

        // update name and/or quantity with unit
        m_recipesModel->itemFromIndex(selectedIngredientIndices[0])->setText(name);
        m_recipesModel->itemFromIndex(selectedIngredientIndices[1])->setText(quantityAndUnit);

        // clear line edits and spin box
        m_ingredientNameLineEdit->clear();
        m_ingredientQuantitySpinbox->setValue(0);
        m_ingredientUnitLineEdit->clear();

        // clear selection
        m_ingredientsTableView->selectionModel()->clearSelection();
    }
}

void EditRecipe::slotDeleteIngredients()
{
    // get selected indices
    auto selectedIngredientIndices = new QList<QModelIndex>(m_ingredientsTableView->selectionModel()->selectedIndexes());

    // get index with ingredients
    auto ingredientsIndex = m_recipesModel->itemFromIndex(*m_recipeIndex)->child(ingredientsChildItem)->index();

    // remove row of each selected pair of indices
    while (!selectedIngredientIndices->isEmpty())
    {
        // remove row containg selected index
        m_recipesModel->removeRow(selectedIngredientIndices->last().row(), ingredientsIndex);

        // as indices are in pairs remove both of them from the list
        selectedIngredientIndices->removeLast();
        selectedIngredientIndices->removeLast();

        // delete previous list of ingredients indices and get new one with updated indices
        delete selectedIngredientIndices;
        selectedIngredientIndices = new QList<QModelIndex>(m_ingredientsTableView->selectionModel()->selectedIndexes());
    }

    // free memory alocated for the list
    delete selectedIngredientIndices;

    // clear selection
    m_ingredientsTableView->selectionModel()->clearSelection();
}

void EditRecipe::slotUpdateButtonsAndFields()
{
    // get number of selected items in the list
    auto selectedItems = m_ingredientsTableView->selectionModel()->selectedIndexes().count();

    switch (selectedItems)
    {
    case 0: // when nothing is selected allow only adding a new ingredient
        m_addIngredientButton->setDisabled(false);
        m_editIngredientButton->setDisabled(true);
        m_deleteIngredientsButton->setDisabled(true);

        m_ingredientNameLineEdit->clear();
        m_ingredientQuantitySpinbox->setValue(0);
        m_ingredientUnitLineEdit->clear();

        break;

    case 2: // when one ingredient is selected allow to edit or delete it (it has to be case as one row counts as two indices)
        m_addIngredientButton->setDisabled(true);
        m_editIngredientButton->setDisabled(false);
        m_deleteIngredientsButton->setDisabled(false);
        break;

    default: // when ingredients are selected allow only for deleting
        m_addIngredientButton->setDisabled(true);
        m_editIngredientButton->setDisabled(true);
        m_deleteIngredientsButton->setDisabled(false);

        m_ingredientNameLineEdit->clear();
        m_ingredientQuantitySpinbox->setValue(0);
        m_ingredientUnitLineEdit->clear();

        break;
    }
}

void EditRecipe::slotFillEditFields(const QModelIndex &clickedIndex)
{
    // get clicked row and ingredient index
    auto clickedRow = clickedIndex.row();
    auto ingredientsItem = m_recipesModel->itemFromIndex(*m_recipeIndex)->child(ingredientsChildItem);

    // update ingredient's name line edit
    m_ingredientNameLineEdit->setText(ingredientsItem->child(clickedRow, 0)->text());

    // get ingredient's quantity and unit
    auto quantityWithUnit = ingredientsItem->child(clickedRow, 1)->text();

    // update ingredient's unit line edit and quantity spin box
    m_ingredientQuantitySpinbox->setValue(getQuantity(quantityWithUnit).toDouble());
    m_ingredientUnitLineEdit->setText(getUnit(quantityWithUnit));
}

void EditRecipe::slotDiscardChanges()
{
    // remove currently processed recipe from model
    m_recipesModel->removeRow(m_recipeIndex->row());

    // if it is a new recipe, do nothing more; else restore recipe from copy
    if (m_mode == Edit)
        m_recipesModel->appendRow(m_copyOfRecipeItem);
}

void EditRecipe::slotCloseWindow()
{
    // close the window
    close();
}

void EditRecipe::slotConfirmRecipe()
{
    // update recipe name
    m_recipesModel->itemFromIndex(*m_recipeIndex)->setText(m_nameLineEdit->text());

    // get description from text edit and split it
    auto splittedDescription = m_descriptionTextEdit->toPlainText().split('\n');

    // get item with description
    auto descriptionItem = m_recipesModel->itemFromIndex(*m_recipeIndex)->child(descriptionChildItem);

    // if recipe is in edit mode remove previous description from model
    if (m_mode == Edit)
        m_recipesModel->removeRows(0, descriptionItem->rowCount(), descriptionItem->index());

    // update description
    for (auto &&line : splittedDescription)
        descriptionItem->appendRow(new QStandardItem(line));

    // free memory of copied recipe item
    if (m_mode == Edit)
        delete m_copyOfRecipeItem;

    // emit singal with changed item and close window
    emit modelChanged(m_recipesModel->itemFromIndex(*m_recipeIndex));
    accept();
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

    // string for description
    QString concatenatedDescription = "";

    // get item with description
    auto descriptionItem = m_recipesModel->itemFromIndex(*m_recipeIndex)->child(descriptionChildItem);

    // create one description string out of each line
    for (auto row = 0; row < descriptionItem->rowCount(); row++)
        concatenatedDescription += descriptionItem->child(row)->text() + '\n';

    // chop additional line break
    concatenatedDescription.chop(1);

    // text edit definition
    m_descriptionTextEdit = new QTextEdit(this);
    m_descriptionTextEdit->setPlainText(concatenatedDescription);
}

void EditRecipe::createPushButtons()
{
    // buttons definition
    m_confirmUpdateRecipeButton = new QPushButton(m_mode == Add ? "&Ok" : "&Update", this); // determine whether it is new recipe or existing one
    m_cancelButton = new QPushButton("&Cancel", this);
    m_addIngredientButton = new QPushButton("&Add", this);
    m_editIngredientButton = new QPushButton("&Edit", this);
    m_deleteIngredientsButton = new QPushButton("&Delete", this);

    // disable delete and update button as default option
    // it will be enabled when ingredient is being selected from the list
    m_deleteIngredientsButton->setDisabled(true);
    m_editIngredientButton->setDisabled(true);

    // connect buttons
    connect(m_addIngredientButton, &QPushButton::clicked, this, &EditRecipe::slotAddIngredient);
    connect(m_editIngredientButton, &QPushButton::clicked, this, &EditRecipe::slotEditIngredient);
    connect(m_deleteIngredientsButton, &QPushButton::clicked, this, &EditRecipe::slotDeleteIngredients);
    connect(m_cancelButton, &QPushButton::clicked, this, &EditRecipe::slotCloseWindow);
    connect(m_confirmUpdateRecipeButton, &QPushButton::clicked, this, &EditRecipe::slotConfirmRecipe);
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
    m_ingredientButtonLayout->addWidget(m_deleteIngredientsButton);

    m_ingredientButtonLayout->setAlignment(Qt::AlignLeft);

    // ingredients group box
    m_ingredientLayout->addWidget(m_ingredientsTableView);
    m_ingredientLayout->addLayout(m_ingredientNameLayout);
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

    // set main layout
    setLayout(m_mainLayout);
}

void EditRecipe::createSpinbox()
{
    // spinbox definition
    m_ingredientQuantitySpinbox = new QDoubleSpinBox(this);

    // set value range and adaptive step
    m_ingredientQuantitySpinbox->setRange(0, 999);
    m_ingredientQuantitySpinbox->setStepType(QDoubleSpinBox::AdaptiveDecimalStepType);
}

void EditRecipe::setRecipeIndex(QModelIndex *&recipeIndex)
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
}

void EditRecipe::createTable()
{
    // table view definition
    m_ingredientsTableView = new QTableView(this);

    // set model with recipes as model for the list
    m_ingredientsTableView->setModel(m_recipesModel);

    // set root of the view to currently processed recipe's ingredients
    m_ingredientsTableView->setRootIndex(m_recipesModel->index(ingredientsChildItem, 0, *m_recipeIndex));

    // hide headers
    m_ingredientsTableView->horizontalHeader()->setVisible(false);
    m_ingredientsTableView->verticalHeader()->setVisible(false);

    // hide grid
    m_ingredientsTableView->setShowGrid(false);

    // stretch the last column to fill whole table with content
    m_ingredientsTableView->horizontalHeader()->setStretchLastSection(true);

    // set selection rules
    m_ingredientsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ingredientsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_ingredientsTableView->setSelectionMode(QAbstractItemView::MultiSelection);

    // connect table to slot responsible for filling edit fields
    connect(m_ingredientsTableView, &QTableView::clicked, this, &EditRecipe::slotFillEditFields);

    // connect table to the slot responsible for disabling buttons
    connect(m_ingredientsTableView, &QTableView::clicked, this, &EditRecipe::slotUpdateButtonsAndFields);
    connect(m_ingredientsTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &EditRecipe::slotUpdateButtonsAndFields);
}

QString EditRecipe::getUnit(QString quantityWithUnit)
{
    // get substring starting with the first space character
    auto unit = quantityWithUnit.mid(quantityWithUnit.indexOf(' '));

    // remove leading space character
    unit = unit.remove(0, 1);

    return unit;
}

QString EditRecipe::getQuantity(QString quantityWithUnit)
{
    // get first characters before space
    return quantityWithUnit.left(quantityWithUnit.indexOf(' '));
}

int EditRecipe::numberOfSameIngredients(QString name, QString unit)
{
    // initialize counter variable
    auto numberOfSameIngredients = 0;

    // get list of ingredients with the same name
    auto ingredientItems = m_recipesModel->findItems(name, Qt::MatchRecursive);

    for (auto &&ingredient : ingredientItems)
    {
        // get string of quantity with unit
        auto quantityWithUnit = m_recipesModel->itemFromIndex(ingredient->index().siblingAtColumn(1))->text();

        // if there is already an ingredient with the same name and unit increment the counter
        if (ingredient->text() == name && getUnit(quantityWithUnit) == unit)
            numberOfSameIngredients++;
    }

    // return counter's value
    return numberOfSameIngredients;
}

QStandardItem *EditRecipe::copyItem(QStandardItem *original)
{
    // create pointer to copy of recipe item
    auto *copy = new QStandardItem(original->text());

    // append description and ingredients rows
    copy->appendRow(new QStandardItem("recipe"));
    copy->appendRow(new QStandardItem("ingredients"));

    // copy description of recipe
    for (auto row = 0; row < original->child(descriptionChildItem)->rowCount(); row++)
        copy->child(descriptionChildItem)->appendRow(original->child(descriptionChildItem)->child(row)->clone());

    // copy ingredients of recipe
    for (auto row = 0; row < original->child(ingredientsChildItem)->rowCount(); row++)
    {
        // create a temporary list with row of ingredient's name, number and unit
        auto rowList = QList<QStandardItem*>();
        rowList.append(original->child(ingredientsChildItem)->child(row, 0)->clone());
        rowList.append(original->child(ingredientsChildItem)->child(row, 1)->clone());
        rowList[1]->setTextAlignment(Qt::AlignCenter);

        // append both columns
        copy->child(ingredientsChildItem)->appendRow(rowList);
    }

    // return pointer
    return copy;
}
