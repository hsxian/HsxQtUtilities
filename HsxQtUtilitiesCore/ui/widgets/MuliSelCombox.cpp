#pragma execution_character_set("utf-8")
#include "MuliSelCombox.h"

#include <QRect>
#include <QAbstractItemView>
#include <QStyleOptionComboBox>
#include <QApplication>
#include <QScreen>
#include <QStylePainter>
#include <QStandardItemModel>
#include <QCheckBox>

using namespace hsx;

MuliSelCombox::MuliSelCombox(QWidget *parent): QComboBox(parent)
{

}


void MuliSelCombox::addCheckItem(const QString &text, const QVariant &value, const Qt::CheckState state)
{
    auto im = qobject_cast<QStandardItemModel *>(model());
    auto qitem = new QStandardItem();
    qitem->setData(value);
    im->insertRow(im->rowCount(), qitem);
    auto cbk = new QCheckBox(text, this);
    cbk->setCheckState(state);
    this->view()->setIndexWidget(im->index(im->rowCount() - 1, 0), cbk);
}


void MuliSelCombox::paintEvent(QPaintEvent *event)
{
    QString curText = "请选择";
    auto cc = getCheckBoxs(Qt::CheckState::Checked);
    auto ml = model();
    if(cc.size())
    {
        curText = QString("已选择：%1/%2 项").arg(cc.size()).arg(ml->rowCount());
    }
    QStyleOptionComboBox opt;
    QStylePainter painter(this);

    QComboBox::initStyleOption(&opt);
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);
    opt.currentText = curText;
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}

QList<std::pair<int, QCheckBox *> > MuliSelCombox::getCheckBoxs(const Qt::CheckState &state)
{
    QList<std::pair<int, QCheckBox *>>ret;
    auto ml = model();
    for(int i = 0; i < ml->rowCount(); ++i)
    {
        auto cbk = qobject_cast<QCheckBox *>(view()->indexWidget(ml->index(i, 0)));

        if(cbk->checkState() == state)
        {
            ret << std::pair<int, QCheckBox *>(i, cbk);
        }
    }
    return ret;
}

QList<std::pair<int, QCheckBox *> > MuliSelCombox::getCheckBoxs()
{
    QList<std::pair<int, QCheckBox *>>ret;
    auto ml = model();
    for(int i = 0; i < ml->rowCount(); ++i)
    {
        auto cbk = qobject_cast<QCheckBox *>(view()->indexWidget(ml->index(i, 0)));

        ret << std::pair<int, QCheckBox *>(i, cbk);
    }
    return ret;
}

