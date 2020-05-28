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
#include "mainwindow.h"
#include "cwidgets.h"
#include "chartsmode.h"
#include "custommode.h"
#include "menumode.h"

#include "qcanpool/fancytabwidget.h"
#include "qcanpool/fancytabbar.h"
#include "qcanpool/modemanager.h"
#include "qcanpool/fancybar.h"
#include "qcanpool/quickaccessbar.h"
#include "qcanpool/fancydialog.h"
#include "qcanpool/fancynavbar.h"
#include "qcanpool/pagesorter.h"
#include "qcanpool/fancybanner.h"

#include "skindialog.h"
#include "theme.h"

#define COMPANY_NAME    QString::fromUtf8("科技有限公司")

MainWindow *MainWindow::m_instance = nullptr;//全局唯一?

MainWindow::MainWindow(QWidget *parent)
    : FancyWindow(parent)
{
    m_instance = this;
    m_styleAction = nullptr;
    m_styleActions.clear();

    Theme::init();//初始化主题

    m_modeStack = new FancyTabWidget(this);
    m_modeManager = new ModeManager(m_modeStack);
    m_statusBar = m_modeStack->statusBar();

    createWindow();
    createConnects();

    setAcceptDrops(true);//允许拖动放置

    setWindowTitle(tr("Fancy demo"));//软件名称
    setWindowIcon(QIcon(":/main/logo"));//软件logo
    setMinimumSize(QSize(800,600));//最小大小
    setMouseTracking(true);
	
	updateTheme();
}

MainWindow::~MainWindow()
{
    delete m_modeManager;
    deleteOthers();
}

MainWindow *MainWindow::instance()
{
    return m_instance;
}

void MainWindow::createWindow()
{
    createQuickAccessBar();//快速访问栏按钮
    createMenuBar();//整条菜单栏
    createToolBar();
    createStatusBar();//底部状态栏
	//createDockWidget();
    createCentralWidget();//整体布局
    createModeBar();//中间整个横向模块
    createAdditionalControls();//附属模块,工具搜索区域、换肤区域

    createOthers();
}

void MainWindow::createQuickAccessBar()
{
    if (QuickAccessBar* quickAccessBar = fancyBar()->quickAccessBar())//如果存在
    {
        QAction* action = quickAccessBar->actionCustomizeButton();//更多按钮的第一行
        action->setToolTip(tr("Customize Quick Access Bar"));

        QAction* smallButton = quickAccessBar->addAction(QIcon(":/tools/new"), tr("New"));//新建按钮
        smallButton->setShortcut(QKeySequence::New);//smallButton->setShortcut(tr("Ctrl+n"));
        smallButton->setToolTip(tr("New File or Project\nCtrl+N"));
        connect(smallButton, SIGNAL(triggered()), this, SLOT(slotNew()));
		//quickAccessBar->setActionVisible(smallButton, false);

        smallButton = quickAccessBar->addAction(QIcon(":/tools/open"), tr("Open"));//打开按钮
        smallButton->setShortcut(tr("Ctrl+O"));
        smallButton->setToolTip(tr("Open File or Project\nCtrl+O"));
        //connect(smallButton, SIGNAL(triggered()), this, SLOT(slotOpen()));
        //quickAccessBar->setActionVisible(smallButton, false);

        QMenu *menu = new QMenu(this);
        menu->addAction(tr("action1"));
        menu->addAction(tr("action2"));
        menu->addAction(tr("action3"));
        menu->addAction(tr("action4"));
        smallButton->setMenu(menu);//打开按钮中嵌入菜单

        smallButton = quickAccessBar->addAction(QIcon(":/tools/save"), tr("Save"));//保存按钮
        smallButton->setShortcut(tr("Ctrl+S"));
        smallButton->setToolTip(tr("Save the active document\nCtrl+S"));
        connect(smallButton, SIGNAL(triggered()), this, SLOT(slotSave()));

        smallButton = quickAccessBar->addAction(QIcon(":/tools/save_all"), tr("Save All"));//保存所有按钮
        smallButton->setShortcut(tr("Ctrl+Shift+S"));
        smallButton->setToolTip(tr("Save all active documents\nCtrl+Shift+S"));
		//connect(smallButton, SIGNAL(triggered()), this, SLOT(pressButton()));

        smallButton = quickAccessBar->addAction(QIcon(":/tools/undo"), tr("&Undo"));//撤销按钮
        smallButton->setShortcut(QKeySequence::Undo);
        smallButton->setEnabled(false);

        smallButton = quickAccessBar->addAction(QIcon(":/tools/redo"), tr("&Redo"));//redo按钮
        smallButton->setShortcut(QKeySequence::Redo);
		//smallButton->setEnabled(false);

        fancyBar()->showQuickAccess(true);//显示快速访问栏

        fancyBar()->setBackgroundColor(QColor(0, 100, 200, 100));
    }
}

void MainWindow::createMenuBar()
{
    if(QMenuBar* menuBar = fancyBar()->menuBar())//menuBar若存在
    {
        QAction *action = new QAction(tr("action1"),this);
        QMenu* menu = menuBar->addMenu(tr("&File"));//文件
        menu->addAction(action);
        action = new QAction(tr("action1"),this);
        menu = menuBar->addMenu(tr("&Edit"));//编辑
        menu->addAction(action);
        action = new QAction(tr("action1"),this);
        menu = menuBar->addMenu(tr("&Debug"));//Debug
        menu->addAction(action);
        action = new QAction(tr("action1"),this);
        menu = menuBar->addMenu(tr("&View"));//查看
        menu->addAction(action);
        action = new QAction(tr("action1"),this);
        menu = menuBar->addMenu(tr("&Window"));//窗口
        menu->addAction(action);
        action = new QAction(tr("action1"),this);
        menu = menuBar->addMenu(tr("&Tools"));//工具
        menu->addAction(action);
        action = new QAction(tr("action1"),this);
        menu = menuBar->addMenu(tr("&Help"));//帮助
        menu->addAction(action);

        fancyBar()->showMenuBar(true);
    }

    QTextBrowser *text = new QTextBrowser(this);
    fancyBar()->setApplicationWidget(tr("Application"), text);
}

void MainWindow::createAdditionalControls()
{
	//最右上角加入附属控件,menu,help,skin
    QAction *menuAction = new QAction(QIcon(":/tools/menu"), tr("main menu"), this);
    fancyBar()->addAdditionalControl(menuAction, FancyBar::TitlePosition);
    QAction *helpAction = new QAction(QIcon(":/tools/help"), tr("Help"), this);
    connect(helpAction, SIGNAL(triggered(bool)), this, SLOT(slotHelp()));
    fancyBar()->addAdditionalControl(helpAction, FancyBar::TitlePosition);
    QAction *skinAction = new QAction(QIcon(":/tools/skin"), tr("Skin"), this);
    connect(skinAction, SIGNAL(triggered(bool)), this, SLOT(slotSkin()));
    fancyBar()->addAdditionalControl(skinAction, FancyBar::TitlePosition);

    QMenu *menu = new QMenu();//按钮触发的总menu
    // style
    QAction *styleActionMenu = new QAction(tr("Style"));
    menu->addAction(styleActionMenu);//第一个style action包含有一个嵌套menu
    QMenu *styleMenu = new QMenu(this);//menu style中嵌套的风格选项
    QActionGroup *actionGroup = new QActionGroup(this);//组合成组,只有一个能被选中
    QAction *styleAction = new QAction(tr("Window Style"));//window风格
    styleAction->setCheckable(true);//可选
    styleMenu->addAction(styleAction);//加入界面的嵌套menu
    actionGroup->addAction(styleAction);//加入触发组中
    m_styleActions.append(styleAction);//加入预设风格组
    styleAction = new QAction(tr("Classic Style"));
    styleAction->setCheckable(true);
    styleMenu->addAction(styleAction);
    actionGroup->addAction(styleAction);
    m_styleActions.append(styleAction);
    styleAction = new QAction(tr("Merged Style"));
    styleAction->setCheckable(true);
    styleMenu->addAction(styleAction);
    actionGroup->addAction(styleAction);
    m_styleActions.append(styleAction);
    styleActionMenu->setMenu(styleMenu);//设定嵌套menu
    connect(actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(slotSetStyle(QAction*)));//触发后切换风格
    m_styleActions.at(0)->setChecked(true);//设定第一个被选中

    menu->addAction(tr("action2"));
    menu->addAction(tr("action3"));
    menu->addAction(tr("action4"));
    menuAction->setMenu(menu);

	//菜单栏的附件
    QAction *action = new QAction(QIcon(":/tools/option"), tr("Option"), this);//工具按钮
    fancyBar()->addAdditionalControl(action, FancyBar::MenuPosition);

    QLineEdit *lineEdit = new QLineEdit();//编辑框
    fancyBar()->addAdditionalControl(lineEdit, FancyBar::MenuPosition);
}

void MainWindow::createToolBar()
{
    setContextMenuPolicy(Qt::NoContextMenu);//本对象不显示右键菜单,由父对象显示
}

void MainWindow::createStatusBar()
{
	//m_statusBar = m_modeStack->statusBar();//放在了初始化

    m_pNavBar = new FancyNavBar();
    m_pNavBar->setSideExpand(true);
//    m_pNavBar->setVisible(false);
    QSplitter *splitter = m_modeStack->addCornerWidget(m_pNavBar->panel(), FancyTabWidget::Bottom);//bottom加入的panel
    m_pNavBar->setSplitter(splitter);

    QTextBrowser *tb1 = new QTextBrowser(this);
    QTextBrowser *tb2 = new QTextBrowser(this);
	//加入两个测试按钮
    m_pNavBar->add(tr("tb1"), tb1);
    m_pNavBar->add(tr("tb2"), tb2, new QLineEdit());

	//m_pNavBar->remove(tb1);
	//delete tb1;//remove后delete

    m_statusBar->addWidget(m_pNavBar);//加入导航栏
}

void MainWindow::createDockWidget()
{
    QTreeWidget *m_pTreeWidget = new QTreeWidget(this);
    m_pTreeWidget->setHeaderHidden(true);
    m_pTreeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTreeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTreeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
//    connect(m_pTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
//            this, SLOT(SlotItemActivated(QTreeWidgetItem*,int)));

    QDockWidget *pDockWidgetTest = new QDockWidget(tr("Test View"), this);
    pDockWidgetTest->setAllowedAreas(Qt::AllDockWidgetAreas);
    pDockWidgetTest->setWidget(m_pTreeWidget);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidgetTest);

    QTableView *m_pTableView = new QTableView(this);
    QDockWidget *pDockWidgetScd = new QDockWidget(tr("SCD View"), this);
    pDockWidgetScd->setAllowedAreas(Qt::AllDockWidgetAreas);
    pDockWidgetScd->setWidget(m_pTableView);
    //pDockWidgetScd->hide();
    addDockWidget(Qt::RightDockWidgetArea, pDockWidgetScd);

    QTextEdit* m_pTextEdit = new QTextEdit(this);
    QDockWidget *pDockWidgetOutput = new QDockWidget(tr("Output"), this);
    pDockWidgetOutput->setAllowedAreas(Qt::AllDockWidgetAreas);
    pDockWidgetOutput->setWidget(m_pTextEdit);
    addDockWidget(Qt::RightDockWidgetArea, pDockWidgetOutput);
}

void MainWindow::createCentralWidget()
{
    setCentralWidget(m_modeStack);

//    QTextBrowser *tb1 = new QTextBrowser(this);
//    QTextBrowser *tb2 = new QTextBrowser(this);
//    tb2->setFixedHeight(30);
//    QTextBrowser *tb3 = new QTextBrowser(this);
//    QTextBrowser *tb4 = new QTextBrowser(this);
//    m_modeStack->addCornerWidget(tb1, FancyTabWidget::Left);
//    m_modeStack->addCornerWidget(tb2, FancyTabWidget::Top);
//    m_modeStack->addCornerWidget(tb3, FancyTabWidget::Right);
//    m_modeStack->addCornerWidget(tb4, FancyTabWidget::Bottom);
}

void MainWindow::createModeBar()
{
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0,0,0,20));

    FancyTabBar *modeBar = m_modeStack->tabBar();
    modeBar->setPalette(palette);
    modeBar->setAutoFillBackground(true);

    m_pChartsMode = new ChartsMode();
    m_pCustomMode = new CustomMode();
    m_pMenuMode = new MenuMode();

    m_modeManager->objectAdded(m_pCustomMode);
    m_modeManager->objectAdded(m_pChartsMode);
    m_modeManager->objectAdded(m_pMenuMode);

    m_modeManager->setCurrentMode(m_pCustomMode);

//    m_modeManager->setEnabled(m_pChartsMode, false);

    // mode action
    QAction *action = new QAction(QIcon(":/tools/start"), tr("Start"),this);
    action->setShortcut(tr("Ctrl+R"));
    action->setToolTip(tr("Start <i>Ctrl+R</i>"));
//    action->setEnabled(false);
    modeBar->addAction(action);

    action = new QAction(QIcon(":/tools/suspend"), tr("Suspend"),this);
    modeBar->addAction(action);
//    action->setEnabled(false);
    action = new QAction(QIcon(":/tools/stop"), tr("Stop"),this);
    modeBar->addAction(action/*, FancyTabBar::Front*/);
//    action->setEnabled(false);

    modeBar->setTabIconSize(QSize(24,24));
    modeBar->setTextColor(QColor(255,255,255));
    modeBar->setSelectedTextColor(QColor(255,255,255));
    modeBar->setHoverColor(QColor(255,255,255,50));

//    modeBar->setHeadSpace(5);

//    modeBar->setFixedWidth(200);
}

void MainWindow::createOthers()
{
}

void MainWindow::deleteOthers()
{

}

void MainWindow::createConnects()
{
    connect(this, SIGNAL(resizable(bool)), this, SLOT(slotResizable(bool)));
}

void MainWindow::updateTheme()
{
    FancyTabBar *modeBar = m_modeStack->tabBar();
    modeBar->setTextColor(Theme::g_fancyTabTextColor);
    modeBar->setSelectedTextColor(Theme::g_fancyTabSelectedTextColor);
    modeBar->setHoverColor(Theme::g_fancyTabHoverColor);
    modeBar->setPressColor(Theme::g_fancyTabPressColor);
    modeBar->setBackgroundColor(Theme::g_fancyTabBackgroundColor);

    fancyBar()->setHoverColor(Theme::g_fancyBarHoverColor);
    fancyBar()->setPressColor(Theme::g_fancyBarPressColor);
    fancyBar()->setTextColor(Theme::g_fancyBarTextColor);
    fancyBar()->setBackgroundColor(Theme::g_fancyBarBackgroundColor);
}

void MainWindow::slotHelp()
{
    QTextBrowser *tb = new QTextBrowser(this);
    FancyDialog dialog;
    dialog.setCentralWidget(tb);
    dialog.exec();
}

void MainWindow::slotNew()
{
//    QDialog dlg;
//    QHBoxLayout layout;
//    PageSorter sorter;
//    sorter.setMaxPage(5);
//    layout.addWidget(&sorter);
//    dlg.setLayout(&layout);
//    dlg.exec();

    FancyDialog dialog;
    dialog.setTitleBarHeight(30);
    dialog.addAdditionalControl(new QAction(QIcon(":/tools/new"), tr("Stop"),this));
    dialog.addAdditionalControl(new QAction(QIcon(":/tools/open"), tr("Stop"),this));
    dialog.addAdditionalControl(new QAction(QIcon(":/tools/save"), tr("Stop"),this));
    dialog.addAdditionalControl(new QAction(QIcon(":/tools/cut"), tr("Stop"),this));
    dialog.exec();
}

void MainWindow::slotSkin()
{
    SkinDialog dlg;
    connect(&dlg, SIGNAL(changeSkin(QString)), this, SLOT(slotChangeSkin(QString)));
    connect(&dlg, SIGNAL(themeChanged()), this, SLOT(updateTheme()));
    dlg.exec();
}

void MainWindow::slotSave()
{
    QDialog dialog;
    FancyBanner banner;
    for(int i = 0; i < 10; i++){
        banner.addPage(QPixmap(QString(":/skin/%1").arg(i)));
    }
    QHBoxLayout layout;
    layout.setMargin(0);
    layout.setSpacing(0);
    layout.addWidget(&banner);
    dialog.setLayout(&layout);
    dialog.resize(300, 80);
    dialog.exec();
}

void MainWindow::slotResizable(bool resizable)
{
    m_statusBar->setSizeGripEnabled(resizable);
}

void MainWindow::slotChangeSkin(QString skinName)
{
    QSettings settings("Canpool", "qtcanpool");
    settings.setValue("skin", skinName);
    update();
}

void MainWindow::slotSetStyle(QAction *action)
{
    if(m_styleAction == action){
        return;
    }
    m_styleAction = action;
    int index = m_styleActions.indexOf(action);
    fancyBar()->setFancyStyle((FancyBar::FancyStyle)index);
}

void MainWindow::readSettings()
{
    QSettings settings("Canpool", "qtproject");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings("Canpool", "qtproject");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
