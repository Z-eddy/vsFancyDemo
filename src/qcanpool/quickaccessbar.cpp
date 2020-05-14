/***************************************************************************
 **
 **  Copyright (C) 2018 MaMinJie <canpool@163.com>
 **  Contact: https://github.com/canpool
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see http://www.gnu.org/licenses/.
 **
****************************************************************************/
#include "quickaccessbar.h"

#include <QApplication>
#include <QStyleOption>
#include <QToolButton>
#include <QMenu>
#include <QPainter>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "fancybar.h"
#include "fancybutton.h"

class QuickAccessBarPrivate : public QWidget
{
    Q_OBJECT
public:
    QuickAccessBarPrivate(QWidget *parent = nullptr);
    void init();

    void addAction(QAction *action);

public:
    QuickAccessBar *q;

    FancyButton *m_accessButton;//更多菜单
    QMenu       *m_menu;//点击更多菜单,弹出的菜单
    bool        m_bAddEnable;
    QList<QAction *> m_checkActions;
    QList<QAction *> m_actions;
    QList<FancyButton *> m_actionButtons;

    QHBoxLayout *m_accessArea;//存放access各功能项

    QAction *m_customizeAction;
    QAction *m_separatorAction;

    FancyButton *m_menuButton;//当前点击的快速访问栏按钮

private Q_SLOTS:
    void aboutToShowCustomizeMenu();
    void aboutToHideCustomizeMenu();
    void accessMenuTriggered(QMouseEvent *e);
    void menuTriggered(QMouseEvent *e);
    void aboutToShowMenu();
    void aboutToHideMenu();
};

QuickAccessBarPrivate::QuickAccessBarPrivate(QWidget *parent)
    : QWidget(parent)
{
    q = nullptr;
    m_accessButton = nullptr;
    m_menu = nullptr;
    m_bAddEnable = true;
    m_checkActions.clear();
    m_actions.clear();
    m_actionButtons.clear();
    m_accessArea = nullptr;
    m_customizeAction = nullptr;
    m_separatorAction = nullptr;
    m_menuButton = nullptr;
}

void QuickAccessBarPrivate::init()
{
    m_accessButton = new FancyButton();//更多菜单
    m_accessButton->setHasBorder(false);
    m_accessButton->setHasMenu(true);
    m_accessButton->setIcon(QIcon(":/main/arrow"));
    m_accessButton->setToolTip(tr("Quick Access Menu"));
    connect(m_accessButton, SIGNAL(menuTriggered(QMouseEvent *)), this, SLOT(accessMenuTriggered(QMouseEvent *)));
    m_accessArea = new QHBoxLayout();//存放access各功能项
    m_accessArea->setContentsMargins(2, 0, 0, 0);
    m_accessArea->setSpacing(0);
    m_customizeAction = new QAction(tr("Customize Quick Access Bar"), this);//更多菜单的第一行:Customize Quick Access Bar
    m_customizeAction->setCheckable(false);
    m_customizeAction->setIconVisibleInMenu(false);
    m_customizeAction->setEnabled(false);
    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowCustomizeMenu()));
    connect(m_menu, SIGNAL(aboutToHide()), this, SLOT(aboutToHideCustomizeMenu()));
    m_menu->addAction(m_customizeAction);
    m_separatorAction = m_menu->addSeparator();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addLayout(m_accessArea);//首先放置快速访问栏功能项
    layout->addWidget(m_accessButton/*, 0, Qt::AlignTop*/);//然后放置 更多 按钮
    setLayout(layout);
}

void QuickAccessBarPrivate::addAction(QAction *action)
{
	/*
	此处使用一个没有任何内容的checkAction嵌入到更多菜单里,
	当checkAction更改状态时,关联了实际action的button则触发显示/隐藏
	*/
    QAction *checkAction = new QAction(action->text(), this);//空白action,仅持有同样的名字
    FancyButton *button = new FancyButton();
    button->setHasBorder(false);
	//可以直接设定defaultAction
    button->setText(action->text());
    button->setToolTip(action->toolTip());
    button->setIcon(action->icon());

    button->setDefaultAction(action);

    connect(button, SIGNAL(menuTriggered(QMouseEvent *)), this, SLOT(menuTriggered(QMouseEvent *)));//弹出菜单
    connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered(bool)));//执行本身功能
    checkAction->setCheckable(true);
    checkAction->setChecked(true);
    m_accessArea->addWidget(button/*, 0, Qt::AlignTop*/);
    m_menu->insertAction(m_separatorAction, checkAction);//功能项插入
    connect(checkAction, SIGNAL(toggled(bool)), button, SLOT(setVisible(bool)));
    m_actions.append(action);
    m_checkActions.append(checkAction);
    m_actionButtons.append(button);
}

void QuickAccessBarPrivate::aboutToShowCustomizeMenu()
{//显示更多按钮
    m_accessButton->select(true);
}

void QuickAccessBarPrivate::aboutToHideCustomizeMenu()
{//隐藏更多按钮
    m_accessButton->select(false);
}

void QuickAccessBarPrivate::accessMenuTriggered(QMouseEvent *e)
{//设置(点击更多按钮)弹出菜单的位置
    FancyButton *button = qobject_cast<FancyButton *>(sender());

    if (button == nullptr) {
        return;
    }

    int x = e->x();
    int y = e->y();
    QPoint pos = e->globalPos();
    pos.setX(pos.x() - x);
    pos.setY(pos.y() - y + button->height());
//    emit q->menuTriggered(index, pos);
    m_menu->popup(pos);
}

void QuickAccessBarPrivate::menuTriggered(QMouseEvent *e)
{//点击快速访问栏的功能项按钮弹出对应的menu
    FancyButton *button = qobject_cast<FancyButton *>(sender());

    if (button == nullptr) {
        return;
    }

    QAction *action = button->defaultAction();
    QMenu *menu = action->menu();

    if (menu == nullptr) {
        return;
    }

    connect(menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowMenu()));//触发按钮所在菜单栏显示、隐藏时按钮显示、隐藏
    connect(menu, SIGNAL(aboutToHide()), this, SLOT(aboutToHideMenu()));
    m_menuButton = button;
    int x = e->x();
    int y = e->y();
    QPoint pos = e->globalPos();
    pos.setX(pos.x() - x);
    pos.setY(pos.y() - y + button->height());
    menu->popup(pos);
}

void QuickAccessBarPrivate::aboutToShowMenu()
{
    if (m_menuButton) {
        m_menuButton->select(true);
    }
}

void QuickAccessBarPrivate::aboutToHideMenu()
{
    if (m_menuButton) {
        m_menuButton->select(false);
    }
}

QuickAccessBar::QuickAccessBar(QWidget *parent)
    : QToolBar(parent), d(new QuickAccessBarPrivate(this))
{
    d->q = this;
    d->init();
    this->setStyleSheet("QToolBar{border:none;background-color: transparent;margin:0px;}");
    addWidget(d);
}

QAction *QuickAccessBar::actionCustomizeButton() const
{
    return d->m_customizeAction;
}

void QuickAccessBar::setActionVisible(QAction *action, bool visible)
{
    int index = d->m_actions.indexOf(action);

    if (index == -1) {
        return;
    }

    d->m_checkActions.at(index)->setChecked(visible);
}

bool QuickAccessBar::isActionVisible(QAction *action) const
{
    int index = d->m_actions.indexOf(action);

    if (index == -1) {
        return false;
    }

    return d->m_checkActions.at(index)->isChecked();
}

int QuickAccessBar::visibleCount() const
{
    int cnt = 0;

    for (int i = 0; i < d->m_checkActions.count(); ++i) {
        if (d->m_checkActions.at(i)->isChecked()) {
            ++cnt;
        }
    }

    return cnt;
}

void QuickAccessBar::setHoverColor(const QColor &color)
{
    foreach (FancyButton * button, d->m_actionButtons) {
        button->setHoverColor(color);
    }
}

void QuickAccessBar::setPressColor(const QColor &color)
{
    foreach (FancyButton * button, d->m_actionButtons) {
        button->setPressColor(color);
    }
}

void QuickAccessBar::setTextColor(const QColor &color)
{
    foreach (FancyButton * button, d->m_actionButtons) {
        button->setTextColor(color);
    }
}

void QuickAccessBar::actionEvent(QActionEvent *event)
{
    if (d->m_bAddEnable) {//只有第一次触发action时执行?
        d->m_bAddEnable = false;
        QToolBar::actionEvent(event);
        return;
    }

    switch (event->type()) {
        case QEvent::ActionAdded: {//触发增加action时
            QAction *action = event->action();
            d->addAction(action);
            break;
        }
        default:
            break;
    }
}

#include "quickaccessbar.moc"
