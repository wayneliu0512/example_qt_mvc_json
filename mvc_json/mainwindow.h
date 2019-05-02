#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <json_translator.h>
#include <my_delegate.h>

namespace Ui {
class MainWindow;
}

class QStandardItemModel;
class QAbstractItemDelegate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void SaveJsonFile();
    void LoadJsonFile();

private:
    Ui::MainWindow *ui;
    MyDelegate *delegate_;
    QStandardItemModel *model_;
    JsonTranslator json_translator_;
};

#endif // MAINWINDOW_H
