#include "m_CSV.h"

m_CSV::m_CSV()
{

}

m_CSV::~m_CSV()
{

}

void m_CSV::ParseCSV(QStringList data, QStringList& listCol)
{
	
	for each (auto var in data)
	{
		listCol = var.split(",");

	}
}


void m_CSV::slotWriteCSV(QString filePath, QStringList data)
{
	QFile file(filePath);
	if (file.open(QIODevice::WriteOnly))
	{
		for each (auto var in data)
		{
			file.write(var.toStdString().c_str());
		}
		file.close();
	}
	else
	{
		qDebug() << tr("csv文件打开失败!");
	}
}

void m_CSV::slotReadCSV(QString filePath, QStringList& data)
{
	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);
		while (!stream.atEnd())
		{
			data.push_back(stream.readLine());
		}
		file.close();
	}

}