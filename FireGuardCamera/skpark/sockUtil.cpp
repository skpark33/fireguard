#include "stdAfx.h"
#   define VERSION_MAJOR         2
#   define VERSION_MINOR         0

#include "sockUtil.h"
#include "skpark/TraceLog.h"

sockUtil* 	sockUtil::_instance = 0; 

sockUtil*	
sockUtil::getInstance() {
	if(!_instance) {
		if(!_instance) {
			_instance = new sockUtil;
		}
	}
	return _instance;
}

void	
sockUtil::clearInstance() {
	if(_instance) {
		if(_instance) {
			delete _instance;
			_instance =0;
		}
	}
}

sockUtil::sockUtil() 
{
	TraceLog(("sockUtil()\n"));
	//// Load WinSock DLL 
	//WORD VersionRequested = MAKEWORD(VERSION_MAJOR, VERSION_MINOR);
	//WSADATA       WsaData;            
	//if ( WSAStartup(VersionRequested, &WsaData) != 0 ) {		
	//	WSACleanup();	
	//}
}
sockUtil::~sockUtil() 
{
	//WSACleanup();	
}

boolean 
sockUtil::sock_connect(const char* ipAddress, int portNo, SOCKET& socket_fd)
{
	TraceLog(("sock_connect(%s,%d)\n", ipAddress, portNo));

	socket_fd = -1;
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		TraceLog(("Can't open socket.\n"));
		return false;
	}

	struct sockaddr_in server_addr;	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ipAddress);
	server_addr.sin_port = htons(portNo);

	if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		TraceLog(("Can't connect IP server.[%s:%d]\n", ipAddress, portNo));
		closesocket(socket_fd);
		return false;
	}
	TraceLog(("socket connect (%s,%d)\n", ipAddress, portNo));

	return true;
}

boolean 
sockUtil::sock_close(SOCKET socket_fd)
{
	TraceLog(("sock_close(%d)\n", socket_fd));
	closesocket(socket_fd);	
	return true;
}


boolean
sockUtil::dialog(const char* ipAddress,
				 int portNo, 
				 int waitTime,
				 const char* input, 
				 string& output)
{
	//TraceLog(("dialog(%s,%d,%s)\n", ipAddress, portNo,input));

	SOCKET socket_fd;		
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		TraceLog(("Can't open socket.\n"));
		return false;
	}

	struct sockaddr_in server_addr;	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ipAddress);
	server_addr.sin_port = htons(portNo);

	if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		TraceLog(("Can't connect IP server.[%s:%d]\n", ipAddress, portNo));
		closesocket(socket_fd);
		return false;
	}
	TraceLog(("socket connect (%s,%d)\n", ipAddress, portNo));

	TraceLog(("send start (%s)\n", input));
	if(!talk(socket_fd,input,CLASS_REGACY,false)){
		TraceLog(("Can't send to server.[%s]\n", ipAddress));
		closesocket(socket_fd);
		return false;
	}
	TraceLog(("send end\n"));

	TraceLog(("receive start ()\n"));
	if(!hear(socket_fd,waitTime,output)){
		TraceLog(("Can't receive from server.[%s]\n", ipAddress));
		closesocket(socket_fd);
		return false;
	}
	TraceLog(("receive end\n"));

	closesocket(socket_fd);	
	return true;
}

boolean
sockUtil::dialog(const char* ipAddress,
				 int portNo, 
				 int waitTime,
				 const char* input, 
				 string& output,
				 string& errMsg)
{
	//TraceLog(("dialog(%s,%d,%s)\n", ipAddress, portNo,input));

	char err_buf[256];
	memset(err_buf,0x00,256);

	SOCKET socket_fd;		
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		sprintf(err_buf,"Can't open socket.\n");
		errMsg=err_buf;
		return false;
	}

	struct sockaddr_in server_addr;	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ipAddress);
	server_addr.sin_port = htons(portNo);

	if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		sprintf(err_buf,"Can't connect IP server.[%s:%d]\n", ipAddress, portNo);
		closesocket(socket_fd);
		errMsg=err_buf;
		return false;
	}
	//TraceLog(("socket connect (%s,%d)\n", ipAddress, portNo));

	//TraceLog(("send start (%s)\n", input));
	if(!talk(socket_fd,input,CLASS_REGACY,false)){
		sprintf(err_buf,"Can't send to server.[%s]\n", ipAddress);
		closesocket(socket_fd);
		errMsg=err_buf;
		return false;
	}
	//TraceLog(("send end\n"));

	//TraceLog(("receive start ()\n"));
	if(!hear(socket_fd,waitTime,output)){
		sprintf(err_buf,"Can't receive from server.[%s]\n", ipAddress);
		closesocket(socket_fd);
		errMsg=err_buf;
		return false;
	}
	//TraceLog(("receive end\n"));

	closesocket(socket_fd);	
	return true;
}


boolean
sockUtil::talk(SOCKET socket_fd, const char* input,const char* classCode, boolean interactive)
{
	int len = strlen(input);
	if(len==0){
		return false;
	}
	string buf = input;
	if(buf[len-1]!=SOCK_DATA_TERMINATOR){
		buf += SOCK_DATA_TERMINATOR;
	}

	string value;
	if(classCode ==string(CLASS_REGACY)){
		value = buf;
	}else{
		this->addHeader(buf.c_str(),classCode,interactive,value);
	}
	int msg_size;
	if ((msg_size = send(socket_fd, value.c_str(), (int)value.size(), 0)) <= 0) {
		TraceLog(("Can't send to server\n"));
		return false;
	}
	return true;
}

boolean
sockUtil::talkLong(SOCKET socket_fd, const char* input,const char* classCode, boolean interactive)
{
	_errStr = "";

	int len = strlen(input);
	if(len==0){
		_errStr = "send data size is 0";	
		return false;
	}
	string value = input;
	if(value[len-1]!=SOCK_DATA_TERMINATOR){
		value += SOCK_DATA_TERMINATOR;
	}

	if(classCode !=string(CLASS_REGACY)){
		string header;
		this->addHeader("",classCode,interactive,header);
		int msg_size;
		if ((msg_size = send(socket_fd, header.c_str(), (int)header.size(), 0)) <= 0) {
			_errStr = "Can't send header to server";
			return false;
		}
	}

	int start_pos = 0;
	while(len >0){
		int sendSize = MAX_BUFFER_LEN;
		if(len < MAX_BUFFER_LEN){
			sendSize = len;
		}
		int msg_size;
		if ((msg_size = send(socket_fd, value.substr(start_pos,sendSize).c_str(),sendSize, 0)) <= 0) {
			_errStr = "Can't send to server : " + value.substr(start_pos,sendSize);
			return false;
		}
		start_pos	+=  MAX_BUFFER_LEN;
		len			-=	MAX_BUFFER_LEN;
	}
	_errStr = "send succeed";
	return true;
}


boolean
sockUtil::hear(SOCKET socket_fd, int waitTime, string& output)
{
	string errString;
	string classCode;
	boolean interactive;
	return hear(socket_fd, waitTime, output, errString,classCode,interactive);

}

boolean
sockUtil::hear(SOCKET socket_fd, int waitTime, string& output, string& err, string& classCode, boolean& interactive)
{
	output = "";
	int bodySize = 0;
	int readSize = 0;
	
	timeval ReceiveTimeout;
	ReceiveTimeout.tv_sec  = waitTime;
	ReceiveTimeout.tv_usec = 0;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket_fd, &fds);

	//TraceLog(("Wait for return ...\n"));
	int iRC = select(2, &fds, NULL, NULL, &ReceiveTimeout);
	// Timeout
	if(iRC==0) {
		TraceLog(("TimeOut!!!\n"));
		err = "TimeOut!!!";
		return false;
	}
	// Error
	if(iRC < 0) {
		TraceLog(("Select Error!!!\n"));
		err = "Select Error!!!";
		return false;
	}
	if(!FD_ISSET(socket_fd, &fds)){
		TraceLog(("Somthing wrong!!!\n"));
		err = "Somthing wrong!!!";
		return false;
	}

	boolean terminated_normally = false;
	//TraceLog(("recv start:"));

	char init_buf[11];
	memset(init_buf,0x00,11);
	int msg_size = 0;
	if ((msg_size = recv(socket_fd, init_buf, 1, 0)) <= 0) {
		err = "Can't recv from server";
		TraceLog(("Can't recv from server\n"));
		return false;
	}
	if(strlen(init_buf)==0 || init_buf[0] == SOCK_DATA_TERMINATOR){
		err = "No Data";
		TraceLog(("No Datar\n"));
		return false;
	}
	//TraceLog(("First 1 byte = %s\n",init_buf));

	if(init_buf[0] == SOCK_DATA_STARTER){
		memset(init_buf,0x00,11);
		if ((msg_size = recv(socket_fd, init_buf, 10, 0)) <= 0) {
			err = "Can't recv from server";
			TraceLog(("Can't recv from server\n"));
			return false;
		}
		if(msg_size != 10){
			err = "Can't recv from server";
			TraceLog(("Can't recv from server\n"));
			return false;
		}
		//TraceLog(("NEW VERSION\n"));
		string bufStr = init_buf;
		interactive = atoi(bufStr.substr(0,1).c_str());
		classCode=bufStr.substr(1,4).c_str();
		bodySize=atoi(bufStr.substr(5,5).c_str());
		if(bodySize>MAX_BUFFER_LEN){
			readSize = MAX_BUFFER_LEN;
		}else{
			readSize = bodySize;
		}
	}else{
		//TraceLog(("OLD VERSION\n"));
		output += init_buf;
		interactive = false;
		classCode=CLASS_REGACY;
		bodySize=0;
		readSize=1;
	}

	//TraceLog(("classCode=%s,interactive=%d,bodySize=%d,", classCode.c_str(), interactive, bodySize);

	while(1){
		char buf[MAX_BUFFER_LEN+1];
		memset(buf,0x00,MAX_BUFFER_LEN+1);
		msg_size = 0;
		if ((msg_size = recv(socket_fd, buf, readSize, 0)) <= 0) {
			err = "Can't recv from server";
			TraceLog(("Can't recv from server\n"));
			break;
		}
		buf[msg_size]=0;
		//TraceLog((buf));//
		if( buf[msg_size-1] == SOCK_DATA_TERMINATOR){
			//TraceLog(("socket terminated with TERMINATOR\n"));//
			terminated_normally = true;
			if(msg_size > 1){
				buf[msg_size-1]=0;
				output += buf;
			}
			break;
		}
		output += buf;
		if(bodySize>0){
			bodySize = bodySize - msg_size;
			if(bodySize<= 0){
				TraceLog(("socket terminated without Anold -_-;;;\n"));//
				terminated_normally = true;
				break;
			}
			if(bodySize < readSize){
				readSize = bodySize;
			}
		}
	}
	//TraceLog((" receive %d byte\n", output.size()));
	if(output.empty()){
		err = err + ", output empty!!!";
		return false;
	}
	// skpark 2010.09.13
	if(!terminated_normally){
		err = err + ", terminated abnormally!!!";
		return false;
	}
	return true;
	
}
/*
boolean
sockUtil::hear(SOCKET socket_fd, int waitTime, list<string>& outputList, string& err)
{
	timeval ReceiveTimeout;
	ReceiveTimeout.tv_sec  = waitTime;
	ReceiveTimeout.tv_usec = 0;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket_fd, &fds);

	TraceLog(("Wait for return ...\n"));
	int iRC = select(2, &fds, NULL, NULL, &ReceiveTimeout);
	// Timeout
	if(iRC==0) {
		TraceLog(("TimeOut!!!\n"));
		err = "TimeOut!!!";
		return false;
	}
	// Error
	if(iRC < 0) {
		TraceLog(("Select Error!!!\n"));
		err = "Select Error!!!";
		return false;
	}
	if(!FD_ISSET(socket_fd, &fds)){
		TraceLog(("Somthing wrong!!!\n"));
		err = "Somthing wrong!!!";
		return false;
	}
	boolean terminated_normally = false;
	TraceLog(("recv start : "));


	string output;
	while(1){
		char buf[2];
		memset(buf,0x00,2);
		//TraceLog(("."));
		int msg_size = 0;
		if ((msg_size = recv(socket_fd, buf, 1, 0)) <= 0) {
			err = "Can't recv from server";
			TraceLog(("Can't recv from server\n"));
			break;
		}
		buf[msg_size]=0;
		if(strlen(buf)==0 || buf[0] == SOCK_DATA_TERMINATOR){
			terminated_normally = true;
			//break;
			outputList.push_back(output);
			output = "";
			continue;
		}
		output += buf;
	}

	if(outputList.size()==0){
		err = err + ", output empty!!!";
		return false;
	}
	// skpark 2010.09.13
	if(!terminated_normally){
		err = err + ", terminated abnormally!!!";
		return false;
	}
	return true;
	
}
*/
void
sockUtil::addHeader(const char* input, const char* classCode, boolean interactive,string& output)
{
	char header[12];
	memset(header,0x00,12);
	sprintf(header,"%c%d%4.4s%05d",SOCK_DATA_STARTER,interactive, classCode,strlen(input));
	output = header;
	output += input;
}
