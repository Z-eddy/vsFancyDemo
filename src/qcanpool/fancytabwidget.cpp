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
#include "fancytabwidget.h"
#include "fancytabbar.h"
#include "minisplitter.h"
#include <QStackedWidget>
#include <QStatusBar>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QIcon>
#include<QDebug>
#include<QLabel>

class FancyTabWidgetPrivate : public QObject
{
public:
    FancyTabWidgetPrivate();
    ~FancyTabWidgetPrivate();
    void updateTabBarPosition();
    void init();

    FancyTabBar *m_tabBar;//设计图的mode bar
    QStackedWidget *m_stack;//设计图的stack
    QStatusBar *m_statusBar;//设计图的status bar
    QWidget *m_center;//设计图上,位于mode bar右侧的整块

//    QVBoxLayout *m_leftCorner;
//    QVBoxLayout *m_rightCorner;
    QHBoxLayout *m_topCorner;//top窗口的layout
//    QHBoxLayout *m_bottomCorner;

    FancyTabWidget *q;//持有上级类

    QBoxLayout *m_layout;
    FancyTabWidget::TabPosition m_pos;//mode bard所在位置

    MiniSplitter *m_rightSplitter;//设计图的right
    MiniSplitter *m_bottomSplitter;//设计图的bottom
    MiniSplitter *m_leftSplitter;//设计图的left;
};

FancyTabWidgetPrivate::FancyTabWidgetPrivate()
    : m_tabBar(nullptr), m_stack(nullptr), m_statusBar(nullptr), m_center(nullptr),
      m_topCorner(nullptr), q(nullptr), m_layout(nullptr), m_pos(FancyTabWidget::West)
{}

FancyTabWidgetPrivate::~FancyTabWidgetPrivate()
{}

void FancyTabWidgetPrivate::updateTabBarPosition()
{
    switch (m_pos) {
        case FancyTabWidget::North://pos在北方
            m_tabBar->setDirection(FancyTabBar::Horizontal);//tabBar水平分布
            m_layout->setDirection(QBoxLayout::TopToBottom);//layout布局为上到下
            break;

        case FancyTabWidget::South://pos在南方
            m_tabBar->setDirection(FancyTabBar::Horizontal);//tabBar水平向
            m_layout->setDirection(QBoxLayout::BottomToTop);//layout布局下到上
            break;

        case FancyTabWidget::West://pos在西方
            m_tabBar->setDirection(FancyTabBar::Vertical);//tabBar垂直向
            m_layout->setDirection(QBoxLayout::LeftToRight);//layout布局左到右
            break;

        case FancyTabWidget::East://pos在东方
            m_tabBar->setDirection(FancyTabBar::Vertical);//tabBar垂直向
            m_layout->setDirection(QBoxLayout::RightToLeft);//layout布局右到左
            break;
            /*default:*/
    }
}

/*******************************************************************************************
 *
 *  TabBar + Center
 *
 *  -------------------------------------------------------------------------|
 *  |   |                               Top                                  |
 *  |   |--------------------------------------------------------------------|
 *  |   |           |                                           |            |
 *  |   |           |                                           |            |
 *  |   |           |                                           |            |
 *  |   |           |                                           |            |
 *  |   |           |                                           |            |
 *  | M |           |                                           |            |
 *  | O |           |                                           |            |
 *  | D |           |                  Stack                    |   Right    |
 *  | E |           |                                           |            |
 *  |   |           |                                           |            |
 *  | B |    Left   |                                           |            |
 *  | A |           |                                           |            |
 *  | R |           |                                           |            |
 *  |   |           |                                           |            |
 *  |   |           |                                           |            |
 *  |   |           |--------------------------------------------------------|
 *  |   |           |                                                        |
 *  |   |           |                  Bottom                                |
 *  |   |           |                                                        |
 *  |   |           |                                                        |
 *  |   |--------------------------------------------------------------------|
 *  |   |                           Status Bar                               |
 *  -------------------------------------------------------------------------|
 *
 ******************************************************************************************/

void FancyTabWidgetPrivate::init()
{
    m_tabBar = new FancyTabBar(q);//左侧的modeBar
    m_tabBar->setObjectName(QLatin1String("qcanpool_modebar"));//用于反射机制,可以通过名字搜索到类,一般会让名字和类名一致
    m_stack = new QStackedWidget(q);//stack模块
    m_stack->setObjectName(QLatin1String("qcanpool_modestack"));
    m_stack->setLineWidth(0);
	//m_stack->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::TabWidget));
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(240, 240, 240, 240));//背景几乎白色
    m_stack->setPalette(palette);
    m_stack->setAutoFillBackground(true);
    connect(m_tabBar, SIGNAL(currentChanged(int)), m_stack, SLOT(setCurrentIndex(int)));
    connect(m_tabBar, SIGNAL(menuTriggered(int, QPoint)), q, SIGNAL(menuTriggered(int, QPoint)));//信号触发信号
    connect(m_stack, SIGNAL(currentChanged(int)), q, SIGNAL(currentChanged(int)));//信号触发信号
    m_rightSplitter = new MiniSplitter(Qt::Horizontal, MiniSplitter::Light);
    m_bottomSplitter = new MiniSplitter(Qt::Vertical, MiniSplitter::Light);
    m_leftSplitter = new MiniSplitter(Qt::Horizontal, MiniSplitter::Light);
    m_leftSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // top, bottom, left, right corner
    QWidget *topWidget = new QWidget();//顶部窗口
    m_topCorner = new QHBoxLayout();//顶部窗口使用的layout,后面使用layout加入子项
    m_topCorner->setMargin(0);
    m_topCorner->setSpacing(0);
	//m_topCorner->addWidget(new QLabel{ "testLabel0" });//显示在stack上面
    topWidget->setLayout(m_topCorner);
    topWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	//右边持有stack,下面持有右边,左边持有右边,详见设计图
    // stack + right
    m_rightSplitter->addWidget(m_stack);
    m_rightSplitter->setStretchFactor(0, 1);
    // stack + right
    // + bottom
    m_bottomSplitter->addWidget(m_rightSplitter);
    m_bottomSplitter->setStretchFactor(0, 1);
    // left + ...
    m_leftSplitter->addWidget(m_bottomSplitter);

    QVBoxLayout *centralLayout = new QVBoxLayout();//持有top,left
    centralLayout->setMargin(0);
    centralLayout->setSpacing(1);
    centralLayout->addWidget(topWidget);
    centralLayout->addWidget(m_leftSplitter);

    m_center = new QWidget();//没有初始化status bar,设计图上center应该包含了mode bar右边的所有
    m_center->setLayout(centralLayout);

    m_layout = new QBoxLayout(QBoxLayout::LeftToRight);//默认从左到右排列
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_tabBar);//mode bar在左侧
    m_layout->addWidget(m_center);//center在右侧
    q->setLayout(m_layout);

	//this->updateTabBarPosition();//初始化其他项后,调整项位置//可以隐藏
}

FancyTabWidget::FancyTabWidget(QWidget *parent)
    : QWidget(parent), d(new FancyTabWidgetPrivate())
{
    d->q = this;
    d->init();
}

FancyTabWidget::~FancyTabWidget()
{
    delete d;
}

int FancyTabWidget::addTab(QWidget *widget, const QString &label, bool hasMenu)
{
    return insertTab(-1, widget, label, hasMenu);
}

int FancyTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label, bool hasMenu)
{
    return insertTab(-1, widget, icon, label, hasMenu);
}

int FancyTabWidget::insertTab(int index, QWidget *widget, const QString &label, bool hasMenu)
{
    return insertTab(index, widget, QIcon(), label, hasMenu);
}

int FancyTabWidget::insertTab(int index, QWidget *widget, const QIcon &icon,
                               const QString &label, bool hasMenu)
{
    if (!widget)
        return -1;
    index = d->m_stack->insertWidget(index, widget);//index为-1时相当于appendWidget
    d->m_tabBar->insertTab(index, icon, label, hasMenu);//mode bar插入对应的图标

    return index;
}

void FancyTabWidget::removeTab(int index)
{
    if (QWidget *w = d->m_stack->widget(index)) {
        d->m_stack->removeWidget(w);
        d->m_tabBar->removeTab(index);
    }
}

void FancyTabWidget::setTabEnabled(int index, bool enable)
{
    d->m_tabBar->setTabEnabled(index, enable);
}

bool FancyTabWidget::isTabEnabled(int index) const
{
    return d->m_tabBar->isTabEnabled(index);
}

void FancyTabWidget::setTabVisible(int index, bool visible)
{
    if (QWidget *w = d->m_stack->widget(index)) {
        w->setHidden(!visible);//隐藏对应页面
        d->m_tabBar->setTabVisible(index, visible);//隐藏对应按钮?
    }
}

bool FancyTabWidget::isTabVisible(int index) const
{
    return d->m_tabBar->isTabVisible(index);
}

void FancyTabWidget::setTabToolTip(int index, QString toolTip)
{
    d->m_tabBar->setTabToolTip(index, toolTip);
}

QString FancyTabWidget::tabToolTip(int index) const
{
    return d->m_tabBar->tabToolTip(index);
}

FancyTabWidget::TabPosition FancyTabWidget::tabPosition() const
{
    return d->m_pos;
}

void FancyTabWidget::setTabPosition(FancyTabWidget::TabPosition pos)
{
    if (d->m_pos == pos) {
        return;
    }

    d->m_pos = pos;//必须先更新,updateTabBarPosition依赖
    d->updateTabBarPosition();
}

int FancyTabWidget::currentIndex() const
{
    return d->m_tabBar->currentIndex();
}

QStatusBar *FancyTabWidget::statusBar() const
{
    if (d->m_statusBar == nullptr) {//调用时自动创建
        d->m_statusBar = new QStatusBar;
        d->m_center->layout()->addWidget(d->m_statusBar);//status bar加入center中
    }

    return d->m_statusBar;
}

FancyTabBar *FancyTabWidget::tabBar() const
{
    return d->m_tabBar;
}

void FancyTabWidget::setTabStyle(FancyTabWidget::TabType type, FancyTabWidget::TabStyle style)
{
    d->m_tabBar->setTabStyle(static_cast<FancyTabBar::TabType>(type),
                             static_cast<FancyTabBar::TabStyle>(style));
}

void FancyTabWidget::setTabFont(FancyTabWidget::TabType type, QFont &font)
{
    d->m_tabBar->setTabFont(static_cast<FancyTabBar::TabType>(type), font);
}

void FancyTabWidget::setTabSpace(FancyTabWidget::TabType type, int space)
{
    d->m_tabBar->setTabSpace(static_cast<FancyTabBar::TabType>(type), space);
}

void FancyTabWidget::setTabIconSize(QSize size)
{
    d->m_tabBar->setTabIconSize(size);
}

int FancyTabWidget::addAction(QAction *action, FancyTabWidget::ActionPosition position)
{
    return d->m_tabBar->addAction(action, static_cast<FancyTabBar::ActionPosition>(position));
}

void FancyTabWidget::removeAction(QAction *action)
{
    d->m_tabBar->removeAction(action);
}

void FancyTabWidget::setActionStyle(QAction *action, FancyTabWidget::TabStyle style)
{
    d->m_tabBar->setActionStyle(action, static_cast<FancyTabBar::TabStyle>(style));
}

QSplitter *FancyTabWidget::addCornerWidget(QWidget *widget, FancyTabWidget::CornerPosition position)
{
    QSplitter *splitter = nullptr;

    switch (position) {
        case Left: {
            int count = d->m_leftSplitter->count();
            d->m_leftSplitter->insertWidget(count - 1, widget);//又不是mode bar,需要这么操作吗?
            splitter = d->m_leftSplitter;
            break;
        }
        case Top:
            d->m_topCorner->addWidget(widget);
            break;
        case Right:
            d->m_rightSplitter->addWidget(widget);
            splitter = d->m_rightSplitter;
            break;
        case Bottom:
            d->m_bottomSplitter->addWidget(widget);
            splitter = d->m_bottomSplitter;
            break;
    }

    return splitter;
}

void FancyTabWidget::setHoverColor(const QColor &color)
{
    d->m_tabBar->setHoverColor(color);
}

QColor FancyTabWidget::hoverColor() const
{
    return d->m_tabBar->hoverColor();
}

void FancyTabWidget::setPressColor(const QColor &color)
{
    d->m_tabBar->setPressColor(color);
}

QColor FancyTabWidget::pressColor() const
{
    return d->m_tabBar->pressColor();
}

void FancyTabWidget::setTextColor(const QColor &color)
{
    d->m_tabBar->setTextColor(color);
}

void FancyTabWidget::setSelectedTextColor(const QColor &color)
{
    d->m_tabBar->setSelectedTextColor(color);
}

void FancyTabWidget::setBackgroundColor(const QColor &color)
{
    d->m_tabBar->setBackgroundColor(color);
}

void FancyTabWidget::setCurrentIndex(int index)
{
//    d->m_stack->setCurrentIndex(index);
    d->m_tabBar->setCurrentIndex(index);//已经连接stack消息
}

void FancyTabWidget::hideMenu(int index)
{
    d->m_tabBar->hideMenu(index);
}

