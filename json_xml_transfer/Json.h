#pragma once
#include "cJSON.h"
#include <string>
using namespace std;
class CJson
{
public:
	CJson(void);
	virtual ~CJson(void);
	static string Json2Xml(const string &strJson);
	
	static string Xml2Json(const string &strxml);
	static void remove_char(string &str, char c);  //清除指定字符
	static string&  replace_all(string& str, const string& old_value, const string& new_value);
	static string GetXmlKey(const string &strxml);
	static string GetXmlValueFromKey(const string &strxml, const string &strKey);
	static string GoToNextItem(const string &strxml, const string &strKey);
	static int GetArrayItem(const string stxml);   //获取xml文件对应的json格式中数组元素的个数
	static string GetNumFromJson(const string& strJson, const string& strKey); //根据key从json字符串中获取数字字符
};
