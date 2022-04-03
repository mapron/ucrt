#include <stringapiset.h>

#include <corecrt_internal.h>

#include <fenv.h>

int __cdecl __acrt_MultiByteToWideChar(_In_ UINT _CodePage, _In_ DWORD _DWFlags,
                                       _In_ LPCSTR _LpMultiByteStr,
                                       _In_ int _CbMultiByte,
                                       _Out_writes_opt_(_CchWideChar)
                                           LPWSTR _LpWideCharStr,
                                       _In_ int _CchWideChar) {
  return MultiByteToWideChar(_CodePage, _DWFlags, _LpMultiByteStr, _CbMultiByte,
                             _LpWideCharStr, _CchWideChar);
}

int __cdecl __acrt_WideCharToMultiByte(
    _In_ UINT _CodePage, _In_ DWORD _DWFlags, _In_ LPCWSTR _LpWideCharStr,
    _In_ int _CchWideChar, _Out_writes_opt_(_CbMultiByte) LPSTR _LpMultiByteStr,
    _In_ int _CbMultiByte, _In_opt_ LPCSTR _LpDefaultChar,
    _Out_opt_ LPBOOL _LpUsedDefaultChar) {
  return WideCharToMultiByte(_CodePage, _DWFlags, _LpWideCharStr, _CchWideChar,
                             _LpMultiByteStr, _CbMultiByte, _LpDefaultChar,
                             _LpUsedDefaultChar);
}


_Success_(return)
BOOL __cdecl __acrt_GetStringTypeW(
    _In_                DWORD       _DWInfoType,
    _In_NLS_string_(_CchSrc) PCWCH  _LpSrcStr,
    _In_                int         _CchSrc,
    _Out_               LPWORD      _LpCharType
		) {
	return GetStringTypeW(_DWInfoType, _LpSrcStr, _CchSrc, _LpCharType);
}


void __cdecl __acrt_initialize_user_matherr(void* encoded_null) {}
bool __cdecl __acrt_has_user_matherr(void) { return false; }
int  __cdecl __acrt_invoke_user_matherr(struct _exception*) { return 0; }

windowing_model_policy __cdecl __acrt_get_windowing_model_policy(void) {
	return windowing_model_policy_hwnd;
}
developer_information_policy __cdecl __acrt_get_developer_information_policy(void) {
	return developer_information_policy_ui;
}
process_end_policy __cdecl __acrt_get_process_end_policy(void) {
	return process_end_policy_exit_process;
}

begin_thread_init_policy __cdecl __acrt_get_begin_thread_init_policy(void) {
	return begin_thread_init_policy_none; // ro_initialize is for WinRT apps.
}

_ACRTIMP _Success_(return == 0) int __cdecl feholdexcept(_Out_ fenv_t* _Env){
	return 0;
}
