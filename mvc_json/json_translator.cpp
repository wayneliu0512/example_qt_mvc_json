#include "json_translator.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

JsonTranslator::JsonTranslator(QObject *parent) : QObject(parent)
{}

void JsonTranslator::ReadFromJson(QStandardItemModel* model, const QJsonObject& j_obj)
{
    auto root_item = model->invisibleRootItem();
    for (auto key : j_obj.keys()) {
        if (!j_obj[key].isObject()) {
            qDebug() << "error: not a object";
            return;
        }
        QJsonObject obj_1(j_obj[key].toObject());
        if (obj_1.contains("box_type_role")) {
            auto top_item = createItemFromJsonObj(key, obj_1);
            root_item->appendRow(top_item);
        } else {
            QList<QStandardItem *> top_item = { createDisplayItem(key), createEmptyItem()};
            root_item->appendRow(top_item);
            for (auto key_1 : obj_1.keys()) {
                if (!obj_1[key_1].isObject()) {
                    qDebug() << "error: not a object_1";
                    return;
                }
                QJsonObject obj_2(obj_1[key_1].toObject());
                auto sub_item = createItemFromJsonObj(key_1, obj_2);
                top_item[0]->appendRow(sub_item);
            }
        }
    }
}

QJsonObject JsonTranslator::WriteToJson(QStandardItemModel* model)
{
    QJsonObject j_obj_root;
    auto root_item = model->invisibleRootItem();
    for (int i = 0; i < root_item->rowCount(); ++i) {
        auto top_item = root_item->child(i, 0);
        if (top_item->hasChildren()) {
            QJsonObject j_obj_top;
            for (int j = 0; j < top_item->rowCount(); ++j) {
                insertJsonObjFromItem(j_obj_top, top_item->child(j, 0), top_item->child(j, 1));
            }
            j_obj_root.insert(top_item->data(Qt::DisplayRole).toString(), j_obj_top);
        } else {
            insertJsonObjFromItem(j_obj_root, top_item, root_item->child(i, 1));
        }
    }
    return j_obj_root;
}

QStandardItem* JsonTranslator::createCheckableItem(Qt::CheckState display_value)
{
    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setAutoTristate(false);
    item->setData(display_value, Qt::CheckStateRole);
    item->setData(static_cast<int>(MyDelegate::DelegateType::kCheckbox), MyDelegate::kBoxTypeRole);
    item->setEditable(false);
    return item;
}

QStandardItem* JsonTranslator::createItem(const QVariant& display_value,
                                         const QVariant& data_value,
                                         MyDelegate::DelegateType type)
{
    QStandardItem *item = new QStandardItem;
    item->setData(display_value, Qt::DisplayRole);
    item->setData(data_value, MyDelegate::kEditRole);
    item->setData(static_cast<int>(type), MyDelegate::kBoxTypeRole);
    item->setEditable(type == MyDelegate::DelegateType::kNull ? false : true);
    return item;
}

QStandardItem* JsonTranslator::createDisplayItem(const QString& display_value)
{
    return createItem(display_value, QVariant(), MyDelegate::DelegateType::kNull);
}

QStandardItem* JsonTranslator::createEmptyItem()
{
    QStandardItem *item = new QStandardItem;
    item->setEditable(false);
    return item;
}

QList<QStandardItem*> JsonTranslator::createItemFromJsonObj(const QString& obj_name,
                                                            const QJsonObject& obj)
{
    QList<QStandardItem *> item;
    auto type = static_cast<MyDelegate::DelegateType>(obj["box_type_role"].toInt());
    switch (type) {
    case MyDelegate::DelegateType::kText:
    case MyDelegate::DelegateType::kSpinbox:
    case MyDelegate::DelegateType::kDoubleSpinbox:
    case MyDelegate::DelegateType::kNull:
        item = {createDisplayItem(obj_name),
                createItem(obj["display_role"].toVariant(),
                obj["edit_role"].toVariant(),
                type) };
        break;
    case MyDelegate::DelegateType::kCheckbox:
        item = {createDisplayItem(obj_name),
                    createCheckableItem(static_cast<Qt::CheckState>(obj["check_state_role"].toInt())) };
        break;
    case MyDelegate::DelegateType::kCombobox:
        QVariant variant_list(obj["edit_role"].toArray().toVariantList());
        item = {createDisplayItem(obj_name),
                    createItem(obj["display_role"].toVariant(),
                               variant_list,
                               MyDelegate::DelegateType::kCombobox) };
        break;
    }
    return item;
}

void JsonTranslator::insertJsonObjFromItem(QJsonObject& top_obj, const QStandardItem* property_item,
                                                  const QStandardItem* value_item)
{
    QJsonObject j_obj_value;
    auto box_type_role = value_item->data(MyDelegate::kBoxTypeRole);
    if (static_cast<MyDelegate::DelegateType>(box_type_role.toInt()) == MyDelegate::DelegateType::kCheckbox) {
        auto check_state_role = value_item->data(Qt::CheckStateRole);
        j_obj_value.insert("check_state_role", check_state_role.toJsonValue());
    } else {
        auto display_role = value_item->data(Qt::DisplayRole);
        auto edit_role = value_item->data(MyDelegate::kEditRole);
        j_obj_value.insert("display_role", display_role.toJsonValue());
        j_obj_value.insert("edit_role", edit_role.toJsonValue());
    }
    j_obj_value.insert("box_type_role", box_type_role.toJsonValue());

    auto property = property_item->data(Qt::DisplayRole).toString();
    top_obj.insert(property, j_obj_value);
}
