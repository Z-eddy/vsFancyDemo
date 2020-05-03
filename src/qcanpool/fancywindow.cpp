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
#include "fancywindow.h"
#include "fancybar.h"
#include "screenhelper.h"
#include "qcanpool_p.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QStatusBar>
#include <QSettings>
#include <QPainter>

//管理FancyBar(定制的标题栏)
class FancyWindowPrivate
{
public:
    FancyWindowPrivate(FancyBar*item);
	FancyBar* fancyBar();

private:
    FancyBar *fancyBar_;
};

FancyWindowPrivate::FancyWindowPrivate(FancyBar*item) :fancyBar_(item)
{
}

FancyBar* FancyWindowPrivate::fancyBar() {
	return fancyBar_;
}

FancyWindow::FancyWindow(QWidget *parent)
    : QMainWindow(parent), d(new FancyWindowPrivate(new FancyBar(this)))
{
	//为了无边框
    QMainWindow::setWindowFlags(Qt::FramelessWindowHint//无边框
                   | Qt::WindowSystemMenuHint
                   | Qt::WindowMinimizeButtonHint
                   | Qt::WindowMaximizeButtonHint
                   | Qt::Window
                  );
    connect(d->fancyBar(), SIGNAL(maximizationChanged(bool)), this, SIGNAL(resizable(bool)));
    this->setMenuWidget(d->fancyBar());
    this->setMouseTracking(true);
    QRect geom = ScreenHelper::normalRect();
    this->resize(geom.width(), geom.height());
    this->raise();//Z在最前面
    this->activateWindow();//设定为活动窗口,效果类似鼠标点击了title bar
}

FancyWindow::~FancyWindow()
{
}

FancyBar *FancyWindow::fancyBar() const
{
    return d->fancyBar();
}

void FancyWindow::setFixedSize(const QSize &s)
{
    setFixedSize(s.width(), s.height());
}

void FancyWindow::setFixedSize(int w, int h)
{
    d->fancyBar()->setWidgetResizable(false);//这一堆fix函数只为了这一条
    QWidget::setFixedSize(w, h);
}

void FancyWindow::setFixedWidth(int w)
{
    setFixedSize(w, this->height());
}

void FancyWindow::setFixedHeight(int h)
{
    setFixedSize(this->width(), h);
}

void FancyWindow::setWindowFlags(Qt::WindowFlags type)
{
    QMainWindow::setWindowFlags(type);
    d->fancyBar()->updateWidgetFlags();
}

//绘画背景
void FancyWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
	//背景保存在注册表中
    QSettings settings(QCanpoolPrivate::g_settingsOrganization,
                       QCanpoolPrivate::g_settingsApplication);
    QString skinName = settings.value("skin").toString();

    if (skinName.isEmpty()) {
        skinName = QString(":/skin/default");
    }

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.drawPixmap(QRect(0, 0, this->width(), this->height()),
                       QPixmap(skinName));
}

