#include "recipeplanner.h"

RecipePlanner::RecipePlanner(QWidget *parent)
    : QMainWindow(parent)
{
    // load recipes from json
    readRecipesFromJson();

    // set window size
    setMinimumSize(400, 330);
    resize(600, 500);

    // set window title
    setWindowTitle("Recipe Planner");

    // central widget definition
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    createModel();
    createList();
    createButtons();
    createLayouts();
    createActions();
    createMenu();
}

void RecipePlanner::slotCloseApplication()
{
    close();
}

void RecipePlanner::slotAddRecipe()
{
    // create window with add mode and pass the model
    m_editRecipeWindow = new EditRecipe(EditRecipe::Add, m_recipesModel, this);

    // connect slot which updates JSON object
    connect(m_editRecipeWindow, &EditRecipe::modelChanged, this, &RecipePlanner::slotUpdateJsonObject);

    // open window
    m_editRecipeWindow->exec();
}

void RecipePlanner::slotEditRecipe()
{
    // get index of currently edited recipe
    auto selectedRecipeIndex = m_recipesListView->selectionModel()->selectedIndexes()[0];

    // create window with edit mode and pass the model with edited recipe's index
    m_editRecipeWindow= new EditRecipe(EditRecipe::Edit, m_recipesModel, this, &selectedRecipeIndex);

    // connect edit recipe window to the slot responsible for updating the buttons
    connect(m_editRecipeWindow, &EditRecipe::rejected, this, &RecipePlanner::slotUpdateButtons);

    // connect slot which updates JSON object
    connect(m_editRecipeWindow, &EditRecipe::modelChanged, this, &RecipePlanner::slotUpdateJsonObject);

    // open window
    m_editRecipeWindow->exec();
}

void RecipePlanner::slotDeleteRecipes()
{
    // get selected indices
    auto selectedRecipes = new QList<QModelIndex>(m_recipesListView->selectionModel()->selectedRows());

    // remove row of each selected pair of indices
    while (!selectedRecipes->isEmpty())
    {
        // remove recipe from JSON
        m_recipesJson.remove(m_recipesModel->itemFromIndex(selectedRecipes->last())->text());

        // remove row containg selected index
        m_recipesModel->removeRow(selectedRecipes->last().row());
        selectedRecipes->removeLast();

        // delete previous list of ingredients indices and get new one with updated indices
        delete selectedRecipes;
        selectedRecipes = new QList<QModelIndex>(m_recipesListView->selectionModel()->selectedRows());
    }

    // free memory alocated for the list
    delete selectedRecipes;

    // clear selection
    m_recipesListView->selectionModel()->clearSelection();

    // update JSON file
    saveRecipesToJson();
}

void RecipePlanner::slotCreateMenu()
{
    // create window
    m_createMenuWindow = new CreateMenu(m_recipesModel, this);

    // open window
    m_createMenuWindow->exec();
}

void RecipePlanner::slotLoadFile()
{
}

void RecipePlanner::slotUpdateButtons()
{
    // get number of selected items in the list
    auto selectedItems = m_recipesListView->selectionModel()->selectedIndexes().count();

    switch (selectedItems)
    {
    case 0: // when nothing is selected allow only adding a new recipe
        m_addButton->setDisabled(false);
        m_editButton->setDisabled(true);
        m_deleteButton->setDisabled(true);
        break;

    case 1: // when one recipe is selected allow to edit or delete it
        m_addButton->setDisabled(true);
        m_editButton->setDisabled(false);
        m_deleteButton->setDisabled(false);
        break;

    default: // when multiple recipes are selected allow only for deleting
        m_addButton->setDisabled(true);
        m_editButton->setDisabled(true);
        m_deleteButton->setDisabled(false);
        break;
    }
}

void RecipePlanner::slotUpdateJsonObject(QStandardItem *updatedRecipeItem)
{
    // get description item
    auto descriptionItem = updatedRecipeItem->child(descriptionChildItem);

    // create JSON array with description
    QJsonArray descriptionJson;
    for (auto row = 0; row < descriptionItem->rowCount(); row++)
        descriptionJson.append(descriptionItem->child(row)->text());

    // create JSON object with recipe and add description
    QJsonObject recipeJson;
    recipeJson["recipe"] = descriptionJson;

    // get ingredients item
    auto ingredientsItem = updatedRecipeItem->child(ingredientsChildItem);

    // add ingredients to recipe JSON
    for (auto row = 0; row < ingredientsItem->rowCount(); row++)
        recipeJson[ingredientsItem->child(row, 0)->text()] = ingredientsItem->child(row, 1)->text();

    // get recipe name
    auto recipeName = updatedRecipeItem->text();

    // update main JSON object
    m_recipesJson[recipeName] = recipeJson;

    saveRecipesToJson();
}

void RecipePlanner::createButtons()
{
    // buttons definition
    m_addButton = new QPushButton("&Add", m_centralWidget);
    m_editButton = new QPushButton("&Edit", m_centralWidget);
    m_deleteButton = new QPushButton("&Delete", m_centralWidget);

    // set tooltips
    m_addButton->setToolTip("Add new recipe");
    m_editButton->setToolTip("Edit selected recipe");
    m_deleteButton->setToolTip("Delete selected recipes");

    // disable edit and delete buttons on default
    m_editButton->setDisabled(true);
    m_deleteButton->setDisabled(true);

    // connect buttons
    connect(m_addButton, &QPushButton::clicked, this, &RecipePlanner::slotAddRecipe);
    connect(m_editButton, &QPushButton::clicked, this, &RecipePlanner::slotEditRecipe);
    connect(m_deleteButton, &QPushButton::clicked, this, &RecipePlanner::slotDeleteRecipes);
}

void RecipePlanner::createLayouts()
{
    // layouts definition
    m_mainLayout = new QHBoxLayout(m_centralWidget);
    m_buttonLayout = new QVBoxLayout();

    m_buttonLayout->setAlignment(Qt::AlignTop); // move buttons to the top of right pane
    m_buttonLayout->setSpacing(15);             // make some space between buttons

    // add buttons to button layout
    m_buttonLayout->addWidget(m_addButton);
    m_buttonLayout->addWidget(m_editButton);
    m_buttonLayout->addWidget(m_deleteButton);

    // add list and button layout to the main layout
    m_mainLayout->addWidget(m_recipesListView);
    m_mainLayout->addLayout(m_buttonLayout);

    // set QWidget with content as central widget
    m_centralWidget->setLayout(m_mainLayout);
}

void RecipePlanner::createActions()
{
    // actions definition
    m_createMenuAction = new QAction("&Create menu", this);
    m_exitAction = new QAction("&Exit", this);
    m_openFile = new QAction("&Open", this);

    // set actions status tips
    m_createMenuAction->setStatusTip("Create menu from selected recipes");

    // add shortcuts for actions in menu
    m_createMenuAction->setShortcut(QKeySequence::New);
    m_openFile->setShortcut(QKeySequence::Open);
    m_exitAction->setShortcut(QKeySequence::Quit);

    // connect slots
    connect(m_exitAction, &QAction::triggered, this, &RecipePlanner::slotCloseApplication);
    connect(m_createMenuAction, &QAction::triggered, this, &RecipePlanner::slotCreateMenu);
}

void RecipePlanner::createMenu()
{
    // menu definition
    m_fileMenu = menuBar()->addMenu("&File");

    // action assignment
    m_fileMenu->addAction(m_createMenuAction);
    m_fileMenu->addAction(m_openFile);
    m_fileMenu->addAction(m_exitAction);
}

void RecipePlanner::createModel()
{
    // model definition
    m_recipesModel = new QStandardItemModel(this);

    // create root item as the model is a tree
    auto modelRoot = m_recipesModel->invisibleRootItem();

    auto currentRow = 0; // current row in the tree

    for (auto &&recipeName : m_recipesJson.keys())
    {
        // add the name of recipe to the tree
        modelRoot->appendRow(new QStandardItem(recipeName));
        auto recipeItem = modelRoot->child(currentRow);

        // get json object with currently processed recipe
        auto recipeJsonObject = m_recipesJson[recipeName].toObject();

        // add description of recipe to the item with the name
        recipeItem->appendRow(new QStandardItem("recipe"));
        auto recipeDescription = recipeItem->child(descriptionChildItem);
        auto recipeDescriptionArray = recipeJsonObject["recipe"].toArray();

        for (auto &&line : recipeDescriptionArray)
            recipeDescription->appendRow(new QStandardItem(line.toString()));

        // add ingredients to the item with the name
        recipeItem->appendRow(new QStandardItem("ingredients"));
        auto recipeIngredients = recipeItem->child(ingredientsChildItem);

        for (auto &&ingredient : recipeJsonObject.keys())
        {
            if (ingredient != "recipe") // if it is description then ignore it
            {
                // create a temporary list with row of ingredient's name, number and unit
                auto rowList = QList<QStandardItem*>();
                rowList.append(new QStandardItem(ingredient));
                rowList.append(new QStandardItem(recipeJsonObject[ingredient].toString()));
                rowList[1]->setTextAlignment(Qt::AlignCenter);

                // add content from list to the model
                recipeIngredients->appendRow(rowList);
            }
        }

        // go to the next recipe
        currentRow++;
    }
}

void RecipePlanner::createList()
{
    // list view definition
    m_recipesListView = new QListView(m_centralWidget);

    // set model with recipes as model for the list
    m_recipesListView->setModel(m_recipesModel);

    // set list's properties
    m_recipesListView->setSelectionMode(QAbstractItemView::MultiSelection);
    m_recipesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // connect list to the slot responsible for disabling buttons
    connect(m_recipesListView, &QListView::pressed, this, &RecipePlanner::slotUpdateButtons);
}

bool RecipePlanner::readRecipesFromJson(QString fileName)
{
    // open selected file or deafualt - recipes.json
    QFile loadFile(fileName);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qFatal("Could not open file with recipes."); // inform if something went wrong and then return false
        return false;
    }

    // load data from file
    auto recipesData = loadFile.readAll();
    loadFile.close();

    // save data in QJsonObject object
    m_recipesJson = QJsonDocument::fromJson(recipesData).object();
    return true;
}

bool RecipePlanner::saveRecipesToJson(QString fileName)
{
    // open selected file or deafualt - recipes.json
    QFile saveFile(fileName);

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qFatal("Could not open file with recipes."); // inform if something went wrong and then return false
        return false;
    }

    // create JSON document object
    QJsonDocument saveDoc(m_recipesJson);
    saveFile.write(saveDoc.toJson());
    saveFile.close();

    return true;
}
