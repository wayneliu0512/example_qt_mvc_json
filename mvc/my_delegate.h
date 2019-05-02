#ifndef MY_DELEGATE_H
#define MY_DELEGATE_H

#include <QStyledItemDelegate>

class MyDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    enum class DelegateType { kNull, kCheckbox, kText, kSpinbox, kDoubleSpinbox, kCombobox };
    static const int kBoxTypeRole;
    static const int kEditRole;

    explicit MyDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // MY_DELEGATE_H
