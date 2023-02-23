#pragma once
#pragma execution_character_set("utf-8")
#include "ui_Login.h"
#include <QMessageBox>
#include <QtXml>

#define USER_ADMIN_LEVEL 100
#define USER_MANAGE_LEVEL 10
#define USER_OPATER_LEVEL 1

enum LoginMode
{
	Login_Edit,
	Login_Add
};

class Login :public QWidget
{
	Q_OBJECT
public:
	Login(QWidget *parent = Q_NULLPTR) : QWidget(parent)
	{
		ui.setupUi(this);
		this->setWindowIcon(QIcon(":/MainWindow/resouse/icon.png"));
		this->setAutoFillBackground(true);
		QPalette palette = this->palette();
		/*palette.setBrush(QPalette::Window,QBrush(QPixmap(":/MainWindow/resouse/background.png").
			scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));*/
		this->setPalette(palette);
		//ui.MainWidget->setStyleSheet("background-image: url(:/MainWindow/resouse/background.png)");
		//this->setStyleSheet("background-image: url(:/MainWindow/resouse/background.png)");
		ui.LineEd_UserPassword->setEchoMode(QLineEdit::Password);
	}
	~Login() {};

	void setStackedWidgetIndex(int index) 
	{
		ui.stackedWidget->setCurrentIndex(index);
	}

	void setUserEditInfo(QString username,QString password,int userLevel) 
	{
		ui.LineEd_UserNameEdit->setText(username);
		ui.LineEd_UserPasswordEdit->setText(password);
		ui.LineEd_UserPasswordEditC->setText(password);
		if (userLevel == USER_ADMIN_LEVEL)
		{
			ui.CBox_UserLevelEdit->setCurrentIndex(0);
		}else if(userLevel == USER_MANAGE_LEVEL)
		{
			ui.CBox_UserLevelEdit->setCurrentIndex(1);
		}
		else {
			ui.CBox_UserLevelEdit->setCurrentIndex(2);
		}
	}
	void setLoginMode(int mode) 
	{
		this->iLoginMode = mode;
	}
public slots:
	void on_Btn_Login_clicked()
	{
		QString strPermission = ui.cBx_permission->currentText();
		QString strUserName = ui.LineEd_UserName->text();
		QString strPassword = ui.LineEd_UserPassword->text();

		if (strUserName == "")
		{
			QMessageBox::warning(this, "提示", "用户名不能为空");
		}
		else if (strPassword == "")
		{
			QMessageBox::warning(this, "提示", "密码不能为空");
		}
		else {

			//数据库下查询用户名和用户密码
			emit sigLogin(strPermission,strUserName, strPassword);

			////Recipe下参数读取
			//QString tempstr = QString("%1/data/User.xml").arg(QApplication::applicationDirPath());
			//QFile file(tempstr);  //关联文件名字
		 //  //只读方式打开文件
			//bool isOK = file.open(QIODevice::ReadOnly);

			////文件file和xml关联
			//QDomDocument doc;
			//isOK = doc.setContent(&file);
			//if (false == isOK) {

			//}
			////关联成功后，可以关闭文件
			//file.close();
			////获取根节点
			//QDomElement root = doc.documentElement();
			//if (root.hasChildNodes()) {  //有没有子节点
			//	QDomNode node1 = root.firstChild();
			//	while (!node1.isNull())
			//	{
			//		if (node1.nodeName() == "UserList")
			//		{
			//			QDomNode node2 = node1.firstChild();
			//			while (!node2.isNull())
			//			{
			//				if (node2.nodeName() == "User")
			//				{
			//					if (node2.toElement().attribute("name") == strUserName )
			//					{
			//						if (node2.toElement().attribute("password") == strPassword)
			//						{
			//							emit sigLogin(strUserName);
			//							QMessageBox::information(this, "提示", "登录成功");
			//						}
			//						else {
			//						
			//							QMessageBox::warning(this, "提示", "密码错误！");
			//						}
			//						
			//					}

			//				}
			//				node2 = node2.nextSibling();

			//			}

			//		}
			//		node1 = node1.nextSibling();
			//	}

			//}
		}
	}

	void on_Btn_Cancel_clicked()
	{
		emit sigLogin("", "","");
		this->hide();
		//this->close();
	}

	void slotLogStatus(bool  bLogin)
	{
		if (bLogin)
		{
			QMessageBox::information(this, "提示", "登录成功");
			ui.LineEd_UserName->clear();
			ui.LineEd_UserPassword->clear();
			this->hide();
			this->close();
		}
		else
		{
			ui.LineEd_UserPassword->clear();
			QMessageBox::warning(this, "提示", "用户名或密码错误！");
		}
	}

	void on_Btn_LoginEdit_clicked()
	{
		
		//增加用户修改用户
		QString strUserName = ui.LineEd_UserNameEdit->text();
		QString strPassword = ui.LineEd_UserPasswordEdit->text();
		QString strPasswordC = ui.LineEd_UserPasswordEditC->text();
		int level = 1;
		if (ui.CBox_UserLevelEdit->currentIndex() == 0)
		{
			level = 100;
		}else if (ui.CBox_UserLevelEdit->currentIndex() == 1)
		{
			level = 10;
		}
		else
		{
			level = 1;
		}
			
		if (strUserName == "")
		{
			QMessageBox::warning(this, "提示", "用户名不能为空");
		}
		else if (strPassword == "" || strPasswordC == "")
		{
			QMessageBox::warning(this, "提示", "密码不能为空");
		}
		else if (strPassword != strPasswordC)
		{
			QMessageBox::warning(this, "提示", "两次输入密码不一致");
		}
		else {
			emit sigLoginEditAdd(iLoginMode, strUserName, strPassword, level);
		}
		
	}
	void on_Btn_CancelEdit_clicked() 
	{
		ui.LineEd_UserNameEdit->setText("");
		ui.LineEd_UserPasswordEdit->setText("");
		ui.LineEd_UserPasswordEditC->setText("");
		ui.CBox_UserLevelEdit->setCurrentIndex(0);
		this->hide();
	};
signals:
	void sigLogin(QString, QString,QString);
	void sigLoginEditAdd(int mode, QString username, QString password, int level);
private:
	Ui::Form_Login ui;
	int iLoginMode = Login_Edit;
};