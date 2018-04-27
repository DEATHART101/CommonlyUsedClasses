#include "XSocket.h"

XSocket::XSocket()
{
}


XSocket::~XSocket()
{
}

bool XSocket::Init()
{
	WSADATA inet_WsaData;//1
	WSAStartup(MAKEWORD(2, 0), &inet_WsaData);//1
	if (LOBYTE(inet_WsaData.wVersion) != 2 || HIBYTE(inet_WsaData.wVersion) != 0)//1
	{
		UnInit();
		return false;
	}

	return true;
}

SOCKET XSocket::MakeSocket(xstring host, int port)
{
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);//1
	struct hostent * hp = ::gethostbyname(host.c_str());//2
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	memcpy(&saddr.sin_addr, hp->h_addr, 4);//3
	if (connect(tcp_socket, (const struct sockaddr *)&saddr, sizeof(saddr)) == -1)//1
	{
		cerr << "error in connection" << endl;
	}
	return tcp_socket;
}

xstring XSocket::MakeHTTP(const xstring& host, HTTP_FUNCTION func, const xstring& content, bool connection)
{
	xstring result(100);

	switch (func)
	{
	case XSocket::GET:
		result += "GET ";
		break;
	default:
		break;
	}

	result +=
		content + " HTTP/1.1\r\n" +
		"Host:" + host + "\r\n";
	
	if (connection)
	{
		result += "Connection:Open\r\n";
	}
	else
	{
		result += "Connection:Close\r\n";
	}

	result += "\r\n";

	return result;
}

xstring XSocket::URLtoHost(const xstring & url)
{
	int host_start = url.indexOf("//");
	if (host_start == -1)
	{
		host_start = 0;
	}
	else
	{
		host_start += 2;
	}

	int content_start = url.indexOf('/', host_start);
	return url.substring(host_start, content_start - 1);
}

xstring XSocket::URLtoContent(const xstring & url)
{
	int host_start = url.indexOf("//");
	if (host_start == -1)
	{
		host_start = 0;
	}
	else
	{
		host_start += 2;
	}

	int content_start = url.indexOf('/', host_start);
	return url.substring(content_start, url.length() - 1);
}

void XSocket::URLtoHost_Content(const xstring& url, xstring & host, xstring & content)
{
	int host_start = url.indexOf("//");
	if (host_start == -1)
	{
		host_start = 0;
	}
	else
	{
		host_start += 2;
	}

	int content_start = url.indexOf('/', host_start);
	host = url.substring(host_start, content_start - 1);
	content = url.substring(content_start, url.length() - 1);
}

void XSocket::UnInit()
{
	WSACleanup();
}
