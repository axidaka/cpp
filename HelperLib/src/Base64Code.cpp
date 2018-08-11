#include "stdafx.h"
#include "Base64Code.h"

namespace duowanTool
{
	std::string EncodeBase64(const std::string& strSrc)
	{
		if (strSrc.empty())
			return std::string();
		const char* Data = strSrc.c_str();
		int len = (int)strSrc.size();

		return EncodeBase64((unsigned char*)Data, len);
	}

	std::string DecodeBase64(const std::string& strSrc)
	{
		if (strSrc.empty())
			return std::string();

		int len = (int)strSrc.size();
		const char* Data = strSrc.c_str();

		return DecodeBase64((unsigned char*)Data, len);

	}

	std::string EncodeBase64(const unsigned char* Data, int len)
	{
		std::string strEncode;

		//编码表
		const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		unsigned char Tmp[3]={0};
		int LineLength=0;

		for(int i=0;i<(int)(len / 3);i++)
		{
			Tmp[0] = *Data++;
			Tmp[1] = *Data++;
			Tmp[2] = *Data++;

			strEncode+= EncodeTable[Tmp[0] >> 2];
			strEncode+= EncodeTable[((Tmp[0] << 4) | (Tmp[1] >> 4)) & 0x3F];
			strEncode+= EncodeTable[((Tmp[1] << 2) | (Tmp[2] >> 6)) & 0x3F];
			strEncode+= EncodeTable[Tmp[2] & 0x3F];
			//if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
		}

		//对剩余数据进行编码
		int Mod = len % 3;
		if(Mod == 1)
		{
			Tmp[0] = *Data++;
			strEncode += EncodeTable[(Tmp[0] & 0xFC) >> 2];
			strEncode += EncodeTable[((Tmp[0] & 0x03) << 4)];
			strEncode += "==";
		}
		else if(Mod==2)
		{
			Tmp[0] = *Data++;
			Tmp[1] = *Data++;
			strEncode += EncodeTable[(Tmp[0] & 0xFC) >> 2];
			strEncode += EncodeTable[((Tmp[0] & 0x03) << 4) | ((Tmp[1] & 0xF0) >> 4)];
			strEncode += EncodeTable[((Tmp[1] & 0x0F) << 2)];
			strEncode += "=";
		}

		return strEncode;
	}

	std::string DecodeBase64(const unsigned char* Data, int len)
	{
		std::string strDecode;

		//解码表
		const char DecodeTable[] =
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			62, // '+'
			0, 0, 0,
			63, // '/'
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
			0, 0, 0, 0, 0, 0, 0,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
			0, 0, 0, 0, 0, 0,
			26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
			39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
		};

		int nValue;
		int i= 0;

		while (i < len)
		{
			if (*Data != '\r' && *Data!='\n')
			{
				nValue = DecodeTable[*Data++] << 18;
				nValue += DecodeTable[*Data++] << 12;
				strDecode+=(nValue & 0x00FF0000) >> 16;

				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++] << 6;
					strDecode+=(nValue & 0x0000FF00) >> 8;

					if (*Data != '=')
					{
						nValue += DecodeTable[*Data++];
						strDecode+=nValue & 0x000000FF;
					}
				}
				i += 4;
			}
			else// 回车换行,跳过
			{
				Data++;
				i++;
			}
		}

		return strDecode;
	}


    std::string EncodeMyBase64(const std::string& strSrc)
	{
		if (strSrc.empty())
			return std::string();
		const char* Data = strSrc.c_str();
		int len = (int)strSrc.size();

		return EncodeMyBase64((unsigned char*)Data, len);
	}

    std::string DecodeMyBase64(const std::string& strSrc)
	{
		if (strSrc.empty())
			return std::string();

		int len = (int)strSrc.size();
		const char* Data = strSrc.c_str();
		
		return DecodeMyBase64((unsigned char*)Data, len);
	
	}

    std::string EncodeMyBase64(const unsigned char* Data, int len)
	{
	    std::string strEncode;

		//编码表		
		const char EncodeTable[]="AaBcCbDdEfFeGgHiIhJjKkLmMlNnOoPpQqRsSrTtUuVvWxXwYyZz0213498675+/";

		unsigned char Tmp[3]={0};
		int LineLength=0;

		for(int i=0;i<(int)(len / 3);i++)
		{
			Tmp[0] = *Data++;
			Tmp[1] = *Data++;
			Tmp[2] = *Data++;

			strEncode+= EncodeTable[Tmp[0] >> 2];
			strEncode+= EncodeTable[((Tmp[0] << 4) | (Tmp[1] >> 4)) & 0x3F];
			strEncode+= EncodeTable[((Tmp[1] << 2) | (Tmp[2] >> 6)) & 0x3F];
			strEncode+= EncodeTable[Tmp[2] & 0x3F];
			//if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
		}

		//对剩余数据进行编码
		int Mod = len % 3;
		if(Mod == 1)
		{
			Tmp[0] = *Data++;
			strEncode += EncodeTable[(Tmp[0] & 0xFC) >> 2];
			strEncode += EncodeTable[((Tmp[0] & 0x03) << 4)];
			strEncode += "?}";
		}
		else if(Mod==2)
		{
			Tmp[0] = *Data++;
			Tmp[1] = *Data++;
			strEncode += EncodeTable[(Tmp[0] & 0xFC) >> 2];
			strEncode += EncodeTable[((Tmp[0] & 0x03) << 4) | ((Tmp[1] & 0xF0) >> 4)];
			strEncode += EncodeTable[((Tmp[1] & 0x0F) << 2)];
			strEncode += "?";
		}

		return strEncode;
	}

    std::string DecodeMyBase64(const unsigned char* Data, int len)
	{
	    std::string strDecode;

		//解码表
		const char DecodeTable[] =
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			62, // '+'
			0, 0, 0,
			63, // '/'
			52, 54, 53, 55, 56, 61, 59, 60, 58, 57, // '0'-'9'
			0, 0, 0, 0, 0, 0, 0,
			0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24,
			26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, // 'A'-'Z'
			0, 0, 0, 0, 0, 0,
			1, 5, 3, 7, 11, 9, 13, 17, 15, 19, 21, 25, 23,
			27, 29, 31, 33, 37, 35, 39, 41, 43, 47, 45, 49, 51, // 'a'-'z'
		};

		int nValue;
		int i= 0;

		while (i < len)
		{
			if (*Data != '\r' && *Data!='\n')
			{
				nValue = DecodeTable[*Data++] << 18;
				nValue += DecodeTable[*Data++] << 12;
				strDecode+=(nValue & 0x00FF0000) >> 16;

				if (*Data != '?')
				{
					nValue += DecodeTable[*Data++] << 6;
					strDecode+=(nValue & 0x0000FF00) >> 8;

					if (*Data != '}')
					{
						nValue += DecodeTable[*Data++];
						strDecode+=nValue & 0x000000FF;
					}
				}
				i += 4;
			}
			else// 回车换行,跳过
			{
				Data++;
				i++;
			}
		}

		return strDecode;
	}

}
