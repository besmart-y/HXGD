#pragma once
#include <qobject.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdebug.h>

class m_CSV:public QObject
{
	Q_OBJECT
public:
	m_CSV();
	~m_CSV();
	void ParseCSV(QStringList data,QStringList& listCol);

private:


public slots:
	void slotWriteCSV(QString filePath,QStringList data);
	void slotReadCSV(QString filePath, QStringList& data);


};

