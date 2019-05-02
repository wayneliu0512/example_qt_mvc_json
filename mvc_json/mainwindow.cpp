#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    delegate_(new MyDelegate(this)),
    model_(new QStandardItemModel(this)),
    json_translator_(this)
{
    ui->setupUi(this);
    ui->tree_view_->setItemDelegate(delegate_);
    ui->tree_view_->setModel(model_);

    connect(ui->pb_load_, &QPushButton::clicked, this, &MainWindow::LoadJsonFile);
    connect(ui->pb_save_, &QPushButton::clicked, this, &MainWindow::SaveJsonFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SaveJsonFile()
{
    auto file_name = QFileDialog::getOpenFileName(this, "open json",
                                                  "D:\\source\\qt_mvc_json",
                                                  "Json (*.json)");
    QFile save_file(file_name);
    if (!save_file.open(QIODevice::WriteOnly)) {
        qDebug() << "write file error.";
        return;
    }

    auto json_obj = json_translator_.WriteToJson(model_);
    QJsonDocument json_doc(json_obj);
    save_file.write(json_doc.toJson());
    save_file.close();
}

void MainWindow::LoadJsonFile()
{
    if (model_->invisibleRootItem()->hasChildren()) {
        model_->clear();
    }

    auto file_name = QFileDialog::getOpenFileName(this, "open json",
                                                  "D:\\source\\qt_mvc_json",
                                                  "Json (*.json)");
    QFile open_file(file_name);
    if (!open_file.open(QIODevice::ReadOnly)) {
        qDebug() << "open file error.";
        return;
    }

    QByteArray barray(open_file.readAll());
    open_file.close();

    QJsonObject json_obj = QJsonDocument::fromJson(barray).object();
    json_translator_.ReadFromJson(model_, json_obj);
    model_->setHeaderData(0, Qt::Orientation::Horizontal, "property", Qt::DisplayRole);
    model_->setHeaderData(1, Qt::Orientation::Horizontal, "value", Qt::DisplayRole);
}
