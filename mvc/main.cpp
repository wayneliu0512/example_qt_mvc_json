#include <QTreeView>
#include <QApplication>
#include <QStandardItemModel>
#include <my_delegate.h>

QStandardItem* createCheckableItem(Qt::CheckState display_value) {
    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setAutoTristate(false);
    item->setData(display_value, Qt::CheckStateRole);
    item->setData(static_cast<int>(MyDelegate::DelegateType::kCheckbox), MyDelegate::kBoxTypeRole);
    item->setEditable(false);
    return item;
}

QStandardItem* createItem(const QVariant& display_value, const QVariant& data_value,
                          MyDelegate::DelegateType type) {
    QStandardItem *item = new QStandardItem;
    item->setData(display_value, Qt::DisplayRole);
    item->setData(data_value, MyDelegate::kEditRole);
    item->setData(static_cast<int>(type), MyDelegate::kBoxTypeRole);
    item->setEditable(type == MyDelegate::DelegateType::kNull ? false : true);
    return item;
}

QStandardItem* createDisplayItem(const QString& display_value) {
    return createItem(display_value, QVariant(), MyDelegate::DelegateType::kNull);
}

QStandardItem* createEmptyItem() {
    QStandardItem *item = new QStandardItem;
    item->setEditable(false);
    return item;
}

void createModel(QStandardItemModel& model) {
    model.setHorizontalHeaderItem(0, new QStandardItem("property"));
    model.setHorizontalHeaderItem(1, new QStandardItem("value"));
    auto item = model.invisibleRootItem();
    QList<QStandardItem *> r1 = { createDisplayItem("r1"), createEmptyItem()};
    QList<QStandardItem *> r2 = { createDisplayItem("r2"), createEmptyItem()};
    QList<QStandardItem *> r11 = { createDisplayItem("testcheck"),
                                   createCheckableItem(Qt::CheckState::Checked) };
    QVariantList list = { 0, "one", "two", "three"};
    QList<QStandardItem *> r12 = { createDisplayItem("testcombo"),
                                   createItem("one", QVariant(list), MyDelegate::DelegateType::kCombobox) };
    QList<QStandardItem *> r21 = { createDisplayItem("testspin"),
                                   createItem(20, 20, MyDelegate::DelegateType::kSpinbox) };
    QList<QStandardItem *> r22 = { createDisplayItem("testText"),
                                   createItem("writestring", "writestring", MyDelegate::DelegateType::kText) };
    QList<QStandardItem *> r23= { createDisplayItem("testDoubleSpin"),
                                   createItem(1.35, 1.35, MyDelegate::DelegateType::kDoubleSpinbox) };
    item->appendRow(r1);
    item->appendRow(r2);

    r1[0]->appendRow(r11);
    r1[0]->appendRow(r12);

    r2[0]->appendRow(r21);
    r2[0]->appendRow(r22);
    r2[0]->appendRow(r23);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTreeView tree_view;
    QStandardItemModel model;
    createModel(model);

    MyDelegate delegate;

    tree_view.setModel(&model);
    tree_view.setItemDelegate(&delegate);
    tree_view.show();
    return a.exec();
}
