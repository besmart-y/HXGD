#pragma once
#include <multicam.h>
#include <afx.h>
//#include <afxwin.h>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <qobject.h>
#include <qdebug.h>
#define EURESYS_SURFACE_COUNT 3


// Callback function declaration
void WINAPI GlobalCallback(PMCSIGNALINFO SigInfo);

class m_EuresysBoard:public QObject
{
	Q_OBJECT
public:
	cv::Mat m_mat;
	m_EuresysBoard();
	~m_EuresysBoard();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void CreateChannel();
	void ActivateChannel();
	void InactivateChannel();
	void WebAcquistion(const char * camFilePath);
	void PageAcquistion(const char* camFilePath);


private:
	int count = 0;
	
public:
	MCHANDLE m_Channel;
	void Callback(PMCSIGNALINFO SigInfo);
	PVOID m_pCurrent;
	int m_SizeX;
	int m_SizeY;
	int m_BufferPitch;
	volatile BOOL m_bScreenRefreshCompleted;
signals:
	void sigSendOutPic(cv::Mat mat,int);

};

