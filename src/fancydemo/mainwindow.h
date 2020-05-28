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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcanpool/fancywindow.h"

class QMenu;
class QLabel;
class QToolBar;
class QStatusBar;
class QStackedWidget;

class ChartsMode;
class CustomMode;
class PaintMode;
class MenuMode;
class FancyTabWidget;
class ModeManager;
class FancyNavBar;

class MainWindow : public FancyWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* instance();

private:
    void createWindow();
    void createQuickAccessBar();//快速访问栏按钮
    void createMenuBar();//整条菜单栏
    void createAdditionalControls();//附属模块,工具搜索区域、换肤区域
    void createToolBar();//本对象不显示右键菜单,由父对象显示
    void createStatusBar();//底部状态栏
    void createDockWidget();
    void createCentralWidget();//整体布局
    void createModeBar();//中间整个横向模块

    void createOthers();
    void deleteOthers();

    void createConnects();

    void readSettings();
    void writeSettings();
	
public slots:
    void updateTheme();
	
private slots:
    void slotHelp();
    void slotNew();
    void slotSave();
    void slotSkin();
    void slotResizable(bool resizable);
    void slotChangeSkin(QString skinName);
    void slotSetStyle(QAction *action);

private:
    static MainWindow *m_instance;
    ModeManager *m_modeManager;
    FancyTabWidget *m_modeStack;//主体区,整个布局
    QStatusBar *m_statusBar;//下面的状态栏
    FancyNavBar *m_pNavBar;//导航条

    ChartsMode *m_pChartsMode;
    CustomMode *m_pCustomMode;
    PaintMode *m_pPaintMode;
    MenuMode *m_pMenuMode;
    QList<QAction *> m_styleActions;
    QAction *m_styleAction;
};

#endif // MAINWINDOW_H
