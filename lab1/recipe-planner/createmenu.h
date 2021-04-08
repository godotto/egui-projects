#ifndef CREATEMENU_H
#define CREATEMENU_H

#include <QDialog>

#include <QStandardItemModel>
#include <QStringListModel>

namespace Ui {
class CreateMenu;
}

class CreateMenu : public QDialog
{
    Q_OBJECT

private:
    // pointer on form object
    Ui::CreateMenu *ui;

    // methods for constructor
    void adjustLayouts();
    void createModels();
    void adjustLists();
    void adjustTable();

public:
    explicit CreateMenu(QStandardItemModel*, QWidget *parent = nullptr);
    ~CreateMenu();
};

#endif // CREATEMENU_H
