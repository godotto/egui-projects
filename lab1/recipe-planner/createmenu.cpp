#include "createmenu.h"
#include "ui_createmenu.h"

CreateMenu::CreateMenu(QStandardItemModel *recipeModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateMenu)
{
    ui->setupUi(this);

    // set main layout
    ui->m_mainLayout->setContentsMargins(20, 20, 20, 20);
    setLayout(ui->m_mainLayout);
    ui->m_buttonLayout->setAlignment(Qt::AlignLeft);
}

CreateMenu::~CreateMenu()
{
    delete ui;
}
