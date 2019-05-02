#ifndef JSON_TRANSLATOR_H
#define JSON_TRANSLATOR_H

#include <QObject>
#include <QStandardItemModel>
#include <my_delegate.h>

class JsonTranslator : public QObject
{
    Q_OBJECT
public:
    explicit JsonTranslator(QObject *parent = nullptr);

    void ReadFromJson(QStandardItemModel* model, const QJsonObject& j_obj);
    QJsonObject WriteToJson(QStandardItemModel *model);

private:
    QStandardItem* createCheckableItem(Qt::CheckState display_value);
    QStandardItem* createItem(const QVariant& display_value, const QVariant& data_value,
                              MyDelegate::DelegateType type);
    QStandardItem* createDisplayItem(const QString& display_value);
    QStandardItem* createEmptyItem();

    QList<QStandardItem *> createItemFromJsonObj(const QString& obj_name,
                                                 const QJsonObject& obj);
    void insertJsonObjFromItem(QJsonObject& top_obj, const QStandardItem* property_item,
                                      const QStandardItem* value_item);
};

#endif // JSON_TRANSLATOR_H
