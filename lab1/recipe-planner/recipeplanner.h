#ifndef RECIPEPLANNER_H
#define RECIPEPLANNER_H

#include "editrecipe.h"
#include "createmenu.h"

#include <QMainWindow>

#include <QListView>
#include <QMenuBar>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>

#include <QFileDialog>

class RecipePlanner : public QMainWindow
{
    Q_OBJECT

private:
    // main widget
    QWidget *m_centralWidget;

    // menu bar
    QMenuBar *m_menuBar;

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
    QListView *m_recipesListView = nullptr;

    // json file
    QJsonObject m_recipesJson;

    // dialog windows
    EditRecipe *m_editRecipeWindow;
    CreateMenu *m_createMenuWindow;

    // file name
    QString m_jsonFileName;

    // methods for constructor
    void createButtons();
    void createLayouts();
    void createActions();
    void createMenu();
    void createModel();
    void createList();

    // json handling
    bool readRecipesFromJson(QString);
    bool saveRecipesToJson(QString);

    // constant values to get rid of magic numbers
    const int descriptionChildItem = 0;
    const int ingredientsChildItem = 1;

public:
    RecipePlanner(QWidget *parent = nullptr);

private slots:
    void slotCloseApplication();    // close window main window with application
    void slotAddRecipe();           // open add recipe window
    void slotEditRecipe();          // open edit selected recipe
    void slotDeleteRecipes();        // open delete selected recipes
    void slotCreateMenu();          // open create menu window
    void slotLoadFile();            // open json file with recipes
    void slotUpdateButtons();       // disable or enable buttons according to the current state of selection
    void slotUpdateJsonObject(QStandardItem*);

};
#endif // RECIPEPLANNER_H
