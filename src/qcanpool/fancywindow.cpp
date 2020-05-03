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

//����FancyBar(���Ƶı�����)
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
	//Ϊ���ޱ߿�
    QMainWindow::setWindowFlags(Qt::FramelessWindowHint//�ޱ߿�
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
    this->raise();//Z����ǰ��
    this->activateWindow();//�趨Ϊ�����,Ч�������������title bar
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
    d->fancyBar()->setWidgetResizable(false);//��һ��fix����ֻΪ����һ��
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

//�滭����
void FancyWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
	//����������ע�����
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

