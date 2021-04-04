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

    // set QWidget with content as central widget
    m_centralWidget->setLayout(m_mainLayout);
}

void RecipePlanner::slotCloseApplication()
{
    close();
}

void RecipePlanner::slotAddRecipe()
{
    auto *editRecipeWindow = new EditRecipe(EditRecipe::Add, this);
    editRecipeWindow->exec();
}

void RecipePlanner::slotEditRecipe()
{

}

void RecipePlanner::slotDeleteRecipe()
{

}

void RecipePlanner::slotCreateMenu()
{

}

void RecipePlanner::slotLoadFile()
{

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
    auto *modelRoot = m_recipesModel->invisibleRootItem();

    int currentRow = 0; // current row in the tree

    for (auto &&recipeName : m_recipesJson.keys())
    {
        // add the name of recipe to the tree
        modelRoot->appendRow(new QStandardItem(recipeName));
        auto recipeItem = modelRoot->child(currentRow);

        // get json object with currently processed recipe
        auto recipeJsonObject = m_recipesJson[recipeName].toObject();

        // add description of recipe to the item with the name
        recipeItem->appendRow(new QStandardItem("recipe"));
        auto recipeDescription = modelRoot->child(currentRow)->child(0);
        auto recipeDescriptionArray = recipeJsonObject["recipe"].toArray();

        for (auto &&line : recipeDescriptionArray)
            recipeDescription->appendRow(new QStandardItem(line.toString()));

        int currentRowIngredient = 1; // current index of ingredient

        // add ingredients to the item with the name
        for (auto &&ingredient : recipeJsonObject.keys())
        {
            if (ingredient != "recipe") // if it is description then ignore it
            {
                recipeItem->appendRow(new QStandardItem(ingredient));
                recipeItem->child(currentRowIngredient)->appendRow(new QStandardItem(recipeJsonObject[ingredient].toString()));

                // go to the next ingredient
                currentRowIngredient++;
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

    // set lists properties
    m_recipesListView->setSelectionMode(QAbstractItemView::MultiSelection);
    m_recipesListView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
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
    m_recipesJson = QJsonDocument(QJsonDocument::fromJson(recipesData)).object();
    return true;
}
