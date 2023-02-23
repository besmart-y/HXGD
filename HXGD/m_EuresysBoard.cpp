#include "m_EuresysBoard.h"

m_EuresysBoard::m_EuresysBoard()
{
}

m_EuresysBoard::~m_EuresysBoard()
{
}

BOOL m_EuresysBoard::InitInstance()
{
	MCSTATUS ret = McOpenDriver(NULL);
	if (ret!=0)
	{
		return false;
	}
	return true;
}

int m_EuresysBoard::ExitInstance()
{
	MCSTATUS ret = McCloseDriver();
	return ret;
}

void m_EuresysBoard::CreateChannel()
{
    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel);
    McSetParamInt(m_Channel, MC_DriverIndex, 0);

    // In order to use single camera on connector A
    // MC_Connector need to be set to A for Grablink DualBase
    // For all the other Grablink boards the parameter has to be set to M  

    // For all GrabLink boards but Grablink DualBase
    McSetParamStr(m_Channel, MC_Connector, "M");
    // For Grablink DualBase
    // McSetParamStr(m_Channel, MC_Connector, "A");

    // Choose the CAM file
    MCSTATUS ret = McSetParamStr(m_Channel, MC_CamFile, "F:/work/TESTCAMFILES/2023-01-07.cam");//L101k-2k_L2048SC

    int res = 0;
    ret = McGetParamInt(m_Channel, MC_Expose_us, &res);
    int x = res;

    // Choose the camera expose duration
    ret = McSetParamInt(m_Channel, MC_Expose_us, res);
    // Choose the pixel color format
    ret = McSetParamInt(m_Channel, MC_ColorFormat, MC_ColorFormat_Y8);

    // Set the acquisition mode
    ret = McSetParamInt(m_Channel, MC_AcquisitionMode, MC_AcquisitionMode_WEB);

    // Choose the number of lines per page
    ret = McSetParamInt(m_Channel, MC_PageLength_Ln, 600);
    // Choose the way the first acquisition is triggered
    ret = McSetParamInt(m_Channel, MC_TrigMode, MC_TrigMode_IMMEDIATE);
    // Choose the triggering mode for subsequent acquisitions
    ret = McSetParamInt(m_Channel, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
    // Choose the number of images to acquire
    ret = McSetParamInt(m_Channel, MC_SeqLength_Ln, MC_INDETERMINATE);

    // Retrieve image dimensions
    ret = McGetParamInt(m_Channel, MC_ImageSizeX, &m_SizeX);
    ret = McGetParamInt(m_Channel, MC_ImageSizeY, &m_SizeY);
    ret = McGetParamInt(m_Channel, MC_BufferPitch, &m_BufferPitch);

    // The memory allocation for the images is automatically done by MultiCam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    ret = McSetParamInt(m_Channel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);

    // Enable MultiCam signals
    ret = McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    ret = McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

    // Register the callback function
    ret = McRegisterCallback(m_Channel, GlobalCallback, this);
}

void m_EuresysBoard::ActivateChannel()
{
    // Start an acquisition sequence by activating the channel
    McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_ACTIVE);

    // Generate a soft trigger event (STRG)
    McSetParamInt(m_Channel, MC_ForceTrig, MC_ForceTrig_TRIG);
}

void m_EuresysBoard::InactivateChannel()
{
    // Stop an acquisition sequence by deactivating the channel
   MCSTATUS ret = McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_IDLE);
}

void m_EuresysBoard::WebAcquistion(const char* camFilePath)
{
    // Create a channel and associate it with the first connector on the first board
    MCSTATUS ret = McCreate(MC_CHANNEL, &m_Channel);
    if (ret != 0)
    {
        qDebug() << "create channel failed!";
    }
    ret = McSetParamInt(m_Channel, MC_DriverIndex, 0);
    // In order to use single camera on connector A
    // MC_Connector need to be set to A for Grablink DualBase
    // For all the other Grablink boards the parameter has to be set to M  
    // For all GrabLink boards but Grablink DualBase
    ret = McSetParamStr(m_Channel, MC_Connector, "M");
    // For Grablink DualBase
    // McSetParamStr(m_Channel, MC_Connector, "A");
    // Choose the CAM file
    ret = McSetParamStr(m_Channel, MC_CamFile, camFilePath);//L101k-2k_L2048SC   "F:/work/TESTCAMFILES/2023-01-07.cam"
    // Choose the camera expose duration
    int temp = 0;
    ret = McGetParamInt(m_Channel, MC_Expose_us, &temp);
    ret = McSetParamInt(m_Channel, MC_Expose_us, temp);
    // Choose the pixel color format
    ret = McGetParamInt(m_Channel, MC_ColorFormat, &temp);
    ret = McSetParamInt(m_Channel, MC_ColorFormat, temp);
    // Set the acquisition mode
    ret = McGetParamInt(m_Channel, MC_AcquisitionMode, &temp);
    ret = McSetParamInt(m_Channel, MC_AcquisitionMode, temp);
    // Choose the number of lines per page
    ret = McGetParamInt(m_Channel, MC_PageLength_Ln, &temp);
    ret = McSetParamInt(m_Channel, MC_PageLength_Ln, temp);
    // Choose the way the first acquisition is triggered
    ret = McGetParamInt(m_Channel, MC_TrigMode, &temp);
    ret = McSetParamInt(m_Channel, MC_TrigMode, temp);
    // Choose the triggering mode for subsequent acquisitions
    ret = McGetParamInt(m_Channel, MC_NextTrigMode, &temp);
    ret = McSetParamInt(m_Channel, MC_NextTrigMode, temp);
    // Choose the number of images to acquire
    ret = McGetParamInt(m_Channel, MC_SeqLength_Ln, &temp);
    ret = McSetParamInt(m_Channel, MC_SeqLength_Ln, temp);
    // Retrieve image dimensions
    ret = McGetParamInt(m_Channel, MC_ImageSizeX, &m_SizeX);
    ret = McGetParamInt(m_Channel, MC_ImageSizeY, &m_SizeY);
    ret = McGetParamInt(m_Channel, MC_BufferPitch, &m_BufferPitch);
    // The memory allocation for the images is automatically done by MultiCam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    ret = McSetParamInt(m_Channel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);
    // Enable MultiCam signals
    //ret = McGetParamInt(m_Channel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, &temp);
    ret = McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    //ret = McGetParamInt(m_Channel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, &temp);
    ret = McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);
    // Register the callback function
    ret = McRegisterCallback(m_Channel, GlobalCallback, this);

}

void m_EuresysBoard::PageAcquistion(const char* camFilePath)
{
    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel);
    McSetParamInt(m_Channel, MC_DriverIndex, 0);

    // In order to use single camera on connector A
    // MC_Connector need to be set to A for Grablink DualBase
    // For all the other Grablink boards the parameter has to be set to M  

    // For all GrabLink boards but Grablink DualBase
    McSetParamStr(m_Channel, MC_Connector, "M");
    // For Grablink DualBase
    // McSetParamStr(m_Channel, MC_Connector, "A");

    // Choose the CAM file
    McSetParamStr(m_Channel, MC_CamFile, camFilePath);
  
    // Choose the camera expose duration
    int temp = 0;
    McGetParamInt(m_Channel, MC_Expose_us, &temp);
    McSetParamInt(m_Channel, MC_Expose_us, temp);
    // Choose the pixel color format
    McGetParamInt(m_Channel, MC_ColorFormat, &temp);
    McSetParamInt(m_Channel, MC_ColorFormat, temp);

    // Set the acquisition mode
    McGetParamInt(m_Channel, MC_AcquisitionMode, &temp);
    McSetParamInt(m_Channel, MC_AcquisitionMode, temp);

    // Choose the number of lines per page
    McGetParamInt(m_Channel, MC_PageLength_Ln, &temp);
    McSetParamInt(m_Channel, MC_PageLength_Ln, temp);
    // Configure triggering mode
    McGetParamInt(m_Channel, MC_TrigMode, &temp);
    McSetParamInt(m_Channel, MC_TrigMode, temp);
    McGetParamInt(m_Channel, MC_NextTrigMode, &temp);
    McSetParamInt(m_Channel, MC_NextTrigMode, temp);

    // Configure triggering line
    // A rising edge on the triggering line generates a trigger.
    // See the TrigLine Parameter and the board documentation for more details.
    McGetParamInt(m_Channel, MC_TrigLine, &temp);
    McSetParamInt(m_Channel, MC_TrigLine, temp);
    McGetParamInt(m_Channel, MC_TrigEdge, &temp);
    McSetParamInt(m_Channel, MC_TrigEdge, temp);
    McGetParamInt(m_Channel, MC_TrigFilter, &temp);
    McSetParamInt(m_Channel, MC_TrigFilter, temp);

    // Parameter valid for all Grablink but Full, DualBase, Base
    // McSetParamInt(m_Channel, MC_TrigCtl, MC_TrigCtl_ITTL);
    // Parameter valid only for Grablink Full, DualBase, Base
    McGetParamInt(m_Channel, MC_TrigCtl, &temp);
    McSetParamInt(m_Channel, MC_TrigCtl, temp);

    // Choose the number of pages
    McSetParamInt(m_Channel, MC_SeqLength_Pg, 20);

    // Retrieve image dimensions
    McGetParamInt(m_Channel, MC_ImageSizeX, &m_SizeX);
    McGetParamInt(m_Channel, MC_ImageSizeY, &m_SizeY);
    McGetParamInt(m_Channel, MC_BufferPitch, &m_BufferPitch);

    // The memory allocation for the images is automatically done by MultiCam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    McSetParamInt(m_Channel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);

    // Enable MultiCam signals
    McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

    // Register the callback function
    McRegisterCallback(m_Channel, GlobalCallback, this);

    //McCreate(MC_CHANNEL, &m_Channel);
    //McSetParamInt(m_Channel, MC_DriverIndex, 0);

    //// In order to use single camera on connector A
    //// MC_Connector need to be set to A for Grablink DualBase
    //// For all the other Grablink boards the parameter has to be set to M  

    //// For all GrabLink boards but Grablink DualBase
    //McSetParamStr(m_Channel, MC_Connector, "M");
    //// For Grablink DualBase
    //// McSetParamStr(m_Channel, MC_Connector, "A");

    //// Choose the CAM file
    //MCSTATUS ret = McSetParamStr(m_Channel, MC_CamFile, "F:/work/TESTCAMFILES/page2.cam");//"L101k-2k_L2048SP"
    //int res = 0;
    //McGetParamInt(m_Channel, MC_Expose_us, &res);
    //int x = res;
    //// Choose the camera expose duration
    //McSetParamInt(m_Channel, MC_Expose_us, 10000);
    //// Choose the pixel color format
    //McSetParamInt(m_Channel, MC_ColorFormat, MC_ColorFormat_Y8);

    //// Set the acquisition mode
    //McSetParamInt(m_Channel, MC_AcquisitionMode, MC_AcquisitionMode_PAGE);

    //// Choose the number of lines per page
    //McSetParamInt(m_Channel, MC_PageLength_Ln, 600);
    //// Configure triggering mode
    //McSetParamInt(m_Channel, MC_TrigMode, MC_TrigMode_COMBINED);
    //McSetParamInt(m_Channel, MC_NextTrigMode, MC_NextTrigMode_COMBINED);

    //// Configure triggering line
    //// A rising edge on the triggering line generates a trigger.
    //// See the TrigLine Parameter and the board documentation for more details.
    //McSetParamInt(m_Channel, MC_TrigLine, MC_TrigLine_NOM);
    //McSetParamInt(m_Channel, MC_TrigEdge, MC_TrigEdge_GOHIGH);
    //McSetParamInt(m_Channel, MC_TrigFilter, MC_TrigFilter_ON);

    //// Parameter valid for all Grablink but Full, DualBase, Base
    //// McSetParamInt(m_Channel, MC_TrigCtl, MC_TrigCtl_ITTL);
    //// Parameter valid only for Grablink Full, DualBase, Base
    //McSetParamInt(m_Channel, MC_TrigCtl, MC_TrigCtl_ISO);

    //// Choose the number of pages
    //McSetParamInt(m_Channel, MC_SeqLength_Pg, 20);

    //// Retrieve image dimensions
    //McGetParamInt(m_Channel, MC_ImageSizeX, &m_SizeX);
    //McGetParamInt(m_Channel, MC_ImageSizeY, &m_SizeY);
    //McGetParamInt(m_Channel, MC_BufferPitch, &m_BufferPitch);

    //// The memory allocation for the images is automatically done by MultiCam when activating the channel.
    //// We only set the number of surfaces to be created by MultiCam.
    //McSetParamInt(m_Channel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);

    //// Enable MultiCam signals
    //McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    //McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

    //// Register the callback function
    //McRegisterCallback(m_Channel, GlobalCallback, this);
}

// GlobalCallback

void WINAPI GlobalCallback(PMCSIGNALINFO SigInfo)
{
    if (SigInfo && SigInfo->Context)
    {
        m_EuresysBoard* pDoc = (m_EuresysBoard*)SigInfo->Context;
        pDoc->Callback(SigInfo);
    }
}

void m_EuresysBoard::Callback(PMCSIGNALINFO SigInfo)
{
    // + GrablinkWeb Sample Program
    unsigned char* ptrChar;
    //cv::Mat mat;
    //cv::Mat src=cv::Mat(this->m_SizeY, this->m_SizeX, CV_8UC1);

    switch (SigInfo->Signal)
    {
    case MC_SIG_SURFACE_PROCESSING:
        // Update "current" surface address pointer
        McGetParamPtr(SigInfo->SignalInfo, MC_SurfaceAddr, &m_pCurrent);
        ptrChar = (unsigned char*)m_pCurrent;
       
        m_mat = cv::Mat(this->m_SizeY,this->m_SizeX, CV_8UC1,ptrChar);
        emit sigSendOutPic(m_mat,this->property("CAM").toInt());
        ++count;
        std::cout << "count = ";
        std::cout << count;
        std::cout << "\n";
        break;
    case MC_SIG_ACQUISITION_FAILURE:
        //MessageBox(NULL, "Acquisition Failure !", "GrablinkWeb", MB_OK);
        break;
    default:
        break;

    }
    // - GrablinkWeb Sample Program
}
