#ifndef RECIPEPLANNER_H
#define RECIPEPLANNER_H

#include "editrecipe.h"

#include <QMainWindow>

#include <QListView>
#include <QMenuBar>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardItemModel>

class RecipePlanner : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *m_centralWidget;   // main widget
    QMenuBar *m_menuBar;        // menu bar

    // buttons
    QPushButton *m_addButton;
    QPushButton *m_editButton;
    QPushButton *m_deleteButton;

    // layouts
    QHBoxLayout *m_mainLayout;
    QVBoxLayout *m_buttonLayout;

    // actions and menu
    QAction *m_createMenuAction;
    QAction *m_openFile;
    QAction *m_exitAction;
    QMenu *m_fileMenu;

    // model for recipes and list of recipes
    QStandardItemModel *m_recipesModel;
    QListView *m_recipesListView;

    // json file
    QJsonObject m_recipesJson;

    // methods for constructor
    void createButtons();
    void createLayouts();
    void createActions();
    void createMenu();
    void createModel();
    void createList();

    // json handling
    bool readRecipesFromJson(QString fileName = "recipes.json");
    bool saveRecipesToJson();

public:
    RecipePlanner(QWidget *parent = nullptr);

private slots:
    void slotCloseApplication();    // close window main window with application
    void slotAddRecipe();           // open add recipe window
    void slotEditRecipe();          // open edit selected recipe
    void slotDeleteRecipe();        // open delete selected recipes
    void slotCreateMenu();          // open create menu window
    void slotLoadFile();            // open json file with recipes

};
#endif // RECIPEPLANNER_H
