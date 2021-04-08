#ifndef CREATEMENU_H
#define CREATEMENU_H

#include <QDialog>

#include <QStandardItemModel>
#include <QStringListModel>

#include <QTableView>

namespace Ui {
class CreateMenu;
}

class CreateMenu : public QDialog
{
    Q_OBJECT

private:
    // pointer on form object
    Ui::CreateMenu *ui;

    // models
    QStandardItemModel *m_recipeModel;
    QStringListModel *m_selectedRecipesModel;
    QStandardItemModel *m_ingredientsModel;

    QTableView *m_test;

    // methods for constructor
    void adjustLayouts();
    void createModels();
    void adjustLists();
    void adjustTable();
    void connectSlots();

    // auxiliary methods
    QString getUnit(QString);
    QString getQuantity(QString);
    void addIngredient(QString, QString, QString);
    void removeIngredient(QString, QString, QString);
    void addIngredients(QStandardItem*);
    void removeIngredients(QString);

public:
    explicit CreateMenu(QStandardItemModel*, QWidget *parent = nullptr);
    ~CreateMenu();

private slots:
    void slotUpdateButtons();
    void slotCloseWindow();
    void slotAddRecipe();
    void slotRemoveRecipe();
};

#endif // CREATEMENU_H
