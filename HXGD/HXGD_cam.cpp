#include "HXGD.h"


void HXGD::InitCamSplic()
{
	
	//创建两台电脑通信的tcp客户端
	if (g_params.tcpsettings->isClient==true&& g_params.tcpsettings->isServer == false)
	{
		client = new m_TCPClient(g_params.tcpsettings->ip, g_params.tcpsettings->port);//此处需传入ip、port
		client->setObjectName("client_PC");
		client->setProperty("client_PC",0);
		p_thread->addThread(client);
		client->Init();
	}
	//创建两台电脑通信的tcp服务端
	if (g_params.tcpsettings->isClient == false && g_params.tcpsettings->isServer == true)
	{
		server = new m_TCPServer(g_params.tcpsettings->port);//此处需传入port
		server->setObjectName("server_PC");
		server->setProperty("server_PC", 0);
		p_thread->addThread(server);
	}
	if (ImageSplicShow == NULL)
	{
		ImageSplicShow = new ImageWidget();
		ui.graphicsView_splicPic->setScene(&ImageSplicShow->scene);
		ImageSplicShow->scene.addItem(ImageSplicShow);
	}
	QIntValidator* intValidator = new QIntValidator();
	intValidator->setRange(0, 16384);
	ui.LE_leftCropCam1->setValidator(intValidator);
	ui.LE_leftCropCam1->setText(QString::number(g_params.camconfig->listCamSplic[0].leftCut));
	ui.LE_rightCropCam1->setValidator(intValidator);
	ui.LE_rightCropCam1->setText(QString::number(g_params.camconfig->listCamSplic[0].rightCut));
	ui.LE_topCropCam1->setValidator(intValidator);
	ui.LE_topCropCam1->setText(QString::number(g_params.camconfig->listCamSplic[0].topCut));
	ui.LE_bottomCropCam1->setValidator(intValidator);
	ui.LE_bottomCropCam1->setText(QString::number(g_params.camconfig->listCamSplic[0].bottomCut));

	ui.LE_leftCropCam2->setValidator(intValidator);
	ui.LE_leftCropCam2->setText(QString::number(g_params.camconfig->listCamSplic[1].leftCut));
	ui.LE_rightCropCam2->setValidator(intValidator);
	ui.LE_rightCropCam2->setText(QString::number(g_params.camconfig->listCamSplic[1].rightCut));
	ui.LE_topCropCam2->setValidator(intValidator);
	ui.LE_topCropCam2->setText(QString::number(g_params.camconfig->listCamSplic[1].topCut));
	ui.LE_bottomCropCam2->setValidator(intValidator);
	ui.LE_bottomCropCam2->setText(QString::number(g_params.camconfig->listCamSplic[1].bottomCut));

	connect(ui.LE_leftCropCam1, &QLineEdit::editingFinished, [=]() {refreshImgSplic(); });
	connect(ui.LE_rightCropCam1, &QLineEdit::editingFinished, [=]() {refreshImgSplic(); });
	connect(ui.LE_topCropCam1, &QLineEdit::editingFinished, [=]() {refreshImgSplic(); });
	connect(ui.LE_bottomCropCam1, &QLineEdit::editingFinished, [=]() {refreshImgSplic(); });
	connect(ui.LE_leftCropCam2, &QLineEdit::editingFinished, [=]() {refreshImgSplic(); });
	connect(ui.LE_rightCropCam2, &QLineEdit::editingFinished, [=]() {refreshImgSplic(); });
	connect(ui.LE_topCropCam2, &QLineEdit::editingFinished, [=]() {refreshImgSplic(); });
	connect(ui.LE_bottomCropCam2, &QLineEdit::editingFinished, [=]() {refreshImgSplic(); });
}

void HXGD::refreshImgSplic()
{
	if (matImg1.data)
	{
		cv::Mat tempMat1 = matImg1(cv::Rect(ui.LE_leftCropCam1->text().toInt(), ui.LE_topCropCam1->text().toInt(),
			matImg1.cols - ui.LE_leftCropCam1->text().toInt() - ui.LE_rightCropCam1->text().toInt(),
			matImg1.rows - ui.LE_topCropCam1->text().toInt() - ui.LE_bottomCropCam1->text().toInt()));
		if (matImg2.data)
		{
			cv::Mat tempMat2 = matImg2(cv::Rect(ui.LE_leftCropCam2->text().toInt(), ui.LE_topCropCam2->text().toInt(),
				matImg2.cols - ui.LE_leftCropCam2->text().toInt() - ui.LE_rightCropCam2->text().toInt(),
				matImg2.rows - ui.LE_topCropCam2->text().toInt() - ui.LE_bottomCropCam2->text().toInt()));
			cv::resize(tempMat2, tempMat2, cv::Size(tempMat2.cols, tempMat1.rows));

			cv::Mat SplicMat = cv::Mat::zeros(cv::Size(tempMat1.cols+tempMat2.cols+1,tempMat1.rows),CV_8UC3);
			cv::Mat BlueMat = cv::Mat(cv::Size(1,tempMat1.rows),CV_8UC3,cv::Scalar(0,0,255));
			tempMat1.copyTo(SplicMat(cv::Rect(0,0,tempMat1.cols,tempMat1.rows)));
			BlueMat.copyTo(SplicMat(cv::Rect(tempMat1.cols, 0, 1, tempMat1.rows)));
			tempMat2.copyTo(SplicMat(cv::Rect(tempMat1.cols+1,0,tempMat2.cols,tempMat2.rows)));
			showSplicImg(ToolClass::Mat2QImage(SplicMat));
		}
		else
		{
			showSplicImg(ToolClass::Mat2QImage(tempMat1));
		}
	}
	else if (matImg2.data)
	{
		cv::Mat tempMat2 = matImg2(cv::Rect(ui.LE_leftCropCam2->text().toInt(),ui.LE_topCropCam2->text().toInt(),
			matImg2.cols-ui.LE_leftCropCam2->text().toInt()-ui.LE_rightCropCam2->text().toInt(),
			matImg2.rows-ui.LE_topCropCam1->text().toInt()-ui.LE_bottomCropCam1->text().toInt()));
		showSplicImg(ToolClass::Mat2QImage(tempMat2));
	}
}

void HXGD::showSplicImg(QImage img)
{
	QPixmap ConvertPixmap = QPixmap::fromImage(img);
	int nWidth = ui.graphicsView_splicPic->width();
	int nHeight= ui.graphicsView_splicPic->height();
	ImageSplicShow->reloadImage(&ConvertPixmap,nWidth,nHeight);
}

void HXGD::leftCamDebugLoadData()
{
	ui.LE_SavePicPath->setText(g_params.camconfig->camsettings.saveImagePath);
	ui.cb_savePic->setChecked(g_params.camconfig->camsettings.saveImage);
	ui.cb_saveSplic->setChecked(g_params.camconfig->camsettings.bSaveOneImage);
	ui.tableWidget_SplicParam->setRowCount(g_params.camconfig->camsettings.cameraCount);
	for (size_t i = 0; i < g_params.camconfig->camsettings.cameraCount; i++)
	{
		ui.tableWidget_SplicParam->setItem(i,0,new QTableWidgetItem(QString::number(g_params.camconfig->listCamSplic[i].leftCut)));
		ui.tableWidget_SplicParam->setItem(i,1,new QTableWidgetItem(QString::number(g_params.camconfig->listCamSplic[i].rightCut)));
		ui.tableWidget_SplicParam->setItem(i,2,new QTableWidgetItem(QString::number(g_params.camconfig->listCamSplic[i].topCut)));
		ui.tableWidget_SplicParam->setItem(i,3,new QTableWidgetItem(QString::number(g_params.camconfig->listCamSplic[i].bottomCut)));
		ui.tableWidget_SplicParam->setItem(i,4,new QTableWidgetItem(QString::number(g_params.camconfig->listCamSplic[i].overlapNumActual)));
		ui.tableWidget_SplicParam->setItem(i,5,new QTableWidgetItem(QString::number(g_params.camconfig->listCamSplic[i].overlapSet)));
	}
}

void HXGD::on_pb_loadCam1_clicked()
{
	QString path = QFileDialog::getOpenFileName(NULL,tr("Open Image"),"D:/",tr("Image Files(*.jpg *.bmp)"));
	if (path.length() == 0) return;
	if (path.isEmpty()) return;

	try
	{
		matImg1 = cv::imread(path.toLocal8Bit().toStdString(), cv::IMREAD_COLOR);
	}
	catch (const std::exception&)
	{

	}
	refreshImgSplic();
}

void HXGD::on_pb_loadCam2_clicked()
{
	QString path = QFileDialog::getOpenFileName(NULL,tr("Open Image"),"D:/",tr("Image File(*.jpg *.bmp)"));
	if (path.length() == 0) return;
	if (path.isEmpty()) return;

	try
	{
		matImg2 = cv::imread(path.toLocal8Bit().toStdString(),cv::IMREAD_COLOR);
	}
	catch (const std::exception&)
	{

	}
	refreshImgSplic();
}

void HXGD::on_pb_saveSplicParam_clicked()
{
	QFile file(g_params.iniCamConfigPath);
	if (!file.exists())
	{
		QMessageBox::warning(this,"错误","无法找到相机参数文件！");
		return;
	}
	g_params.camconfig->listCamSplic[0].topCut = ui.LE_topCropCam1->text().toInt();
	g_params.camconfig->listCamSplic[0].bottomCut = ui.LE_bottomCropCam1->text().toInt();
	g_params.camconfig->listCamSplic[0].leftCut = ui.LE_leftCropCam1->text().toInt();
	g_params.camconfig->listCamSplic[0].rightCut = ui.LE_rightCropCam1->text().toInt();
	g_params.camconfig->listCamSplic[1].topCut = ui.LE_topCropCam2->text().toInt();
	g_params.camconfig->listCamSplic[1].bottomCut = ui.LE_bottomCropCam2->text().toInt();
	g_params.camconfig->listCamSplic[1].leftCut = ui.LE_leftCropCam2->text().toInt();
	g_params.camconfig->listCamSplic[1].rightCut = ui.LE_rightCropCam2->text().toInt();
	g_params.SaveDataToCamConfig(*g_params.camconfig);
	QMessageBox::information(this,"保存","相机参数文件保存完成！");
	//修改后更新界面数据
	leftCamDebugLoadData();
}
