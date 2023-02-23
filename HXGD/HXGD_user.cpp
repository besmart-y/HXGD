#include "HXGD.h"

void HXGD::InitUserUI()
{
	//ui.tableWidget_userInfo
		//表格设置自适应宽度
		ui.tableWidget_userInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_userInfo->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//设置选择行为时每次选择一行
	ui.tableWidget_userInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	//设置不可编辑
	ui.tableWidget_userInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置选中背景色
	ui.tableWidget_userInfo->setStyleSheet("selection-background-color:lightblue;");
	//设置表头背景色
	ui.tableWidget_userInfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	ui.tableWidget_userInfo->setContextMenuPolicy(Qt::CustomContextMenu);


	connect(this, &HXGD::signalLogStatus, login, &Login::slotLogStatus);
	connect(login, &Login::sigLogin, this, &HXGD::slotLogin);
	connect(login,&Login::sigLoginEditAdd,this,&HXGD::slotEditAddUser);
	
	connect(ui.tableWidget_userInfo,&QTableWidget::currentCellChanged,this,&HXGD::slotUserInfoCurrentCellChanged);
	db = QSqlDatabase::addDatabase("QSQLITE");
	QString str = QApplication::applicationDirPath();
	db.setDatabaseName(QString("%1/Database/HXGD.db").arg(str));
	if (db.open())
	{
		LoadUserInfoFromDB();
		db.close();
	}
	else
	{
		qDebug() << db.lastError().text();
	}

}

void HXGD::LoadUserInfoFromDB()
{
	if (db.isOpen()||db.open())
	{
		QSqlQuery query("SELECT level,permit,userName FROM UserManager;", db);
		int rowid = 0;
		if (query.exec())
		{
			while (query.next())
			{
				ui.tableWidget_userInfo->setRowCount(rowid+1);
				ui.tableWidget_userInfo->setItem(rowid, 0,new QTableWidgetItem(query.value(0).toString()));//用户ID
				ui.tableWidget_userInfo->setItem(rowid, 1,new QTableWidgetItem(query.value(2).toString()));//用户名
				ui.tableWidget_userInfo->setItem(rowid, 2,new QTableWidgetItem(query.value(1).toString()));//用户权限
				++rowid;
			}
		}
	}
	
}

void HXGD::on_pb_login_clicked()
{
	login->setStackedWidgetIndex(0);
	login->show();
}

void HXGD::on_pb_addUser_clicked()
{
	if(loginLevel <USER_ADMIN_LEVEL)
	{
		QMessageBox::warning(this, "错误", "管理员未登录，无法操作！");
		return;
	}
	iLoginMode = Login_Add;
	login->setStackedWidgetIndex(1);
	login->setUserEditInfo("","",USER_OPATER_LEVEL);
	login->show();
}

void HXGD::on_pb_editUser_clicked()
{
	if (loginLevel < USER_ADMIN_LEVEL)
	{
		QMessageBox::warning(this,"错误","管理员未登录，无法操作！");
		return;
	}
	QString UserName = ui.tableWidget_userInfo->item(iCurrentUserInfoRow, 1)->text();
	if (UserName == "admin")
	{
		QMessageBox::information(this, "用户操作", "无法操作核心管理员！");
		return;
	}
	login->setStackedWidgetIndex(1);
	iLoginMode = Login_Edit;
	if (db.isOpen() || db.open())
	{
		QSqlQuery query("SELECT * FROM UserManager;", db);
		if (query.exec())
		{
			while (query.next())
			{
				QString str = query.value(2).toString();
				QString str2 = ui.tableWidget_userInfo->item(iCurrentUserInfoRow, 1)->text();
				if ( str == str2)
				{
					QString query3=query.value(3).toString();
					int query0 = query.value(0).toInt();
					login->setUserEditInfo(query.value(2).toString(), query.value(3).toString(), query.value(0).toInt());
				}
				
			}
		}
		db.close();
	}
	login->show();
}

void HXGD::on_pb_deleteUser_clicked()
{
	if (loginLevel < USER_ADMIN_LEVEL)
	{
		QMessageBox::warning(this, "错误", "管理员未登录，无法操作！");
		return;
	}
	if (iCurrentUserInfoRow<0 || iCurrentUserInfoRow > ui.tableWidget_userInfo->rowCount())
	{
		return;
	}
	QString UserName = ui.tableWidget_userInfo->item(iCurrentUserInfoRow, 1)->text();
	if (UserName == "admin")
	{
		QMessageBox::information(this, "用户操作", "无法操作核心管理员！");
	}
	switch (QMessageBox::information(this, tr("用户操作"),
		tr(QString("确定删除用户 %1 吗？").arg(UserName).toStdString().c_str()), tr("是"), tr("否"), 0, 1))
	{
	case 0:
		break;
	case 1:

	default:
		return;
	}

	if (db.open())
	{
		QSqlQuery query(QString("DELETE FROM UserManager WHERE userName = '%1';").arg(UserName),db);
		if (query.exec())
		{
			ui.tableWidget_userInfo->removeRow(iCurrentUserInfoRow);
			QMessageBox::information(this,"用户操作","用户删除成功！");
		}
		db.close();
	}
	LoadUserInfoFromDB();
}

void HXGD::on_pb_queryUserOperate_clicked()
{

}

void HXGD::slotLogin(QString permission, QString str, QString pwd)
{
	if (str == "")
	{
		cbx_permit->setCurrentIndex(3);
		loginLevel = -1;
		if (loginLevel < USER_ADMIN_LEVEL)
		{

		}
	}
	else
	{
		if (db.open())
		{
			QSqlQuery query("SELECT * FROM UserManager;", db);
			bool bloginSuccess = false;
			if (query.exec())
			{
				while (query.next())
				{
					//QString strID = query.value(0).toString();
					int roleLevel = query.value(0).toInt();
					QString strPermit = query.value(1).toString();
					QString strName = query.value(2).toString();
					QString strPwd = query.value(3).toString();
					//int roleLevel = query.value(4).toInt();
					if (strPermit == permission && strName == str && strPwd == pwd)
					{
						bloginSuccess = true;
						userNameLogin = strName;
						loginLevel = roleLevel;
						cbx_permit->setCurrentText(QString(strPermit));
						emit signalLogStatus(true);
					}
					
				}
			}
			if (!bloginSuccess)
			{
				cbx_permit->setCurrentIndex(3);
				userNameLogin = "";
				loginLevel = -1;
				emit signalLogStatus(false);

			}
			db.close();
		}
		
	}
}

void HXGD::slotEditAddUser(int mode, QString username, QString password, int level)
{
	if (db.isOpen() || db.open())
	{
		QSqlQuery query("SELECT * FROM UserManager;", db);
		if (query.exec())
		{
			while (query.next())
			{
				if (query.value(2).toString() == username && query.value(2).toString() != ui.tableWidget_userInfo
					->item(iCurrentUserInfoRow, 1)->text())
				{
					QMessageBox::warning(this,"用户操作","用户名重复，请修改！");
					return;
				}
			}
		}
		db.close();
	}
	QString roleLabel;
	if (level == USER_ADMIN_LEVEL)
	{
		roleLabel = "管理员";
	}
	if (level == USER_MANAGE_LEVEL)
	{
		roleLabel = "工程师";
	}
	if (level == USER_OPATER_LEVEL)
	{
		roleLabel = "操作者";
	}
	if (iLoginMode == Login_Add)
	{
		if (db.open())
		{
			QSqlQuery query(db);
			QString querystr = QString("INSERT INTO UserManager(level,permit,userName,userPwd,updateTime) VALUES('%1','%2'\
				,'%3','%4','%5')").arg(level).arg(roleLabel).arg(username).arg(password).
				arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"));
			if (query.exec(querystr))
			{
				QMessageBox::information(this,"用户操作","用户添加成功！");
				login->hide();
			}
			else
			{
				qDebug() << tr((QString("添加用户 %1 失败，错误: %2").arg(username).arg(query.lastError().text()))
					.toStdString().c_str());
			}
			db.close();
		}
	}
	if (iLoginMode == Login_Edit)
	{
		int id = ui.tableWidget_userInfo->item(iCurrentUserInfoRow,0)->text().toInt();
		QString oriName = ui.tableWidget_userInfo->item(iCurrentUserInfoRow,1)->text();
		if (db.open())
		{
			QSqlQuery query(db);
			QString querystr = QString("UPDATE  UserManager SET level='%1',permit='%2',userName='%3',userPwd='%4',updateTime='%5' WHERE level='%6' AND userName='%7';"
			).arg(level).arg(roleLabel).arg(username).arg(password).arg(QDateTime::currentDateTime().
				toString("yyyy-MM-dd hh-mm-ss")).arg(id).arg(oriName);
			if (query.exec(querystr))
			{
				QMessageBox::information(this, "用户操作", "用户修改成功！");
				login->hide();
			}
			else
			{
				qDebug() << tr((QString("修改用户 %1 失败，错误: %2").arg(username).arg(query.lastError().text()))
					.toStdString().c_str());
			}
			db.close();
		}
	}
	if (iLoginMode == -1)
	{
		return;
	}
	LoadUserInfoFromDB();
}


void HXGD::slotUserInfoCurrentCellChanged(int currentRow,int currentColumn,int previousRow,int previousColumn) 
{
	if (currentRow<0 || currentRow>ui.tableWidget_userInfo->rowCount())
	{
		return;
	}
	iCurrentUserInfoRow = currentRow;
}