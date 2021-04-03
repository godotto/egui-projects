#include "recipeplanner.h"

RecipePlanner::RecipePlanner(QWidget *parent)
    : QMainWindow(parent)
{
    // set window size
    setMinimumSize(400, 330);
    resize(600, 500);

    // set window title
    setWindowTitle("Recipe Planner");

    // central widget definition
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    // list defiinition
    m_recipeList = new QListWidget(m_centralWidget);

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
    EditRecipe *editRecipeWindow = new EditRecipe(EditRecipe::Add, this);
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
    m_mainLayout->addWidget(m_recipeList);
    m_mainLayout->addLayout(m_buttonLayout);
}

void RecipePlanner::createActions()
{
    // actions definition
    m_createMenuAction = new QAction("&Create menu", this);
    m_exitAction = new QAction("&Exit", this);

    // set actions status tips
    m_createMenuAction->setStatusTip("Create menu from selected recipes");

    // add shortcuts for actions in menu
    m_createMenuAction->setShortcut(QKeySequence::New);
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
    m_fileMenu->addAction(m_exitAction);
}
