// json_xml_transfer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Json.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string strjson = "{\
		\"ip\": \"202.2.1.2\",\
		\"maliciousActType\": 1,\
		\"activity\": 1,\
		\"discoveryTime\": \"2016-01-20 22:02:03\",\
		\"updateTime\": \"2016-01-20 22:02:03\",\
		\"infoSource\": \"cnvd\",\
		\"credit\": 75,\
		\"fileMd5\": \"\",\
		\"serviceInfo\": [\
			{\
				\"serviceBanner\": \"SSH-2.0-OpenSSH_6.0p1 Debian-4+deb7u2\",\
				\"serviceProduct\": \"OpenSSH\",\
				\"serviceVendor\": \"OpenBSD\",\
				\"seviceVersion\": \"6.0p1\",\
				\"serviceCpe\": \"cpe:/a:microsoft:internet_explorer:10\",\
				\"protocol\": \"SSH\",\
				\"port\": 22\
			},\
			{\
				\"serviceBanner\": \"SSH-2.0-OpenSSH_6.0p1 Debian-4+deb7u2\",\
				\"serviceProduct\": \"OpenSSH\",\
				\"serviceVendor\": \"OpenBSD\",\
				\"seviceVersion\": \"6.0p1\",\
				\"serviceCpe\": \"cpe:/a:microsoft:internet_explorer:10\",\
				\"protocol\": \"SSH\",\
				\"port\": 222222\
			},\
			{\
				\"serviceBanner\": \"SSH-2.0-OpenSSH_6.0p1 Debian-4+deb7u2\",\
				\"serviceProduct\": \"OpenSSH\",\
				\"serviceVendor\": \"OpenBSD\",\
				\"seviceVersion\": \"6.0p1\",\
				\"serviceCpe\": \"cpe:/a:microsoft:internet_explorer:10\",\
				\"protocol\": \"SSH\",\
				\"port\": {\
					\"remote\" : 222,\
					\"local\": 223\
				}\
			}\
		],\
		\"osInfo\": {\
			\"osProduct\": \"linux\",\
			\"osVendor\": \"Debian\",\
			\"osVersion\": \"7.0\",\
			\"osCpe\": \"cpe:/o:microsoft:windows_7:-:sp1\"\
		},\
		\"desc\": \"sshd[24804]: last message repeated 6 times ,Did not receive identification string from 103.230.222.254\",\
		\"customInfo\": {\
			\"key1\": \"value1\",\
			\"key2\": \"value2\"\
		}\
	}";

	string strxml = CJson::Json2Xml(strjson);
	
	cout<<strxml<<endl;

	strjson = CJson::Xml2Json(strxml);
	cout<<strjson<<endl;

	return 0;
}