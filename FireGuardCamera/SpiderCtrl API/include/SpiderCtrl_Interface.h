

#ifndef __SPIDERCTRL_INTERFACE_H__
#define __SPIDERCTRL_INTERFACE_H__

#ifdef SPIDERCTRL_EXPORTS
#define SPIDERCTRLAPI __declspec(dllexport)
#else
#define SPIDERCTRLAPI __declspec(dllimport)
#endif





#pragma pack(1)


//////////////////////////////////////////////////////////////
//
//
//				discover structures and enum
//
//
enum ENUM_DISCOVERY_RESULT_CODE
{
	E_DISCOVERY_OK = 0,
	E_DISCOVERY_FAIL,
};

#define DISCOVERY_UPNP							0x0001
#define DISCOVERY_ZEROCONF						0x0002
#define DISCOVERY_ONVIF							0x0004

struct ST_SPIDER_DISCOVERY_CAMEAR_INFO
{
	CHAR			strDeviceName[64];
	CHAR			strModelName[64];
	CHAR			strSerialNum[64];
	CHAR			strMacAddr[64];
	CHAR			strVendorName[64];
	CHAR			strFirmware[64];
	CHAR			strDeviceURL[64];
	__time32_t		DeviceStartTime;
	CHAR			strDeviceDes[64];
	USHORT			nsDiscoverySupport; // bitmask ; upnp 0x0001, zeroconf 0x0002, onvif 0x0004
	CHAR			strIpAddress[64];
	CHAR			strSubnetMask[64];
	CHAR			strGateway[64];
};




//server connect result
enum ENUM_SERVER_STATUS
{
	E_SERVER_RECVDATA = 0,		//recv data
	E_SERVER_CLOSED,			//closed server
	E_SERVER_FAIL_CREATE_SOCKET,	//failed to create socket. check network status.
	E_SERVER_FAIL_OPEN_PORT,		//failed to open server port. check the port was opend or not.
	E_SERVER_FAIL_START,		//failed to start server. this is running server. stop this first.
};



//syslog enumerate
enum ENUM_SYSLOG_FACILITY
{
	esyslog_fac_kernel_messages							= 0,
	esyslog_fac_user_level_messages							,
	esyslog_fac_mail_system									,
	esyslog_fac_system_daemons								,
	esyslog_fac_security_authorization_messages1			,
	esyslog_fac_messages_generated_internally_by_syslogd	,
	esyslog_fac_line_printer_subsystem						,
	esyslog_fac_network_news_subsystem						,
	esyslog_fac_UUCP_subsystem								,
	esyslog_fac_clock_daemon1								,
	esyslog_fac_security_authorization_messages2			,
	esyslog_fac_FTP_daemon									,
	esyslog_fac_NTP_subsystem								,
	esyslog_fac_log_audit									,
	esyslog_fac_log_alert									,
	esyslog_fac_clock_daemon2								,
	esyslog_fac_local_use_0									,
	esyslog_fac_local_use_1									,
	esyslog_fac_local_use_2									,
	esyslog_fac_local_use_3									,
	esyslog_fac_local_use_4									,
	esyslog_fac_local_use_5									,
	esyslog_fac_local_use_6									,
	esyslog_fac_local_use_7									,
};
enum ENUM_SYSLOG_SEVERITY
{
	esyslog_severity_Emergency			= 0	,	//: system is unusable            
	esyslog_severity_Alert					,	//: action must be taken immediately  
	esyslog_severity_Critical				,	//: critical conditions            
	esyslog_severity_Error					,	//: error conditions                  
	esyslog_severity_Warning				,	//: warning conditions              
	esyslog_severity_Notice					,	//: normal but significant condition 
	esyslog_severity_Informational			,	//: informational messages    
	esyslog_severity_Debug					,	//: debug-level messages
};		





enum ENUM_CTRL_RESULT_CODE
{
	E_CTRL_OK						= 0,

	//error...
	E_CTRL_INVALID_CAMERA_HANDLE	= 1,
	E_CTRL_NULL_ID					= 2,
	E_CTRL_NULL_ARGUMENT			= 3,
	E_CTRL_FAILED_FIND_ID			= 4,
	E_CTRL_FAILED_FIND_INDEX		= 5,
	E_CTRL_FAILED_SEND_PACKET		= 6,
	E_CTRL_FAILED_PARSING_RESPONSE	= 7,
	E_CTRL_FAILED_AUTH_USER			= 8,
	E_CTRL_NOT_SUPPORTED			= 9,
	E_CTRL_BAD_REQUEST				= 10,
	E_CTRL_FAILED_UPDATE_CAMERA		= 11,	// camera is busy.. or camera return error.. 
	E_CTRL_FAIL						= 12,
	E_CTRL_INVALID_SERVER_HANDLE	= 13,
	E_CTRL_FAILED_CREATE_SOCK		= 14,
	E_CTRL_NODATA					= 15,
	E_CTRL_FAILED_CONNECT			= 16,
	E_CTRL_INVALID_EVENT_HANDLE		= 17,
	E_CTRL_FAILED_WAIT_RESPONSE		= 18,	// time-out waiting response.. 	
	E_CTRL_INSUFFICIENT_BUFFER		= 19,	// The data area passed to a system call is too small.
	E_CTRL_ALREADY_EXISTS			= 20,
};

// custom video output
enum VIDEO_OUTPUT_FORMAT
{
	VIDEO_OUTPUT_FORMAT_NTSC = 0,
	VIDEO_OUTPUT_FORMAT_PAL
};

// custom resChange
enum RESOLUTION_SIZE
{
	RESOLUTION_SIZE_2M = 0,
	RESOLUTION_SIZE_3M, 
	RESOLUTION_SIZE_5M
};

// custom Defog
enum DEFOG_TYPE
{
	DEFOG_TYPE_OFF = 0,
	DEFOG_TYPE_LOW,
	DEFOG_TYPE_MID,
	DEFOG_TYPE_HIGH
};

// custom Auto gain
enum AUTO_GAIN_TYPE
{
	AUTO_GAIN_TYPE_OFF = 0,
	AUTO_GAIN_TYPE_LOW,
	AUTO_GAIN_TYPE_MID,
	AUTO_GAIN_TYPE_HIGH, 
	AUTO_GAIN_TYPE_MANUAL
};

// custom Pre/Post Record type
enum RECORD_TYPE
{
	RECORD_TYPE_VIDEO = 0,
	RECORD_TYPE_IMAGE
};

// custom DNS Name
enum DNS_NAME
{
	FREEDNS = 0,	// freedns.afraid.org
	DYNDNS,			// dyndns.org
	ZONEEIDT,		// zoneeidt.com
	NOIP			// no_ip.com
};


/////////////////////////////////////////////////////////////////
//
//
//			Event Recevier Structures & enumerator
//

//Event  							Code number	    attribute				비고 
enum ENUM_CTRL_SEVENT_CODE
{
	E_CTRL_SEVENT_TRIGGER =				0x1100, 	//don't care			hardware/software trigger 
	E_CTRL_SEVENT_MOTION = 				0x1200, 	//don't care			VMD 
	E_CTRL_SEVENT_ALARM = 				0x1300, 	//input	port number	 	non-trigger model camera의 digital input port	
	E_CTRL_SEVENT_FAILRECORD =			0x1500, 	//don't care			SD recording중 storage full or process error , E_CTRL_SEVENT_FAILRECORD is not support yet. 2013-02-20
	E_CTRL_SEVENT_THERMAL_ALARM =		0x1600,
	E_CTRL_SEVENT_CONNECT_LIMIT =		0x2000, 	// TCP connection limit exceeded
};


struct ST_CTRL_SEVENT_SETUP_NOTIFY
{
	int nId;                //nId 0 ~ 2
	char notificationMethod[30];		// "" , "email" , "syslog" , "FTP" , "IO" , "TCP" 
	int outputIOPortID;
	int notificationInterval;			//milliseconds
};

struct ST_CTRL_SEVENT_SETUP_INFO
{
	//event type
	int eId;				//eId 0 ~ 5
	char eventType[30];		// "IO" , "trigger" , "VMD" , // 2013-02-20 not support yet : "recordingfailure" , "networkerror" 
	int inputIOPortID;
	char eventDescription[256];

	//notification
	ST_CTRL_SEVENT_SETUP_NOTIFY stNotify;
};

struct ST_CTRL_SEVENT_STATUS
{
	int struct_size;
	unsigned short nEventCode;			//ENUM_CTRL_SEVENT_CODE
	unsigned short nEventAttribute;

	unsigned short nYear;	
	unsigned short nMonth;
	unsigned short nDay;
	unsigned short nHour;
	unsigned short nMinute;
	unsigned short nSecond;

	unsigned char AttachCount;	//0 ~ n
	// if 0 , no attached data (file).
	//이벤트와 연관된 영상데이터(Attach)가 있는 경우엔 값이 있고 이벤트만 있으면 이 값은 0.
	//하나의 이벤트에 여러개의 영상(Attach)이 있을수도 있다.								

	//이하값은 AttachCount 의 값이 있을때만 참조.
	//refer to below when 'AttachCount' is not 0.
	unsigned char AttachIndex;			// from 0 to (AttachCount-1)
	unsigned char AttachType;			// 0 = jpg snapshot, 1 = yuv snapshot, 2 = video clip
	unsigned int AttachFileSize;
	char AttachFileName[500];
};

struct ST_CTRL_SEVENT_STATUS_EX
{
	int struct_size;
	unsigned short nEventCode;			//ENUM_CTRL_SEVENT_CODE
	unsigned short nEventAttribute;

	unsigned short nYear;	
	unsigned short nMonth;
	unsigned short nDay;
	unsigned short nHour;
	unsigned short nMinute;
	unsigned short nSecond;

	unsigned char AttachCount;	//0 ~ n
	// if 0 , no attached data (file).
	//이벤트와 연관된 영상데이터(Attach)가 있는 경우엔 값이 있고 이벤트만 있으면 이 값은 0.
	//하나의 이벤트에 여러개의 영상(Attach)이 있을수도 있다.								

	//이하값은 AttachCount 의 값이 있을때만 참조.
	//refer to below when 'AttachCount' is not 0.
	unsigned char AttachIndex;			// from 0 to (AttachCount-1)
	unsigned char AttachType;			// 0 = jpg snapshot, 1 = yuv snapshot, 2 = video clip
	unsigned int AttachFileSize;
	char AttachFileName[500];

	unsigned short nEventDetailInfo;
};

enum ENUM_CTRL_SEVENT_STATUS
{
	E_CTRL_SEVENT_STATUS_EVENT = 0,
	E_CTRL_SEVENT_STATUS_BUFFER_FULL = 1,
	E_CTRL_SEVENT_STATUS_DISCONNECTED = 2,

	E_CTRL_SEVENT_STATUS_UNDEFINDED,
};























//////////////////////////////////////////////////////////////
//
//				camera control & info
//

struct ST_POS
{
	ULONG	nPositionX;
	ULONG	nPositionY;
};

struct ST_SIZE
{
	ULONG	nWidth;
	ULONG	nHeight;
};



//////////////////////////////////////////////////////////////
//
//			camera info
//
#define DEF_DEVICE_PORT		80
struct ST_SPIDER_DEVICE_INFO
{
	char			device_name[100];
	char			device_id[100];
	char			model_name[50];
	char			serial_number[50];
	char			mac_address[30];
	char			firmware_version[50];
};


//////////////////////////////////////////////////////////////
//
//			camera video 
//
enum ENUM_POWER_MODE
{
	E_POWER_MODE_50HZ = 0,		// "50hz"
	E_POWER_MODE_60HZ				// "60hz"
};
enum ENUM_WB_MODE
{
	E_WB_MODE_MANUAL = 0,
	E_WB_MODE_AUTO,
	E_WB_MODE_INDOOR,
	E_WB_MODE_OUTDOOR,
	E_WB_MODE_INVALID = 0xFFFF,
};
enum ENUM_EXP_MODE
{
	E_EXP_MODE_MANUAL = 0,		// "manual"
	E_EXP_MODE_AUTO				// "auto"
};

enum ENUM_DAYNIGHT_MODE
{
	E_DAYNIGHT_MODE_DAY = 0,		// "day"
	E_DAYNIGHT_MODE_NIGHT,			// "night"
	E_DAYNIGHT_MODE_AUTO,			// "auto"
};
enum ENUM_IRIS_MODE
{
	E_IRIS_MODE_MANUAL = 0,     // "manual"
	E_IRIS_MODE_AUTO,			  // "auto"
	E_IRIS_MODE_OVERRIDE,		  // "override"
	E_IRIS_MODE_OFF,			  // "off"
	E_IRIS_MODE_ON			  // "on"
};

struct ST_SPIDER_FEATURE_INFO	
{
	char		strID[10];
	char		strPort[10];
	int			power_line_frequency_mode;	//ENUM_POWER_MODE
	ENUM_WB_MODE	white_balance_mode;
	int			nsWBLevel;
	int			nsExpMode;					//ENUM_EXP_MODE
	int			nExpTarget;
	int			nExpAutoMin;
	int			nExpAutoMax;
	int			nsGainLevel;
	int			nsGammaLevel;
	int			nsBrightLevel;
	int			nsContrastLevel;
	int			nsSharpLevel;
	int			nsSaturationLevel;
	int			nsHueLevel;
	int			nsDayNightMode;				//ENUM_DAYNIGHT_MODE
	int			nsDayNightScheduleEnable;
	char		strBeginTime[20];
	char		strEndTime[20];
	USHORT		nsIrisMode;					//ENUM_IRIS_MODE
	int			nIris;
	int			nFocus;
	int			nsGammaEnable;
};


//////////////////////////////////////////////////////////////
//
//			camera text overlay 
//
enum ENUM_ALIGN_TYPE
{
	E_ALIGN_TYPE_LEFT = 0, // "left"
	E_ALIGN_TYPE_RIGHT,	 // "right"
	E_ALIGN_TYPE_CENTER,	 // "center"
	E_ALIGN_TYPE_TOP,		 // "top"
	E_ALIGN_TYPE_BOTTOM	 // "bottom"
};
struct ST_SPIDER_TEXT_INFO
{
	CHAR	strID[10];
	USHORT	nsEnable;
	USHORT	nsTimeStampEnable;
	CHAR	strDateTimeFormat[20];
	CHAR	strBackColor[8];
	CHAR	strFontColor[8];
	USHORT	nsFontSize;
	USHORT	nsReserved;
	CHAR	strDisplayText[64];
	USHORT	nsHoriAlignType; // ENUM_ALIGN_TYPE
	USHORT	nsVertAlignType; // ENUM_ALIGN_TYPE
};



//////////////////////////////////////////////////////////////
//
//			camera privacymask
//
struct ST_SPIDER_PRIVACYMASK
{
	USHORT	nsEnabled;
};

struct ST_SPIDER_PRIVACYMASK_REGION
{
	CHAR	strID[10];
	USHORT	nsEnabled;
	USHORT	nsNumCoordinate;
	ST_POS Coordinate[16];
};




//////////////////////////////////////////////////////////////
//
//			camera motion
//
enum ENUM_DIRECTION_SENS_TYPE
{
	E_DIRECTION_SENS_TYPE_LEFT_RIGHT = 0, // "left-right"
	E_DIRECTION_SENS_TYPE_RIGHT_LEFT,		// "right-left"
	E_DIRECTION_SENS_TYPE_UP_DOWN,		// "up-down"
	E_DIRECTION_SENS_TYPE_DOWN_UP
};
enum ENUM_REGION_TYPE
{
	E_REGION_TYPE_GRID = 0,	// "grid"
	E_REGION_TYPE_ROI			// "roi"
};

struct ST_SPIDER_MOTION
{
	CHAR	strID[10];
	USHORT	nsEnable;
	USHORT	nsNumRegionList;		// count of MotionDetectionReion
	ULONG	nSamplingIntv;
	ULONG	nStartTriggerTime;
	ULONG	nEndTriggerTime;
	USHORT	nsDirectionSensType;	// ENUM_DIRECTION_SENS_TYPE
	USHORT	nsRegionType;			// ENUM_REGION_TYPE
	ULONG	nMinObjectSize;
	ULONG	nMaxObjectSize;
	ULONG	nGridRowGran;
	ULONG	nGridColGran;
	ULONG	nMinRoiHResolution;
	ULONG	nMinRoiVResolution;
};

// Motion detection region
struct ST_SPIDER_MOTION_REGION
{
	CHAR	strID[10];
	USHORT	nsEnable;
	USHORT	nsMaskEnable;
	USHORT	nsSensLevel;
	USHORT	nsDetectThreshold;
	USHORT	nsNumCoordinate;
	ST_POS Coordinate[16];
};




//////////////////////////////////////////////////////////////
//
//			camera audio
//
// 
enum ENUM_AUDIO_MODE
{
	E_AUDIO_MODE_LISTEN_ONLY = 0,				// "listenonly"
	E_AUDIO_MODE_TALK_ONLY,					// "talkonly"
	E_AUDIO_MODE_TALK_OR_LISTEN,				// "talkorlisten"
	E_AUDIO_MODE_TALK_AND_LISTEN				// "talkandlisten"
};
enum ENUM_AUDIO_MIC_SRC
{
	E_AUDIO_MIC_SRC_INTERNAL = 0,				// "internal"
	E_AUDIO_MIC_SRC_EXTERNAL					// "external"
};
struct ST_SPIDER_AUDIO
{
	CHAR	strID[30];
	USHORT	nsEnabled;
	USHORT	nsAudioMode;		//ENUM_AUDIO_MODE	
	USHORT	nsMicEnabled;
	USHORT	nsMicSrc;			//ENUM_AUDIO_MIC_SRC
	USHORT	nsMicVolum;
	USHORT	nsSpeakEnabled;
	USHORT	nsSpeakVolum;
};






//////////////////////////////////////////////////////////////
//
//			camera stream 
//

struct ST_SPIDER_STREAM_TRANSPORT
{
	USHORT		nsRtspPortNo;
	ULONG		nMaxPacketSize;	
	ULONG		nAudioPacketLength;
	ULONG		nAudioInboundPacketLength;
	USHORT		nsAudioInboundPortNo;
	USHORT		nsAudioSrcPortNo;
	USHORT		nsVideoSrcPortNo;	
	USHORT		nsTransportProtocolMask;		//http = 1<<0, rtsp = 1<<1
	USHORT		nsUniEnble;
	USHORT		nsUniRtpType;					//	transport type : "RTP/UDP" = 0,	"RTP/TCP" = 1
	char		strInterfaceID[10];
	USHORT		nsMultiEnable;
	USHORT		nsUserTriggerThreshold;
	char		strMultiDestIpAddr[20];
	USHORT		nsMultiVideoDestPortNo;
	USHORT		nsMultiAudioDestPortNo;
	char		strMultiDestIpAddrV6[20];
	USHORT		nsTTL;
	USHORT		nsSecuEnable;
};

enum ENUM_CODEC_TYPE
{
	E_COCEC_MPEG4 = 0,
	E_COCEC_MJPEG = 1,
	E_COCEC_3GP = 2,
	E_COCEC_H264 = 3,
	E_COCEC_MPNG = 4,
	E_COCEC_UNDEFINDED,
};
enum ENUM_STREAM_SCAN_TYPE
{
	E_STREAM_SCAN_TYPE_PROG = 0,			// "progressive"
	E_STREAM_SCAN_TYPE_INTER,				// "interlaced"
};

enum ENUM_STREAM_QC_TYPE
{
	E_STREAM_QC_TYPE_CBR = 0,			// "cbr"
	E_STREAM_QC_TYPE_VBR				// "vbr"
};

struct ST_SPIDER_STREAM_VIDEO
{
	char		strVideoChannelID[10];
	USHORT		nsVideoEnable;
	USHORT		nsVideoCodecType;		//ENUM_CODEC_TYPE
	USHORT		nsVideoScanType;		//ENUM_STREAM_SCAN_TYPE
	USHORT		nsVideoQCType;			//ENUM_STREAM_QC_TYPE
	ULONG		nWidth;
	ULONG		nHeight;
	ULONG		nOffsetX;
	ULONG		nOffsetY;
	USHORT		nsConstBitRate;
	USHORT		nsFixedQuality;
	USHORT		nsvbrUpCap;
	USHORT		nsvbrLowCap;
	USHORT		nsMaxFrameRate;
	USHORT		nsKeyFrameItv;
	USHORT		nsRotationDegree;
	USHORT		nsMirrorEnable;
	USHORT		nsSnapshotImageType;	// ENUM_STREAM_SNAP_TYPE
};


enum ENUM_AUDIO_COMP_TYPE
{
	E_AUDIO_COMP_TYPE_G711ALAW = 0,		// "G.711alaw"
	E_AUDIO_COMP_TYPE_G711ULAW,			// "G.711ulaw"
	E_AUDIO_COMP_TYPE_G726,				// "G.726"
	E_AUDIO_COMP_TYPE_G729,				// "G.729"
	E_AUDIO_COMP_TYPE_G729A,				// "G.729a"
	E_AUDIO_COMP_TYPE_G729B,				// "G.729b"
	E_AUDIO_COMP_TYPE_PCM,				// "PCM"
	E_AUDIO_COMP_TYPE_MP3,				// "MP3"
	E_AUDIO_COMP_TYPE_AC3,				// "AC3"
	E_AUDIO_COMP_TYPE_AAC,				// "AAC"
	E_AUDIO_COMP_TYPE_ADPCM				// "ADPCM"
};
struct ST_SPIDER_STREAM_AUDIO
{
	char		strAudioChannelID[10];
	USHORT		nsAudioEnable;
	USHORT		nsAudioCompressType;			//ENUM_AUDIO_COMP_TYPE
	USHORT		nsAudioInboundCompressType;		//ENUM_AUDIO_COMP_TYPE
	USHORT		nsAudioBitRate;
	USHORT		nsAudioSampleRate;
	USHORT		nsAudioResolution;
};		

struct ST_SPIDER_STREAM_INFO
{
	char	strID[10];
	char	strChannelName[50];
	USHORT	nsEnable;

	ST_SPIDER_STREAM_TRANSPORT stTransport;
	ST_SPIDER_STREAM_VIDEO stVideo;
	ST_SPIDER_STREAM_AUDIO stAudio;
};





//////////////////////////////////////////////////////////////
//
//			camera stream resolution
//
struct ST_SPIDER_STREAM_RESOLUTION_INFO
{
	int width;
	int height;
};



//////////////////////////////////////////////////////////////
//
//			camera stream capabilities
//
struct ST_RANGE
{
	USHORT	min;
	USHORT	max;
};
// streaming capabilities
struct ST_SPIDER_STREAM_CAPABILITY
{
	unsigned int	nStructSize;

	ULONG		nIDLength;			
	ST_RANGE	ChannelName;				// <channelName>	
	BOOL		bDefStreamEnable;			// <enabled>
	ST_RANGE	RtspPortNo;					// <rtspPortNo>	
	ULONG		nDefRtspPortNo;
	ST_RANGE	MaxPacketSize;				// <maxPacketSize>	
	ST_RANGE	AudioPacketLength;			// <audioPacketLength>	
	ST_RANGE	AudioInPacketLength;		// <audioInboundPacketLength>
	ST_RANGE	AudioInPortNo;				// <audioInboundPortNo>
	ST_RANGE	VideoSrcPortNo;				// <videoSourcePortNo>
	ST_RANGE	AudioSrcPortNo;				// <audioSourcePortNo>
	BOOL		bDefUnicastEnable;			// <Unicast> -> <enable>
	BOOL		bDefMulticastEnable;		// <Multicast> -> <enable>
	ST_RANGE	VideoDestPortNo;			// <videoDestPortNo>
	ST_RANGE	AudioDestPortNo;			// <audioDestPortNo>
	ST_RANGE	destIPAddress;				// <destIPAddress>
	ST_RANGE	destIPAddressV6;			// <destIPv6Address>
	ST_RANGE	TTL;						// <ttl>
	USHORT		nsDefTTL;			
	USHORT		nsReserved;
	BOOL		bDefSecurityEnable;			// <Security> -> <enable>
	BOOL		bDefVideoEnable;			// <Video> -> <enable>
	ULONG		nVideoInputChIDLength;
	ST_RANGE	Width;						// <videoResolutionWidth>
	ST_RANGE	Height;						// <videoResolutionHeight>
	ST_RANGE	OffsetX;					// <videoPositionX>
	ST_RANGE	OffsetY;					// <videoPositionY>
	ST_RANGE	ConstantBitRate;			// <constantBitRate>
	ST_RANGE	FixedQuality;				// <fixedQuality>
	ST_RANGE	keyFrameInterval;			// <keyFrameInterval>
	BOOL		bDefMirrorEnable;			// <mirrorEnabled>
	BOOL		bDefAudioEnable;			// <Audio> -> <enabled>
};

//////////////////////////////////////////////////////////////
//
//			camera multichannel stream ROI
//
enum ENUM_MULTI_CHANNEL_INDEX
{
	E_MULTI_CHANNEL_ALL	= 0,
	E_MULTI_CHANNEL_CH1,
	E_MULTI_CHANNEL_CH2,
	E_MULTI_CHANNEL_CH3,
	E_MULTI_CHANNEL_CH4
};

struct ST_SPIDER_STREAM_ROI
{
	UINT		nID;
	BOOL		bEnable;
	UINT		nStartX;
	UINT		nStartY;
	UINT		nWidth;
	UINT		nHeight;
};


struct ST_SPIDER_STREAM_ROI_INFO
{
	UINT					nStreamROICount;
	ST_SPIDER_STREAM_ROI	nSteamROI[3];
};



//////////////////////////////////////////////////////////////
//
//			camera user 
//
struct ST_SPIDER_USER_ACCOUNT_INFO	
{
	char	strID[10];
	char	strUserName[50];
	char	strUserPw[20];
	char	strProtocolID[10];		//unused
};





//////////////////////////////////////////////////////////////
//
//			camera time
//
enum ENUM_TIME_MODE
{
	E_TIME_MODE_NTP = 0, // "NTP"
	E_TIME_MODE_MANUAL   // "manual"
};
struct ST_SPIDER_TIME_INFO
{
	USHORT		nsTimeMode;			//ENUM_TIME_MODE
	CHAR		strLocalTime[100];
	CHAR		strTimeZone[100];
};


//////////////////////////////////////////////////////////////
//
//			camera time ntp server info
//
enum ENUM_ADDRESS_FORMAT_TYPE
{
	E_ADDRESS_FORMAT_TYPE_IPADDR = 0, // "ipaddress"
	E_ADDRESS_FORMAT_TYPE_HOSTNAME    // "hostname"
};
struct ST_SPIDER_TIME_NTPSERVER_INFO
{
	char		strID[10];
	USHORT		nsFormatType;		//ENUM_ADDRESS_FORMAT_TYPE
	USHORT		nsPort;
	char		strHostName[50];
	char		strIpAddrV4[20];
	char		strIpAddrV6[20];
};






//////////////////////////////////////////////////////////////
//
//			camera event trigger
//
enum ENUM_EVENT_TRIGGER_TYPE
{
	E_EVENT_TRIGGER_TYPE_IO = 0,		// "IO"
	E_EVENT_TRIGGER_TYPE_VMD,			// "VMD"
	E_EVENT_TRIGGER_TYPE_VIDEO_LOSS,	// "videoloss"
	E_EVENT_TRIGGER_TYPE_RAID_FAIL,	// "raidfailure"
	E_EVENT_TRIGGER_TYPE_RECORD_FAIL,	// "recordingfailure"
	E_EVENT_TRIGGER_TYPE_BAD_VIDEO,	// "badvideo"
	E_EVENT_TRIGGER_TYPE_POS,			// "POS"
	E_EVENT_TRIGGER_TYPE_ANALYTIC,	// "analytics"
	E_EVENT_TRIGGER_TYPE_FAN_FAIL,	// "fanfailure"
	E_EVENT_TRIGGER_TYPE_OVERHEAT		// "overheat"
};

struct ST_SPIDER_EVENT_TRIGGER_INFO	
{
	char	strID[20];
	char	strEventDesc[30];		
	char	strIoInputID[20];
	unsigned short	nsEventType;			//ENUM_EVENT_TRIGGER_TYPE
	unsigned short	nsReserved;
	unsigned long	nIntervalOfEvent;
	unsigned long	nNotificationNum;
};


//////////////////////////////////////////////////////////////
//
//			camera event trigger's notification
//
enum ENUM_EVENT_NOTY_METHOD_TYPE
{
	E_EVENT_NOTY_METHOD_TYPE_EMAIL = 0,	// "email"
	E_EVENT_NOTY_METHOD_TYPE_IM,			// "IM"
	E_EVENT_NOTY_METHOD_TYPE_IO,			// "IO"
	E_EVENT_NOTY_METHOD_TYPE_SYSLOG,		// "syslog"
	E_EVENT_NOTY_METHOD_TYPE_HTTP,		// "HTTP"
	E_EVENT_NOTY_METHOD_TYPE_FTP			// "FTP"
};
enum ENUM_EVENT_NOTY_RECUR
{
	E_EVENT_NOTY_RECUR_BEGIN = 0,		// "beginning"
	E_EVENT_NOTY_RECUR_BEGIN_END,		// "beginningandend"
	E_EVENT_NOTY_RECUR_RECUR			// "recurring"
};

struct ST_SPIDER_EVENT_TRIGGER_NOTIFY_INFO
{
	char		strID[20];
	char		strIoOutputID[20];
	USHORT		nsNotyMethod;			// ENUM_EVENT_NOTY_METHOD_TYPE
	USHORT		nsNotyRecurrence;		// ENUM_EVENT_NOTY_RECUR
	ULONG		nNotyInterval;
};





//////////////////////////////////////////////////////////////
//
//			camera event schedule
//
struct ST_SPIDER_EVENT_SCHEDULE_INFO
{
	CHAR	strBeginTime[65];
	CHAR	strEndTime[65];
	ULONG	nNumTimeBlock;
};



//////////////////////////////////////////////////////////////
//
//			camera event schedule time block
//
enum ENUM_DAY_OF_WEEK
{
	E_DAYWEEK_UNKNOWN = 0,
	E_DAYWEEK_MON,
	E_DAYWEEK_TUE,
	E_DAYWEEK_WED,
	E_DAYWEEK_THU,
	E_DAYWEEK_FRI,
	E_DAYWEEK_SAT,
	E_DAYWEEK_SUN,
};

struct ST_SPIDER_EVENT_SCHEDULE_TIME_INFO
{
	ULONG	nDayOfWeek;			//ENUM_DAY_OF_WEEK
	CHAR	strBitString[35];	//24H
	CHAR	strBeginTime[65];
	CHAR	strEndTime[65];
};






//////////////////////////////////////////////////////////////
//
//			camera event notification info
//
// 
enum ENUM_STREAM_SNAP_TYPE
{
	E_STREAM_SNAP_TYPE_JPEG = 0,	// "JPEG"
	E_STREAM_SNAP_TYPE_GIF,		// "GIF"
	E_STREAM_SNAP_TYPE_PNG		// "PNG"
};
enum ENUM_VIDEOCLIP_TYPE
{
	E_VIDEOCLIP_TYPE_ASF = 0,		// "ASF"
	E_VIDEOCLIP_TYPE_MP4,			// "MP4"
	E_VIDEOCLIP_TYPE_3GP,			// "3GP"
	E_VIDEOCLIP_TYPE_264			// "264"
};

struct ST_SPIDER_EVENT_NOTIFICATION
{
	USHORT	nsNumMailing;
	USHORT	nsNumFTP;
	USHORT	nsNumHttpHost;
	USHORT	nsUploadSnapshotEnabled;
	USHORT	nsUploadVideoClipEnabled;
	USHORT	nsAttachedVideoURLEnabled;
	USHORT	nsAttchedSnapshotEnabled;
	USHORT	nsAttchedVideoClipEnabled;
	USHORT	nsSnapshotImageType;		// ENUM_STREAM_SNAP_TYPE
	USHORT	nsVideoClipFormatType;		// ENUM_VIDEOCLIP_TYPE
	ULONG	nPreCaptureLength;
	ULONG	nPostcaptureLength;
	CHAR	strSenderEmail[70];
	CHAR	strRecvEmail[70];
	CHAR	strSubject[70];	
};




//////////////////////////////////////////////////////////////
//
//			camera event notification mailing
//
// 
enum ENUM_MAILING_MODE
{
	E_MAILING_MODE_NONE = 0,		// "none"
	E_MAILING_MODE_SMTP,			// "SMTP"
	E_MAILING_MODE_POP_SMTP		// "POP/SMTP"
};
struct ST_SPIDER_EVENT_NOTIFICATION_MAILING
{
	CHAR	strID[30];
	CHAR	strHostName[20];
	CHAR	strIpAddr[20];
	CHAR	strIpAddrV6[30];
	CHAR	strPopHostName[20];
	CHAR	strPopIpAddr[20];
	CHAR	strPopIpAddrV6[30];
	CHAR	strAccountName[30];
	CHAR	strPassword[30];
	USHORT	nsAuthenticMode;			 // ENUM_MAILING_MODE
	USHORT	nsAddrFormatType;			 // ENUM_ADDRESS_FORMAT_TYPE
	USHORT	nsPort;
	USHORT	nsPopAddrFormatType;		 // ENUM_ADDRESS_FORMAT_TYPE
};






//////////////////////////////////////////////////////////////
//
//			camera event notification ftp
//
// 

struct ST_SPIDER_EVENT_NOTIFICATION_FTP
{
	CHAR	strID[30];
	CHAR	strHostName[20];
	CHAR	strIpAddr[20];
	CHAR	strIpAddrV6[30];
	CHAR	strUserName[30];
	CHAR	strPassword[30];
	CHAR	strUploadPath[64];
	CHAR	strBaseFileName[64];
	USHORT	nsAddrFormatType;			 // ENUM_ADDRESS_FORMAT_TYPE
	USHORT	nsPort;			
	USHORT	nsPassiveModeEnabled;
	USHORT	nsReserved;
};





//////////////////////////////////////////////////////////////
//
//			camera event notification http
//
// 
enum ENUM_HTTPHOST_PROTOCOL_TYPE
{
	E_HTTPHOST_PROTOCOL_TYPE_HTTP = 0, // "HTTP"
	E_HTTPHOST_PROTOCOL_TYPE_HTTPS	 // "HTTPS"
};
enum ENUM_HTTPHOST_PARAMETER_TYPE
{
	E_HTTPHOST_PARAMETER_TYPE_XML = 0, // "XML"
	E_HTTPHOST_PARAMETER_TYPE_QUERY	 // "querystring"
};
enum ENUM_HTTPHOST_AUTH_METHOD
{
	E_HTTPHOST_AUTH_METHOD_NONE = 0,	// "none"
	E_HTTPHOST_AUTH_METHOD_MD5DIGEST  // "MD5digest"
};

struct ST_SPIDER_EVENT_NOTIFICATION_HTTP
{
	CHAR	strID[30];
	CHAR	strUrl[64];
	CHAR	strHostName[20];
	CHAR	strIpAddr[20];
	CHAR	strIpAddrV6[30];
	CHAR	strUserName[30];
	CHAR	strPassword[30];
	USHORT	nsProtocolType;				 // ENUM_HTTPHOST_PROTOCOL_TYPE
	USHORT	nsParaFormatType;			 // ENUM_HTTPHOST_PARAMETER_TYPE
	USHORT	nsAddrFormatType;			 // ENUM_ADDRESS_FORMAT_TYPE
	USHORT	nsPort;
	USHORT	nsHttpAuthMethod;			 // ENUM_HTTPHOST_AUTH_METHOD
	USHORT	nsReserved;
};





//////////////////////////////////////////////////////////////
//
//			camera network ip address
//
struct ST_SPIDER_NETWORK_INFO
{
	char		strID[10];
};

//////////////////////////////////////////////////////////////
//
//			camera network ip address
//

enum ENUM_IP_VERSION
{
	E_IP_VERSION_V4 = 0,		// "v4"
	E_IP_VERSION_V6,			// "v6"
	E_IP_VERSION_DUAL			// "dual"
};
enum ENUM_IP_ADDRESS_TYPE
{
	E_IP_ADDRESS_TYPE_STATIC = 0,		// "static"
	E_IP_ADDRESS_TYPE_DYNAMIC,		// "dynamic"
	E_IP_ADDRESS_TYPE_APIPA			// "apipa"
};
struct ST_SPIDER_NETWORK_IPADDR_INFO
{
	USHORT		nsIpVers;			//ENUM_IP_VERSION
	USHORT		nsType;				//ENUM_IP_ADDRESS_TYPE
	char		strIpAddr[20];
	char		strIpMask[20];
	char		strIpAddrV6[30];
	ULONG		nBitMask;
	char		strGwIp[20];
	char		strGwIpV6[30];
	char		strFirstDns[20];
	char		strFirstDnsV6[30];
	char		strSecDns[20];
	char		strSecDnsV6[30];
};




//////////////////////////////////////////////////////////////
//
//			camera network discovery
//
struct ST_SPIDER_NETWORK_DISCOVERY_INFO
{
	BOOL	bEnableUpnp;
	BOOL	bEnableZeroConf;
	BOOL	bMulticastEnble;
	char	strMulticastIpAddr[20];
	char	strMulticastIpAddrV6[30];
	USHORT	nsMulticastPortNum;
	USHORT	nsMulticastTTL;
};





//////////////////////////////////////////////////////////////
//
//			camera network syslog
//
struct ST_SPIDER_NETWORK_SYSLOG_INFO
{
	USHORT	nsEnable;
};

struct ST_SPIDER_NETWORK_SYSLOG_SERVER
{
	CHAR	strID[10];
	USHORT	nsAddressingType;  // ENUM_ADDRESS_FORMAT_TYPE
	USHORT	nsPort;
	CHAR	strHostName[50];
	CHAR	strIpAddrV4[20];
	CHAR	strIpAddrV6[30];
	CHAR	strFacilityType[50];
	CHAR	strSeverity[50];
};


//////////////////////////////////////////////////////////////
//
//			camera network ipfilter
//
enum ENUM_PERMISSION_TYPE
{
	E_PERMISSION_TYPE_DENY = 0,	// "deny"
	E_PERMISSION_TYPE_ALLOW		// "allow"
};

struct ST_SPIDER_NETWORK_IPFILTER
{
	USHORT	nsEnabled;
	USHORT	nsPermissionType;	//ENUM_PERMISSION_TYPE
};

enum ENUM_ADDRESS_FILTER_TYPE
{
	E_ADDRESS_FILTER_TYPE_MASK = 0, // "mask"
	E_ADDRESS_FILTER_TYPE_RANGE	  // "range" 
};

struct ST_SPIDER_NETWORK_IPFILTER_ADDRESS
{
	CHAR	strID[10];
	USHORT	nsPermissionType;	// ENUM_PERMISSION_TYPE
	USHORT	nsFilterType;		// ENUM_ADDRESS_FILTER_TYPE
	CHAR	strStartIpAddrV4[20];
	CHAR	strEndIpAddrV4[20];
	CHAR	strStartIpAddrV6[30];
	CHAR	strEndIpAddrV6[30];
	CHAR	strIpMaskV4[20];
	CHAR	strIpMaskV6[30];
	CHAR	strBitMask[35];
};



//////////////////////////////////////////////////////////////
//
//			camera storage volume
//
struct ST_SPIDER_STORAGE_VOLUME_INFO
{
	CHAR	strID[30];
	CHAR	strVolumeName[80];
	CHAR	strVolumePath[80];
	CHAR	strVolumeDesc[80];
	CHAR	strVolumeType[80];
	CHAR	strStorageDesc[80];
	CHAR	strStorageLocation[80];
	CHAR	strStorageType[80];
	FLOAT	fCapacity;
};

struct ST_SPIDER_STORAGE_VOLUME_STATUS
{
	float		freeSpace;   //in MB
};


//////////////////////////////////////////////////////////////
//
//			camera storage volume files
//
struct ST_SPIDER_STORAGE_VOLUME_FILE_INFO
{
	CHAR	strID[30];
	CHAR	strFileName[80];
	CHAR	strFileTime[80];
	FLOAT	fFileSize;	//in MB
};






//////////////////////////////////////////////////////////////
//
//			camera custom trigger
//
enum ENUM_TRIGGER_POLARITY
{
	E_TRIGGER_POLARITY_HIGH = 0,	// "high"
	E_TRIGGER_POLARITY_LOW		// "low"
};
enum ENUM_TRIGGER_SOURCE
{
	E_TRIGGER_SOURCE_SOFTWARE = 0,	// "software"
	E_TRIGGER_SOURCE_HARDWARE,			// "hardware"
	E_TRIGGER_SOURCE_PERIODIC,		// "periodic"
	E_TRIGGER_SOURCE_FILTERCTRL,		// "filterCtrl"
};
enum ENUM_CAPTURE_FILE
{
	E_CAPTURE_FILE_NETWORK = 0,	// "network"
	E_CAPTURE_FILE_SD_CYCLIC,		// "sdStorageWithCyclic"
	E_CAPTURE_FILE_SD_NO_CYCLIC	// "sdStorageWithoutCyclic"
};
enum ENUM_CAPTURE_FORMAT
{
	E_CAPTURE_FORMAT_YUV = 0, // "yuv"
	E_CAPTURE_FORMAT_JPG      // "jpg"
};
struct ST_SPIDER_CUSTOM_TRIGGER_INFO	
{
	CHAR		strIpAddr[20];	
	ULONG		nDelay;		
	USHORT		nsOnOff;						// 0 = off, 1 = on
	USHORT		nsPortNo;
	USHORT		nsPolarity;						// ENUM_TRIGGER_POLARITY
	USHORT		nsSource;						// ENUM_TRIGGER_SOURCE
	USHORT		nsMode;
	USHORT		nsParameter;
	USHORT		nsTriggerNoiseFilter;
	USHORT		nsCaptureFileSaving;			// ENUM_CAPTURE_FILE
	USHORT		nsPeriodicIntv;					// min 1 ~ max 1200 seconds
	USHORT		nsCaptureImageFormat;			// ENUM_CAPTURE_FORMAT
	USHORT		nsGainShutterTableOnOff;		// 0 = off, 1 = on     added 20121030 kslee
};


//////////////////////////////////////////////////////////////
//
//			camera custom trigger's param (gain, shutter)
//
struct ST_SPIDER_CUSTOM_TRIGGER_PARAM_INFO
{
	char		strID[10];
	ULONG		nGain;					// 0 ~ 100 (%)
	ULONG		nShutter;				//in micro seconds
	USHORT		nsIncrement;
};



//////////////////////////////////////////////////////////////
//
//			camera custom strobe
//
enum ENUM_STROBE_MODE
{
	E_STROBE_MODE_TRIGGER = 0, // "triggerOnly"
	E_STROBE_MODE_TRIGGER_LIVE, // "triggerAndLiveMode"
};
struct ST_SPIDER_CUSTOM_STROBE_INFO	
{
	CHAR		strID[20];
	USHORT		nsOnOff;					// 0 = off, 1 = on
	USHORT		nsPolarity;					// ENUM_TRIGGER_POLARITY
	USHORT		nsStrobeMode;				// ENUM_STROBE_MODE
	ULONG		nMinDelay;	
	ULONG		nMaxDelay;
	ULONG		nDelay;
	ULONG		nDuration;
};



//////////////////////////////////////////////////////////////
//
//			camera custom misc
//
struct ST_SPIDER_CUSTOM_MISC	
{
	USHORT	nsAutoGainMin;
	USHORT	nsAutoGainMax;
	USHORT	nsAutoGainRef;
	USHORT	nsRedGainValue;
	USHORT	nsGreenGainValue;
	USHORT	nsBlueGainValue;
	USHORT	nsAutoIrisModeOnOff;		// ENUM_IRIS_MODE
	USHORT	nsAutoIrisModeSelector;		// ENUM_IRIS_MODE
	USHORT	nsAutoIrisTargetValue;
	USHORT	nsAutoIrisSpeedCtrl;
};



struct ST_SPIDER_CUSTOM_BRIGHT
{
	USHORT	nsHorizontalStart;
	USHORT	nsHorizontalWidth;
	USHORT	nsVerticalStart;
	USHORT	nsVerticalWidth;
	USHORT	nsValue;
	USHORT	nsReserved;
};

struct ST_SPIDER_CUSTOM_USER_SET
{
	CHAR	strID[20];
	CHAR	strCurrentUsersetLoad[20];
	CHAR	strCurrentUsersetDefault[20];
	USHORT	nsUsersetCmd;
	USHORT	nsReserved;
};





//////////////////////////////////////////////////////////////
//
//			camera custom storage
//
enum ENUM_STORAGE_VOLUME_STATUS
{
	E_STORAGE_STATUS_OK = 0,	// "SD_OK"
	E_STORAGE_STATUS_DETACH,	// "detached"
	E_STORAGE_STATUS_FULL,		// "storageFull"
	E_STORAGE_STATUS_PROTECT,	// "writeProtected"
	E_STORAGE_STATUS_UNMOUNT	// "unMounted"
};

struct ST_SPIDER_CUSTOM_STORAGE_VOLUME
{
	USHORT	nsStatus;		// ENUM_STORAGE_VOLUME_STATUS
	USHORT  nsReserved;
	FLOAT	fCapacity;
	FLOAT	fFreeSize;
	FLOAT	fUsedSize;
	FLOAT	fUsedPercentage;
};

struct ST_SPIDER_CUSTOM_STORAGE_CTRL
{
	CHAR	strPrefix[32];
	BOOL	bCyclicEnabled;
	BOOL	bRecord;
	BOOL	bAudioEnabled;
	ULONG	nSaveTime;
};





//custom aspectRatioControl
enum ENUM_SPIDER_CUSTOM_ASPECTRATIO
{
	E_ASPECT_RAIO_CROPPING = 0, // "cropping"
	E_ASPECT_RAIO_SQUEEZING	   // "squeezing"
};

//custom linkSpeed
enum ENUM_SPIDER_CUSTOM_LINKSPEED
{
	E_LINK_SPEED_AUTO = 0, // "autoNego"
	E_LINK_SPEED_FULL100,  // "100Mfull"
	E_LINK_SPEED_HALF100,  // "100Mhalf"
	E_LINK_SPEED_FULL10,   // "10Mfull"
	E_LINK_SPEED_HALF10    // "10Mhalf"
};

//custom Auto Clip
struct ST_SPIDER_CUSTOM_AUTOCLIP
{
	BOOL	bEnabled;
	ULONG	nNightClipValue;	//min 0 ~ max 0x7F
};


enum ENUM_SPIDER_TALK_METHOD
{
	E_TALK_METHOD_SIPRTP = 0,	// "sipRtp"			
	E_TALK_METHOD_TCPDIRECT		// "tcpDirect"
};

enum ENUM_SPIDER_TALK_FORMAT
{
	E_TALK_FORMAT_G711U = 0,	// "g711u"
	E_TALK_FORMAT_PCM_UNCOMP	// "pcmUncompressed"
};

//custom audio talk
struct ST_SPIDER_TALK_INFO
{
	USHORT nsMethod;			// ENUM_SPIDER_TALK_METHOD
	USHORT nsPort;				// tcp port number
	USHORT nsAudioFormat;		// ENUM_SPIDER_TALK_FORMAT
};



// custom denoise
enum ENUM_SPIDER_CUSTOM_DENOISE_WINDOW
{
	E_CUSTOM_DENOISE_WINDOW_3X3 = 0, // "3x3"
	E_CUSTOM_DENOISE_WINDOW_5X5,		// "5x5"
	E_CUSTOM_DENOISE_WINDOW_7X7		// "7x7"
};
struct ST_SPIDER_CUSTOM_DENOISE
{
	BOOL bEnabled;
	USHORT nsStrength;		
	USHORT nsWindowSize;			// ENUM_SPIDER_CUSTOM_DENOISE_WINDOW
};

// custom product type
struct ST_SPIDER_CUSTOM_PRODUCT_TYPE
{
	CHAR strType[64];		//model type
	ULONG nWidth;
	ULONG nHeight;
};

// custom jpg quality
struct ST_SPIDER_CUSTOM_JPG_QUALITY
{
	ULONG nQuality;
};


// custom auto exposure area
struct ST_SPIDER_CUSTOM_AUTO_EXPOSURE_AREA
{
	ULONG nStartX;
	ULONG nStartY;
	ULONG nWidth;
	ULONG nHeight;
};

// custom customized isp
enum ENUM_SPIDER_CUSTOM_ISP_TYPE
{
	E_CUSTOM_ISP_TYPE_GENERAL = 0, // "general"
	E_CUSTOM_ISP_TYPE_INDOOR,		 // "indoor"
	E_CUSTOM_ISP_TYPE_OUTDOOR,	 // "outdoor"
	E_CUSTOM_ISP_TYPE_INDOOR_LOW_LUX // "indoorLowLux"
};
struct ST_SPIDER_CUSTOM_ISP
{
	BOOL bEnabled;
	USHORT nsType;		// ENUM_SPIDER_CUSTOM_ISP_TYPE
	USHORT nsReserved;
};




//////////////////////////////////////////////////////////////
//
//			camera serial port 
//
enum ENUM_SPIDER_SERIAL_PORT_TYPE
{
	E_SERIAL_PORT_TYPE_RS485 = 0,
	E_SERIAL_PORT_TYPE_RS422 = 1,
	E_SERIAL_PORT_TYPE_RS232 = 2,
};

enum ENUM_SPIDER_SERIAL_DUPLEX_MODE
{
	E_SERIAL_DUPLEX_MODE_HALF = 0,
	E_SERIAL_DUPLEX_MODE_FULL = 1,	
};

enum ENUM_SPIDER_SERIAL_DIRECTION
{
	E_SERIAL_DIRECTION_MONO = 0,
	E_SERIAL_DIRECTION_BDIRECT = 1,	
};

enum ENUM_SPIDER_SERIAL_PARITY
{
	E_SERIAL_PARITY_NONE = 0,
	E_SERIAL_PARITY_EVEN = 1,
	E_SERIAL_PARITY_ODD = 2,
	E_SERIAL_PARITY_MARK = 3,
	E_SERIAL_PARITY_SPACE = 4,
};

enum ENUM_SPIDER_SERIAL_STOPBITS
{
	E_SERIAL_STOPBITS_1 = 0,	// "1"
	E_SERIAL_STOPBITS_1_5 = 1,	// "1.5"
	E_SERIAL_STOPBITS_2 = 2,	// "2"
};

struct ST_SPIDER_SERIAL_PORT_INFO	
{
	char		strID[10];
	USHORT		nsEnable;		// "disable" = 0, "enable" = 1 
	USHORT		nsPortType;		// ENUM_SPIDER_SERIAL_PORT_TYPE
	USHORT		nsDuplexMode;	// ENUM_SPIDER_SERIAL_DUPLEX_MODE
	USHORT		nsDirection;	// ENUM_SPIDER_SERIAL_DIRECTION
	ULONG		nBaudRate;
	ULONG		nDataBits;
	USHORT		nsParityType;	// ENUM_SPIDER_SERIAL_PARITY
	USHORT		nsStopBits;		// ENUM_SPIDER_SERIAL_STOPBITS
};


struct ST_CTRL_EVENT_POLLING_INFO
{
	int				eventState;
	int				eventType;		// 0 : Trigger, 1 : VMD, 3 : DIO0, 4 : DIO1
	int				imgType;		// 0 : JPEG, 1 : YUV
	int				nLength;
};

//////////////////////////////////////////////////////////////
//
//			Thermal camera control
//


struct ST_SPIDER_THERMAL_SPOT
{
	UINT			nID;
	BOOL			bEnable;
	ST_POS			posSpot;
	FLOAT			fTemperature;
};

struct ST_SPIDER_THERMAL_SPOT_INFO
{
	ST_POS			posMinTemperature;
	ST_POS			posMaxTemperature;
	FLOAT			fCenterTemperature;
	FLOAT			fMinTemperature;
	FLOAT			fMaxTemperature;
	FLOAT			fAverageTemperature;
	ST_SPIDER_THERMAL_SPOT spotInfo[10];
};

struct ST_SPIDER_CUSTOM_DDNS
{
	BOOL			bEnabled;
	DNS_NAME		dnsName;
	CHAR			strHostName[32];
	CHAR			strUserName[16];
	CHAR			strUserPwd[16];
	CHAR			strHashCode[128];
	LONG			nUpdatePeriod;			// Range 10 ~ 86400
};

struct ST_SPIDER_DIGITAL_INPUT_INFO
{
	UINT			nID;
	BOOL			bEnabled;
	BOOL			bActived;
};

struct ST_SPIDER_DIGITAL_INPUTS
{
	UINT							nTotalCnt;
	ST_SPIDER_DIGITAL_INPUT_INFO	stDigitalInputInfo[5];
};

struct ST_SPIDER_DIGITAL_OUTPUT_INFO
{
	UINT			nID;
	BOOL			bEnabled;
	BOOL			bActived;
	LONG			nlDuration;			// Range 0 ~ 10000
};

struct ST_SPIDER_DIGITAL_OUTPUTS
{
	UINT							nTotalCnt;
	ST_SPIDER_DIGITAL_OUTPUT_INFO	stDigitalOutputInfo[5];
};

struct ST_SPIDER_THERMAL_COMPONENTS
{
	ULONG ulStructSize;		// The size of this data structure, in bytes. Set this member to sizeof(ST_SPIDER_THERMAL_COMPONENTS).
	CHAR szThermalAPIVer[32];	// Thermal API Version;
	BYTE bEnableDPC;		// deadPixelCompensation 
	BYTE bEnableSRA;		// setRegionAlarm 
	BYTE bEnableSCB;		// showColorBar 
	BYTE bEnableSTP;		// showTemp 
	BYTE bEnableSID;		// showIndicator 
	BYTE bEnableSCR;		// showCenter 
	BYTE bEnableSNC;		// setNuc 
	BYTE bEnableNAT;		// nucAuto 
	BYTE bEnableNTE;		// nucTime 
	BYTE bEnableTRM;		// teperatureMode 
	BYTE bEnableDCE;		// distanceCorrEnabled 
	BYTE bEnableNRF;		// noiseReductionFilter 
	BYTE bEnableEEH;		// edgeEnhancement 
	BYTE bEnableCIT;		// colorInvert 
	BYTE bEnableCRT;		// contrast 
	BYTE bEnableBRT;		// brightness 
	BYTE bEnableAGC;		// agcMode 
	BYTE bEnableCOL;		// colorBar 
	BYTE bEnableCOR;		// correction 
};


enum EM_THERMAL_PALETTE
{
	PaletteGrey = 0,
	PaletteIron, 
	PaletteRainbow, 
	PaletteGreyRed, 
	PaletteGlowbow, 
	PaletteYellow, 
	PaletteMidGrey
};

enum EM_THERMAL_AGC
{
	NotSupportAgc = 0, 
	AgcAuto, 
	AgcManual
};

enum EM_THERMAL_SENSITIVITY
{
	NotSupportSensitivity = 0, 
	OffSensitivity, 
	LowSensitivity, 
	MiddleSensitivity, 
	HighSensitivity
};

enum EM_THERMAL_ONOFF
{
	NotSupportOnOff = 0, 
	ThermalOff, 
	ThermalOn
};

enum EM_THERMAL_SYSNUC
{
	NotSupportSysNUC = 0, 
	NUCOff, 
	NUCMin1, 
	NUCMin2, 
	NUCMin3, 
	NUCMin4, 
	NUCMin5, 
	NUCMin10, 
	NUCMin20, 
	NUCMin30, 
	NUCMin60
};

enum EM_THERMAL_TEMP_MODE
{
	NotSupportTempMode = 0, 
	TempModeNormal, 
	TempModeHigh
};

enum EM_THERMAL_ALARM_MODE
{
	AlarmCenter = 0, 
	AlarmAverage, 
	AlarmMin, 
	AlarmMax
};

#define NOT_SUPPORT_THERMAL				0xffffffff

struct ST_SPIDER_THERMAL_ALARM
{
	BOOL bAlarmEnable;
	EM_THERMAL_ALARM_MODE emAlarmMode;
	FLOAT fAlarmTemp;						// range : -20.0 ~ 650.0
	LONG nlAlarmStartDelay;					// range : 0 ~ 180
	LONG nlAlarmStopDelay;					// range : 0 ~ 180
	LONG nlAlarmCondition;					// 0:Over, 1:Under
};

struct ST_SPIDER_THERMAL_CTRL_ADV
{
	EM_THERMAL_PALETTE emPaletteClr;		// COL
	EM_THERMAL_AGC emVidAgc;				// AGC
	LONG nlManualGainMin;					// range : 0 ~ 16383
	LONG nlManualGainMax;					// range : 0 ~ 16383
	LONG nlBrightness;						// BRT (range : 0 ~ 100)
	LONG nlContrast;						// CRT (range : 0 ~ 100)
	EM_THERMAL_SENSITIVITY emNRF;			// NRF
	EM_THERMAL_ONOFF emVidInvert;			// CIT
	EM_THERMAL_ONOFF emShowCenter;			// SCR
	EM_THERMAL_ONOFF emShowIndi;			// SID
	EM_THERMAL_ONOFF emShowTemp;			// STP
	EM_THERMAL_ONOFF emShowPalette;			// SCB
	EM_THERMAL_SYSNUC emVidSysNuc;			// NTE
	EM_THERMAL_SENSITIVITY emNUCAutoMode;	// NAT
	EM_THERMAL_TEMP_MODE emTempMode;		// TRM
	EM_THERMAL_SENSITIVITY emEdgeEnhance;	// EEH
};

struct ST_SPIDER_THERMAL_REGION
{
	UINT			nID;
	BOOL			bEnable;
	ST_POS			ptRegion;		// ( PosX Range : 20 ~ Max.Width - 20), (PosY Range : 20 ~ max.Height - 20)
	ST_SIZE			sizeRegion;		// ( SizeWitdh Range : 20 ~ Max.Width - 20), (SizeHeight Range : 20 ~ max.Height - 20)
	ST_SPIDER_THERMAL_ALARM RegionAlaramInfo;
};

struct ST_SPIDER_THERMAL_REGION_INFO
{
	UINT nMaxRegionCnt;
	ST_SPIDER_THERMAL_REGION ThermalRegion[10];
};

struct ST_SPIDER_THERMAL_REGION_TEMP
{
	BOOL bValid;
	BOOL bAlarmOn;
	ST_POS ptMinTemp;
	ST_POS ptMaxTemp;
	FLOAT fCenterTempValue;
	FLOAT fMinTempValue;
	FLOAT fMaxTempValue;
	FLOAT fAvgTempValue;
};

struct ST_SPIDER_THERMAL_REGION_TEMP_INFO
{
	ST_SPIDER_THERMAL_REGION_TEMP ThermalFullTemp;
	ST_SPIDER_THERMAL_REGION_TEMP ThermalRegionTemp[10];
};

struct ST_SPIDER_THERMAL_CORRECT_INFO
{
	BOOL bEnable;
	FLOAT fEmissivity;					// range : 0.00 ~ 1.00
	FLOAT fTransmission;				// range : 0.00 ~ 1.00
	FLOAT fAtmosphere;					// range : -50.0 ~ 100.0
	FLOAT fZeroOffset;					// range : -20.0 ~ 20.0
	LONG nlDistCorrection;				// range : 0 ~ 100
};


#pragma pack()


#ifdef __cplusplus
extern "C"
#else
extern
#endif
{

	/////////////////////////////
	//
	//			discovery
	//
	//return ENUM_DISCOVERY_RESULT_CODE
	SPIDERCTRLAPI	long SPIDER_InitDiscovery(IN HANDLE* phDiscoveryEvent, IN int nWaitTime = 100);
	SPIDERCTRLAPI	long SPIDER_GetDiscoveryCameraCount();
	SPIDERCTRLAPI	long SPIDER_GetDiscoveryCameraInfo(IN int nCameraIndex, OUT ST_SPIDER_DISCOVERY_CAMEAR_INFO* pstInfo);
	SPIDERCTRLAPI	long SPIDER_UninitDiscovery();								




	// added 20130911
	/////////////////////////////
	//
	//			Connection for Serial Port command read & write 
	//
#define HSERIAL		PVOID
	SPIDERCTRLAPI	HSERIAL SPIDER_ConnectSerialPort(IN const char* ipaddress, IN int nPort = 4083, IN int nTimeOutSec = 10);
	//return ENUM_CTRL_RESULT_CODE
	SPIDERCTRLAPI	long SPIDER_DisconnectSerialPort(IN HSERIAL hSerial);
	SPIDERCTRLAPI	long SPIDER_ReadSerialData(IN HSERIAL hSerial, IN int nBufferSize, OUT char* pstrBuffer, OUT int* pnRead);
	SPIDERCTRLAPI	long SPIDER_WriteSerialData(IN HSERIAL hSerial, IN const char* pstrData, IN int nLength, OUT int* pnWritten);





	/////////////////////////////
	//
	//			event receiver
	//
#define HSEVENT		PVOID
#define WM_SEVENT		(WM_USER+92)		//wparam = HSEVENT, lparam is 0

	SPIDERCTRLAPI HSEVENT SPIDER_Event_CreateHandle(const char* camera_ipaddress, int nHTTPPort, const char* userid, const char* password);
	//return ENUM_CTRL_RESULT_CODE
	SPIDERCTRLAPI long SPIDER_Event_DestroyHandle(HSEVENT hSEvent);

	SPIDERCTRLAPI long SPIDER_Event_GetParameter(HSEVENT hSEvent, char* camera_ipaddress, int* pnHTTPPort, char* userid, char* password);
	SPIDERCTRLAPI long SPIDER_Event_SetParameter(HSEVENT hSEvent, const char* camera_ipaddress, int nHTTPPort, const char* userid, const char* password);

	SPIDERCTRLAPI long SPIDER_Event_GetEventPortNumber(HSEVENT hSEvent, int* pnPort);
	SPIDERCTRLAPI long SPIDER_Event_SetEventPortNumber(HSEVENT hSEvent, int nPort);

	// get / set event setup info
	SPIDERCTRLAPI long SPIDER_Event_GetSetupCount(HSEVENT hSEvent, int* pnCount);
	SPIDERCTRLAPI long SPIDER_Event_GetSetupInfo(HSEVENT hSEvent, int nIndex, ST_CTRL_SEVENT_SETUP_INFO* pstInfo);
	SPIDERCTRLAPI long SPIDER_Event_SetSetupInfo(HSEVENT hSEvent, ST_CTRL_SEVENT_SETUP_INFO stInfo);
	SPIDERCTRLAPI long SPIDER_Event_AddSetupInfo(HSEVENT hSEvent, ST_CTRL_SEVENT_SETUP_INFO stInfo);	//eId 0 ~ 5
	SPIDERCTRLAPI long SPIDER_Event_DeleteSetupInfo(HSEVENT hSEvent, int eId); // delete the event id is 'eId' include all notification of the event.
	// To get setup notify use function SPIDER_Event_GetSetupInfo .
	SPIDERCTRLAPI long SPIDER_Event_SetSetupNotify(HSEVENT hSEvent, int eId, ST_CTRL_SEVENT_SETUP_NOTIFY stInfo);
	SPIDERCTRLAPI long SPIDER_Event_AddSetupNotify(HSEVENT hSEvent, int eId, ST_CTRL_SEVENT_SETUP_NOTIFY stInfo);	//eId 0 ~ 5, nId 0 ~ 2
	SPIDERCTRLAPI long SPIDER_Event_DeleteSetupNotify(HSEVENT hSEvent, int eId, int nId); // delete a notificaion only. keep the event info.



	SPIDERCTRLAPI long SPIDER_Event_Start(HSEVENT hSEvent, HWND hNotifyWnd, HANDLE hNotifyEvent);
	SPIDERCTRLAPI long SPIDER_Event_Stop(HSEVENT hSEvent);

	SPIDERCTRLAPI long SPIDER_Event_GetEventStatus(HSEVENT hSEvent, int* pnStatus /* = ENUM_CTRL_SEVENT_STATUS */, ST_CTRL_SEVENT_STATUS* pInfo);
	SPIDERCTRLAPI long SPIDER_Event_GetAttachFileData(HSEVENT hSEvent, unsigned char* pFileData);
	SPIDERCTRLAPI long SPIDER_Event_GetQueueCount(HSEVENT hSEvent, unsigned int* pnCount);
	SPIDERCTRLAPI long SPIDER_Event_FlushQueue(HSEVENT hSEvent);


	// added 20130911
	// Trigger시 각 채널별 resolution 사이즈의 이미지를 받고자 할 때 사용 (리사이징 아님. Trim)
	// SPIDER_Event_SetTriggerCaptureEvent 에 의해 캡쳐된 이미지는 기존 트리거 영상 수신과 동일하게 Event Receiver 를 통해 수신.
	SPIDERCTRLAPI	long SPIDER_Event_GetTriggerCaptureInfo(IN HSEVENT hSEvent, OUT ST_POS* pstOffset0, OUT ST_POS* pstOffset1, OUT ST_POS* pstOffset2);
	SPIDERCTRLAPI	long SPIDER_Event_SetTriggerCaptureInfo(IN HSEVENT hSEvent, IN ST_POS stOffset0, IN ST_POS stOffset1, IN ST_POS stOffset2);
	SPIDERCTRLAPI	long SPIDER_Event_SetTriggerCaptureEvent(IN HSEVENT hSEvent, IN int nChannel /* 0~ 2 */);


	SPIDERCTRLAPI long SPIDER_Event_GetEventPolling(IN HSEVENT hsEvent, OUT ST_CTRL_EVENT_POLLING_INFO* pInfo);
	SPIDERCTRLAPI long SPIDER_Event_GetPollingData(IN HSEVENT hsEvent, OUT unsigned char* pBuffer, IN int nLength);





	/////////////////////////////
	//
	//			trigger server
	//
	SPIDERCTRLAPI	void SPIDER_InitTriggerServer();
	SPIDERCTRLAPI	void SPIDER_UninitTriggerServer();

#define HTRIGGER	PVOID
#define WM_TRIGGER	(WM_USER+737)		//wparam = HTRIGGER, lparam = ENUM_SERVER_STATUS
	SPIDERCTRLAPI	HTRIGGER SPIDER_StartTriggerServer(IN HWND hNotifyWnd, IN int nPort);
	//return ENUM_CTRL_RESULT_CODE
	SPIDERCTRLAPI	long SPIDER_StopTriggerServer(IN HTRIGGER hTrigger);
	SPIDERCTRLAPI	long SPIDER_GetTriggerPortNumber(IN HTRIGGER hTrigger, OUT int* pnPort);
	// func name changed. SPIDER_GetTriggerData -> SPIDER_GetTriggerPointer
	SPIDERCTRLAPI	long SPIDER_GetTriggerPointer(IN HTRIGGER hTrigger, OUT char* cam_ip, OUT char* file_name, OUT unsigned int* file_size, OUT unsigned char** file_data);
	SPIDERCTRLAPI	long SPIDER_GetTriggerImage(IN HTRIGGER hTrigger, OUT char* cam_ip, OUT char* file_name, OUT unsigned int* file_size, IN unsigned char* file_data);
	SPIDERCTRLAPI	long SPIDER_ConvertYUV420ToRGB24(IN unsigned char* pYUV, IN unsigned int nLen, IN unsigned int nWidth, IN unsigned int nHeight, OUT unsigned char* pRGB);




	/////////////////////////////
	//
	//			syslog server
	//
	SPIDERCTRLAPI	void SPIDER_InitSyslogServer();
	SPIDERCTRLAPI	void SPIDER_UninitSyslogServer();

#define HSYSLOG		PVOID
#define WM_SYSLOG	(WM_USER+736)		//wparam = HSYSLOG, lparam = ENUM_SERVER_STATUS
	SPIDERCTRLAPI	HSYSLOG SPIDER_StartSyslogServer(IN HWND hNotifyWnd, IN int nPort);
	//return ENUM_CTRL_RESULT_CODE
	SPIDERCTRLAPI	long SPIDER_StopSyslogServer(IN HSYSLOG hSyslog);
	SPIDERCTRLAPI	long SPIDER_GetSyslogPortNumber(IN HSYSLOG hSyslog, OUT int* pnPort);
	SPIDERCTRLAPI	long SPIDER_GetSyslogData(IN HSYSLOG hSyslog, OUT char* cam_ip, OUT ENUM_SYSLOG_FACILITY* pFacility, OUT ENUM_SYSLOG_SEVERITY* pSeverity, OUT unsigned int* msg_len, OUT char** pszMessage);




	/////////////////////////////
	//
	//			live streaming
	//
	SPIDERCTRLAPI	void SPIDER_InitStreaming();
	SPIDERCTRLAPI	void SPIDER_UninitStreaming();

#define HSTREAM		PVOID
#define WM_STREAM	(WM_USER+735)		//wparam = HSTREAM, lparam = not used.

	enum ENUM_CTRL_DEC_TYPE
	{
		E_CTRL_DEC_YUV420 = 0,		//planar yuv420(YV12) 12bpp
		E_CTRL_DEC_RGB24 = 1,		//rgb 24

		E_CTRL_DEC_DONOT = 0xFF,	//do not decompress the data. recv compressed format.	//added 20130205
	};

	typedef struct _tagCallbackFrameInfo
	{
		DWORD dwFrameInfoSize;			// The size of this data structure, in bytes.
		int nDataType;					// ENUM_STREAM_DATA_TYPE
		UINT unFrameLength;
		PBYTE pFrameData;
		bool bAudio;
		UINT unWidth;
		UINT unHeight;
	} ST_SPIDER_STREAM_CALLBACK, *PST_SPIDER_STREAM_CALLBACK;

	// typedef void(WINAPI *FRAMECALLBACK)(DWORD_PTR dwUserData, int nDataType /*ENUM_STREAM_DATA_TYPE*/, unsigned int nFrameLength, BYTE* pFrameData, bool bAudio);
	typedef void(WINAPI *FRAMECALLBACK)(DWORD_PTR dwUserData, PST_SPIDER_STREAM_CALLBACK pCallbackFrameInfo);

	SPIDERCTRLAPI	HSTREAM SPIDER_CreateStreamingHandle(IN HWND hNotifyWnd
		, IN HANDLE hNotifyEvent
		, const char* Address		/* rtsp address OR camera ip for http streaming */
		, const char* UserID, const char* Password
		, int nTransportType		/* transport type : "RTP/UDP" = 0,	"RTP/TCP" = 1, "HTTP" = 2*/
		, int nDecompressType		/* ENUM_CTRL_DEC_TYPE */
		, int nChannelIndex	= 0		/* for http streaming only */
		, int nHTTPPort = 80		/* for http streaming only */);

	SPIDERCTRLAPI	HSTREAM SPIDER_CreateStreamingHandleEx(IN DWORD_PTR dwUserData
		, IN FRAMECALLBACK fpFrameCallback
		, const char* Address
		, const char* UserID, const char* Password
		, int nTransportType		/* transport type : "RTP/UDP" = 0,	"RTP/TCP" = 1, "HTTP" = 2*/
		, int nDecompressType		/* ENUM_CTRL_DEC_TYPE */
		, int nChannelIndex	= 0		/* for http streaming only */													
		, int nHTTPPort	= 80		/* for http streaming only */);

	//return ENUM_CTRL_RESULT_CODE
	SPIDERCTRLAPI	long SPIDER_DestroyStreamingHandle(HSTREAM hStream);
	SPIDERCTRLAPI	long SPIDER_GetStreamingParameter(HSTREAM hStream, OUT char* Address, OUT char* UserID, OUT char* Password, OUT int* pnTransportType, OUT int* pnChannelIndex, OUT int* pnCodecType, OUT int* pnHTTPPort);
	SPIDERCTRLAPI	long SPIDER_SetStreamingMethod(HSTREAM hStream, IN HWND hNotifyWnd, IN HANDLE hEvent);
	SPIDERCTRLAPI	long SPIDER_GetStreamingMethod(HSTREAM hStream, OUT HWND* phNotifyWnd, OUT HANDLE* phEvent);
	SPIDERCTRLAPI	long SPIDER_StartStreaming(HSTREAM hStream);
	SPIDERCTRLAPI	long SPIDER_StopStreaming(HSTREAM hStream);
	SPIDERCTRLAPI	long SPIDER_StopStreamingEx(HSTREAM hStream, DWORD dwTimeOutMilliSec);
	SPIDERCTRLAPI	long SPIDER_SetStreamingTimeout(HSTREAM hStream, DWORD dwTimeoutSec);

	// 넘겨줄 데이터가 없으면 파라미터 pFrameLength 값은 0. 넘겨줄 데이터가 있으면 그 길이값을 가짐.
	// 파라미터 pnDataType : 0=audio, 1=mjpeg, 2=h.264 sps, 3=h.264 pps, 4=h.264 I frame, 5=h.264 p frame
	// 파라미터 pbAudio : 오디오데이터일 때는 True, 비디오데이터일때는 False. 
	enum ENUM_DATA_TYPE
	{
		E_DATA_AUDIO_UNDEFINED = 0,		//modified 20130410		E_DATA_AUDIO -> E_DATA_AUDIO_UNDEFINED
		E_DATA_JPEG = 1,
		E_DATA_H264_SPS = 2,
		E_DATA_H264_PPS = 3,
		E_DATA_H264_I = 4,
		E_DATA_H264_P = 5,
		E_DATA_MPEG4_I = 6,				//added 201301
		E_DATA_MPEG4_P = 7,				//added 201301

		E_DATA_AUDIO_ALAW = 100,		//added 20130410 v2.4
		E_DATA_AUDIO_ULAW = 101,		//added 20130410 v2.4
		E_DATA_AUDIO_G726 = 102,		//added 20130410 v2.4

		E_DATA_CHANGERESOLUTION = 200,
		E_DATA_DISCONNECTED = 201,
		E_DATA_BUFFERFULL = 202,
		E_DATA_CHANGECODEC = 203,
		E_DATA_ERR_MEM_ALLOC = 204,		//added 20160303 

		E_DATA_UNDEFINDED,
	};
	SPIDERCTRLAPI	long SPIDER_GetFrameData(HSTREAM hStream, int* pnDataType /*ENUM_DATA_TYPE*/, unsigned int* pnFrameLength, BYTE* pFrameData, bool* pbAudio);
	//SPIDER_GetFrameData 의 pnDataType == E_DATA_CHANGERESOLUTION 일때마다 SPIDER_GetResolution 를 호출하여 width, height 의 변경여부를 확인 권장.
	SPIDERCTRLAPI	long SPIDER_GetResolution(HSTREAM hStream, unsigned int* pWidth, unsigned int* pHeight);
	SPIDERCTRLAPI	long SPIDER_GetQueuedFrameCount(HSTREAM hStream, unsigned int* pnCount);
	SPIDERCTRLAPI	long SPIDER_FlushQueuedFrames(HSTREAM hStream);

	SPIDERCTRLAPI	long SPIDER_StartAVIWrite(HSTREAM hStream, char* pstrFileName);
	SPIDERCTRLAPI	long SPIDER_StopAVIWrite(HSTREAM hStream);
	SPIDERCTRLAPI	bool SPIDER_IsAVIWriting(HSTREAM hStream);
	SPIDERCTRLAPI	long SPIDER_GetFrameSize(HSTREAM hStream, int* pnDataType /*ENUM_DATA_TYPE*/, unsigned int* pnFrameLength);
	SPIDERCTRLAPI	long SPIDER_SetH264SPIFrameMerge(HSTREAM hStream, bool bMerge);








#define HCAMERA		PVOID 
	/////////////////////////////
	//
	//			create handle
	//
	//return ENUM_CTRL_RESULT_CODE
	SPIDERCTRLAPI	HCAMERA SPIDER_CreateCameraHandle(const char* ipaddr, int nHTTPPort = 80, const char* userid = NULL, const char* password = NULL);								
	SPIDERCTRLAPI	long SPIDER_GetCameraConnectParameter(IN HCAMERA hCamera, OUT char* ipaddr, OUT int* pnHTTPPort, OUT char* userid, OUT char* password);	
	SPIDERCTRLAPI	long SPIDER_DestroyCameraHandle(IN HCAMERA hCamera);


	//E_CTRL_OK = connected and login OK 
	//E_CTRL_FAILED_CONNECT = Failed to connect. 
	//E_CTRL_FAILED_AUTH_USER = connected but failed to login with user id, pass 
	//else E_CTRL_FAILED_SEND_PACKET / E_CTRL_NOT_SUPPORTED / E_CTRL_INVALID_CAMERA_HANDLE
	SPIDERCTRLAPI	long SPIDER_IsConnectable(IN HCAMERA hCamera, int nTimeOut = 3);


	/////////////////////////////
	//
	//			make url
	//
	SPIDERCTRLAPI	long SPIDER_GetStreamingURL(IN HCAMERA hCamera, IN int nStreamIndex, OUT char* URL);




	/////////////////////////////
	//
	//			configuration functions
	//
	//device info
	SPIDERCTRLAPI	long SPIDER_GetDeviceInfo(IN HCAMERA hCamera, OUT ST_SPIDER_DEVICE_INFO* pstInfo);
	SPIDERCTRLAPI	long SPIDER_SetDeviceName(IN HCAMERA hCamera, IN const char* pszName, IN int nNameLen);

	//videio feature
	SPIDERCTRLAPI	long SPIDER_GetFeatureInfo(IN HCAMERA hCamera, ST_SPIDER_FEATURE_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetFeatureInfo(IN HCAMERA hCamera, ST_SPIDER_FEATURE_INFO stInfo);

	//video overlay list
	SPIDERCTRLAPI	long SPIDER_GetTextOverlayCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetTextOverlayInfo(IN HCAMERA hCamera, int nInfoIndex, ST_SPIDER_TEXT_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetTextOverlayInfo(IN HCAMERA hCamera, ST_SPIDER_TEXT_INFO stInfo);
	//reserved. SPIDERCTRLAPI	long SPIDER_AddTextOverlayInfo(IN HCAMERA hCamera, ST_SPIDER_TEXT_INFO stInfo);
	//reserved. SPIDERCTRLAPI	long SPIDER_DeleteTextOverlayInfo(IN HCAMERA hCamera, int nInfoIndex);

	//privacy mask
	SPIDERCTRLAPI	long SPIDER_GetPrivacyMaskInfo(IN HCAMERA hCamera, ST_SPIDER_PRIVACYMASK* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetPrivacyMaskInfo(IN HCAMERA hCamera, ST_SPIDER_PRIVACYMASK stInfo);

	//privacy mask region
	SPIDERCTRLAPI	long SPIDER_GetPrivacyMaskRegionCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetPrivacyMaskRegionInfo(IN HCAMERA hCamera, int nInfoIndex, ST_SPIDER_PRIVACYMASK_REGION* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetPrivacyMaskRegionInfo(IN HCAMERA hCamera, ST_SPIDER_PRIVACYMASK_REGION stInfo);
	SPIDERCTRLAPI	long SPIDER_AddPrivacyMaskRegionInfo(IN HCAMERA hCamera, ST_SPIDER_PRIVACYMASK_REGION stInfo);
	SPIDERCTRLAPI	long SPIDER_DeletePrivacyMaskRegionInfo(IN HCAMERA hCamera, int nInfoIndex);

	//motion
	SPIDERCTRLAPI	long SPIDER_GetMotionInfo(IN HCAMERA hCamera, ST_SPIDER_MOTION* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetMotionInfo(IN HCAMERA hCamera, ST_SPIDER_MOTION stInfo);

	//motion region
	SPIDERCTRLAPI	long SPIDER_GetMotionRegionCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetMotionRegionInfo(IN HCAMERA hCamera, int nInfoIndex, ST_SPIDER_MOTION_REGION* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetMotionRegionInfo(IN HCAMERA hCamera, ST_SPIDER_MOTION_REGION stInfo);
	SPIDERCTRLAPI	long SPIDER_AddMotionRegionInfo(IN HCAMERA hCamera, ST_SPIDER_MOTION_REGION stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteMotionRegionInfo(IN HCAMERA hCamera, int nInfoIndex);

	//audio list
	SPIDERCTRLAPI	long SPIDER_GetAudioCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetAudioInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_AUDIO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetAudioInfo(IN HCAMERA hCamera, ST_SPIDER_AUDIO stInfo);

	//stream channel list
	SPIDERCTRLAPI	long SPIDER_GetStreamCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetStreamInfo(IN HCAMERA hCamera, int nStreamIndex, ST_SPIDER_STREAM_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetStreamInfo(IN HCAMERA hCamera, ST_SPIDER_STREAM_INFO stInfo);

	SPIDERCTRLAPI	long SPIDER_GetStreamTransportInfo(IN HCAMERA hCamera, int nStreamIndex, ST_SPIDER_STREAM_TRANSPORT* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetStreamTransportInfo(IN HCAMERA hCamera, int nStreamIndex, ST_SPIDER_STREAM_TRANSPORT stInfo);

	SPIDERCTRLAPI	long SPIDER_GetStreamVideoInfo(IN HCAMERA hCamera, int nStreamIndex, ST_SPIDER_STREAM_VIDEO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetStreamVideoInfo(IN HCAMERA hCamera, int nStreamIndex, ST_SPIDER_STREAM_VIDEO stInfo);

	SPIDERCTRLAPI	long SPIDER_GetStreamAudioInfo(IN HCAMERA hCamera, int nStreamIndex, ST_SPIDER_STREAM_AUDIO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetStreamAudioInfo(IN HCAMERA hCamera, int nStreamIndex, ST_SPIDER_STREAM_AUDIO stInfo);


	//stream capability list - resolution
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaResolutionCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaResolutionInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_STREAM_RESOLUTION_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaMaxResolution(IN HCAMERA hCamera, ST_SPIDER_STREAM_RESOLUTION_INFO* pstMaxResolution);

	//stream capability list - range (min, max)
	SPIDERCTRLAPI	long SPIDER_GetStreamCapabilityRangeInfo(IN HCAMERA hCamera, int nStreamIndex, ST_SPIDER_STREAM_CAPABILITY* pInfo);

	//stream capability list - protocol					//http = (1<<0), rtsp = (1<<1)
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaProtocolCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaProtocolInfo(IN HCAMERA hCamera, int nStreamIndex, int nProtocolIndex, int* pnOption);

	//stream capability list	- RTPTransportType		// RTP transport type : "RTP/UDP" = 0,	"RTP/TCP" = 1
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaRTPTransportTypeCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaRTPTransportTypeInfo(IN HCAMERA hCamera, int nStreamIndex, int nTypeIndex, int* pnOption);

	//stream capability list	- VideoInputChID
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaVideoInputChIDCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaVideoInputChIDInfo(IN HCAMERA hCamera, int nStreamIndex, int nIDIndex, char* pszOption);

	//stream capability list	- VideoCodecType		// ENUM_CODEC_TYPE
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaVideoCodecTypeCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaVideoCodecTypeInfo(IN HCAMERA hCamera, int nStreamIndex, int nTypeIndex, int* pnOption);

	//stream capability list	- VideoScanType			// ENUM_STREAM_SCAN_TYPE
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaVideoScanTypeCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaVideoScanTypeInfo(IN HCAMERA hCamera, int nStreamIndex, int nTypeIndex, int* pnOption);

	//stream capability list	- VideoQualityType		//ENUM_STREAM_QC_TYPE
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaVideoQualityTypeCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaVideoQualityTypeInfo(IN HCAMERA hCamera, int nStreamIndex, int nTypeIndex, int* pnOption);

	//stream capability list	- MaxFrameRate
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaMaxFrameRateCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaMaxFrameRateInfo(IN HCAMERA hCamera, int nStreamIndex, int nRateIndex, int* pnOption);

	//stream capability list	- Rotation
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaRotationCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaRotationInfo(IN HCAMERA hCamera, int nStreamIndex, int nRotateIndex, int* pnOption);

	//stream capability list	- SnapImageType			//ENUM_STREAM_SNAP_TYPE
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaSnapImageTypeCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaSnapImageTypeInfo(IN HCAMERA hCamera, int nStreamIndex, int nTypeIndex, int* pnOption);

	//stream capability list	- AudioInputChID
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioInputChIDCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioInputChIDInfo(IN HCAMERA hCamera, int nStreamIndex, int nIDIndex, char* pszOption);

	//stream capability list	- AudioCompressType		//ENUM_AUDIO_COMP_TYPE
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioCompressTypeCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioCompressTypeInfo(IN HCAMERA hCamera, int nStreamIndex, int nTypeIndex, int* pnOption);

	//stream capability list	- AudioBitRate
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioBitRateCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioBitRateInfo(IN HCAMERA hCamera, int nStreamIndex, int nRateIndex, int* pnOption);

	//stream capability list	- AudioSampleRate
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioSampleRateCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioSampleRateInfo(IN HCAMERA hCamera, int nStreamIndex, int nRateIndex, int* pnOption);

	//stream capability list	- AudioResolution
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioResolutionCount(IN HCAMERA hCamera, int nStreamIndex);
	SPIDERCTRLAPI	long SPIDER_GetStreamCapaAudioResolutionInfo(IN HCAMERA hCamera, int nStreamIndex, int nResIndex, int* pnOption);




	//user list
	SPIDERCTRLAPI	long SPIDER_GetUserAccountCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetUserAccountInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_USER_ACCOUNT_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetUserAccountInfo(IN HCAMERA hCamera, ST_SPIDER_USER_ACCOUNT_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_AddUserAccountInfo(IN HCAMERA hCamera, ST_SPIDER_USER_ACCOUNT_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteUserAccountInfo(IN HCAMERA hCamera, int nInfoIndex);



	//time info
	SPIDERCTRLAPI	long SPIDER_GetTimeInfo(IN HCAMERA hCamera, OUT ST_SPIDER_TIME_INFO* pstInfo);
	SPIDERCTRLAPI	long SPIDER_SetTimeInfo(IN HCAMERA hCamera, IN ST_SPIDER_TIME_INFO stInfo);

	//time ntp server list
	SPIDERCTRLAPI	long SPIDER_GetTimeNTPServerCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetTimeNTPServerInfo(IN HCAMERA hCamera, int nNTPServerIndex, ST_SPIDER_TIME_NTPSERVER_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetTimeNTPServerInfo(IN HCAMERA hCamera, ST_SPIDER_TIME_NTPSERVER_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_AddTimeNTPServerInfo(IN HCAMERA hCamera, ST_SPIDER_TIME_NTPSERVER_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteTimeNTPServerInfo(IN HCAMERA hCamera, int nInfoIndex);


	//event trigger list
	SPIDERCTRLAPI	long SPIDER_GetEventTriggerCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetEventTriggerInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_EVENT_TRIGGER_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetEventTriggerInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_TRIGGER_INFO stInfo, int nCount, ST_SPIDER_EVENT_TRIGGER_NOTIFY_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_AddEventTriggerInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_TRIGGER_INFO stInfo, int nCount, ST_SPIDER_EVENT_TRIGGER_NOTIFY_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteEventTriggerInfo(IN HCAMERA hCamera, int nInfoIndex);

	//event trigger notification list
	SPIDERCTRLAPI	long SPIDER_GetEventTriggerNotifyCount(IN HCAMERA hCamera, int nTriggerIndex);
	SPIDERCTRLAPI	long SPIDER_GetEventTriggerNotifyInfo(IN HCAMERA hCamera, int nTriggerIndex, unsigned int nNotifyIndex, ST_SPIDER_EVENT_TRIGGER_NOTIFY_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetEventTriggerNotifyInfo(IN HCAMERA hCamera, int nTriggerIndex, ST_SPIDER_EVENT_TRIGGER_NOTIFY_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_AddEventTriggerNotifyInfo(IN HCAMERA hCamera, int nTriggerIndex, ST_SPIDER_EVENT_TRIGGER_NOTIFY_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteEventTriggerNotifyInfo(IN HCAMERA hCamera, int nTriggerIndex, int nNotifyIndex);

	//event schedule info
	SPIDERCTRLAPI	long SPIDER_GetEventScheduleInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_SCHEDULE_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetEventScheduleInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_SCHEDULE_INFO stInfo, int nCount, ST_SPIDER_EVENT_SCHEDULE_TIME_INFO* pInfo);

	//event schedule time list
	SPIDERCTRLAPI	long SPIDER_GetEventScheduleTimeCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetEventScheduleTimeInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_EVENT_SCHEDULE_TIME_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetEventScheduleTimeInfo(IN HCAMERA hCamera, int nCount, ST_SPIDER_EVENT_SCHEDULE_TIME_INFO* pInfo);


	//notification info
	SPIDERCTRLAPI	long SPIDER_GetNotificationInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_NOTIFICATION* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNotificationInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_NOTIFICATION stInfo);

	//notification mail list
	SPIDERCTRLAPI	long SPIDER_GetNotificationMailingCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetNotificationMailingInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_EVENT_NOTIFICATION_MAILING* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNotificationMailingInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_NOTIFICATION_MAILING stInfo);
	SPIDERCTRLAPI	long SPIDER_AddNotificationMailingInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_NOTIFICATION_MAILING stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteNotificationMailingInfo(IN HCAMERA hCamera, int nInfoIndex);

	//notification FTP list
	SPIDERCTRLAPI	long SPIDER_GetNotificationFTPCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetNotificationFTPInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_EVENT_NOTIFICATION_FTP* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNotificationFTPInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_NOTIFICATION_FTP stInfo);
	SPIDERCTRLAPI	long SPIDER_AddNotificationFTPInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_NOTIFICATION_FTP stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteNotificationFTPInfo(IN HCAMERA hCamera, int nInfoIndex);

	//notification HTTP Post list
	SPIDERCTRLAPI	long SPIDER_GetNotificationHTTPCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetNotificationHTTPInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_EVENT_NOTIFICATION_HTTP* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNotificationHTTPInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_NOTIFICATION_HTTP stInfo);
	SPIDERCTRLAPI	long SPIDER_AddNotificationHTTPInfo(IN HCAMERA hCamera, ST_SPIDER_EVENT_NOTIFICATION_HTTP stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteNotificationHTTPInfo(IN HCAMERA hCamera, int nInfoIndex);



	SPIDERCTRLAPI	long SPIDER_GetNetworkCount(IN HCAMERA hCamera);
	//network info
	SPIDERCTRLAPI	long SPIDER_GetNetworkInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_INFO* pInfo);
	//network ipaddr
	SPIDERCTRLAPI	long SPIDER_GetNetworkIPAddrInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_IPADDR_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNetworkIPAddrInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_IPADDR_INFO stInfo);
	//network discovery
	SPIDERCTRLAPI	long SPIDER_GetNetworkDiscoveryInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_DISCOVERY_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNetworkDiscoveryInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_DISCOVERY_INFO stInfo);

	//network syslog info
	SPIDERCTRLAPI	long SPIDER_GetNetworkSyslogInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_SYSLOG_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNetworkSyslogInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_SYSLOG_INFO stInfo);

	//network syslog server list
	SPIDERCTRLAPI	long SPIDER_GetNetworkSyslogServerCount(IN HCAMERA hCamera, int nNetworkIndex);
	SPIDERCTRLAPI	long SPIDER_GetNetworkSyslogServerInfo(IN HCAMERA hCamera, int nNetworkIndex, int nIndex, ST_SPIDER_NETWORK_SYSLOG_SERVER* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNetworkSyslogServerInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_SYSLOG_SERVER stInfo);
	SPIDERCTRLAPI	long SPIDER_AddNetworkSyslogServerInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_SYSLOG_SERVER stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteNetworkSyslogServerInfo(IN HCAMERA hCamera, int nNetworkIndex, int nServerIndex);

	//network ip filter
	SPIDERCTRLAPI	long SPIDER_GetNetworkIPFilterInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_IPFILTER* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNetworkIPFilterInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_IPFILTER stInfo);

	//network ip filter address list
	SPIDERCTRLAPI	long SPIDER_GetNetworkIPFilterAddressCount(IN HCAMERA hCamera, int nNetworkIndex);
	SPIDERCTRLAPI	long SPIDER_GetNetworkIPFilterAddressInfo(IN HCAMERA hCamera, int nNetworkIndex, int nIndex, ST_SPIDER_NETWORK_IPFILTER_ADDRESS* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetNetworkIPFilterAddressInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_IPFILTER_ADDRESS stInfo);
	SPIDERCTRLAPI	long SPIDER_AddNetworkIPFilterAddressInfo(IN HCAMERA hCamera, int nNetworkIndex, ST_SPIDER_NETWORK_IPFILTER_ADDRESS stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteNetworkIPFilterAddressInfo(IN HCAMERA hCamera, int nNetworkIndex, int nAddressIndex);



	//storage volume
	SPIDERCTRLAPI	long SPIDER_GetStorageVolumeCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetStorageVolumeInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_STORAGE_VOLUME_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_GetStorageVolumeStatus(IN HCAMERA hCamera, int nIndex, ST_SPIDER_STORAGE_VOLUME_STATUS* pInfo);

	//storage files
	SPIDERCTRLAPI	long SPIDER_GetStorageVolumeFileCount(IN HCAMERA hCamera, int nStorageIndex);
	SPIDERCTRLAPI	long SPIDER_GetStorageVolumeFileInfo(IN HCAMERA hCamera, int nStorageIndex, int nIndex, ST_SPIDER_STORAGE_VOLUME_FILE_INFO* pInfo);
	SPIDERCTRLAPI	ULONGLONG SPIDER_GetStorageVolumeFileData(IN HCAMERA hCamera, int nStorageIndex, int nIndex, unsigned char* pBuffer, ULONGLONG ullBufferLength);
	SPIDERCTRLAPI	long SPIDER_DeleteStorageVolumeFile(IN HCAMERA hCamera, int nStorageIndex, int nIndex);



	//custom trigger set
	//20121029 add param nTimeOut for waiting response
	SPIDERCTRLAPI	long SPIDER_SetCustomTriggerSoftwareEvent(IN HCAMERA hCamera, int nTimeOut = 3);

	//custom trigger info
	SPIDERCTRLAPI	long SPIDER_GetCustomTriggerInfo(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_TRIGGER_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomTriggerInfo(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_TRIGGER_INFO stInfo);

	//custom trigger param list (gain, shutter table)
	SPIDERCTRLAPI	long SPIDER_GetCustomTriggerParamCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetCustomTriggerParamInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_CUSTOM_TRIGGER_PARAM_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomTriggerParamInfo(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_TRIGGER_PARAM_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_AddCustomTriggerParamInfo(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_TRIGGER_PARAM_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_DeleteCustomTriggerParamInfo(IN HCAMERA hCamera, int nInfoIndex);

	//custom strobe list
	SPIDERCTRLAPI	long SPIDER_GetCustomStrobeCount(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_GetCustomStrobeInfo(IN HCAMERA hCamera, int nIndex, ST_SPIDER_CUSTOM_STROBE_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomStrobeInfo(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_STROBE_INFO stInfo);

	//custom misc
	SPIDERCTRLAPI	long SPIDER_GetCustomMisc(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_MISC* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomMisc(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_MISC stInfo);

	//custom brightness level
	//20121029 add param nTimeOut for waiting response
	SPIDERCTRLAPI	long SPIDER_GetCustomBright(IN HCAMERA hCamera, IN int x, IN int y, IN int w, IN int h, OUT int* pnbrightness, IN int nTimeOut = 3);

	//custom storage volume
	SPIDERCTRLAPI	long SPIDER_GetCustomStorageVolume(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_STORAGE_VOLUME* pInfo);

	//custom storage control
	SPIDERCTRLAPI	long SPIDER_GetCustomStorageCtrl(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_STORAGE_CTRL* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomStorageCtrl(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_STORAGE_CTRL stInfo);

	//custom aspectRatioControl
	SPIDERCTRLAPI	long SPIDER_GetCustomAspectRatio(IN HCAMERA hCamera, ENUM_SPIDER_CUSTOM_ASPECTRATIO* peIndex);
	SPIDERCTRLAPI	long SPIDER_SetCustomAspectRatio(IN HCAMERA hCamera, ENUM_SPIDER_CUSTOM_ASPECTRATIO eIndex);

	//custom linkSpeed
	SPIDERCTRLAPI	long SPIDER_GetCustomLinkSpeed(IN HCAMERA hCamera, ENUM_SPIDER_CUSTOM_LINKSPEED* peIndex);
	SPIDERCTRLAPI	long SPIDER_SetCustomLinkSpeed(IN HCAMERA hCamera, ENUM_SPIDER_CUSTOM_LINKSPEED eIndex);

	//custom SuppressColorNightMode 
	SPIDERCTRLAPI	long SPIDER_GetCustomSuppressColorNightMode(IN HCAMERA hCamera, bool* pbEnable);
	SPIDERCTRLAPI	long SPIDER_SetCustomSuppressColorNightMode(IN HCAMERA hCamera, bool bEnable);

	//custom Auto Clip
	SPIDERCTRLAPI	long SPIDER_GetCustomAutoClip(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_AUTOCLIP* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomAutoClip(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_AUTOCLIP stInfo);

	//custom Audio Talk
	enum ENUM_TALK_NOTIFY_ID
	{
		TALK_NOTIFY_ID_DISCONNECT = 0,			// Disconnect by socket error
		TALK_NOTIFY_ID_CONNECT_NEW_TALK = 1,	// Disconnect by new talk
		TALK_NOTIFY_ID_COMPLETE_SEND_FILE = 2,	// Complete send PCM Raw file
	};

	typedef void(WINAPI *TALKCALLBACK)(DWORD_PTR dwUserData, HCAMERA hCamera, ENUM_TALK_NOTIFY_ID emTalkNotifyID);

	SPIDERCTRLAPI	long SPIDER_GetTalkInfo(IN HCAMERA hCamera, ST_SPIDER_TALK_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetTalkInfo(IN HCAMERA hCamera, ST_SPIDER_TALK_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_StartTalking(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_StopTalking(IN HCAMERA hCamera);
	SPIDERCTRLAPI	long SPIDER_StartFileTalking(IN HCAMERA hCamera, char* pszPCMRawPath);
	SPIDERCTRLAPI	long SPIDER_SetTalkNotify(IN HCAMERA hCamera, IN DWORD_PTR dwUserData, IN TALKCALLBACK fpTalkCallback);

	// Custom  deimpulse
	SPIDERCTRLAPI	long SPIDER_GetCustomDeimpulse(IN HCAMERA hCamera, bool* pbEnable);
	SPIDERCTRLAPI	long SPIDER_SetCustomDeimpulse(IN HCAMERA hCamera, bool bEnable);

	// Custom  denoise
	SPIDERCTRLAPI	long SPIDER_GetCustomDenoise(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_DENOISE* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomDenoise(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_DENOISE stInfo);

	// Custom  captureJpgQuality
	SPIDERCTRLAPI	long SPIDER_GetCustomJPGQuality(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_JPG_QUALITY* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomJPGQuality(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_JPG_QUALITY stInfo);

	// Custom productType
	SPIDERCTRLAPI	long SPIDER_GetCustomProductType(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_PRODUCT_TYPE* pInfo);

	// Custom auto exposure area
	SPIDERCTRLAPI	long SPIDER_GetCustomAutoExposureArea(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_AUTO_EXPOSURE_AREA* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomAutoExposureArea(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_AUTO_EXPOSURE_AREA stInfo);

	// Custom customizedIsp
	SPIDERCTRLAPI	long SPIDER_GetCustomISP(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_ISP* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetCustomISP(IN HCAMERA hCamera, ST_SPIDER_CUSTOM_ISP stInfo);


	//Reboot
	SPIDERCTRLAPI	long SPIDER_Reboot(IN HCAMERA hCamera);


	// added 20130205 
	// Configuration import / export (filelen : min 14866 bytes ~ max 30720 bytes) 
	// not imported / exported info : model name / serial no. / ip address / subnetmask / gateway / etc, such as belong to the camera own. 
	SPIDERCTRLAPI	long SPIDER_ExportConfig(IN HCAMERA hCamera, OUT int* pnLength, OUT BYTE* pFileData);	//make sure pFileData buffer length is more than 14866 bytes.
	// After importing, reboot camera automatically. it takes 1 minute 30 seconds...
	SPIDERCTRLAPI	long SPIDER_ImportConfig(IN HCAMERA hCamera, IN int nLength, IN BYTE* pFileData);


	// added 20130801
	SPIDERCTRLAPI	long SPIDER_GetSnapShotImage(IN HCAMERA hCamera, OUT int* pnLength, OUT BYTE* pFileData);
	SPIDERCTRLAPI	long SPIDER_GetChannelSnapShotImage(IN HCAMERA hCamera, OUT int* pnLength, OUT BYTE* pFileData, IN int nChannel);

	// serial info
	SPIDERCTRLAPI	long SPIDER_GetSerialPortInfo(IN HCAMERA hCamera, ST_SPIDER_SERIAL_PORT_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetSerialPortInfo(IN HCAMERA hCamera, ST_SPIDER_SERIAL_PORT_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_SetSerialPortCommand(IN HCAMERA hCamera, const char* command);	//command max len 64 byte


	// added 20130911
	// rs485
	SPIDERCTRLAPI	long SPIDER_GetRS485Info(IN HCAMERA hCamera, OUT int* pnAddress /* 1 ~ 32 */ , OUT char* protocol /* path,pelcod,pelcop,.. */);
	SPIDERCTRLAPI	long SPIDER_SetRS485Info(IN HCAMERA hCamera, IN int nAddress /* 1 ~ 32 */ , IN const char* protocol /* path,pelcod,pelcop,.. */);

	//SensorUp (Shutter가 Auto 일 경우에만 SensorUp기능 동작. SensorUp 기능이 지원되는 camera는 Trigger 기능 미지원)
	SPIDERCTRLAPI	long SPIDER_GetSensorUpMaxList(IN HCAMERA hCamera, OUT int* pnCount, OUT int* pnMaxValueList);
	SPIDERCTRLAPI	long SPIDER_GetSensorUpInfo(IN HCAMERA hCamera, OUT bool* pbEnable, OUT int* pnMaxValue /* one of MaxValueList */);
	SPIDERCTRLAPI	long SPIDER_SetSensorUpInfo(IN HCAMERA hCamera, IN bool bEnable, IN int nMaxValue /* one of MaxValueList. */);

	// multichannel
	SPIDERCTRLAPI	long SPIDER_GetMultiChannelCount(IN HCAMERA hCamera, OUT int* pnCount);
	SPIDERCTRLAPI	long SPIDER_SetMultiChannelView(IN HCAMERA hCamera, IN ENUM_MULTI_CHANNEL_INDEX eIndex);
	SPIDERCTRLAPI	long SPIDER_SetMultiChannelSync(IN HCAMERA hCamera);

	// Stream ROI
	SPIDERCTRLAPI	long SPIDER_GetStreamROI(IN HCAMERA hCamera, OUT ST_SPIDER_STREAM_ROI_INFO *pInfo);
	SPIDERCTRLAPI	long SPIDER_SetStreamROI(IN HCAMERA hCamera, IN ST_SPIDER_STREAM_ROI stInfo);

	// Thermal spot (for IMT-812N)
	SPIDERCTRLAPI	long SPIDER_GetThermalSpotInfo(IN HCAMERA hCamera, OUT ST_SPIDER_THERMAL_SPOT_INFO *pInfo);
	SPIDERCTRLAPI	long SPIDER_SetThermalSpot(IN HCAMERA hCamera, IN ST_SPIDER_THERMAL_SPOT stInfo /* Ignore stInfo.fTemperature */);

	// Video Output 
	SPIDERCTRLAPI	long SPIDER_GetVideoOutput(IN HCAMERA hCamera, OUT BOOL* pbEnabled, OUT VIDEO_OUTPUT_FORMAT* pVideoOutputFmt);
	SPIDERCTRLAPI	long SPIDER_SetVideoOutput(IN HCAMERA hCamera, IN BOOL bEnabled, IN VIDEO_OUTPUT_FORMAT VideoOutputFmt);

	// DDNS
	SPIDERCTRLAPI	long SPIDER_GetDDNS(IN HCAMERA hCamera, OUT ST_SPIDER_CUSTOM_DDNS* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetDDNS(IN HCAMERA hCamera, IN ST_SPIDER_CUSTOM_DDNS stInfo);

	// Resolution Change
	SPIDERCTRLAPI	long SPIDER_GetResolutionChange(IN HCAMERA hCamera, OUT RESOLUTION_SIZE* pResolutionSize);
	SPIDERCTRLAPI	long SPIDER_SetResolutionChange(IN HCAMERA hCamera, IN RESOLUTION_SIZE ResolutionSize);

	// Defog
	SPIDERCTRLAPI	long SPIDER_GetDefog(IN HCAMERA hCamera, OUT DEFOG_TYPE* pDefogType);
	SPIDERCTRLAPI	long SPIDER_SetDefog(IN HCAMERA hCamera, IN DEFOG_TYPE DefogType);

	// Digital Inputs/Outputs
	SPIDERCTRLAPI	long SPIDER_GetDigitalInput(IN HCAMERA hCamera, OUT ST_SPIDER_DIGITAL_INPUTS* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetDigitalInput(IN HCAMERA hCamera, IN ST_SPIDER_DIGITAL_INPUT_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_GetDigitalOutput(IN HCAMERA hCamera, OUT ST_SPIDER_DIGITAL_OUTPUTS* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetDigitalOutput(IN HCAMERA hCamera, IN ST_SPIDER_DIGITAL_OUTPUT_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_SetDigitalOutputToggling(IN HCAMERA hCamera, IN UINT nID, IN bool bEnable, IN LONG nlIntervalMilliSec, IN LONG nlMaxDurationSec);
	SPIDERCTRLAPI	long SPIDER_ExeDigitalOutput(IN HCAMERA hCamera, IN UINT nID, IN BOOL bActive);

	// PPPoE
	SPIDERCTRLAPI	long SPIDER_GetPPPoE(IN HCAMERA hCamera, OUT CHAR* pszUserName);
	SPIDERCTRLAPI	long SPIDER_SetPPPoE(IN HCAMERA hCamera, IN CHAR* pszUserName, IN CHAR* pszPwd);

	// Iframe interval
	SPIDERCTRLAPI	long SPIDER_GetIFrameItv(IN HCAMERA hCamera, OUT LONG* pnlStream0, OUT LONG* pnlStream1, OUT LONG* pnlStream2, OUT LONG* pnlStream3 = NULL);
	SPIDERCTRLAPI	long SPIDER_SetIFrameItv(IN HCAMERA hCamera, IN LONG nlStream0, IN LONG nlStream1, IN LONG nlStream2, IN LONG nlStream3 = 0);		// Range 1 ~ 300

	// Auto gain control
	SPIDERCTRLAPI	long SPIDER_GetAutoGainCtrl(IN HCAMERA hCamera, OUT AUTO_GAIN_TYPE* pAutoGainType);
	SPIDERCTRLAPI	long SPIDER_SetAutoGainCtrl(IN HCAMERA hCamera, IN AUTO_GAIN_TYPE AutoGainType);

	// Pre Post Record
	SPIDERCTRLAPI	long SPIDER_GetPrePostRecord(IN HCAMERA hCamera, OUT BOOL* pbEnable, OUT RECORD_TYPE* pRecordType);
	SPIDERCTRLAPI	long SPIDER_SetPrePostRecord(IN HCAMERA hCamera, IN BOOL bEnable, IN RECORD_TYPE RecordType);

	// SNMP
	SPIDERCTRLAPI	long SPIDER_GetSNMP(IN HCAMERA hCamera, OUT BOOL* pbEnable, OUT BOOL* pbCstEnable, OUT CHAR* pszCstIPAddr);
	SPIDERCTRLAPI	long SPIDER_SetSNMP(IN HCAMERA hCamera, IN BOOL bEnable, IN BOOL bCstEnable, IN CHAR* pszCstIPAddr);

	// Strobe Night Only
	SPIDERCTRLAPI	long SPIDER_GetStrobeNightOnly(IN HCAMERA hCamera, OUT BOOL* pbEnable);
	SPIDERCTRLAPI	long SPIDER_SetStrobeNightOnly(IN HCAMERA hCamera, IN BOOL bEnable);

	// Strobe Strength
	SPIDERCTRLAPI	long SPIDER_GetStrobeStrength(IN HCAMERA hCamera, OUT LONG* pnlValue);
	SPIDERCTRLAPI	long SPIDER_SetStrobeStrength(IN HCAMERA hCamera, IN LONG nlValue);				// Range 1 ~ 4

	// Thermal Control
	SPIDERCTRLAPI	long SPIDER_GetThermalCtrlInfo(IN HCAMERA hCamera, OUT ST_SPIDER_THERMAL_COMPONENTS* pInfo);
	SPIDERCTRLAPI	long SPIDER_GetThermalCtrlValue(IN HCAMERA hCamera, OUT ST_SPIDER_THERMAL_CTRL_ADV* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetThermalCtrlValue(IN HCAMERA hCamera, IN ST_SPIDER_THERMAL_CTRL_ADV stInfo);
	SPIDERCTRLAPI	long SPIDER_GetThermalEntireRegionAlarm(IN HCAMERA hCamera, OUT ST_SPIDER_THERMAL_ALARM* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetThermalEntireRegionAlarm(IN HCAMERA hCamera, IN ST_SPIDER_THERMAL_ALARM stInfo);
	SPIDERCTRLAPI	long SPIDER_GetThermalRegionInfo(IN HCAMERA hCamera, OUT ST_SPIDER_THERMAL_REGION_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetThermalRegion(IN HCAMERA hCamera, IN ST_SPIDER_THERMAL_REGION stInfo);
	SPIDERCTRLAPI	long SPIDER_GetThermalRegionTemp(IN HCAMERA hCamera, OUT ST_SPIDER_THERMAL_REGION_TEMP_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_GetThermalCorrectInfo(IN HCAMERA hCamera, OUT ST_SPIDER_THERMAL_CORRECT_INFO* pInfo);
	SPIDERCTRLAPI	long SPIDER_SetThermalCorrectInfo(IN HCAMERA hCamera, IN ST_SPIDER_THERMAL_CORRECT_INFO stInfo);
	SPIDERCTRLAPI	long SPIDER_ExeThermalNUC(IN HCAMERA hCamera);

}; // extern




#endif



