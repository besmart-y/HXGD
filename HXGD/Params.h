#pragma once
#include <QApplication>
#include <QString>
#include <QSettings>
#include <QFile>
#include <QVariant>
#include <QDomDocument>
#include <QTextStream>

struct CamSettings
{
	int bufferCount=1;
	int doesSplit=0;
	int serviceIndexMax = 10;
	int cameraCount = 4;
	int	variableHeight = 0;
	bool	saveImage = false;
	QString	savqImagePath = "D:/testSrc/";
	bool	autoDelete = false;
	int	autoDeleteTimeSpan = 30;
	QString	saveImagePath = "E:/testImage";
	bool	bSaveOneImage = true;
	int	saveCompressionRatio = 10;
	
};

struct Camera {
	QString cameraName = "Xtium-CL_MX4_1_Serial_0";
	int	camIndex = 1;
	int	camType = 0;
	int	scanMode = 0;
	QString	configFile = "D:/S1_CCF/CCD11.ccf";
	int imgCount = 1;
	QString	camDescription = "S1";
	QString	userID = "H1894405";
	int	mode = 1;
	int	serialPort = 0;
	int	station = 1;

};

struct XtiumCLSerial
{
	int camIndex = 1;
	int	numInCam = 1;
	QString	description = "第一个相机第一幅图";
};

struct camsplic
{
	int topCut = 0;
	int	bottomCut = 0;
	int	leftCut = 9;
	int	rightCut = 0;
	int	overlapNumActual = 250;
	int	overlapSet = 20;
};

struct CamConfig
{
	CamSettings camsettings;
	QList<Camera> listCamera;
	QList<XtiumCLSerial> listxtiumCLserial;
	QList<camsplic> listCamSplic;
};

struct SystemParam
{
	int CurrentRecipeID = 0;
	QString ProductId = "FB3201XAB";
	QString	RecipeType = "CF";
	QString	ProcessId = "32INCH";
	int gray_black = 121;
	int	gray_white = 165;
	int gray_gamma = 5;
};

struct DBParam
{
	int type = 1;
	QString host = "127.0.0.1";
	int	port = 3306;
	QString	dbName = "mura";
	QString	userName = "root";
	QString password = "root";
};

struct ResultSend
{
	QString address = "192.168.100.100";
	int	port = 3000;
	int	PCCommonAddress = 12000;
	int	PLCCommonAddress = 10000;
	int	PLCJobDataAddress = 10100;
	int	PCJobDataAddress = 12100;
	int	PLCLocalRecipeIdAddress = 10196;
	int	PCLocalRecipeIdAddress = 12196;
	int	PCRecipeListDataAddress = 12198;
	int	PCRecipeStateAddress = 12261;
	int	PCAlarmStatusAddress = 12012;
	int	PCRecipeIdBingStateAddress = 12774;
	int	PCPRUNDelayAddress = 0;
	bool isAutoChangedRecipe = false;
};

struct FtpParam
{
	QString address = "192.168.3.101";
	int	port = 21;
	QString	username = "test";
	QString	password = "123456";
	QString	localsavepath = "C:/Users/Administrator/Desktop/Ftptest/";
	QString	uploadpath = "/";
};

struct TcpSettings
{
	bool isClient = false;
	bool isServer = false;
	QString ip = "";
	qint16 port = 0;
};

class CParams
{
public:
	CParams();
	~CParams();

	void SetIniPathDir(QString& path);
	QSettings iniSetting;

	QString iniCamConfigPath;
	CamConfig *camconfig = new CamConfig;
	void LoadDataFromCamConfig(CamConfig* camconfig1);
	void SaveDataToCamConfig(CamConfig& camconfig1);

	QString iniSystemParaPath;
	SystemParam* systempara = new SystemParam;
	void LoadDataFromSystemParam(SystemParam* systempara1);
	void SaveDataToSystemParam(SystemParam& systempara1);

	QString iniDBParamPath;
	DBParam* dbparam = new DBParam;
	void LoadDataFromDBParam(DBParam* dbparam1);
	void SaveDataToDBParam(DBParam& dbparam1);

	QString iniResultSendPath;
	ResultSend* resultSend = new ResultSend;
	void LoadDataFromResultSend(ResultSend* resultSend1);
	void SaveDataToResultSend(ResultSend& resultSend1);

	QString iniFtpParaPath;
	FtpParam* ftppara = new FtpParam;
	void LoadDataFromFtpParam(FtpParam* ftppara1);
	void SaveDataToFtpParam(FtpParam& ftppara1);

	QString iniTcpSettingsPath;
	TcpSettings* tcpsettings= new TcpSettings;
	void LoadDataFromTcpConfig(TcpSettings* tcpsettings);
	void SaveDataToTcpSettings(TcpSettings& tcpsettings);


	bool LoadData();
	void SaveIniData();
	void LoadDataFromFile();
	void SaveDataToFile(QString path, void** param);

	QString GetValueKey(const QString& filename, const QString& modelname, const QString& itemname);
	void SetValueKey(const QString& filename, const QString& modelname, const QString& itemname, const QString& itemvalue);

};


extern CParams g_params;