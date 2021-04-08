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
}

void CreateMenu::slotRemoveRecipe()
{
    // get selected recipe
    auto selectedRecipeIndex = ui->m_selectedRecipesListView->selectionModel()->selectedRows();

    // remove itme from the list
    m_selectedRecipesModel->removeRow(selectedRecipeIndex.last().row());

    // update selection
    slotUpdateButtons();
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

    // set table's  properties
    ui->m_ingredientsTableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->m_ingredientsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
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
