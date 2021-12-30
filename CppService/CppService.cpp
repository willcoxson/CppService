// CppService.cpp : Implementation of WinMain


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "CppService_i.h"


using namespace ATL;

#include <stdio.h>

class CCppServiceModule : public ATL::CAtlServiceModuleT< CCppServiceModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_CppServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CPPSERVICE, "{3a266de3-e432-4269-af44-5f76fdd0231f}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication,
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY
		// and an appropriate non-null Security Descriptor.

		HRESULT _hr = S_OK;
		//HRESULT _hr = CoInitializeSecurity(0, -1, 0, 0, RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IDENTIFY, 0, 0, 0);
		//CoInitializeSecurity(0, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

		CSecurityDescriptor sd;
		sd.InitializeFromProcessToken();
		//_hr = CoInitializeSecurity(sd, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

		//_hr = CoInitializeSecurity((PSECURITY_DESCRIPTOR) &LIBID_CppServiceLib, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_APPID, NULL);
		_hr = CoInitializeSecurity((PSECURITY_DESCRIPTOR)&LIBID_CppServiceLib, -1, NULL, NULL, 0, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_APPID, NULL);
		//_hr = CoInitializeSecurity(NULL, -1, 0, NULL, 0, 0, NULL, 0, NULL);
		return _hr;
	}

	HRESULT Run(int nCmdShow) throw()
	{
#ifdef _DEBUG
		if (m_bService)
		{
			//Sleep(15000);
		}
#endif
		OutputDebugString(L"Setting timer");
		g_TimerId = SetTimer(NULL, 1, 10000, TheTimerProc);

		WriteTimerId();

		HRESULT hr =  __super::Run(nCmdShow);

		KillTimer(NULL, g_TimerId);

		return hr;
	}

	static UINT_PTR g_TimerId;
	static void CALLBACK TheTimerProc(HWND hWnd, UINT, UINT_PTR, DWORD)
	{
		//KillTimer(NULL, g_TimerId);
		static bool firstTime = true;
		if (firstTime)
		{
			firstTime = false;
			g_TimerId = SetTimer(NULL, g_TimerId, 60000, TheTimerProc);
		}

		WriteTimerId();

	}

	static void WriteTimerId()
	{
		TCHAR szBuf[256] = _T("");
		wsprintf(szBuf, _T("g_TimerId = %ld"), g_TimerId);
		OutputDebugString(szBuf);
	}
};

CCppServiceModule _AtlModule;
UINT_PTR CCppServiceModule::g_TimerId = 0;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

