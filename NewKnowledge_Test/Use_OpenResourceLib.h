#pragma once

//#ifdef _DEBUG
//#pragma comment(lib, "json_libmtd.lib")
//#else
//#pragma comment(lib, "json_libmt.lib")
//#endif // _DEBUG
//
//#include "json.h"
//#include <fstream>      // std::ifstream
//#include <sstream>      // std::stringstream, std::stringbuf
//
//void Use_Json_Reader()
//{
//	Json::Reader readerTest;
//	Json::Value root;
//
//	//fflush(stdin);
//	wstring DirPath = ToolFunc::Self.GetRootDirectory().substr(0, ToolFunc::Self.GetRootDirectory().find_last_of(L"\\") + 1)
//		+ L"NewKnowledge_Test\\JsonTest\\";
//
//	wstring filePath = DirPath + L"reader.txt";
//
//	ifstream inFile(filePath.c_str());
//
//	if (readerTest.parse(inFile, root, true))
//	{
//		Json::Value jjj = root["JudgeEmpty"];
//		bool kk24 = jjj.isNull();
//		bool ks3 = jjj.empty();
//
//		if (root.isMember("JudgeEmpty"))
//		{
//			Json::Value jjj = root["JudgeEmpty"];
//			string kkk23 = jjj.asString();
//		}
//
//		if (root.isMember("NoExit"))
//		{
//			Json::Value g22 = root["NoExit"];
//			string kk255 = g22.asString();
//		}
//
//		string JsonId = root["JsonID"].asString();
//
//		cout << "JsonID: " << JsonId << endl;
//		Json::Value send = root["Send"];
//		if (!send["Items"].isNull())
//		{
//			string Id = send["ID"].asString();
//
//			cout << "Send:" << endl << "ID: " << Id << endl;
//			cout << "Items:" << endl;
//
//			Json::Value Items = send["Items"];
//			Json::Value::ArrayIndex ItemSize = Items.size();
//			for (Json::Value::ArrayIndex index = 0; index < ItemSize; index++)
//			{
//				cout << "Item " << index << endl;
//				cout << "Count: " << Items[index]["Count"].asInt() << endl;
//				cout << "Code: " << Items[index]["Code"].asString() << endl;
//				cout << "X: " << Items[index]["X"].asInt() << endl;
//				cout << "Y: " << Items[index]["Y"].asInt() << endl;
//
//			}
//		}
//	}
//}
//#include "UnicodeConv.h"
//void Use_Json_Reader3()
//{
//	Json::Reader readerTest;
//	Json::Value root;
//
//	//fflush(stdin);
//	wstring DirPath = ToolFunc::Self.GetRootDirectory().substr(0, ToolFunc::Self.GetRootDirectory().find_last_of(L"\\") + 1)
//		+ L"NewKnowledge_Test\\JsonTest\\";
//
//	wstring filePath = DirPath + L"reader3.txt";
//
//	ifstream inFile(filePath.c_str());
//
//	if (readerTest.parse(inFile, root, true))
//	{
//		Json::Value nameList = root["avaNameList"];
//		Json::Value::ArrayIndex ItemSize = nameList.size();
//		for (Json::Value::ArrayIndex index = 0; index < ItemSize; index++)
//		{
//			wstring sss = ConvertMultiBytesToUnicode(nameList[index].asString());
//			cout << nameList[index].asCString() << endl;
//		}
//	}
//}
//
//#include "writer.h"
//void Use_Json_Writer()
//{
//	Json::Value root;
//	Json::FastWriter fwriter;
//
//	Json::Value person;
//	person["name"] = "haha";
//	person["Age"] = 10;
//
//	cout << person.toStyledString() << endl;
//	cout << person.asString() << endl;
//
//	root.append(person);
//
//	std::string styleString = root.toStyledString();
//	std::string josn_file = fwriter.write(root);
//
//
//}

//#ifdef _DEBUG
//#pragma comment(lib, "zlib1.2.7d.lib")
//#else
//#pragma comment(lib, "zlib1.2.7.lib")
//#endif // _DEBUG
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <errno.h>
//#include <zlib.h>
///* Compress data */
//int zcompress(Bytef *data, uLong ndata,
//	Bytef *zdata, uLong *nzdata)
//{
//	z_stream c_stream;
//	int err = 0;
//
//	if (data && ndata > 0)
//	{
//		c_stream.zalloc = (alloc_func)0;
//		c_stream.zfree = (free_func)0;
//		c_stream.opaque = (voidpf)0;
//		if (deflateInit(&c_stream, Z_DEFAULT_COMPRESSION) != Z_OK) return -1;
//		c_stream.next_in = data;
//		c_stream.avail_in = ndata;
//		c_stream.next_out = zdata;
//		c_stream.avail_out = *nzdata;
//		while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata)
//		{
//			if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
//		}
//		if (c_stream.avail_in != 0) return c_stream.avail_in;
//		for (;;) {
//			if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
//			if (err != Z_OK) return -1;
//		}
//		if (deflateEnd(&c_stream) != Z_OK) return -1;
//		*nzdata = c_stream.total_out;
//		return 0;
//	}
//	return -1;
//}
//
///* Compress gzip data */
//int gzcompress(Bytef *data, uLong ndata,
//	Bytef *zdata, uLong *nzdata)
//{
//	z_stream c_stream;
//	int err = 0;
//
//	if (data && ndata > 0)
//	{
//		c_stream.zalloc = (alloc_func)0;
//		c_stream.zfree = (free_func)0;
//		c_stream.opaque = (voidpf)0;
//		if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
//			-MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
//		c_stream.next_in = data;
//		c_stream.avail_in = ndata;
//		c_stream.next_out = zdata;
//		c_stream.avail_out = *nzdata;
//		while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata)
//		{
//			if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
//		}
//		if (c_stream.avail_in != 0) return c_stream.avail_in;
//		for (;;) {
//			if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
//			if (err != Z_OK) return -1;
//		}
//		if (deflateEnd(&c_stream) != Z_OK) return -1;
//		*nzdata = c_stream.total_out;
//		return 0;
//	}
//	return -1;
//}
//
///* Uncompress data */
//int zdecompress(Byte *zdata, uLong nzdata,
//	Byte *data, uLong *ndata)
//{
//	int err = 0;
//	z_stream d_stream; /* decompression stream */
//
//	d_stream.zalloc = (alloc_func)0;
//	d_stream.zfree = (free_func)0;
//	d_stream.opaque = (voidpf)0;
//	d_stream.next_in = zdata;
//	d_stream.avail_in = 0;
//	d_stream.next_out = data;
//	if (inflateInit(&d_stream) != Z_OK) return -1;
//	while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
//		d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
//		if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
//		if (err != Z_OK) return -1;
//	}
//	if (inflateEnd(&d_stream) != Z_OK) return -1;
//	*ndata = d_stream.total_out;
//	return 0;
//}
//
///* HTTP gzip decompress */
//int httpgzdecompress(Byte *zdata, uLong nzdata,
//	Byte *data, uLong *ndata)
//{
//	int err = 0;
//	z_stream d_stream = { 0 }; /* decompression stream */
//	static char dummy_head[2] =
//	{
//		0x8 + 0x7 * 0x10,
//		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
//	};
//	d_stream.zalloc = (alloc_func)0;
//	d_stream.zfree = (free_func)0;
//	d_stream.opaque = (voidpf)0;
//	d_stream.next_in = zdata;
//	d_stream.avail_in = 0;
//	d_stream.next_out = data;
//	if (inflateInit2(&d_stream, 47) != Z_OK) return -1;
//	while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
//		d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
//		if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
//		if (err != Z_OK)
//		{
//			if (err == Z_DATA_ERROR)
//			{
//				d_stream.next_in = (Bytef*)dummy_head;
//				d_stream.avail_in = sizeof(dummy_head);
//				if ((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK)
//				{
//					return -1;
//				}
//			}
//			else return -1;
//		}
//	}
//	if (inflateEnd(&d_stream) != Z_OK) return -1;
//	*ndata = d_stream.total_out;
//	return 0;
//}
//
///* Uncompress gzip data */
//int gzdecompress(Byte *zdata, uLong nzdata,
//	Byte *data, uLong *ndata)
//{
//	int err = 0;
//	z_stream d_stream = { 0 }; /* decompression stream */
//	static char dummy_head[2] =
//	{
//		0x8 + 0x7 * 0x10,
//		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
//	};
//	d_stream.zalloc = (alloc_func)0;
//	d_stream.zfree = (free_func)0;
//	d_stream.opaque = (voidpf)0;
//	d_stream.next_in = zdata;
//	d_stream.avail_in = 0;
//	d_stream.next_out = data;
//	if (inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) return -1;
//	//if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
//	while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
//		d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
//		if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
//		if (err != Z_OK)
//		{
//			if (err == Z_DATA_ERROR)
//			{
//				d_stream.next_in = (Bytef*)dummy_head;
//				d_stream.avail_in = sizeof(dummy_head);
//				if ((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK)
//				{
//					return -1;
//				}
//			}
//			else return -1;
//		}
//	}
//	if (inflateEnd(&d_stream) != Z_OK) return -1;
//	*ndata = d_stream.total_out;
//	return 0;
//}
//
//bool MyGzipCompress(const string& srcStr, unsigned char** ppGzipCompress, uInt& avail_out)
//{
//	if (srcStr.empty()) return false;
//
//	z_stream c_stream;
//
//	c_stream.zalloc = (alloc_func)0;
//	c_stream.zfree = (free_func)0;
//	c_stream.opaque = (voidpf)0;
//	if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
//		-MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) return false;
//
//	uLong outputBound = 0;
//	outputBound = deflateBound(&c_stream, srcStr.size());
//
//	unsigned char* pOut = new unsigned char[outputBound];
//	memset(pOut, 0, outputBound);
//
//	c_stream.next_in = (Bytef*)srcStr.c_str();
//	c_stream.avail_in = srcStr.size();
//	c_stream.next_out = (Bytef*)pOut;
//	c_stream.avail_out = outputBound;
//
//	if ((deflate(&c_stream, Z_FINISH) != Z_STREAM_END) || (deflateEnd(&c_stream) != Z_OK))
//	{
//		delete[]pOut;
//		pOut = NULL;
//		return false;
//	}
//
//	*ppGzipCompress = pOut;
//	avail_out = c_stream.total_out;
//	
//
//	return true;
//}
//
//#define  BUFF_SIZE  1024*512   // 0.5M
///* Uncompress gzip data */
//bool MyGzipDecompress(const string& srcStr, string& dstStr)
//{
//	if (srcStr.empty())return false;
//
//	z_stream d_stream = { 0 }; /* decompression stream */
//	static char dummy_head[2] =
//	{
//		0x8 + 0x7 * 0x10,
//		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
//	};
//
//	unsigned char* pOut = new unsigned char[BUFF_SIZE];
//	memset(pOut, 0, BUFF_SIZE);
//
//	__try
//	{
//		d_stream.zalloc = (alloc_func)0;
//		d_stream.zfree = (free_func)0;
//		d_stream.opaque = (voidpf)0;
//		d_stream.next_in = (Bytef*)srcStr.c_str();;
//		d_stream.avail_in = 0;
//		d_stream.next_out = pOut;
//
//		uInt uTotalIn = srcStr.size();
//		uInt uTotalOut = BUFF_SIZE;
//
//		int err = 0;
//
//		if (inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) return false;
//		//if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
//		while (d_stream.total_out < uTotalOut && d_stream.total_in < uTotalIn) {
//			d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
//			if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
//			if (err != Z_OK)
//			{
//				if (err == Z_DATA_ERROR)
//				{
//					d_stream.next_in = (Bytef*)dummy_head;
//					d_stream.avail_in = sizeof(dummy_head);
//					if ((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK)
//					{
//						return false;
//					}
//				}
//				else return false;
//			}
//		}
//		if (inflateEnd(&d_stream) != Z_OK) return false;
//
//		dstStr.append((char*)pOut);
//	}
//	__finally
//	{
//		delete[]pOut;
//		pOut = NULL;
//	}
//
//	return true;
//}
//
//#define BUF_SIZE 65535
//
//void Use_Zlib_Test()
//{
//	cout << "Current Zlib Version: " << zlibVersion();
//	//wstring DirPath = ToolFunc::Self.GetRootDirectory().substr(0, ToolFunc::Self.GetRootDirectory().find_last_of(L"\\") + 1)
//	//	+ L"NewKnowledge_Test\\JsonTest\\";
//
//	//wstring filePath = DirPath + L"reader.txt";
//	//ifstream inFile(filePath.c_str());
//	//stringstream strStream;
//	//strStream << inFile.rdbuf();
//
//	//string srcStr = strStream.str();
//
//	//const char *data = srcStr.c_str();//"kjdalkfjdflkjdlkfjdklfjdlkfjlkdjflkdjflddajfkdjfkdfaskf;ldsfk;ldakf;ldskfl;dskf;ld";
//	//uLong ndata = strlen(data);
//	Bytef zdata[BUF_SIZE];
//	uLong nzdata = BUF_SIZE;
//	Bytef  odata[BUF_SIZE];
//	uLong nodata = BUF_SIZE;
//
//	//memset(zdata, 0, BUF_SIZE);
//	////if(zcompress((Bytef *)data, ndata, zdata, &nzdata) == 0)
//	//if (gzcompress((Bytef *)data, ndata, zdata, &nzdata) == 0)
//	//{
//	//	fprintf(stdout, "nzdata:%d %s\n", nzdata, zdata);
//	//	memset(odata, 0, BUF_SIZE);
//	//	//if(zdecompress(zdata, ndata, odata, &nodata) == 0)
//	//	if (gzdecompress(zdata, ndata, odata, &nodata) == 0)
//	//	{
//	//		fprintf(stdout, "%d %s\n", nodata, odata);
//	//	}
//	//}
//
//	wstring DirPath = ToolFunc::Self.GetRootDirectory().substr(0, ToolFunc::Self.GetRootDirectory().find_last_of(L"\\") + 1)
//		+ L"NewKnowledge_Test\\JsonTest\\";
//
//	wstring filePath = DirPath + L"reader.txt";
//	ifstream inFile(filePath.c_str());
//	stringstream strStream;
//	strStream << inFile.rdbuf();
//
//	string srcStr = strStream.str();
//	string kkkk = duowanTool::EncodeMyBase64(srcStr);
//	unsigned char* pGzipCompress = NULL;
//	uInt avail_out = 0;
//	MyGzipCompress(srcStr, &pGzipCompress, avail_out);
//
//	//char pp[2] = { '1', '2' };
//	string myEncodeBase64 = duowanTool::EncodeMyBase64(pGzipCompress, avail_out);
//	
//	string myDecodeBase64 = duowanTool::DecodeMyBase64(myEncodeBase64);
//
//	string kklo0;
//	MyGzipDecompress(myDecodeBase64, kklo0);
//
//	delete[]pGzipCompress;
//
//	char pkkk[6] = { 1, 2, 3, 0, 4, 5 };
//	string encodeBase64 = duowanTool::EncodeMyBase64((unsigned char*)pkkk, sizeof(pkkk));
//
//	string decodeBase64 = duowanTool::DecodeMyBase64(encodeBase64);
//}
