#ifndef __GAME_PROXY_UNICODE_CONV_H__
#define __GAME_PROXY_UNICODE_CONV_H__

#pragma once

#include <string>
#include <wtypes.h>

#define CP_UNICODE		1200
#define CP_UNICODE_BE	1201

std::string ConvertUnicodeToMultiBytes(const std::wstring& wstr, UINT cp = GetACP());
std::wstring ConvertMultiBytesToUnicode(const std::string& astr, UINT cp = GetACP());

#endif
