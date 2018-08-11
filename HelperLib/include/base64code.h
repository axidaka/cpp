////////////////////////////////////////////////////////
/*********************
@	base64 code
*********************/
#pragma once
#include <string>


namespace duowanTool
{
	std::string  EncodeBase64(const std::string& strSrc);
	std::string  DecodeBase64(const std::string& strSrc);

	std::string	 EncodeBase64(const unsigned char* Data, int len);
	std::string	 DecodeBase64(const unsigned char* Data, int len);


    std::string  EncodeMyBase64(const std::string& strSrc);
    std::string  DecodeMyBase64(const std::string& strSrc);

    std::string	 EncodeMyBase64(const unsigned char* Data, int len);
    std::string	 DecodeMyBase64(const unsigned char* Data, int len);
}

