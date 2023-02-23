#include "Params.h"
#pragma execution_character_set("utf-8")
CParams g_params;

CParams::CParams()
{
	
}
void CParams::SetIniPathDir(QString& path) 
{
	iniCamConfigPath = QString("%1/system/%2").arg(path).arg("cameraConfig.ini");

	iniSystemParaPath = QString("%1/system/%2").arg(path).arg("SystemParam.ini");

	//iniDBParamPath = QString("%1/system/%2").arg(path).arg("dbParam.ini");

	//iniResultSendPath = QString("%1/system/%2").arg(path).arg("ResultSendPlug.ini");

	//iniFtpParaPath = QString("%1/system/%2").arg(path).arg("ResultUpLoadData");

	iniTcpSettingsPath = QString("%1/system/%2").arg(path).arg("tcpConfig.ini");
}


CParams::~CParams()
{
}



QString CParams::GetValueKey(const QString& filename, const QString& modelname, const QString& itemname)
{
	QSettings inireader(filename, QSettings::IniFormat);

	QString strTemp;
	strTemp = inireader.value(QString("/%1/%2").arg(modelname).arg(itemname)).toString();
	return strTemp;
}
void CParams::SetValueKey(const QString& filename, const QString& modelname, const QString& itemname, const QString& itemvalue)
{
	QSettings inireader(filename, QSettings::IniFormat);

	inireader.beginGroup(modelname);
	inireader.setValue(itemname, itemvalue);
	inireader.endGroup();
}

bool CParams::LoadData()
{


	return true;
}

void CParams::SaveIniData()
{
	QString filePath = QString("./data/FrameIns.ini");

	QString filePathBk3 = QString(filePath).append(".bak3");
	QString filePathBk2 = QString(filePath).append(".bak2");
	QString filePathBk1 = QString(filePath).append(".bak1");

	QFile iniFile(filePath);
	QFile iniFileBk1(filePathBk1);
	QFile iniFileBk2(filePathBk2);
	QFile iniFileBk3(filePathBk3);

	if (iniFileBk1.exists())
	{
		if (iniFileBk2.exists())
		{
			if (iniFileBk3.exists())
				iniFileBk3.remove();
			QFile::copy(filePathBk2, filePathBk3);

			iniFileBk2.remove();
			QFile::copy(filePathBk1, filePathBk2);

			iniFileBk1.remove();
			QFile::copy(filePath, filePathBk1);
		}
		else {
			QFile::copy(filePathBk1, filePathBk2);
			iniFileBk1.remove();
			QFile::copy(filePath, filePathBk1);
		}
	}
	else {
		QFile::copy(filePath, filePathBk1);
	}

	QSettings iniWriter(filePath, QSettings::IniFormat);
	iniWriter.setIniCodec("UTF-8");
	

	/*iniWriter.beginGroup("ALG");
	for (int i = 0; i < Alg_Total_Num; i++)
	{
		iniWriter.setValue(TFCore::GetParamStr((ALG_PARAMS)i), QString::number(m_dAlgParams[i]));
	}
	iniWriter.endGroup();*/

	iniWriter.beginGroup("UI");
	//iniWriter.setValue(this->getUIParaStr(UI_Para_RecipeMode), QVariant(QString(m_strRecipe)));
	//iniWriter.setValue(this->getUIParaStr(UI_Para_PLCIP), QVariant(QString(m_strRecipe)));
	//iniWriter.setValue(this->getUIParaStr(UI_Para_RecipeMode), QString::number(m_dUIParams[UI_Para_RecipeMode]));
	iniWriter.endGroup();

}

void CParams::LoadDataFromFile()
{
	LoadDataFromCamConfig(this->camconfig);
	LoadDataFromSystemParam(this->systempara);
	LoadDataFromDBParam(this->dbparam);
	LoadDataFromResultSend(this->resultSend);
	LoadDataFromFtpParam(this->ftppara);
	LoadDataFromTcpConfig(this->tcpsettings);
}
void CParams::SaveDataToFile(QString path, void** param) 
{
	
}

void CParams::LoadDataFromCamConfig(CamConfig * camconfig1)
{
	QSettings inireader(iniCamConfigPath, QSettings::IniFormat);
	inireader.setIniCodec("UTF-8");
	inireader.beginGroup("CamSettings");

	camconfig1->camsettings.bufferCount = inireader.value("bufferCount").toInt();
	camconfig1->camsettings.doesSplit = inireader.value("doesSplit").toInt();
	camconfig1->camsettings.serviceIndexMax = inireader.value("serviceIndexMax").toInt();
	camconfig1->camsettings.cameraCount = inireader.value("cameraCount").toInt();
	camconfig1->camsettings.variableHeight = inireader.value("variableHeight").toInt();
	camconfig1->camsettings.saveImage = inireader.value("saveImage").toString() == "true";
	camconfig1->camsettings.savqImagePath = inireader.value("savqImagePath").toString();
	camconfig1->camsettings.autoDelete = inireader.value("autoDelete").toString() == "true";
	camconfig1->camsettings.autoDeleteTimeSpan = inireader.value("autoDeleteTimeSpan").toInt();
	camconfig1->camsettings.saveImagePath = inireader.value("saveImagePath").toString();
	camconfig1->camsettings.bSaveOneImage = inireader.value("bSaveOneImage").toString() == "true";
	camconfig1->camsettings.saveCompressionRatio = inireader.value("saveCompressionRatio").toInt();


	inireader.endGroup();

	camconfig1->listCamera.clear();
	camconfig1->listxtiumCLserial.clear();
	camconfig1->listCamSplic.clear();

	for (int i = 0; i < camconfig->camsettings.cameraCount; i++)
	{
		inireader.beginGroup(QString("Camera_%1").arg(i));

		Camera camera;
		camera.cameraName = inireader.value("cameraName").toString();
		camera.camIndex = inireader.value("camIndex").toInt();
		camera.camType = inireader.value("camType").toInt();
		camera.scanMode = inireader.value("scanMode").toInt();
		camera.configFile = inireader.value("configFile").toString();
		camera.imgCount = inireader.value("imgCount").toInt();
		camera.camDescription = inireader.value("camDescription").toString();
		camera.userID = inireader.value("userID").toString();
		camera.mode = inireader.value("mode").toInt();
		camera.serialPort = inireader.value("serialPort").toInt();
		camera.station = inireader.value("station").toInt();

		camconfig1->listCamera.append(camera);
		inireader.endGroup();

		inireader.beginGroup(QString("Xtium-CL_MX4_%1_Serial_0_Img0").arg(i+1));

		XtiumCLSerial xclSerial;
		xclSerial.camIndex = inireader.value("camIndex").toInt();
		xclSerial.numInCam = inireader.value("numInCam").toInt();
		xclSerial.description = inireader.value("description").toInt();

		camconfig1->listxtiumCLserial.append(xclSerial);
		inireader.endGroup();

		inireader.beginGroup(QString("Cam_%1_Splic").arg(i+1));

		camsplic camsplic;
		camsplic.topCut = inireader.value("topCut").toInt();
		camsplic.bottomCut = inireader.value("bottomCut").toInt();
		camsplic.leftCut = inireader.value("leftCut").toInt();
		camsplic.rightCut = inireader.value("rightCut").toInt();
		camsplic.overlapNumActual = inireader.value("overlapNumActual").toInt();
		camsplic.overlapSet = inireader.value("overlapSet").toInt();

		camconfig1->listCamSplic.append(camsplic);
		inireader.endGroup();
	}
}

void CParams::SaveDataToCamConfig(CamConfig & camconfig1)
{
	QString filePathBk3 = QString(iniCamConfigPath).append(".bak3");
	QString filePathBk2 = QString(iniCamConfigPath).append(".bak2");
	QString filePathBk1 = QString(iniCamConfigPath).append(".bak1");

	QFile iniFile(iniCamConfigPath);
	QFile iniFileBk1(filePathBk1);
	QFile iniFileBk2(filePathBk2);
	QFile iniFileBk3(filePathBk3);

	if (iniFileBk1.exists())
	{
		if (iniFileBk2.exists())
		{
			if (iniFileBk3.exists())
				iniFileBk3.remove();
			QFile::copy(filePathBk2, filePathBk3);

			iniFileBk2.remove();
			QFile::copy(filePathBk1, filePathBk2);

			iniFileBk1.remove();
			QFile::copy(iniCamConfigPath, filePathBk1);
		}
		else {
			QFile::copy(filePathBk1, filePathBk2);
			iniFileBk1.remove();
			QFile::copy(iniCamConfigPath, filePathBk1);
		}
	}
	else {
		QFile::copy(iniCamConfigPath, filePathBk1);
	}

	QSettings iniWriter(iniCamConfigPath, QSettings::IniFormat);
	iniWriter.setIniCodec("UTF-8");

	iniWriter.beginGroup("ALG");
	iniWriter.setValue("bufferCount", QString::number(camconfig->camsettings.bufferCount));
	iniWriter.setValue("doesSplit", QString::number(camconfig->camsettings.doesSplit));
	iniWriter.setValue("serviceIndexMax", QString::number(camconfig->camsettings.serviceIndexMax));
	iniWriter.setValue("cameraCount", QString::number(camconfig->camsettings.cameraCount));
	iniWriter.setValue("variableHeight", QString::number(camconfig->camsettings.variableHeight));
	iniWriter.setValue("saveImage", QString(camconfig->camsettings.saveImage?"true":"false"));
	iniWriter.setValue("savqImagePath", QString(camconfig->camsettings.savqImagePath));
	iniWriter.setValue("autoDelete", QString(camconfig->camsettings.autoDelete ? "true" : "false"));
	iniWriter.setValue("autoDeleteTimeSpan", QString::number(camconfig->camsettings.autoDeleteTimeSpan));
	iniWriter.setValue("saveImagePath", QString(camconfig->camsettings.saveImagePath));
	iniWriter.setValue("bSaveOneImage", QString(camconfig->camsettings.bSaveOneImage ? "true" : "false"));
	iniWriter.setValue("saveCompressionRatio", QString::number(camconfig->camsettings.saveCompressionRatio));
	iniWriter.endGroup();

	for (int i = 0; i < camconfig->camsettings.cameraCount; i++)
	{
		iniWriter.beginGroup(QString("Camera_%1").arg(i));

		iniWriter.setValue("cameraName", QString(camconfig->listCamera[i].cameraName));
		iniWriter.setValue("camIndex", QString::number(camconfig->listCamera[i].camIndex));
		iniWriter.setValue("camType", QString::number(camconfig->listCamera[i].camType));
		iniWriter.setValue("scanMode", QString::number(camconfig->listCamera[i].scanMode));
		iniWriter.setValue("configFile", QString(camconfig->listCamera[i].configFile));
		iniWriter.setValue("imgCount", QString::number(camconfig->listCamera[i].imgCount));
		iniWriter.setValue("camDescription", QString(camconfig->listCamera[i].camDescription));
		iniWriter.setValue("userID", QString(camconfig->listCamera[i].userID));
		iniWriter.setValue("mode", QString::number(camconfig->listCamera[i].mode));
		iniWriter.setValue("serialPort", QString::number(camconfig->listCamera[i].serialPort));
		iniWriter.setValue("station", QString::number(camconfig->listCamera[i].station));

		iniWriter.endGroup();

		iniWriter.beginGroup(QString("Xtium-CL_MX4_%1_Serial_0_Img0").arg(i + 1));

		iniWriter.setValue("camIndex", QString::number(camconfig->listxtiumCLserial[i].camIndex));
		iniWriter.setValue("numInCam", QString::number(camconfig->listxtiumCLserial[i].numInCam));
		iniWriter.setValue("description", camconfig->listxtiumCLserial[i].description);

		iniWriter.endGroup();

		iniWriter.beginGroup(QString("Cam_%1_Splic").arg(i + 1));

		iniWriter.setValue("topCut", QString::number(camconfig->listCamSplic[i].topCut));
		iniWriter.setValue("bottomCut", QString::number(camconfig->listCamSplic[i].bottomCut));
		iniWriter.setValue("leftCut", QString::number(camconfig->listCamSplic[i].leftCut));
		iniWriter.setValue("rightCut", QString::number(camconfig->listCamSplic[i].rightCut));
		iniWriter.setValue("overlapNumActual", QString::number(camconfig->listCamSplic[i].overlapNumActual));
		iniWriter.setValue("overlapSet", QString::number(camconfig->listCamSplic[i].overlapSet));

		iniWriter.endGroup();
	}

}

void CParams::LoadDataFromSystemParam(SystemParam * systempara1)
{
	QSettings inireader(iniSystemParaPath, QSettings::IniFormat);
	inireader.setIniCodec("UTF-8");
	inireader.beginGroup("SystemParam");
	systempara1->CurrentRecipeID = inireader.value("CurrentRecipe").toInt();
	systempara1->ProductId = inireader.value("ProductId").toString();
	systempara1->RecipeType = inireader.value("RecipeType").toString();
	systempara1->ProcessId = inireader.value("ProcessId").toString();
	systempara1->gray_black = inireader.value("gray_black").toInt();
	systempara1->gray_white = inireader.value("gray_white").toInt();
	systempara1->gray_gamma = inireader.value("gray_gamma").toInt();

	inireader.endGroup();
}

void CParams::SaveDataToSystemParam(SystemParam & systempara)
{
	QString filePathBk3 = QString(iniSystemParaPath).append(".bak3");
	QString filePathBk2 = QString(iniSystemParaPath).append(".bak2");
	QString filePathBk1 = QString(iniSystemParaPath).append(".bak1");

	QFile iniFile(iniSystemParaPath);
	QFile iniFileBk1(filePathBk1);
	QFile iniFileBk2(filePathBk2);
	QFile iniFileBk3(filePathBk3);

	if (iniFileBk1.exists())
	{
		if (iniFileBk2.exists())
		{
			if (iniFileBk3.exists())
				iniFileBk3.remove();
			QFile::copy(filePathBk2, filePathBk3);

			iniFileBk2.remove();
			QFile::copy(filePathBk1, filePathBk2);

			iniFileBk1.remove();
			QFile::copy(iniSystemParaPath, filePathBk1);
		}
		else {
			QFile::copy(filePathBk1, filePathBk2);
			iniFileBk1.remove();
			QFile::copy(iniSystemParaPath, filePathBk1);
		}
	}
	else {
		QFile::copy(iniSystemParaPath, filePathBk1);
	}

	QSettings iniWriter(iniSystemParaPath, QSettings::IniFormat);
	iniWriter.setIniCodec("UTF-8");

	iniWriter.beginGroup("SystemParam");
	iniWriter.setValue("CurrentRecipe", QString::number(systempara.CurrentRecipeID));
	iniWriter.setValue("ProductId", QString(systempara.ProductId));
	iniWriter.setValue("RecipeType", QString(systempara.RecipeType));
	iniWriter.setValue("ProcessId", QString(systempara.ProcessId));
	iniWriter.setValue("gray_black", QString::number(systempara.gray_black));
	iniWriter.setValue("gray_white", QString::number(systempara.gray_white));
	iniWriter.setValue("gray_gamma", QString::number(systempara.gray_gamma));
	iniWriter.endGroup();
}

void CParams::LoadDataFromDBParam(DBParam * dbparam1)
{
	QSettings inireader(iniDBParamPath, QSettings::IniFormat);
	inireader.setIniCodec("UTF-8");
	inireader.beginGroup("General");
	dbparam1->type = inireader.value("type").toInt();
	inireader.endGroup();

	inireader.beginGroup("mysql");
	dbparam1->host = inireader.value("host").toString();
	dbparam1->port = inireader.value("port").toInt();
	dbparam1->dbName = inireader.value("dbName").toString();
	dbparam1->userName = inireader.value("userName").toString();
	dbparam1->password = inireader.value("password").toString();
	inireader.endGroup();
}

void CParams::SaveDataToDBParam(DBParam & dbparam)
{
}

void CParams::LoadDataFromResultSend(ResultSend * resultSend1)
{
	QSettings inireader(iniResultSendPath, QSettings::IniFormat);
	inireader.setIniCodec("UTF-8");
	inireader.beginGroup("General");
	resultSend1->address = inireader.value("address").toString();
	resultSend1->port = inireader.value("port").toInt();
	resultSend1->PCCommonAddress = inireader.value("PCCommonAddress").toInt();
	resultSend1->PLCCommonAddress = inireader.value("PLCCommonAddress").toInt();
	resultSend1->PLCJobDataAddress = inireader.value("PLCJobDataAddress").toInt();
	resultSend1->PCJobDataAddress = inireader.value("PCJobDataAddress").toInt();
	resultSend1->PLCLocalRecipeIdAddress = inireader.value("PLCLocalRecipeIdAddress").toInt();
	resultSend1->PCLocalRecipeIdAddress = inireader.value("PCLocalRecipeIdAddress").toInt();
	resultSend1->PCRecipeListDataAddress = inireader.value("PCRecipeListDataAddress").toInt();
	resultSend1->PCRecipeStateAddress = inireader.value("PCRecipeStateAddress").toInt();
	resultSend1->PCAlarmStatusAddress = inireader.value("PCAlarmStatusAddress").toInt();
	resultSend1->PCRecipeIdBingStateAddress = inireader.value("PCRecipeIdBingStateAddress").toInt();
	resultSend1->PCPRUNDelayAddress = inireader.value("PCPRUNDelayAddress").toInt();
	inireader.endGroup();
	inireader.beginGroup("Result");
	resultSend1->isAutoChangedRecipe = inireader.value("isAutoChangedRecipe").toString()=="true";
	inireader.endGroup();
}

void CParams::SaveDataToResultSend(ResultSend & resultSend)
{

}

void CParams::LoadDataFromFtpParam(FtpParam * ftppara1)
{
	QSettings inireader(iniFtpParaPath, QSettings::IniFormat);
	inireader.setIniCodec("UTF-8");
	inireader.beginGroup("FtpParam");
	ftppara1->address = inireader.value("address").toString();
	ftppara1->port = inireader.value("port").toInt();
	ftppara1->username = inireader.value("username").toString();
	ftppara1->password = inireader.value("password").toString();
	ftppara1->localsavepath = inireader.value("localsavepath").toString();
	ftppara1->uploadpath = inireader.value("uploadpathS").toString();
	inireader.endGroup();
}

void CParams::SaveDataToFtpParam(FtpParam & ftppara)
{

}

void CParams::LoadDataFromTcpConfig(TcpSettings* tcpsettings)
{
	QSettings inireader(iniTcpSettingsPath, QSettings::IniFormat);
	inireader.setIniCodec("UTF-8");
	inireader.beginGroup("TcpSettings");
	tcpsettings->isClient = inireader.value("isClient").toBool();
	tcpsettings->isServer= inireader.value("isServer").toBool();
	tcpsettings->ip = inireader.value("ipOfServer").toString();
	tcpsettings->port = inireader.value("portOfServer").toInt();
	inireader.endGroup();
}

void CParams::SaveDataToTcpSettings(TcpSettings& tcpsettings)
{

}
