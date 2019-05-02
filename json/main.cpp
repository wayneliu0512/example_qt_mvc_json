#include <QCoreApplication>
#include <QtCore>

#define READ_DEMO

#ifdef READ_DEMO

void readJson(const QJsonObject& j_obj) {
    for (auto key : j_obj.keys()) {
        if (j_obj[key].isObject())
            readJson(j_obj[key].toObject());
        else if (j_obj[key].isArray()) {
            QJsonArray array(j_obj[key].toArray());
            for (auto value : array)
                qDebug() << value.toVariant();
        } else {
            qDebug() << key << ": " << j_obj[key].toVariant();
        }
    }
}

#endif

int main(int argc, char *argv[])
{

#ifdef WRITE_DEMO

    QFile save_file(QString("save.json"));
    if (!save_file.open(QIODevice::WriteOnly)) {
        qDebug() << "open file error.";
        return 0;
    }

    QJsonObject j_object;

    j_object.insert("name", "Wayne");

    QJsonObject j_tree;
    j_tree.insert("h1", QJsonValue());
    j_tree.insert("h2", "fdsa");
    j_object.insert("wname", j_tree );

    j_object.insert("boolean", true);

    QJsonObject j_tree3;
    j_tree3.insert("fds", "test");
    j_tree3.insert("fdsads", "test");
    QJsonObject j_tree2;
    j_tree2.insert("int", 123);
    j_tree2.insert("double", 123.456);
    j_tree2.insert("tree3", j_tree3);
    j_object.insert("tree2", j_tree2);

    QJsonArray j_array;
    j_array.append("tree");
    j_array.append(123);
    j_object.insert("array", j_array);

    QJsonDocument j_doc(j_object);
    save_file.write(j_doc.toJson());
    save_file.close();

#endif

#ifdef READ_DEMO

    QFile save_file(QString("save.json"));
    if (!save_file.open(QIODevice::ReadOnly)) {
        qDebug() << "open file error.";
        return 0;
    }

    QByteArray barray(save_file.readAll());
    save_file.close();

    QJsonDocument j_doc(QJsonDocument::fromJson(barray));
    QJsonObject j_obj(j_doc.object());
    readJson(j_obj);

#endif

    return 0;
}
