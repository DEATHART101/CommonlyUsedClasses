#pragma once

//customizable socket function, simplify oprations.

#include <winsock2.h>
#include <utility>
#include <Windows.h>
#include <iostream>
#include "XCharString.h"

#define WSWENS MAKEWORD(2, 0)

typedef class XSocket
{
public:
	enum HTTP_FUNCTION { GET};

public:
	XSocket();
	~XSocket();

	static bool Init();
	static void UnInit();
	static SOCKET MakeSocket(xstring host, int port);
	static xstring MakeHTTP(const xstring& host, HTTP_FUNCTION func, const xstring& content, bool connection = false);

	static xstring URLtoHost(const xstring& url);
	static xstring URLtoContent(const xstring& url);
	static void URLtoHost_Content(const xstring& url, xstring& host, xstring& content);
	
}xsocket;

