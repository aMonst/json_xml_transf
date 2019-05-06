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
	cJSON *pRoot = cJSON_Parse(strJson.c_str());
	if (NULL == pRoot)
	{
		return "";
	}

	cJSON *pChild = pRoot->child;
	while (pChild != NULL)
	{
		if (pChild->child != NULL) //�����ӽڵ�����
		{
			if (cJSON_Array == pChild->type) //�������������
			{
				std::string strArrayValue = "";
				int nSize = cJSON_GetArraySize(pChild);
				for (int i = 0; i < nSize; i++)
				{
					cJSON *item = cJSON_GetArrayItem(pChild, i);
					std::string strValue = Json2Xml(cJSON_Print(item));
					strArrayValue += strValue;
				}

				strXml = strXml + "<" + pChild->string + ">" + strArrayValue + "</" + pChild->string + ">";
				return strXml;
			}else
			{
				std::string strSubKey = pChild->string; //��ȡ���ļ�

				std::string strSubValue = Json2Xml(cJSON_Print(pChild)); //��ȡ����ֵ
				std::string strSubXml = "<" + strSubKey + ">" + strSubValue + "</" + strSubKey + ">";
				strXml += strSubXml;
			}
		}else
		{
			std::string strKey = pChild->string;
			std::string strVal = "";
			if (pChild->valuestring != NULL)
			{
				string strTemp = pChild->valuestring;
				strVal = "\"" + strTemp + "\"";
			}else
			{
				//���������Ϊ��������
				strVal = cJSON_Print(pChild);
			}
			
			strXml = strXml + "<" + strKey + ">" + strVal + "</" + strKey + ">";
			
		}

		pChild = pChild->next;
	}

	if(NULL != pRoot)
	{
		cJSON_Delete(pRoot);
	}

	return strXml;
}

//��ʱ������xml��ǩ�д�������ֵ������
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
		int LabelPos = strValue.find("<"); // < ����λ��
		int nMarkPos = strValue.find("\""); // " ����λ��
		if (strValue != "" && LabelPos != -1 && LabelPos < nMarkPos) //���ų����ڱ�ǩ֮��
		{
			//���滹�б�ǩ
			string strNextKey = GetXmlKey(strNext);
			//��һ���ı�ǩ�������ͬ����Ϊһ������
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
				//����Ϊ��ͨ����
				string strSubJson = Xml2Json(strValue);
				cJSON *pSubJsonItem = cJSON_CreateObject();
				pSubJsonItem = cJSON_Parse(strSubJson.c_str());
				cJSON_AddItemToObject(pJsonRoot, strKey.c_str(), pSubJsonItem);
			}
		}
		else
		{
			if (strValue.find("\"") == -1) //���������
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
	if (NULL != pJsonRoot)
	{
		cJSON_Delete(pJsonRoot);
	}
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

	if (-1 == nValueE || nValueS == -1) //û�н�β��ǩ
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

//��ȡ��ͬ��ǩ�ĸ���
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
	return iCnt + 1; //�����n����ǩ��ȣ���ô�� n + 1��Ԫ��
}

string CJson::GetNumFromJson(const string& strJson, const string& strKey)
{
	string strNum = "";
	int nPos = strJson.find(strKey);
	if (nPos != string::npos)
	{
		//���ҵ���Ӧ��ֵ
		string strItem = strJson.substr(nPos);
		int nFirst = strItem.find(":");
		int nEnd = 0;
		const char *pTemp = strItem.c_str();
		while (NULL != pTemp && '\0' != *pTemp && (!isalpha(*pTemp) || ' ' == *pTemp)) //Ϊ���ֻ��߿ո�
		{
			pTemp++;
		}
		return strItem.substr(nFirst, nEnd - nFirst);
	}
	return "";
}