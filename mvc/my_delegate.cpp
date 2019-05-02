#include "my_delegate.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>

const int MyDelegate::kBoxTypeRole = Qt::UserRole + 1;
const int MyDelegate::kEditRole = Qt::UserRole + 2;

MyDelegate::MyDelegate(QObject *parent)
      : QStyledItemDelegate (parent)
{

}

QWidget *MyDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    qDebug() << "createEditor";
    auto type = index.model()->data(index, kBoxTypeRole).toInt();
    auto value = index.model()->data(index, kEditRole);

    qDebug() << value;
    switch (static_cast<DelegateType>(type)) {
    case DelegateType::kText:
        return new QLineEdit(parent);
    case DelegateType::kCombobox:{
        QComboBox *combo_box = new QComboBox(parent);
        QStringList list(value.toStringList());
        list.pop_front();
        combo_box->addItems(list);
        return combo_box;
    }
    case DelegateType::kSpinbox:
        return new QSpinBox(parent);
    case DelegateType::kDoubleSpinbox:
        return new QDoubleSpinBox(parent);
    case DelegateType::kCheckbox:
    case DelegateType::kNull:
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void MyDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    qDebug() << "setEditorData";
    auto value = index.model()->data(index, kEditRole);
    auto type = index.model()->data(index, kBoxTypeRole).toInt();

    switch (static_cast<DelegateType>(type)) {
    case DelegateType::kText:{
        QLineEdit *line_edit = static_cast<QLineEdit*>(editor);
        line_edit->setText(value.toString());
        break;
    }
    case DelegateType::kCombobox:{
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentIndex(value.toList().first().toInt());
        break;
    }
    case DelegateType::kSpinbox: {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value.toInt());
        break;
    }
    case DelegateType::kDoubleSpinbox: {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(value.toDouble());
        break;
    }
    case DelegateType::kCheckbox:
    case DelegateType::kNull: {
        QStyledItemDelegate::setEditorData(editor, index);
        break;
    }
    }
}

void MyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    qDebug() << "setModelData";
    auto value = index.model()->data(index, kEditRole);
    auto type = index.model()->data(index, kBoxTypeRole).toInt();

    switch (static_cast<DelegateType>(type)) {
    case DelegateType::kText:{
        QLineEdit *line_edit = static_cast<QLineEdit*>(editor);
        model->setData(index, line_edit->text(), kEditRole);
        model->setData(index, line_edit->text(), Qt::DisplayRole);
        break;
    }
    case DelegateType::kCombobox:{
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int current_index = comboBox->currentIndex();
        QVariantList list;
        for (int i = 0; i < comboBox->count(); ++i) {
            list.append(comboBox->itemText(i));
        }
        list.push_front(current_index);
        model->setData(index, QVariant(list), kEditRole);
        model->setData(index, comboBox->currentText(), Qt::DisplayRole);
        break;
    }
    case DelegateType::kSpinbox: {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        model->setData(index, spinBox->value(), kEditRole);
        model->setData(index, spinBox->value(), Qt::DisplayRole);
        break;
    }
    case DelegateType::kDoubleSpinbox: {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->interpretText();
        model->setData(index, spinBox->value(), kEditRole);
        model->setData(index, spinBox->value(), Qt::DisplayRole);
        break;
    }
    case DelegateType::kCheckbox:
    case DelegateType::kNull: {
        QStyledItemDelegate::setModelData(editor, model, index);
        break;
    }
    }
}

void MyDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
