#pragma once

//pretty powerful string class.
//dummy interface.

#include "XStatus.h"
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;

#define DEFAULT_CHAR_ARRAY_LENGTH 1
#define WCHAR_CHAR_TRANSFROM_EXTRA_BYTE 1
#define GETLENGTH_DETECTION_LENGTH 50000
#define ASCIICODE_TOTAL_LENGTH 128

typedef class XCharString
{
public:
	enum SearchAlgorithm {String_H, Brute_Force, KMP, BM};
	enum Unicode_Encoding{NONE = -1, UTF8, UTF16_BigEndian, UTF16_LittleEndian, UTF32_BigEndian, UTF32_LittleEndian};

private:
	XCharString(char* value, int string_length, int string_mem_length);

public:
	XCharString();
	XCharString(int length, bool clear_mem = true);
	XCharString(const XCharString& other);
	XCharString(XCharString&& local_other);
	XCharString(const char* value);
	XCharString(const char value);
	XCharString(const string& value);
	~XCharString();

	const char* c_str() const;
	const char* data() const;

	int size() const;
	int length() const;
	int count_times(const XCharString& to_search) const;
	int count_times(const char& to_search) const;
	int indexOf(const XCharString& to_search, int begin_pos = 0, SearchAlgorithm algorithm = SearchAlgorithm::String_H) const;
	int indexOf(const char & to_search, int begin_pos = 0) const;
	int indexOf_Prev(const char & to_search, int begin_pos) const;
	XCharString substring(int start) const;
	XCharString substring(int start, int end) const;
	XCharString& concat(const XCharString& other);
	XCharString& concat(const char * other);
	XCharString Html_UnicodeString_to_XCharString();
	void Html_Pop_Markups();
	Status assign(const XCharString& other);
	Status assign(XCharString&& local_other);
	Status pop_out(int start, int end);
	void swap(XCharString& other);
	int replace(char from, char to);
	void reverse();
	void clear();
	int getHashCode() const;

	bool operator== (const XCharString& other) const;
	bool operator!= (const XCharString& other) const;
	bool operator> (const XCharString& other) const;
	bool operator< (const XCharString& other) const;
	XCharString& operator=(const XCharString& other);
	XCharString& operator=(XCharString&& other);
	XCharString operator+(const XCharString& other) const;
	XCharString& operator+=(const XCharString& other);
	XCharString& operator+=(const char* other);
	XCharString operator+(const char* other) const;
	const char& operator[](int pos) const;

	operator int() const;
	operator bool() const;
	
	friend XCharString operator+(const char* str1, XCharString str2);
	friend ostream& operator<< (ostream& out, const XCharString& to_out);
	friend istream& operator >> (istream& in, XCharString& to_in);

	//Debug
	void PrintCharCodes() const;

private:
	void l_assign(XCharString& other);

	char* content_string;
	int string_length;
	int string_mem_length;

public:
	static size_t WCharStr_to_CharStr(char* dest, wchar_t* src, int wlen);
	static size_t CharStr_to_WCharStr(wchar_t* dest, char* src, int clen);
	static size_t UTF8_to_UTF16(wchar_t* dest, char* src, int clen);
	static Unicode_Encoding CheckBom(char* src, int len);

	static Status GetInt(char* source, int len, int& out_value);
	static Status GetInt(wchar_t* source, int len, int& out_value);

	static int GetLength(wchar_t* source);
	static int GetLength(char* source);

	static XCharString ToString(const int& value);
}xstring;

