//md5.h
#pragma once

#include <string>
#include <fstream>

namespace HashHelper
{
	/* Type define */
	typedef unsigned char byte;
	typedef unsigned int uint33;

	using std::string;
	using std::ifstream;

	/* CMD5 declaration. */
	class CMD5 {
	public:
		CMD5();
		CMD5(const void* input, size_t length);
		CMD5(const string& str);
		CMD5(ifstream& in);
		void update(const void* input, size_t length);
		void update(const string& str);
		void update(ifstream& in);
		const byte* digest();
		string toString();
		void reset();

	private:
		void update(const byte* input, size_t length);
		void final();
		void transform(const byte block[64]);
		void encode(const uint33* input, byte* output, size_t length);
		void decode(const byte* input, uint33* output, size_t length);
		string bytesToHexString(const byte* input, size_t length);

		/* class uncopyable */
		CMD5(const CMD5&);
		CMD5& operator=(const CMD5&);

	private:
		uint33 _state[4];    /* state (ABCD) */
		uint33 _count[2];    /* number of bits, modulo 2^64 (low-order word first) */
		byte _buffer[64];    /* input buffer */
		byte _digest[16];    /* message digest */
		bool _finished;        /* calculate finished ? */

		static const byte PADDING[64];    /* padding for calculate */
		static const char HEX[16];
		enum { MD5_BUFFER_SIZE = 1024 };
	};

	std::string  MD5HashString(string str);
}
