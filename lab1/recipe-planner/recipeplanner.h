#ifndef RECIPEPLANNER_H
#define RECIPEPLANNER_H

#include <QMainWindow>

#include <QListWidget>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

class RecipePlanner : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *m_centralWidget;   // main widget
    QListWidget *m_recipeList;  // main list with added recipes
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
    QAction *m_exitAction;
    QMenu *m_fileMenu;

    // methods for constructor
    void createButtons();
    void createLayouts();
    void createActions();
    void createMenu();

public:
    RecipePlanner(QWidget *parent = nullptr);

};
#endif // RECIPEPLANNER_H
