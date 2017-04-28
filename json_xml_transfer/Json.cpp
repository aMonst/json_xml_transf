#include "StdAfx.h"
#include "Json.h"
#include <iostream>
using namespace std;

CJson::CJson(void)
{

}

CJson::~CJson(void)
{

}

void CJson::remove_char(string &str, char c)
{
	while (1)
	{
		int nPos = str.find_first_of(c);
		if (nPos != string::npos)
		{
			str = str.substr(0, nPos) + str.substr(nPos + 1, -1);
		}
		else
			break;
	}
}

string&  CJson::replace_all(string& str, const string& old_value, const string& new_value)
{
	while (true)
	{
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return   str;
}

string CJson::Json2Xml(const string &strJson)
{
	string strXml = "";
	cJSON *root = cJSON_Parse(strJson.c_str());
	if (!root)
	{
		return "";
	}

	cJSON *pNext = root->child;
	if (!pNext)
	{
		return strJson;
	}

	int nPos = 0;
	while (pNext)
	{
		string strChild = cJSON_Print(pNext);
		string strVal = Json2Xml(strChild);

		if (pNext->string != NULL)
		{
			string strKey = pNext->string;
			if ((nPos = strKey.find("-")) == 0)
			{
				// 属性项
				strXml.append(" ");
				strXml.append(strKey.substr(1));
				strXml.append("=");
				strXml.append(strVal);

				if (pNext->next == NULL)
					strXml.append(">");
			}
			else if ((nPos = strKey.find("#")) == 0)
			{
				// 值  
				strXml.append(">");
				strXml.append(strVal);
			}
			else if ((int)(strVal.find("=")) > 0 /*&& (int)(strVal.find("<")) < 0*/)
			{
				// 包含属性项的键值对  
				strXml.append("<" + strKey);
				strXml.append(strVal);
				strXml.append("</" + strKey + ">");
			}
			else
			{
				// 修正底层无键的值数组的键，如：把<JUAN_XJ_preKey>123</JUAN_XJ_preKey>中的JUAN_XJ_preKey修正  
				if ((int)strVal.find("JUAN_XJ_preKey") >= 0)
				{
					replace_all(strVal, "JUAN_XJ_preKey", strKey);
					strXml.append(strVal);
				}
				else
				{
					strXml.append("<" + strKey + ">");
					strXml.append(strVal);
					strXml.append("</" + strKey + ">");
				}
			}
		}
		else
		{
			// 不包含键的值数组， 如：["123", "456"]，暂时转为<JUAN_XJ_preKey>123</JUAN_XJ_preKey>  
			string strPreKey = "JUAN_XJ_preKey";
			strXml.append("<" + strPreKey + ">");
			strXml.append(strVal);
			strXml.append("</" + strPreKey + ">");
		}

		pNext = pNext->next;
	}

	return strXml;
}

//暂时不考虑xml标签中存在属性值的问题
string CJson::Xml2Json(const string &strxml)
{
	cJSON *pJsonRoot = cJSON_CreateObject();
	
	string strNext = strxml;
	int nPos = 0;
	while ((nPos = strNext.find("<")) != -1)
	{
		string strKey = GetXmlKey(strNext);
		string strValue = GetXmlValueFromKey(strNext, strKey);
		string strCurrXml = strNext;
		strNext = GoToNextItem(strNext, strKey);
		
		if (strValue != "" && strValue.find("<") != -1)
		{
			//里面还有标签
			string strNextKey = GetXmlKey(strNext);
			//下一个的标签与这个相同，则为一个数组
			if (strNextKey == strKey)
			{
				cJSON *pArrayObj = cJSON_CreateArray();
				int nCnt = GetArrayItem(strCurrXml);
				for (int i = 0; i < nCnt; i++)
				{
					strKey = GetXmlKey(strCurrXml);
					strValue = GetXmlValueFromKey(strCurrXml, strKey);
					string strArrayItem = Xml2Json(strValue);
					cJSON *pArrayItem = cJSON_Parse(strArrayItem.c_str());
					cJSON_AddItemToArray(pArrayObj, pArrayItem);
					strCurrXml = GoToNextItem(strCurrXml, strKey);
				}

				cJSON_AddItemToObject(pJsonRoot, strNextKey.c_str(), pArrayObj);
				strNext = strCurrXml;
			}else
			{
				//否则为普通对象
				string strSubJson = Xml2Json(strValue);
				cJSON *pSubJsonItem = cJSON_CreateObject();
				pSubJsonItem = cJSON_Parse(strSubJson.c_str());
				cJSON_AddItemToObject(pJsonRoot, strKey.c_str(), pSubJsonItem);
			}
		}
		else
		{
			if (strValue.find("\"") == -1) //这个是数字
			{
				cJSON_AddNumberToObject(pJsonRoot, strKey.c_str(), atof(strValue.c_str()));
			}else
			{
				remove_char(strValue, '\"');
				cJSON_AddStringToObject(pJsonRoot, strKey.c_str(), strValue.c_str());
			}
		}
	}

	string strJson = cJSON_Print(pJsonRoot);
	return strJson;
}

string CJson::GetXmlKey(const string &strxml)
{
	int nKeyS = strxml.find("<");
	int nKeyE = strxml.find(">");
	if (nKeyE == -1 || nKeyS == -1)
	{
		return "";
	}

	string strKey = strxml.substr(nKeyS + 1, nKeyE - nKeyS - 1);
	return strKey;
}

string CJson::GetXmlValueFromKey(const string &strxml, const string &strKey)
{
	int nValueE = strxml.find("</" + strKey + ">");
	int nValueS = strxml.find(">");

	if (-1 == nValueE || nValueS == -1) //没有结尾标签
	{
		return "";
	}

	string strValue = strxml.substr(nValueS + 1, nValueE - nValueS - 1);
	return strValue;
}

string CJson::GoToNextItem(const string &strxml, const string &strKey)
{
	int nPos = strxml.find("</" +  strKey + ">");
	string strNext = strxml.substr(nPos + strKey.size() + 3);
	return strNext;
}

//获取相同标签的个数
int CJson::GetArrayItem(const string stxml)
{
	string strKey = GetXmlKey(stxml);
	int iCnt = 0;
	string strNextXml = stxml;
	string strNextKey = "";
	while (true)
	{
		strNextXml = GoToNextItem(strNextXml, strKey);
		strNextKey = GetXmlKey(strNextXml);

		if (strNextKey != strKey)
		{
			break;
		}
		iCnt++;
	}
	return iCnt + 1; //如果有n个标签相等，那么有 n + 1个元素
}