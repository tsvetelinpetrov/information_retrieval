
// Lab - 7

#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale>
#include <sstream>

using namespace std;
#pragma execution_character_set("utf-8")


//Link with ws2_32.lib
#pragma commnet (lib, "ws2_32.lib")
#pragma warning( disable : 4996)

#define BUFLEN UCHAR_MAX

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(65001);

	std::ofstream MyFile;
	std::string buff;
	std::string url;
	std::string hostname;

	unsigned short  wVersionRequested;

	int err, res, i, sock;

	char* ip = new char();
	char recvbuf[BUFLEN];
	int recvbuflen = BUFLEN;

	WSADATA wsaData;
	sockaddr_in server;
	hostent* he;
	in_addr** addr_list;


	cout << "URL: ";
	cin >> url;
	//url = "www.google.bg";
	hostname = url.substr(url.find("www."), url.find("/", 8) - url.find("www."));
	cout << "\nHostname: " << hostname << endl;
	/*--------------WINSOCKET INITIALIZATION-------------*/

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	if ((err != 0) || (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2))
	{
		printf("Winsock dll failed.\n");
		return 1;
	}
	printf("Winsock Initialized. \n");

	/*-------------HOSTNAME CONVERSION---------------*/

	if ((he = gethostbyname(hostname.c_str())) == NULL)
	{
		printf("gethostbyname failed.\n %d", WSAGetLastError());
		return 1;
	}


	addr_list = (in_addr**)he->h_addr_list;
	for (i = 0; addr_list[i] != NULL; i++)
	{
		strcpy(ip, inet_ntoa(*addr_list[i]));
		printf("%s resolved to: %s\n", hostname, ip);
	}
	WSACleanup();

	/*-----------SOCKET CREATION-----------*/
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket.\n");
		return 1;
	}

	printf("Socket created.\n");

	/*----------CONNECTION TO SERVER-----------*/

	// IPV4 
	// IP_ADDRESS
	// PORT

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_port = htons(80);

	//Connect to remote server
	if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Connect error.\n");
		return 1;
	}

	puts("Connected");


	/*----------GET REQUEST-----------*/
	//const char* getRequest = "GET / HTTP/1.1\nHost: www.tspetrov.ga\nConnection : close";
	string getRequest = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";

	if (send(sock, getRequest.c_str(), strlen(getRequest.c_str()), 0) < 0)
	{
		printf("Error with sending socket%d", WSAGetLastError());
		return 0;
	}

	/*----------GET RESPONSE------------/
	/-----------FILE WRITING-----------*/
	MyFile.open("file.txt");
	do
	{
		res = recv(sock, recvbuf, recvbuflen, 0);
		if (res > 0)
		{
			//printf("Bytes received: %d\n", res);
			MyFile << recvbuf;
		}
		else if (res == 0)
			printf("Connection close\n");
		else
			printf("recv failed.\n");
	} while (res > 0);


	//printf("%d\n", recvbuflen);
	MyFile.close();

	/*----------FILE READING-----------*/
	std::ifstream myfile("file.txt");
	std::cout << "\n-----------------------------\n";
	while (getline(myfile, buff))
	{
		std::cout << buff << std::endl;
	}

	myfile.close();
	std::cout << "\n-----------------------------\n";
	/*-------------Why i need that---------------*/
	res = shutdown(sock, SD_SEND);
	if (res == SOCKET_ERROR)
	{
		printf("Shutdown failed.\n");
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	/*----------------------------------------------*/
	closesocket(sock);
	WSACleanup();

	cout << "\n\nPress ANY key to close.\n\n";
	cin.ignore(); cin.get();

	return 0;
}


int mainn(void) {

	WSADATA wsaData;
	SOCKET Socket;
	SOCKADDR_IN SockAddr;
	int lineCount = 0;
	int rowCount = 0;
	struct hostent* host;
	locale local;
	char buffer[10000];
	int i = 0;
	int nDataLength;
	string website_HTML;

	// website url
	string url = "www.google.bg";

	//HTTP GET
	string get_http = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "WSAStartup failed.\n";
		system("pause");
		//return 1;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	host = gethostbyname(url.c_str());

	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	//SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
	SockAddr.sin_addr.s_addr = inet_addr("192.168.0.100");

	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
		cout << "Could not connect";
		system("pause");
		//return 1;
	}

	// send GET / HTTP
	send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

	// recieve html
	while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

			website_HTML += buffer[i];
			i += 1;
		}
	}

	closesocket(Socket);
	WSACleanup();

	// Display HTML source 
	cout << website_HTML;

	// pause
	cout << "\n\nPress ANY key to close.\n\n";
	cin.ignore(); cin.get();


	return 0;
}
