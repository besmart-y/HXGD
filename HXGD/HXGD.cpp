#include "HXGD.h"

HXGD::HXGD(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
	QTextCodec* codec = QTextCodec::codecForName("gbk");
#else
	QTextCodec* codec = QTextCodec::codecForName("utf-8");
#endif
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);
#else
	QTextCodec* codec = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForLocale(codec);
#endif	

	p_thread = &m_Thread::get_Instance();

	m_red_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:red";
	m_green_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:green";

	ui.stackedWidget->setCurrentIndex(0);
    QList <QScreen*> list_screen = QGuiApplication::screens();
    this->resize(list_screen.at(0)->geometry().width(),list_screen.at(0)->geometry().height());

	//connect(this, &HXGD::signalShowImg, ui.graphicsView_mainview, &m_GraphicsView::slotShowImg);


	LB_USER = new QLabel();
	LB_USER->setText(tr("权限"));
	LB_USER->setMinimumSize(QSize(30,0));
	LB_USER->setAlignment(Qt::AlignCenter);
	ui.statusBar->addWidget(LB_USER);

	cbx_permit = new QComboBox();
	cbx_permit->setMinimumSize(QSize(60,0));
	cbx_permit->addItem("管理员");
	cbx_permit->addItem("工程师");
	cbx_permit->addItem("操作者");
	cbx_permit->addItem("未登录");
	cbx_permit->setCurrentIndex(3);
	cbx_permit->setEnabled(false);
	ui.statusBar->addWidget(cbx_permit);

	LB_CAM1_S = new QLabel();
	LB_CAM1_S->setMinimumSize(QSize(30, 0));
	LB_CAM1_S->setStyleSheet(m_red_SheetStyle);
	ui.statusBar->addWidget(LB_CAM1_S);

	LB_CAM1_T = new QLabel();
	LB_CAM1_T->setText(tr("Cam1"));
	LB_CAM1_T->setMinimumSize(QSize(30, 0));
	LB_CAM1_T->setAlignment(Qt::AlignCenter);
	ui.statusBar->addWidget(LB_CAM1_T);

	LB_CAM2_S = new QLabel();
	LB_CAM2_S->setMinimumSize(QSize(30, 0));
	LB_CAM2_S->setStyleSheet(m_red_SheetStyle);
	ui.statusBar->addWidget(LB_CAM2_S);

	LB_CAM2_T = new QLabel();
	LB_CAM2_T->setText(tr("Cam2"));
	LB_CAM2_T->setMinimumSize(QSize(30, 0));
	LB_CAM2_T->setAlignment(Qt::AlignCenter);
	ui.statusBar->addWidget(LB_CAM2_T);

	LB_PLC_S = new QLabel();
	LB_PLC_S->setMinimumSize(QSize(30, 0));
	LB_PLC_S->setStyleSheet(m_red_SheetStyle);
	ui.statusBar->addWidget(LB_PLC_S);

	LB_PLC_T = new QLabel();
	LB_PLC_T->setText(tr("PLC"));
	LB_PLC_T->setMinimumSize(QSize(30, 0));
	LB_PLC_T->setAlignment(Qt::AlignCenter);
	ui.statusBar->addWidget(LB_PLC_T);


	LB_CLIENT_S = new QLabel();
	LB_CLIENT_S->setMinimumSize(QSize(30, 0));
	LB_CLIENT_S->setStyleSheet(m_red_SheetStyle);
	ui.statusBar->addWidget(LB_CLIENT_S);

	LB_CLIENT_T = new QLabel();
	LB_CLIENT_T->setText(tr("Client"));
	LB_CLIENT_T->setMinimumSize(QSize(30, 0));
	LB_CLIENT_T->setAlignment(Qt::AlignCenter);
	ui.statusBar->addWidget(LB_CLIENT_T);

	

	//ui.statusBar->addWidget(new QSpacerItem(100, 20, QSizePolicy::Expanding));
	QString path = QApplication::applicationDirPath();
	g_params.SetIniPathDir(path);
	g_params.LoadDataFromFile();


	m_csv = new m_CSV();
	connect(this, &HXGD::signalWriteCSV, m_csv, &m_CSV::slotWriteCSV);
	connect(this, &HXGD::signalReadCSV, m_csv, &m_CSV::slotReadCSV);

	hxgdAlg = new HXGDAlg();

	connect(ui.listWidget_Iconpic, &QListWidget::itemDoubleClicked, this,&HXGD::slotListWidgetItemDoubleClicked);
    connect(ui.actionopen_2, &QAction::triggered, this, &HXGD::slotOpenFileDialog);
	connect(ui.tb_camPage, &QToolButton::clicked, this, [=]()
		{
			ui.stackedWidget->setCurrentIndex(1);
		});
	connect(ui.tb_mainPage, &QToolButton::clicked, this, [=]()
		{
			ui.stackedWidget->setCurrentIndex(0);
		});
	connect(ui.tb_recipePage, &QToolButton::clicked, this, [=]()
		{
			ui.stackedWidget->setCurrentIndex(2);
		});
	connect(ui.tb_layoutPage, &QToolButton::clicked, this, [=]()
		{
			ui.stackedWidget->setCurrentIndex(3);
		});
	connect(ui.tb_algPage, &QToolButton::clicked, this, [=]()
		{
			ui.stackedWidget->setCurrentIndex(4);
		});
	connect(ui.tb_hisPage, &QToolButton::clicked, this, [=]()
		{
			ui.stackedWidget->setCurrentIndex(5);
		});
	connect(ui.tb_permitPage, &QToolButton::clicked, this, [=]()
		{
			ui.stackedWidget->setCurrentIndex(6);
		});

	//ui.listWidget_imgs->setVisible(bshowListWidget_imgs);
	ui.pushButton_expand->setToolTip(tr("收起"));

	board1 = new m_EuresysBoard();
	board2 = new m_EuresysBoard();
	board1->setProperty("CAM", 0);
	board2->setProperty("CAM", 1);
	p_thread->addThread(board1);
	p_thread->addThread(board2);
	
	connect(board1, &m_EuresysBoard::sigSendOutPic, this, &HXGD::GetCam_1Pic,
		Qt::BlockingQueuedConnection);
	connect(board2, &m_EuresysBoard::sigSendOutPic, this, &HXGD::GetCam_2Pic,
		Qt::BlockingQueuedConnection);

	
	qRegisterMetaType<cv::Mat>("cv::Mat");
	board1->InitInstance();
	board1->PageAcquistion("F:/work/TESTCAMFILES/page2.cam");//CreateChannel();
	
	
	LoadCurrentRecipe();
	InitUILayout();
	InitCamSplic();
	LoadAllLayoutData();
	InitUIRecipe();
	LoadAllRecipes();

	InitUserUI();



	ui.listWidget_Iconpic->setViewMode(QListView::ViewMode::IconMode);
	ui.listWidget_Iconpic->setIconSize(QSize(170,180));
	ui.listWidget_Iconpic->setSpacing(10);
	ui.listWidget_Iconpic->setResizeMode(QListView::ResizeMode::Adjust);
	ui.listWidget_Iconpic->setMovement(QListView::Movement::Static);
	ui.listWidget_Iconpic->setFlow(QListView::Flow::LeftToRight);//从左到右

	

}

HXGD::~HXGD()
{
	//thread.quit();
	//thread.wait();
}

void HXGD::GetCam_1Pic(cv::Mat mat,int camNum)
{
	if (board1->m_mat.data)
	{
		mainMat[0] = board1->m_mat.clone();
		//hxgdAlg->SplitImage(mainMat[1], lineScanCam_0);
		bCam_1Triggered = true;
		
		ui.listWidget_Iconpic->clear();
	}
}

void HXGD::GetCam_2Pic(cv::Mat mat, int camNum)
{
	if (board2->m_mat.data)
	{
		mainMat[1] = board1->m_mat.clone();
		//hxgdAlg->SplitImage(mainMat[1], lineScanCam_1);
		bCam_2Triggered = true;
		
		ui.listWidget_Iconpic->clear();
	}
}

void HXGD::DoJointImgs()
{
	QtConcurrent::run([&]() {
		if (bCam_1Triggered&&bCam_2Triggered)
		{
			if ((!mainMat[0].data) ||
				g_params.camconfig->listCamSplic[0].leftCut < 0 ||
				g_params.camconfig->listCamSplic[0].topCut < 0 ||
				mainMat[0].cols - g_params.camconfig->listCamSplic[0].leftCut - g_params.camconfig->listCamSplic[0].rightCut<0 ||
				mainMat[0].cols - g_params.camconfig->listCamSplic[0].leftCut - g_params.camconfig->listCamSplic[0].rightCut>mainMat[0].cols ||
				mainMat[0].rows - g_params.camconfig->listCamSplic[0].topCut - g_params.camconfig->listCamSplic[0].bottomCut<0 ||
				mainMat[0].rows - g_params.camconfig->listCamSplic[0].topCut - g_params.camconfig->listCamSplic[0].bottomCut>mainMat[0].rows)
			{
				qWarning() << tr("图像切割尺寸错误，请修改参数！");
				return;
			}
			cv::Mat tempMat1 = mainMat[0](cv::Rect(g_params.camconfig->listCamSplic[0].leftCut,
				g_params.camconfig->listCamSplic[0].topCut,
				mainMat[0].cols - g_params.camconfig->listCamSplic[0].leftCut - g_params.camconfig->listCamSplic[0].rightCut,
				mainMat[0].rows - g_params.camconfig->listCamSplic[0].topCut - g_params.camconfig->listCamSplic[0].bottomCut));

			if ((!mainMat[1].data) ||
				g_params.camconfig->listCamSplic[1].leftCut < 0 ||
				g_params.camconfig->listCamSplic[1].topCut < 0 ||
				mainMat[1].cols - g_params.camconfig->listCamSplic[1].leftCut - g_params.camconfig->listCamSplic[1].rightCut<0 ||
				mainMat[1].cols - g_params.camconfig->listCamSplic[1].leftCut - g_params.camconfig->listCamSplic[1].rightCut>mainMat[1].cols ||
				mainMat[1].rows - g_params.camconfig->listCamSplic[1].topCut - g_params.camconfig->listCamSplic[1].bottomCut<0 ||
				mainMat[1].rows - g_params.camconfig->listCamSplic[1].topCut - g_params.camconfig->listCamSplic[1].bottomCut>mainMat[1].rows)
			{
				qWarning() << tr("图像切割尺寸错误，请修改参数！");
				return;
			}
			cv::Mat tempMat2 = mainMat[1](cv::Rect(g_params.camconfig->listCamSplic[1].leftCut,
				g_params.camconfig->listCamSplic[1].topCut,
				mainMat[1].cols - g_params.camconfig->listCamSplic[1].leftCut - g_params.camconfig->listCamSplic[1].rightCut,
				mainMat[1].rows - g_params.camconfig->listCamSplic[1].topCut - g_params.camconfig->listCamSplic[1].bottomCut));
			cv::resize(tempMat2, tempMat2, cv::Size(tempMat2.cols, tempMat1.rows));
			//获得拼接图像
			srcMat = cv::Mat::zeros(cv::Size(tempMat1.cols+tempMat2.cols,tempMat1.rows),CV_8UC1);
			
			tempMat1.copyTo(srcMat(cv::Rect(0,0,tempMat1.cols,tempMat1.rows)));
			tempMat2.copyTo(srcMat(cv::Rect(tempMat1.cols,0,tempMat2.cols,tempMat2.rows)));
			//抽图
			hxgdAlg->SplitImage(srcMat, lineScanCam);
			QListWidgetItem* item1 = new QListWidgetItem();
			QListWidgetItem* item2 = new QListWidgetItem();
			QListWidgetItem* item3 = new QListWidgetItem();
			int w = ui.listWidget_Iconpic->width();
			m_GraphicsView* tempview1 = new m_GraphicsView(w - 40, w - 160);
			m_GraphicsView* tempview2 = new m_GraphicsView(w - 40, w - 160);
			m_GraphicsView* tempview3 = new m_GraphicsView(w - 40, w - 160);
			tempview1->setAttribute(Qt::WA_TransparentForMouseEvents);//设置当前控件只显示，不做任何消息处理
			tempview2->setAttribute(Qt::WA_TransparentForMouseEvents);
			tempview3->setAttribute(Qt::WA_TransparentForMouseEvents);
			//item->setIcon(QIcon(var));
			
			//int h = ui.listWidget_Iconpic->height();
			item1->setSizeHint(QSize(w - 40, w - 160));
			item2->setSizeHint(QSize(w - 40, w - 160));
			item3->setSizeHint(QSize(w - 40, w - 160));
			ui.listWidget_Iconpic->addItem(item1);
			ui.listWidget_Iconpic->addItem(item2);
			ui.listWidget_Iconpic->addItem(item3);

			ui.listWidget_Iconpic->setItemWidget(item1, tempview1);
			ui.listWidget_Iconpic->setItemWidget(item2, tempview2);
			ui.listWidget_Iconpic->setItemWidget(item3, tempview3);
			
			//1、显示图像
			//主-显示CH1-SRC
			//副1-显示CH2-SRC
			//副2-显示CH3-SRC
			//副3-暂存
			ui.graphicsView_mainview->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH0_SRC));
			tempview1->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH1_SRC));
			tempview2->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH2_SRC));
			ui.listWidget_Iconpic->show();

			//ShowMainImg(srcMat);
			//2、存图
			QString picSavePath = g_params.camconfig->camsettings.saveImagePath;
			if (!QDir(picSavePath).exists())
			{
				QDir picSaveDir(picSavePath);
				picSaveDir.mkdir(picSavePath);
					 
			}
			//picSavePath += QString("\\%1").arg(currentJobData.GlassID.isEmpty()&&currentJobData.GlassID.length()>5)?"None":currentJobData.GlassID;



		}
	});
	
}

void HXGD::ShowMainImg(cv::Mat mat)
{
	if (mat.data)
	{
		QPixmap ConvertPixmap = QPixmap::fromImage(ToolClass::Mat2QImage(mat));
		int nWidth = ui.graphicsView_mainview->width();
		int nHeight = ui.graphicsView_mainview->height();
		ImageSplicShow->reloadImage(&ConvertPixmap, nWidth, nHeight);
	}
	else
	{
		qWarning() << tr("srcMat is NULL");
	}
}



void HXGD::slotListWidgetItemDoubleClicked(QListWidgetItem* item)
{
	QWidget* widget = ui.listWidget_Iconpic->itemWidget(item);

	m_GraphicsView* list_view1 = widget->findChild<m_GraphicsView*>("tempview1");
	m_GraphicsView* list_view2 = widget->findChild<m_GraphicsView*>("tempview2");
	m_GraphicsView* list_view3 = widget->findChild<m_GraphicsView*>("tempview3");
	//QListWidgetItem* list_view = widget->findChild<QListWidgetItem*>();
	iCurrentListWidgetRow = ui.listWidget_Iconpic->currentRow();
	if (iCurrentListWidgetRow == 0)//CH2_SRC
	{
		
		ui.listWidget_Iconpic->clear();

		/*	cv::resize(src2,src2,cv::Size(512,512));
			cv::resize(src3,src3,cv::Size(512,512));*/
		QListWidgetItem* item1 = new QListWidgetItem();
		QListWidgetItem* item2 = new QListWidgetItem();
		QListWidgetItem* item3 = new QListWidgetItem();
		int w = ui.listWidget_Iconpic->width();
		m_GraphicsView* tempview1 = new m_GraphicsView(w - 40, w - 160);
		m_GraphicsView* tempview2 = new m_GraphicsView(w - 40, w - 160);
		m_GraphicsView* tempview3 = new m_GraphicsView(w - 40, w - 160);
		tempview1->setAttribute(Qt::WA_TransparentForMouseEvents);//设置当前控件只显示，不做任何消息处理
		tempview2->setAttribute(Qt::WA_TransparentForMouseEvents);
		tempview3->setAttribute(Qt::WA_TransparentForMouseEvents);
		//item->setIcon(QIcon(var));
		/*int w = ui.listWidget_Iconpic->width();
		int h = ui.listWidget_Iconpic->height();*/
		item1->setSizeHint(QSize(w - 40, w - 160));
		item2->setSizeHint(QSize(w - 40, w - 160));
		item3->setSizeHint(QSize(w - 40, w - 160));
		ui.listWidget_Iconpic->addItem(item1);
		ui.listWidget_Iconpic->addItem(item2);
		ui.listWidget_Iconpic->addItem(item3);

		ui.listWidget_Iconpic->setItemWidget(item1, tempview1);
		ui.listWidget_Iconpic->setItemWidget(item2, tempview2);
		ui.listWidget_Iconpic->setItemWidget(item3, tempview3);
		ui.graphicsView_mainview->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH1_SRC));
		tempview1->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH0_SRC));
		tempview2->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH2_SRC));
		tempview3->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH2_SRC));
		ui.listWidget_Iconpic->show();
		
	}
	else if (iCurrentListWidgetRow == 1)//CH3_SRC
	{

		ui.listWidget_Iconpic->clear();

		/*	cv::resize(src2,src2,cv::Size(512,512));
			cv::resize(src3,src3,cv::Size(512,512));*/
		QListWidgetItem* item1 = new QListWidgetItem();
		QListWidgetItem* item2 = new QListWidgetItem();
		QListWidgetItem* item3 = new QListWidgetItem();
		int w = ui.listWidget_Iconpic->width();
		m_GraphicsView* tempview1 = new m_GraphicsView(w - 40, w - 160);
		m_GraphicsView* tempview2 = new m_GraphicsView(w - 40, w - 160);
		m_GraphicsView* tempview3 = new m_GraphicsView(w - 40, w - 160);
		tempview1->setAttribute(Qt::WA_TransparentForMouseEvents);//设置当前控件只显示，不做任何消息处理
		tempview2->setAttribute(Qt::WA_TransparentForMouseEvents);
		tempview3->setAttribute(Qt::WA_TransparentForMouseEvents);
		//item->setIcon(QIcon(var));
		/*int w = ui.listWidget_Iconpic->width();
		int h = ui.listWidget_Iconpic->height();*/
		item1->setSizeHint(QSize(w - 40, w - 160));
		item2->setSizeHint(QSize(w - 40, w - 160));
		item3->setSizeHint(QSize(w - 40, w - 160));
		ui.listWidget_Iconpic->addItem(item1);
		ui.listWidget_Iconpic->addItem(item2);
		ui.listWidget_Iconpic->addItem(item3);

		ui.listWidget_Iconpic->setItemWidget(item1, tempview1);
		ui.listWidget_Iconpic->setItemWidget(item2, tempview2);
		ui.listWidget_Iconpic->setItemWidget(item3, tempview3);
		ui.graphicsView_mainview->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH1_SRC));
		tempview1->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH0_SRC));
		tempview2->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH2_SRC));
		ui.listWidget_Iconpic->show();
		ui.listWidget_Iconpic->show();
	}
	else if (iCurrentListWidgetRow == 2)
	{

	}
	else
	{
		return;
	}
}

void HXGD::on_pushButton_expand_clicked()
{
	if (bshowListWidget_imgs == false)
	{
		bshowListWidget_imgs = true;
		ui.pushButton_expand->setStyleSheet("QPushButton{image: url(:/HXGD/src/shrink.png);}"
			"QPushButton{border:none;background:transparent;}");
		ui.pushButton_expand->setToolTip(tr("收起"));
		//ui.listWidget_imgs->setVisible(true);//show();
		return;
	}
	else if (bshowListWidget_imgs == true)
	{
		bshowListWidget_imgs = false;
		ui.pushButton_expand->setStyleSheet("QPushButton{image: url(:/HXGD/src/expand.png);}"
			"QPushButton{border:none;background:transparent;}");
		ui.pushButton_expand->setToolTip(tr("展开"));
		//ui.listWidget_imgs->setVisible(false);
		return;
	}
}

void HXGD::on_pushButton_go_clicked()
{
	board1->ActivateChannel();
}

void HXGD::on_pushButton_idle_clicked()
{
	board1->InactivateChannel();
}






void HXGD::slotOpenFileDialog()
{
	//QString path = QFileDialog::getOpenFileName(NULL, tr("Choose images"),
	//	"D://", tr("Image Files(*.jpg;*.bmp)"));
	//if (path.length() == 0) return;
	//if (path.isEmpty()) return;
	
	try
	{
		//srcMat = cv::imread(path.toLocal8Bit().toStdString(), cv::IMREAD_GRAYSCALE);
		//if (srcMat.data)
		//{
		//	//condi.wakeAll();
		//}
		//emit sigMainViewImgShow(CAssist::Mat2QImage(srcMat));
		 //定义文件对话框类
		//QStringList fileNames;
	 //QFileDialog * fileDialog = new QFileDialog(this);
		////定义文件对话框标题
		//	fileDialog->setWindowTitle(tr("打开图片"));
		////设置默认文件路径
		// fileDialog->setDirectory(".");
		// //设置文件过滤器
		// fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
		////设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
		//	 fileDialog->setFileMode(QFileDialog::ExistingFiles);
		// //设置视图模式
		//	fileDialog->setViewMode(QFileDialog::Detail);
		////打印所有选择的文件的路径
		//	if (fileDialog->exec())
		//	{
		//		 fileNames = fileDialog->selectedFiles();
		//	}
		/*	QListWidgetItem* item = new QListWidgetItem();
			QIcon icon(fileNames.at(0));
			ui.pushButton_expand->setIcon(icon);*/
			//item->setIcon(icon);//QPixmap(fileNames.at(0))
			//item->setSizeHint(QSize(180, 180));
			//ui.listWidget_Iconpic->addItem(item);
		/*	QCheckBox* CBX = new QCheckBox();
			ui.listWidget_Iconpic->setItemWidget(item,CBX);*/
			//for (QString var : fileNames)
			//{
			//	QListWidgetItem* item = new QListWidgetItem();
			//	//item->setIcon(QIcon(var),"1");
			//	item->setSizeHint(QSize(160, 180));
			//	ui.listWidget_Iconpic->addItem(item);
			//	m_GraphicsView* tempview = new m_GraphicsView();
			//	ui.listWidget_Iconpic->setItemWidget(item, tempview);
			//}
		QFileDialog* fileDialog = new QFileDialog(this);
		//fileDialog->setFileMode(QFileDialog::Directory);
		fileDialog->setFileMode(QFileDialog::ExistingFiles);
		fileDialog->exec();
		auto selectDir = fileDialog->selectedFiles();
		if (selectDir.size() > 0)
		{
			qDebug() << tr("文件夹名称:") << selectDir.at(0);
		}
		///*for each (auto var in selectDir)
		//{
		//	QListWidgetItem* item = new QListWidgetItem();
		//	item->setIcon(QIcon(var));
		//	item->setSizeHint(QSize(100, 100));
		//	ui.listWidget_Iconpic->addItem(item);

		//}*/

		//IMG imgs;
		QStringList split_name;
		//std::vector<IMG> images;

		//SrcAndID src_and_id;
		std::vector <cv::String> resultPath;
		/*cv::String imgPath = cv::format("%s", (selectDir.at(0) + "/*.jpg").toLocal8Bit().constData());
		cv::glob(imgPath, resultPath, false);*/
		if (selectDir.size() != 3)
		{
			return;
		}
		ui.listWidget_Iconpic->clear();
		cv::Mat src1 = cv::imread(cv::format("%s", selectDir.at(0).toLocal8Bit().constData()));
		cv::Mat src2 = cv::imread(cv::format("%s", selectDir.at(1).toLocal8Bit().constData()));
		cv::Mat src3 = cv::imread(cv::format("%s", selectDir.at(2).toLocal8Bit().constData()));
		lineScanCam.CH0_SRC = src1;
		lineScanCam.CH1_SRC = src2;
		lineScanCam.CH2_SRC = src3;
	/*	cv::resize(src2,src2,cv::Size(512,512));
		cv::resize(src3,src3,cv::Size(512,512));*/
		QListWidgetItem* item1 = new QListWidgetItem();
		QListWidgetItem* item2 = new QListWidgetItem();
		QListWidgetItem* item3 = new QListWidgetItem();
		int w = ui.listWidget_Iconpic->width();
		m_GraphicsView* tempview1 = new m_GraphicsView(w - 40, w - 160);
		m_GraphicsView* tempview2 = new m_GraphicsView(w - 40, w - 160);
		m_GraphicsView* tempview3 = new m_GraphicsView(w - 40, w - 160);
		tempview1->setAttribute(Qt::WA_TransparentForMouseEvents);//设置当前控件只显示，不做任何消息处理
		tempview2->setAttribute(Qt::WA_TransparentForMouseEvents);
		tempview3->setAttribute(Qt::WA_TransparentForMouseEvents);
		tempview1->setObjectName("tempview1");
		tempview2->setObjectName("tempview2");
		tempview3->setObjectName("tempview3");
		//item->setIcon(QIcon(var));
		/*int w = ui.listWidget_Iconpic->width();
		int h = ui.listWidget_Iconpic->height();*/
		item1->setSizeHint(QSize(w - 40, w - 160));
		item2->setSizeHint(QSize(w - 40, w - 160));
		item3->setSizeHint(QSize(w - 40, w - 160));
		ui.listWidget_Iconpic->addItem(item1);
		ui.listWidget_Iconpic->addItem(item2);
		ui.listWidget_Iconpic->addItem(item3);

		ui.listWidget_Iconpic->setItemWidget(item1, tempview1);
		ui.listWidget_Iconpic->setItemWidget(item2, tempview2);
		ui.listWidget_Iconpic->setItemWidget(item3, tempview3);
		ui.graphicsView_mainview->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH0_SRC));
		tempview1->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH1_SRC));
		tempview2->slotShowImg(ToolClass::Mat2QImage(lineScanCam.CH2_SRC));
		ui.listWidget_Iconpic->show();
		//for each (auto var in selectDir)//resultPath
		//{
		//	//QString s = var.c_str();
		//	//split_name = s.split("\\");
		//	cv::String s = cv::format("%s", var.toLocal8Bit().constData());
		//	cv::Mat src = cv::imread(s);
		//	QImage image = ToolClass::Mat2QImage(src);
		//	
		//	//QImage temp = CAssist::Mat2QImage(src);
		//	QListWidgetItem* item11 = new QListWidgetItem();
		//	QListWidgetItem* item12 = new QListWidgetItem();
		////	//item->setIcon(QIcon(var));
		//	int w = ui.listWidget_Iconpic->width();
		//	int h = ui.listWidget_Iconpic->height();
		//	item11->setSizeHint(QSize(w-40, w-160));
		//	item12->setSizeHint(QSize(w-40, w-160));
		//	ui.listWidget_Iconpic->addItem(item11);
		//	ui.listWidget_Iconpic->addItem(item12);
		//	m_GraphicsView* tempview11 = new m_GraphicsView();
		//	m_GraphicsView* tempview12 = new m_GraphicsView();
		//	ui.listWidget_Iconpic->setItemWidget(item11, tempview11);
		//	ui.listWidget_Iconpic->setItemWidget(item12, tempview12);
		////	//cv::Mat m = ToolClass::QImage2Mat(image);
		//	QImage image2 = ToolClass::Mat2QImage(src2);
		//	QImage image3 = ToolClass::Mat2QImage(src3);
		//	tempview11->slotShowImg(image2);
		//	tempview12->slotShowImg(image3);
		//	ui.graphicsView_mainview->slotShowImg(image);

		//}
		
	}
	catch (const std::exception&)
	{
		qWarning() << "[ERROR]:Load Image Fail!";
	}
}