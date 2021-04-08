/********************************************************************************
** Form generated from reading UI file 'createmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEMENU_H
#define UI_CREATEMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CreateMenu
{
public:
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_ListLayout;
    QVBoxLayout *m_recipeListLayout;
    QLabel *m_recipesLabel;
    QListView *m_recipeListView;
    QVBoxLayout *m_selectedRecipesLayout;
    QLabel *m_selectedRecipesLabel;
    QListView *m_selectedRecipesListView;
    QVBoxLayout *m_tableLayout;
    QLabel *m_ingredientsLabel;
    QTableView *m_ingredientsTableView;
    QHBoxLayout *m_buttonLayout;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;
    QPushButton *m_cancelButton;

    void setupUi(QDialog *CreateMenu)
    {
        if (CreateMenu->objectName().isEmpty())
            CreateMenu->setObjectName(QString::fromUtf8("CreateMenu"));
        CreateMenu->setWindowModality(Qt::NonModal);
        CreateMenu->resize(700, 400);
        CreateMenu->setMinimumSize(QSize(600, 350));
        CreateMenu->setModal(false);
        verticalLayoutWidget_4 = new QWidget(CreateMenu);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(20, 10, 661, 351));
        m_mainLayout = new QVBoxLayout(verticalLayoutWidget_4);
        m_mainLayout->setObjectName(QString::fromUtf8("m_mainLayout"));
        m_mainLayout->setContentsMargins(0, 0, 0, 0);
        m_ListLayout = new QHBoxLayout();
        m_ListLayout->setObjectName(QString::fromUtf8("m_ListLayout"));
        m_recipeListLayout = new QVBoxLayout();
        m_recipeListLayout->setObjectName(QString::fromUtf8("m_recipeListLayout"));
        m_recipesLabel = new QLabel(verticalLayoutWidget_4);
        m_recipesLabel->setObjectName(QString::fromUtf8("m_recipesLabel"));

        m_recipeListLayout->addWidget(m_recipesLabel);

        m_recipeListView = new QListView(verticalLayoutWidget_4);
        m_recipeListView->setObjectName(QString::fromUtf8("m_recipeListView"));

        m_recipeListLayout->addWidget(m_recipeListView);


        m_ListLayout->addLayout(m_recipeListLayout);

        m_selectedRecipesLayout = new QVBoxLayout();
        m_selectedRecipesLayout->setObjectName(QString::fromUtf8("m_selectedRecipesLayout"));
        m_selectedRecipesLabel = new QLabel(verticalLayoutWidget_4);
        m_selectedRecipesLabel->setObjectName(QString::fromUtf8("m_selectedRecipesLabel"));

        m_selectedRecipesLayout->addWidget(m_selectedRecipesLabel);

        m_selectedRecipesListView = new QListView(verticalLayoutWidget_4);
        m_selectedRecipesListView->setObjectName(QString::fromUtf8("m_selectedRecipesListView"));

        m_selectedRecipesLayout->addWidget(m_selectedRecipesListView);


        m_ListLayout->addLayout(m_selectedRecipesLayout);


        m_mainLayout->addLayout(m_ListLayout);

        m_tableLayout = new QVBoxLayout();
        m_tableLayout->setObjectName(QString::fromUtf8("m_tableLayout"));
        m_ingredientsLabel = new QLabel(verticalLayoutWidget_4);
        m_ingredientsLabel->setObjectName(QString::fromUtf8("m_ingredientsLabel"));

        m_tableLayout->addWidget(m_ingredientsLabel);

        m_ingredientsTableView = new QTableView(verticalLayoutWidget_4);
        m_ingredientsTableView->setObjectName(QString::fromUtf8("m_ingredientsTableView"));

        m_tableLayout->addWidget(m_ingredientsTableView);


        m_mainLayout->addLayout(m_tableLayout);

        m_buttonLayout = new QHBoxLayout();
        m_buttonLayout->setSpacing(10);
        m_buttonLayout->setObjectName(QString::fromUtf8("m_buttonLayout"));
        m_addButton = new QPushButton(verticalLayoutWidget_4);
        m_addButton->setObjectName(QString::fromUtf8("m_addButton"));
        m_addButton->setEnabled(false);

        m_buttonLayout->addWidget(m_addButton);

        m_removeButton = new QPushButton(verticalLayoutWidget_4);
        m_removeButton->setObjectName(QString::fromUtf8("m_removeButton"));
        m_removeButton->setEnabled(false);

        m_buttonLayout->addWidget(m_removeButton);

        m_cancelButton = new QPushButton(verticalLayoutWidget_4);
        m_cancelButton->setObjectName(QString::fromUtf8("m_cancelButton"));

        m_buttonLayout->addWidget(m_cancelButton);


        m_mainLayout->addLayout(m_buttonLayout);


        retranslateUi(CreateMenu);

        QMetaObject::connectSlotsByName(CreateMenu);
    } // setupUi

    void retranslateUi(QDialog *CreateMenu)
    {
        CreateMenu->setWindowTitle(QCoreApplication::translate("CreateMenu", "Create menu", nullptr));
        m_recipesLabel->setText(QCoreApplication::translate("CreateMenu", "Recipes:", nullptr));
        m_selectedRecipesLabel->setText(QCoreApplication::translate("CreateMenu", "Selected recipes:", nullptr));
        m_ingredientsLabel->setText(QCoreApplication::translate("CreateMenu", "Ingredients:", nullptr));
        m_addButton->setText(QCoreApplication::translate("CreateMenu", "&Add", nullptr));
        m_removeButton->setText(QCoreApplication::translate("CreateMenu", "&Remove", nullptr));
        m_cancelButton->setText(QCoreApplication::translate("CreateMenu", "&Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateMenu: public Ui_CreateMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEMENU_H
