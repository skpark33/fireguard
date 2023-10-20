/* 이 파일은  GuardianCenter 과 fireGuardGraph  프로젝트에서 공통으로 쓰이는 파일이다. 항상 동일해야 한다.*/
#ifndef _sockUtil_h_
#define _sockUtil_h_

#include <cstdio>
#include <string>
#include <list>
#include <map>
#include <fstream>

#ifdef _COP_LINUX_
#include <ci/libMSCCompat/ciMSCCompatTypes.h>
#endif

#define	SOCK_DATA_TERMINATOR	'\n'
#define	SOCK_DATA_STARTER		'?'
#define	SOCK_DATA_ACK			"ACK"

#define CLASS_REGACY			"C_RE"
#define CLASS_HOSTINFO			"C_SI"
#define CLASS_DOWNLOADSTATE		"C_DN"
#define CLASS_ACK				"C_AK"
#define CLASS_BYPASS			"C_BP"

#define MAX_BUFFER_LEN			2048

#ifndef _COP_MSC_
using namespace std;
#endif
#ifndef __GUARDIAN_CENTER__
using namespace std;
#endif


class sockUtil {
public:

	static sockUtil*	getInstance();
	static void	clearInstance();

	virtual ~sockUtil() ;

	// include connect/talk/hear/disconnect
	boolean dialog(const char* ipAddress,
				 int portNo, 
				 int waitTime,
				 const char* input,
				 string& output);
	boolean dialog(const char* ipAddress,
				 int portNo, 
				 int waitTime,
				 const char* input,
				 string& output,
				 string& errMsg ,
				 int timeoutMillis = 0
				 );

	boolean sock_connect(const char* ipAddress, int portNo, SOCKET& socket_fd);
	boolean sock_close(SOCKET socket_fd);

	boolean talk(SOCKET socket_fd, const char* input, const char* classCode, boolean interative);
	boolean talkLong(SOCKET socket_fd, const char* input, const char* classCode, boolean interative);
	boolean hear(SOCKET socket_fd, int waitTime, string& output);
	boolean hear(SOCKET socket_fd, int waitTime, string& output,string& err,
						string& classCode, boolean& interative);
	//boolean hear(SOCKET socket_fd, int waitTime, list<string>& outputList,string& err);
	void	addHeader(const char* input, const char* classCode, boolean interactive, string& output);

	const char*	getLastError() { return _errStr.c_str(); }
	SOCKET ConnectWithTimeout(const sockaddr* serverAddr, int timeoutMillis);

protected:
	sockUtil();
	static sockUtil*	_instance;

	string _errStr;

};

#endif // _scratchUtil_h_
