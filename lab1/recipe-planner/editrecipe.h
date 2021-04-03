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
    QPushButton *m_addUpdateIngredientButton;
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

    // group box for ingredients
    QGroupBox *m_ingredientsBox;

    // methods for constructor
    void createLabels();
    void createLineAndTextEdits(QString, QString, QString, QStringList*);
    void createPushButtons(EditMode);
    void createLayouts();
    void createSpinbox();

public:
    EditRecipe(EditMode, QWidget *parent = nullptr);
};

#endif // EDITRECIPE_H
