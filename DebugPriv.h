#include <TlHelp32.h>
BOOL EnableDebugAbility(void) {

	// Get the privilege value for SeDebugPrivilege
	// API - https://msdn.microsoft.com/en-us/library/windows/desktop/aa379180(v=vs.85).aspx
	// Privs - https://msdn.microsoft.com/en-us/library/windows/desktop/aa379180(v=vs.85).aspx

	LUID privilegeLuid;
	if (!LookupPrivilegeValue(NULL, _T("SeDebugPrivilege"), &privilegeLuid)) {

		ErrorDetails(_T("LookupPrivilegeValue()"), true);
	}

	// Fill up the TOKEN_PRIVILEGES structure
	// https://msdn.microsoft.com/zh-cn/library/windows/desktop/aa379630(v=vs.85).aspx

	TOKEN_PRIVILEGES tkPrivs;
	
	tkPrivs.PrivilegeCount = 1; // Only modify one privilege
	tkPrivs.Privileges[0].Luid = privilegeLuid; // specify the privilege to be modified i.e. SeDebugPrivilege
	tkPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // lets enable this privilege
								// All set! Now lets edit the process token
	// Get current process handle https://msdn.microsoft.com/en-us/library/windows/desktop/ms683179(v=vs.85).aspx
								// Get process token 
								// Token access rights 

	HANDLE currentProcessHandle = GetCurrentProcess();
	HANDLE processToken;

	if (!OpenProcessToken(currentProcessHandle, TOKEN_ADJUST_PRIVILEGES, &processToken)) {

		ErrorDetails(_T("OpenProcessToken()"), true);
	}

	// Let us now enable debug privileges in the token!

	if (!AdjustTokenPRivileges(processToken, false, &tkPrivs, 0, NULL, NULL)) {

		ErrorDetails(_T("AdjustTokenPrivileges()"), true);
	}

	
	return TRUE;
}
