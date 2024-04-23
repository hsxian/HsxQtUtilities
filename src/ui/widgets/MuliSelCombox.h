#ifndef MULISELCOMBOX_H
#define MULISELCOMBOX_H

#include <QComboBox>
#include <QCheckBox>


#include <HsxQtUtilities_global.h>

namespace hsx
{
    class HSXQTUTILITIES_EXPORT  MuliSelCombox : public QComboBox
    {
    public:
        MuliSelCombox(QWidget *parent = nullptr);
        //    QString currentText()const;
        void addCheckItem(const QString &text, const QVariant &value, const Qt::CheckState state = Qt::CheckState::Unchecked);
        QList<std::pair<int, QCheckBox *>>getCheckBoxs(const Qt::CheckState &state);
        QList<std::pair<int, QCheckBox *>>getCheckBoxs();
    protected:
        void paintEvent(QPaintEvent *event);
    };
}
#endif // MULISELCOMBOX_H
