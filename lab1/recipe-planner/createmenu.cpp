#include "createmenu.h"
#include "ui_createmenu.h"

CreateMenu::CreateMenu(QStandardItemModel *recipeModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMenu)
{
    // create UI from form file
    ui->setupUi(this);

    // set main recipe model
    m_recipeModel = recipeModel;

    // call helper methods
    adjustLayouts();
    createModels();
    adjustLists();
    adjustTable();
    connectSlots();
}

CreateMenu::~CreateMenu()
{
    delete ui;
}

void CreateMenu::slotUpdateButtons()
{
    // get selected row from recipe list
    auto selectedRecipeMainList = ui->m_recipeListView->selectionModel()->selectedRows();

    // get selected row from chosen recipes list
    auto selectedRecipeChosenList = ui->m_selectedRecipesListView->selectionModel()->selectedRows();

    // disable button if nothing is selected
    selectedRecipeMainList.length() == 0 ? ui->m_addButton->setDisabled(true) : ui->m_addButton->setDisabled(false);
    selectedRecipeChosenList.length() == 0 ? ui->m_removeButton->setDisabled(true) : ui->m_removeButton->setDisabled(false);
}

void CreateMenu::slotCloseWindow()
{
    // close window
    close();
}

void CreateMenu::slotAddRecipe()
{
    // get selected recipe
    auto selectedRecipeIndex = ui->m_recipeListView->selectionModel()->selectedRows();
    auto selectedRecipeItem = m_recipeModel->itemFromIndex(selectedRecipeIndex.first());

    // add selected recipe to chosen recipes model
    auto stringList = m_selectedRecipesModel->stringList();
    stringList.append(selectedRecipeItem->text());
    m_selectedRecipesModel->setStringList(stringList);

    // add all ingredients from recipe
    addIngredients(selectedRecipeItem);
}

void CreateMenu::slotRemoveRecipe()
{
    // get selected recipe
    auto selectedRecipeIndex = ui->m_selectedRecipesListView->selectionModel()->selectedRows();
    auto selectedRecipeName = m_selectedRecipesModel->stringList()[selectedRecipeIndex.last().row()];

    // remove itme from the list
    m_selectedRecipesModel->removeRow(selectedRecipeIndex.last().row());

    // update selection
    slotUpdateButtons();

    // remove all ingredients from recipe
    removeIngredients(selectedRecipeName);
}

void CreateMenu::adjustLayouts()
{
    // set margins for content in this dialog
    ui->m_mainLayout->setContentsMargins(20, 20, 20, 20);

    // align buttons
    ui->m_buttonLayout->setAlignment(Qt::AlignLeft);

    // set main layout
    setLayout(ui->m_mainLayout);
}

void CreateMenu::createModels()
{
    // model definition
    m_selectedRecipesModel = new QStringListModel(this);
    m_ingredientsModel = new QStandardItemModel(this);
}

void CreateMenu::adjustLists()
{
    // connect lists to the models
    ui->m_recipeListView->setModel(m_recipeModel);
    ui->m_selectedRecipesListView->setModel(m_selectedRecipesModel);

    // set lists' properties
    ui->m_recipeListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->m_recipeListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->m_selectedRecipesListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->m_selectedRecipesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void CreateMenu::adjustTable()
{
    // connect view with model
    ui->m_ingredientsTableView->setModel(m_ingredientsModel);

    // set table's selection properties
    ui->m_ingredientsTableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->m_ingredientsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // hide headers
    ui->m_ingredientsTableView->horizontalHeader()->setVisible(false);
    ui->m_ingredientsTableView->verticalHeader()->setVisible(false);

    // hide grid
    ui->m_ingredientsTableView->setShowGrid(false);

    // stretch the last column to fill whole table with content
    ui->m_ingredientsTableView->horizontalHeader()->setStretchLastSection(true);
}

void CreateMenu::connectSlots()
{
    // disable/enable buttons
    connect(ui->m_recipeListView, &QListView::clicked, this, &CreateMenu::slotUpdateButtons);
    connect(ui->m_selectedRecipesListView, &QListView::clicked, this, &CreateMenu::slotUpdateButtons);

    // close window button
    connect(ui->m_cancelButton, &QPushButton::clicked, this, &CreateMenu::slotCloseWindow);

    // add recipe to the menu
    connect(ui->m_addButton, &QPushButton::clicked, this, &CreateMenu::slotAddRecipe);

    // remove recipe from the menu
    connect(ui->m_removeButton, &QPushButton::clicked, this, &CreateMenu::slotRemoveRecipe);
}

QString CreateMenu::getUnit(QString quantityWithUnit)
{
    // get substring starting with the first space character
    auto unit = quantityWithUnit.mid(quantityWithUnit.indexOf(' '));

    // remove leading space character
    unit = unit.remove(0, 1);

    return unit;
}

QString CreateMenu::getQuantity(QString quantityWithUnit)
{
    // get first characters before space
    return quantityWithUnit.left(quantityWithUnit.indexOf(' '));
}

void CreateMenu::addIngredient(QString name, QString quantity, QString unit)
{
    // get ingredients with the same name
    auto ingredientsWithSameName = m_ingredientsModel->findItems(name);

    // determine whether there is already ingredient with the same name
    if (ingredientsWithSameName.length() == 0)
    {
        // create a temporary list with row of ingredient's name, number and unit
        auto rowList = QList<QStandardItem*>();
        rowList.append(new QStandardItem(name));
        rowList.append(new QStandardItem(quantity));
        rowList.append(new QStandardItem(unit));

        // add row to the model
        m_ingredientsModel->appendRow(rowList);
    }
    else
    {
        // check every ingredient with the same name
        for (auto &&ingredient : ingredientsWithSameName)
        {
            // get unit
            auto ingredientIndex = ingredient->index();
            auto unitIndex = m_ingredientsModel->index(ingredientIndex.row(), 2);
            auto ingredientUnit = m_ingredientsModel->itemFromIndex(unitIndex);

            // if unit is the same add quantity to existing one
            if (ingredientUnit->text() == unit)
            {
                // get quantity
                auto quantityIndex = m_ingredientsModel->index(ingredientIndex.row(), 1);
                auto ingredientQuantity = m_ingredientsModel->itemFromIndex(quantityIndex)->text().toDouble();

                // calculate new quantity
                auto newQuantity = ingredientQuantity + quantity.toDouble();

                // update quantity in the model
                m_ingredientsModel->itemFromIndex(quantityIndex)->setText(QString::number(newQuantity));

                // leave method
                return;
            }
        }

        // create a temporary list with row of ingredient's name, number and unit
        auto rowList = QList<QStandardItem*>();
        rowList.append(new QStandardItem(name));
        rowList.append(new QStandardItem(quantity));
        rowList.append(new QStandardItem(unit));

        // add row to the model
        m_ingredientsModel->appendRow(rowList);
    }
}

void CreateMenu::removeIngredient(QString name, QString quantity, QString unit)
{
    // get ingredients with the same name
    auto ingredientsWithSameName = m_ingredientsModel->findItems(name);

    // determine whether there is already ingredient with the same name
    if (ingredientsWithSameName.length() != 0)
    {
        // check every ingredient with the same name
        for (auto &&ingredient : ingredientsWithSameName)
        {
            // get unit
            auto ingredientIndex = ingredient->index();
            auto unitIndex = m_ingredientsModel->index(ingredientIndex.row(), 2);
            auto ingredientUnit = m_ingredientsModel->itemFromIndex(unitIndex);

            // if unit is the same add quantity to existing one
            if (ingredientUnit->text() == unit)
            {
                // get quantity
                auto quantityIndex = m_ingredientsModel->index(ingredientIndex.row(), 1);
                auto ingredientQuantity = m_ingredientsModel->itemFromIndex(quantityIndex)->text().toDouble();

                // calculate new quantity
                auto newQuantity = ingredientQuantity - quantity.toDouble();

                // if new quantity is zero remove ingredient from the model
                if (newQuantity == 0)
                    m_ingredientsModel->removeRow(ingredientsWithSameName.last()->row());
                else
                    m_ingredientsModel->itemFromIndex(quantityIndex)->setText(QString::number(newQuantity));

                // leave method
                return;
            }
        }
    }
}

void CreateMenu::addIngredients(QStandardItem *recipe)
{
    // get ingredients item from model
    auto ingredients = recipe->child(1);

    for (auto row = 0; row < ingredients->rowCount(); row++)
    {
        // get ingredient name, quantity and unit
        auto ingredientName = ingredients->child(row, 0)->text();
        auto ingredientQuantity = getQuantity(ingredients->child(row, 1)->text());
        auto ingredientUnit = getUnit(ingredients->child(row, 1)->text());

        // add ingredient to the model
        addIngredient(ingredientName, ingredientQuantity, ingredientUnit);
    }
}

void CreateMenu::removeIngredients(QString recipeName)
{
    // get ingredients item from model
    auto recipeItem = m_recipeModel->findItems(recipeName);
    auto ingredients = recipeItem.last()->child(1);

    for (auto row = 0; row < ingredients->rowCount(); row++)
    {
        // get ingredient name, quantity and unit
        auto ingredientName = ingredients->child(row, 0)->text();
        auto ingredientQuantity = getQuantity(ingredients->child(row, 1)->text());
        auto ingredientUnit = getUnit(ingredients->child(row, 1)->text());

        // add ingredient to the model
        removeIngredient(ingredientName, ingredientQuantity, ingredientUnit);
    }
}
