#include <windows.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
using std::string;

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS


HINSTANCE hInst;
WSADATA wsaData;

enum TagType
{
    html, htmlc,
    head, headc,
    body, bodyc,
};

void mParseUrl(const char* mUrl, string& serverName, string& filepath, string& filename);
SOCKET connectToServer(char* szServerName, WORD portNum);
int getHeaderLength(char* content);
char* readUrl2(const char* szUrl, long& bytesReturnedOut, char** headerOut);
int getRequest(const char* szUrl);
std::string readFile();
std::vector < std::string> checkHtml(std::string tags, char leftAngle, char rightAngle);
TagType convertStringToEnum(std::string tag);
bool tagChecker(std::vector<std::string> tags);
std::string extractUrl(std::string all);

int main()
{
    std::vector < std::string> tags;
    const char* szUrl = "http://crawlertest.cs.tu-varna.bg/TestIIR.html";
    getRequest(szUrl);
    std::string all = readFile();
    std::cout << all << "\n";
    tags = checkHtml(all, '<', '>');

    if (!tagChecker(tags))
        std::cout << "Missing HTML TAGS" << "\n" << "\n" << "\n";
    else
        std::cout << "[HTML tags Fine]" << "\n" << "\n" << "\n";

    //TAGS that was
    //for (auto& i : tags)
    //{
    //    std::cout << i << "\n";
    //}

    std::string urlInPage = extractUrl(all);
    //second 
    std::cout << urlInPage << "\n";
    getRequest(urlInPage.c_str());
    all = readFile();
    std::cout << all << "\n";
    tags = checkHtml(all, '<', '>');

    if (!tagChecker(tags))
        std::cout << "Missing HTML TAGS" << "\n";
    else
        std::cout << "[HTML tags Fine]" << "\n";

    WSACleanup();
    std::cout << urlInPage << "\n";
    return 0;
}

std::string extractUrl(std::string all)
{
    std::string search_string2 = "href=\"";
    std::string search_string = "href = ";

    // The index of the current search position
    size_t pos = 0;

    // The index of the start of the URL
    size_t url_start;

    // The index of the end of the URL
    size_t url_end;
    size_t url_end1;
    std::string url;
    while ((pos = all.find(search_string, pos)) != std::string::npos)
    {
        // Found a match for the search string, now search for the end of the URL
        url_start = pos + search_string.length();
        url_end1 = all.find("\"", url_start);
        url_end = all.find("\>", url_start);
        if (url_end != std::string::npos)
        {
            // Found the end of the URL, extract the URL and print it
            url = all.substr(url_start, url_end - url_start);
            std::cout << "Found URL: " << url << std::endl;
            pos = url_end;
        }
        else if (url_end1 != std::string::npos)
        {
            // Could not find the end of the URL, stop searching
            url = all.substr(url_start, url_end - url_start);
            std::cout << "Found URL: " << url << std::endl;
            pos = url_end;
            break;
        }
    }
    return url;
}

bool tagChecker(std::vector<std::string> tags)
{
    int stabilizer = 0;

    for (auto& tag : tags)
    {
        switch (convertStringToEnum(tag))
        {
        case TagType::html: {stabilizer++; break; }
        case TagType::htmlc: {stabilizer--; break; }
        case TagType::head: {stabilizer++; break; }
        case TagType::headc: {stabilizer--; break; }
        case TagType::body: {stabilizer++; break; }
        case TagType::bodyc: {stabilizer--; break; }
        }
    }
    return stabilizer == 0;
}
TagType convertStringToEnum(std::string tag)
{
    if (tag == "html")
        return TagType::html;
    if (tag == "/html")
        return TagType::htmlc;
    if (tag == "head")
        return TagType::head;
    if (tag == "/head")
        return TagType::headc;
    if (tag == "body")
        return TagType::body;
    if (tag == "/body")
        return TagType::bodyc;
}

std::vector < std::string> checkHtml(std::string all, char leftAngle, char rightAngle)
{
    int left = 0;
    int right = 0;

    std::string token;
    std::vector <std::string> tokens;

    for (auto& c : all)
    {
        if (c == leftAngle || c == rightAngle)
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
            if (c == leftAngle)
                left++;
            if (c == rightAngle)
                right++;
        }
        else
        {
            token += c;
        }
    }
    if (left == right)
        std::cout << "[Balanced HTML]" << "\n";
    else
        std::cout << "[Unbalanced HTML]" << "\n";
    return tokens;
}

std::string readFile()
{
    std::string buffer;
    std::string all;

    std::ifstream ifile("index.html");
    while (getline(ifile, buffer))
    {
        all += buffer + "\n";
    }
    ifile.close();
    return all;
}
int getRequest(const char* szUrl)
{
    const int bufLen = 1024;
    long fileSize;
    char* memBuffer, * headerBuffer;
    FILE* fp;

    memBuffer = headerBuffer = NULL;

    if (WSAStartup(0x101, &wsaData) != 0)
        return -1;


    memBuffer = readUrl2(szUrl, fileSize, &headerBuffer);
    //printf("returned from readUrl\ndata returned:\n");
    //printf("%s", memBuffer);
    if (fileSize != 0)
    {
        //  printf("Got some data\n");
        fp = fopen("index.html", "wb");
        fwrite(memBuffer, 1, fileSize, fp);
        fclose(fp);
        delete(memBuffer);
        delete(headerBuffer);
    }
}
void mParseUrl(const char* mUrl, string& serverName, string& filepath, string& filename)
{
    string::size_type n;
    string url = mUrl;

    if (url.substr(0, 7) == "http://")
        url.erase(0, 7);

    if (url.substr(0, 8) == "https://")
        url.erase(0, 8);

    n = url.find('/');
    if (n != string::npos)
    {
        serverName = url.substr(0, n);
        filepath = url.substr(n);
        n = filepath.rfind('/');
        filename = filepath.substr(n + 1);
    }

    else
    {
        serverName = url;
        filepath = "/";
        filename = "";
    }
}
SOCKET connectToServer(char* szServerName, WORD portNum)
{
    struct hostent* hp;
    unsigned int addr;
    struct sockaddr_in server;
    SOCKET conn;

    conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (conn == INVALID_SOCKET)
        return NULL;

    if (inet_addr(szServerName) == INADDR_NONE)
    {
        hp = gethostbyname(szServerName);
    }
    else
    {
        addr = inet_addr(szServerName);
        hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
    }

    if (hp == NULL)
    {
        closesocket(conn);
        return NULL;
    }

    server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(portNum);
    if (connect(conn, (struct sockaddr*)&server, sizeof(server)))
    {
        closesocket(conn);
        return NULL;
    }
    return conn;
}
int getHeaderLength(char* content)
{
    const char* srchStr1 = "\r\n\r\n", * srchStr2 = "\n\r\n\r";
    char* findPos;
    int ofset = -1;

    findPos = strstr(content, srchStr1);
    if (findPos != NULL)
    {
        ofset = findPos - content;
        ofset += strlen(srchStr1);
    }

    else
    {
        findPos = strstr(content, srchStr2);
        if (findPos != NULL)
        {
            ofset = findPos - content;
            ofset += strlen(srchStr2);
        }
    }
    return ofset;
}
char* readUrl2(const char* szUrl, long& bytesReturnedOut, char** headerOut)
{
    const int bufSize = 512;
    char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
    char* tmpResult = NULL, * result;
    SOCKET conn;
    string server, filepath, filename;
    long totalBytesRead, thisReadSize, headerLen;

    mParseUrl(szUrl, server, filepath, filename);

    ///////////// step 1, connect //////////////////////
    conn = connectToServer((char*)server.c_str(), 80);

    ///////////// step 2, send GET request /////////////
    sprintf(tmpBuffer, "GET %s HTTP/1.0", filepath.c_str());
    strcpy(sendBuffer, tmpBuffer);
    strcat(sendBuffer, "\r\n");
    sprintf(tmpBuffer, "Host: %s", server.c_str());
    strcat(sendBuffer, tmpBuffer);
    strcat(sendBuffer, "\r\n");
    strcat(sendBuffer, "\r\n");
    send(conn, sendBuffer, strlen(sendBuffer), 0);

    ///////////// step 3 - get received bytes ////////////////
    // Receive until the peer closes the connection
    totalBytesRead = 0;
    while (1)
    {
        memset(readBuffer, 0, bufSize);
        thisReadSize = recv(conn, readBuffer, bufSize, 0);

        if (thisReadSize <= 0)
            break;

        tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

        memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
        totalBytesRead += thisReadSize;
    }

    headerLen = getHeaderLength(tmpResult);
    long contenLen = totalBytesRead - headerLen;
    result = new char[contenLen + 1];
    memcpy(result, tmpResult + headerLen, contenLen);
    result[contenLen] = 0x0;
    char* myTmp;

    myTmp = new char[headerLen + 1];
    strncpy(myTmp, tmpResult, headerLen);
    myTmp[headerLen] = NULL;
    delete(tmpResult);
    *headerOut = myTmp;

    bytesReturnedOut = contenLen;
    closesocket(conn);
    return(result);
}