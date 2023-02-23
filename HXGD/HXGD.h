#pragma once
 
# pragma execution_character_set("utf-8")    
#pragma comment(lib,"opencv_world450.lib")

#include <QtWidgets/QMainWindow>
#include "ui_HXGD.h"
#include <qscreen.h>
#include <qfiledialog.h>
#include <opencv2\opencv.hpp>
#include <qdebug.h>
#include "ToolFunction.h"
#include "m_GraphicsView.h"
#include <qlistwidget.h>
#include <qgridlayout.h>
#include "m_EuresysBoard.h"
#include <qthread.h>
#include <qdom.h>
#include <qmenu.h>
#include <qaction.h>
#include <qtextcodec.h>
#include <qdatetime.h>
#include <qtooltip.h>
#include <qmessagebox.h>
#include <qspaceritem>
#include "imagewidget.h"
#include "Params.h"
#include "m_TCP.h"
#include "m_Thread.h"
#include <QtConcurrent/QtConcurrent>
#include "m_CSV.h"
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include "Login.hpp"

//#define zh_CN QString::toLocal8Bit

typedef struct CONFIGINFO
{
    QString layout_xml =  QString("%1/ConfigureXML/PanelLayout.xml").arg(QApplication::applicationDirPath());
    QString receipe_xml =  QString("%1/ConfigureXML/RecipeInfo.xml").arg(QApplication::applicationDirPath());
}ConfigInfo;

typedef struct _CurrentLayoutInfo
{
    QString layoutName;
    QString productID;
    QString productSize;
    QString updateTime;
    QString layoutDirection;
    QString remarks;
}CurrentLayoutInfo;


typedef struct _SmallPanel
{
    QString layoutname;
    QString x;
    QString y;
    QString width;
    QString height;
    //QString left_margin;
    //QString right_margin;
    //QString top_margin;
    //QString bottom_margin;
    QString remarks;
}SmallPanel;
//typedef QList<PanelInfo> listPanelInfo;
//Q_DECLARE_METATYPE(listPanelInfo);
struct PanelPara
{
    cv::Rect2f panelRect;
    int panelNum;
    QString panelID;
    QString AlgName;
};
struct LayoutPara
{
    double width;
    double height;
    std::vector<PanelPara> listPanel;
};

struct RecipePara
{
    int recipeID;
    QString recipeName;
    QString layoutName;
    LayoutPara layout;
    int ilightValue;
    int iMaxWarnValue;
};

class HXGD : public QMainWindow
{
    Q_OBJECT

public:
    HXGD(QWidget *parent = nullptr);
    ~HXGD();

private:
    Ui::HXGDClass ui;
    ConfigInfo config_info;
    m_Thread* p_thread = NULL;

    /*main page*/
private:
    RecipePara currentRecipe;
    QString m_red_SheetStyle;
    QString m_green_SheetStyle;
  /*  cv::Mat srcMat;*/
    bool    bshowListWidget_imgs = true;
    QThread thread;
    m_EuresysBoard* board1;
    m_EuresysBoard* board2;
    QComboBox* cbx_permit;
    QLabel* LB_USER;
    QLabel* LB_CAM1_S;
    QLabel* LB_CAM1_T;
    QLabel* LB_CAM2_S;
    QLabel* LB_CAM2_T;
    QLabel* LB_PLC_S;
    QLabel* LB_PLC_T;
    QLabel* LB_CLIENT_S;
    QLabel* LB_CLIENT_T;
    bool bCam_1Triggered=false;
    bool bCam_2Triggered=false;
    cv::Mat mainMat[2];
    cv::Mat srcMat;
    cv::Mat dstMat;
    m_CSV* m_csv;
    
public:
    void GetCam_1Pic(cv::Mat mat,int camNum);
    void GetCam_2Pic(cv::Mat mat,int camNum);
    //拼接图像
    void DoJointImgs();
    void ShowMainImg(cv::Mat mat);
    
signals:
    void signalShowImg(cv::Mat mat);
    void signalWriteCSV(QString filePath, QStringList data);
    void signalReadCSV(QString filePath, QStringList& data);
public slots:
    void slotOpenFileDialog();
    void on_pushButton_expand_clicked();
    void on_pushButton_go_clicked();
    void on_pushButton_idle_clicked();

    /*cam page*/
private:
    cv::Mat matImg1;
    cv::Mat matImg2;
    ImageWidget* ImageSplicShow = NULL;
    m_TCPClient* client = NULL;
    m_TCPServer* server = NULL;
public:
    void InitCamSplic();
    void refreshImgSplic();
    void showSplicImg(QImage img);
    void leftCamDebugLoadData();
public slots:
    void on_pb_loadCam1_clicked();
    void on_pb_loadCam2_clicked();
    void on_pb_saveSplicParam_clicked();

signals:
        

    /*Layout page*/
private:
    int currentRowIndex = -1;
    QString product_id;
    QMenu* table_widget_menu;
   
    //listPanelInfo list_panelinfo;
public:
    void InitUILayout();
  
    void LoadAllLayoutData();
    void LoadPanelDataByProductID(QString layoutname, QList<SmallPanel>& templist_panel_Info);
    void ExpandSmallPanel(QTableWidget* tablewidget,QList<SmallPanel> list_panle);
    void ShowCurrentLayoutSettings(CurrentLayoutInfo currentLayoutInfo);
    void LoadSPanelInfo( LayoutBar* layoutbar, QDomNode node);
    //加载Layout
    void LayoutLoadByNameAndWidget(QString str, LayoutBar& bar, LayoutPara& Lp, bool bLoadRecipe);
signals:
        
public slots:
    void slotShowTooltip(QModelIndex index);
    void slotLayoutCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void slotLayoutSmallPanelCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_layoutInfo_customContextMenuRequested(const QPoint& pos);

    void slotActionImportTemplate();
    void slotActionAddLayout();
    void slotActionRefresh();


    /*recipe page*/
private:
    QString currentRecipeName;
    QString currentLayout;
    QString currentSize;
    int mCurrentRecipeRow = -1;
   
public:
    struct RecipePara
    {
        int recipeID;
        QString recipeName;
        QString layoutName;
        //LayoutPara layout;
        //int iLightValue;
        int iMaxWarnValue;
        QList<cv::Rect> shieldList;
    };
    //初始化TableWidget的样式
    void InitUIRecipe();
    //加载所有的recipe
    void LoadAllRecipes();
    void ShowCurrentRecipeInfo(QString currentRecipeName,QString currentSize, QString currentLayout);
    void ReadRecipeByName(QString recipeName, RecipePara& recipePara);
    //主程序加载当前Recipe
    void LoadCurrentRecipe();
signals:

public slots:
    void on_pb_addRecipe_clicked();
    void on_pb_delRecipe_clicked();
    void on_pb_recipeEdit_clicked();
    void on_pb_recipeSave_clicked();
    void on_pb_recipeCancel_clicked();
    void slotRecipeIDCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void slotCurrentRecipeInfoCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void slotCurrentRecipeReposCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);




    /*usersheet*/

private:
    int iCurrentUserInfoRow = -1;
    QSqlDatabase db;

public :
    int iLoginMode = -1;
    int loginLevel = -1;
    Login* login = new Login();
    QString userNameLogin;
    void InitUserUI();
    //从数据库加载所有人员信息
    void LoadUserInfoFromDB();

signals:
    void signalLogStatus(bool bLogin);

public slots:
    void slotUserInfoCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_pb_login_clicked();
    void on_pb_addUser_clicked();
    void on_pb_editUser_clicked();
    void on_pb_deleteUser_clicked();
    void on_pb_queryUserOperate_clicked();

    void slotLogin(QString,QString,QString);
    void slotEditAddUser(int mode,QString username,QString password,int level);




};
