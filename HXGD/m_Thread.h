#pragma once
#include <QThread>
#include <QThreadpool>
//#include "Init.h"


#define  Thread_Max 20
//class Init;
class m_Thread :public QObject
{
	Q_OBJECT
public:
	static m_Thread& get_Instance();
	~m_Thread();
	m_Thread(const m_Thread&) = delete;
	m_Thread& operator=(const m_Thread&) = delete;
	void addThread(QObject*  t);
	//void runThread(QFunctionPointer&  );
	void closeThread();

public:

private:
	m_Thread();
	QList<QThread*>  m_ThreadList;
	//QThreadPool *m_ThreadPool;
};

