#ifndef EDITRECIPE_H
#define EDITRECIPE_H

#include <QDialog>

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QGroupBox>

#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>

class EditRecipe : public QDialog
{
    Q_OBJECT

public:
    // enumeration type determining whether new recipe is added or existing one is being edited
    enum EditMode
    {
        Add, Edit
    };

private:
    // labels
    QLabel *m_nameLabel;
    QLabel *m_descriptionLabel;
    QLabel *m_ingredientNameLabel;
    QLabel *m_ingredientQuantityLabel;
    QLabel *m_ingredientUnitLabel;

    // line edits
    QLineEdit *m_nameLineEdit;
    QLineEdit *m_ingredientNameLineEdit;
    QLineEdit *m_ingredientUnitLineEdit;

    // text edit
    QTextEdit *m_descriptionTextEdit;

    // pushbuttons
    QPushButton *m_addIngredientButton;
    QPushButton *m_editIngredientButton;
    QPushButton *m_deleteIngredientButton;
    QPushButton *m_confirmUpdateRecipeButton;
    QPushButton *m_cancelButton;

    // spin box for quantity
    QDoubleSpinBox *m_ingredientQuantitySpinbox;

    // layouts
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_nameLayout;
    QVBoxLayout *m_descriptionLayout;
    QVBoxLayout *m_ingredientLayout;
    QHBoxLayout *m_ingredientNameLayout;
    QHBoxLayout *m_ingredientQuantityLayout;
    QHBoxLayout *m_ingredientButtonLayout;
    QHBoxLayout *m_buttonLayout;

    // table of ingredients and pointer to model
    QTableView *m_ingredientsTableView;
    QStandardItemModel *m_recipesModel;
    QModelIndex *m_recipeIndex;

    // group box for ingredients
    QGroupBox *m_ingredientsBox;

    // constant values to get rid of magic numbers
    const int descriptionChildItem = 0;
    const int ingredientsChildItem = 1;

    // methods for constructor
    void createLabels();
    void createLineAndTextEdits();
    void createPushButtons(EditMode);
    void createLayouts();
    void createSpinbox();
    void createTable(QModelIndex*&);

public:
    EditRecipe(EditMode, QStandardItemModel*, QWidget *parent = nullptr, QModelIndex *recipeIndex = nullptr);
    ~EditRecipe();
};

#endif // EDITRECIPE_H
