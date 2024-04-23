#ifndef EXCLUSIVEWINDOW_H
#define EXCLUSIVEWINDOW_H

#include <QMap>
#include <QString>
#include <QWidget>
#include <QPropertyAnimation>
#include <functional>

#include <HsxQtUtilities_global.h>
namespace hsx
{

class AnimationGroup
{
public:
    QList<QPropertyAnimation *>shows;
    QList<QPropertyAnimation *>hides;
};
class HSXQTUTILITIES_EXPORT ExclusiveWindow
{
public:
    //    Animation
    ExclusiveWindow();
    void registerWidget(const QString &groupName, QWidget *wgt);
    void registerAnimation(const QString &groupName, AnimationGroup *ani);
    void exclusiveShow(const QString &groupName, QWidget *wgt);
    void exclusiveShow(const QString &groupName, QWidgetList wgts);
    void exclusiveShow(const QString &groupName, const QString &objectName);
    void exclusiveShow(const QString &groupName, const QStringList &objectNames);
    void setCreateAnimationFunc(const std::function<AnimationGroup*(const QString &groupName, QWidget *wgt)> &func);
private:
    QMap<QString, QWidgetList *>m_MapOfWidgets;
    QMap<QWidget *, AnimationGroup *>m_MapOfAnimation;
    std::function<AnimationGroup*(const QString &groupName, QWidget *wgt)> m_CreateAnimationFunc = nullptr;
    void execAnimation(const QString &groupName, QWidget *wgt, bool showOrHide);
    bool existHideAnimation(QWidget *wgt);
};
}
#endif // EXCLUSIVEWINDOW_H
