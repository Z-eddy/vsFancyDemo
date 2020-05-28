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
    void createWindow();//���ø��ִ���,��ϳɴ���
    void createQuickAccessBar();//���ٷ�������ť
    void createMenuBar();//�����˵���
    void createAdditionalControls();//����ģ��,�����������򡢻�������
    void createToolBar();//��������ʾ�Ҽ��˵�,�ɸ�������ʾ
    void createStatusBar();//�ײ�״̬��
    void createDockWidget();//ͣ����top,left,right,bottom�Ĵ���
    void createCentralWidget();//���岼��
    void createModeBar();//�м���������ģ��

    void createOthers();
    void deleteOthers();

    void createConnects();

	//���ڴ�С�趨
    void readSettings();
    void writeSettings();
	
public slots:
    void updateTheme();//���¸�������ɫֵ
	
private slots:
    void slotHelp();//�����ʺ�
    void slotNew();//�½���ť
    void slotSave();//���水ť
    void slotSkin();
    void slotResizable(bool resizable);
    void slotChangeSkin(QString skinName);//д��ע���
    void slotSetStyle(QAction *action);//ѡ��������,����������������action

private:
    static MainWindow *m_instance;
    ModeManager *m_modeManager;
    FancyTabWidget *m_modeStack;//������,��������
    QStatusBar *m_statusBar;//�����״̬��
    FancyNavBar *m_pNavBar;//������

    ChartsMode *m_pChartsMode;//λ��mode bar�Ϸ�
    CustomMode *m_pCustomMode;//λ��mode bar�Ϸ�
    PaintMode *m_pPaintMode;
    MenuMode *m_pMenuMode;//λ��mode bar�·�,Tools
    QList<QAction *> m_styleActions;
    QAction *m_styleAction;
};

#endif // MAINWINDOW_H
