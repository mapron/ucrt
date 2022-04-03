cmake_minimum_required(VERSION 3.12...3.19)
project(winsdk)
enable_language(C ASM_MASM)

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_ENV $ENV{INCLUDE})
set(NO_UCRT_INCLUDES)
set(disableDefaultInc false)
foreach(inc ${INCLUDE_ENV})
	if ("${inc}" MATCHES "\\ucrt")
		message(WARNING "It's better to remove ucrt from INCLUDE environment variable. ")
		set(disableDefaultInc true)
	else()
		list(APPEND NO_UCRT_INCLUDES "${inc}")
	endif()
endforeach()

if (disableDefaultInc)
	add_compile_options(/X)
	include_directories(${NO_UCRT_INCLUDES})
endif()

add_definitions(-D_CORECRT_BUILD -D_CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY -D_CRT_GLOBAL_STATE_ISOLATION)
add_compile_options(/wd4083)

# ucrt_static
file(GLOB_RECURSE sources *.h *.cpp *.c *.asm)
set(mb_sources ${sources})

list(FILTER mb_sources INCLUDE REGEX "mbstring")
list(APPEND mb_sources convert/ismbstr.cpp)

list(FILTER sources EXCLUDE REGEX "mbstring")
list(FILTER sources EXCLUDE REGEX "ismbstr.cpp")
list(FILTER sources EXCLUDE REGEX "appcrt_dllmain.cpp")
list(FILTER sources EXCLUDE REGEX "appcrt_dllmain.cpp")
list(FILTER sources EXCLUDE REGEX "arm/")
list(FILTER sources EXCLUDE REGEX "arm64/")
list(FILTER sources EXCLUDE REGEX "i386/")
list(FILTER sources EXCLUDE REGEX "string/strcspn.c")
list(FILTER sources EXCLUDE REGEX "string/strpbrk.c")
list(FILTER sources EXCLUDE REGEX "string/strspn.c")
list(FILTER sources EXCLUDE REGEX "string/strcmp.c")
list(FILTER sources EXCLUDE REGEX "string/strcat.c")
list(FILTER sources EXCLUDE REGEX "string/strncpy.c")
list(FILTER sources EXCLUDE REGEX "string/strncmp.c")
list(FILTER sources EXCLUDE REGEX "string/strncat.c")

list(FILTER sources EXCLUDE REGEX "sse2_initializer.cpp")
list(FILTER sources EXCLUDE REGEX "fma3_initializer.cpp")
list(FILTER sources EXCLUDE REGEX "test_apps")

if (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	list(FILTER sources EXCLUDE REGEX "dbgrpt.cpp")
	list(FILTER sources EXCLUDE REGEX "debug_fill_threshold.cpp")
	list(FILTER sources EXCLUDE REGEX "dbgrptt.cpp")
	list(FILTER sources EXCLUDE REGEX "debug_heap.cpp")
	list(FILTER sources EXCLUDE REGEX "debug_heap_hook.cpp")
endif()

add_library(ucrt_static ${sources} ${mb_sources})

target_include_directories(ucrt_static PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/inc 
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/shims
	)

set_source_files_properties(internal/winapi_thunks.cpp
	startup/exit.cpp
	shims/shims.cpp
	PROPERTIES COMPILE_FLAGS -D_AMD64_)
set_source_files_properties(
	${mb_sources} 
	PROPERTIES COMPILE_FLAGS -D_MBCS
	)
set_source_files_properties(
	shims/math.c
	PROPERTIES COMPILE_FLAGS -D__x86_64__
	)
if (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set_source_files_properties(
		shims/math.c
		PROPERTIES COMPILE_OPTIONS -O1 # most of math became instrinsic.
		)
endif()

# test applications

file(GLOB __subdirs ${CMAKE_CURRENT_SOURCE_DIR}/test_apps/*)
foreach(subdir ${__subdirs})
	get_filename_component(subdirName "${subdir}" NAME)
	file(GLOB appsource ${subdir}/*)
	add_executable(sample_${subdirName} ${appsource})
	target_link_libraries(sample_${subdirName} PRIVATE ucrt_static)
	target_link_options(sample_${subdirName} PRIVATE /NODEFAULTLIB:ucrtd.lib /NODEFAULTLIB:ucrt.lib)
endforeach()


#[[ 
imports from VCRuntime140D
__C_specific_handler
__C_specific_handler_noexcept
__current_exception
__current_exception_context
__std_type_info_destroy_list
__vcrt_GetModuleFileNameW
__vcrt_GetModuleHandleW
__vcrt_LoadLibraryExW
_local_unwind
memcmp
memcpy
memmove
memset
strchr
strrchr
wcschr

imports from VCRuntime140D_1
__CxxFrameHandler4

imports from kernel.dll
CloseHandle
CompareStringW
CreateFileW
DeleteCriticalSection
EnterCriticalSection
EnumSystemLocalesW
ExitProcess
FindClose
FindFirstFileExW
FindNextFileW
FlsAlloc
FlsFree
FlsGetValue
FlsSetValue
FlushFileBuffers
FreeEnvironmentStringsW
FreeLibrary
GetACP
GetCPInfo
GetCommandLineA
GetCommandLineW
GetConsoleMode
GetConsoleOutputCP
GetCurrentProcess
GetCurrentProcessId
GetCurrentThread
GetCurrentThreadId
GetDateFormatW
GetEnvironmentStringsW
GetFileSizeEx
GetFileType
GetLastError
GetLocaleInfoW
GetModuleFileNameW
GetModuleHandleExW
GetModuleHandleW
GetOEMCP
GetProcAddress
GetProcessHeap
GetStartupInfoW
GetStdHandle
GetStringTypeW
GetSystemInfo
GetSystemTimeAsFileTime
GetTimeFormatW
GetUserDefaultLCID
HeapAlloc
HeapFree
HeapQueryInformation
HeapReAlloc
HeapSize
HeapValidate
InitializeCriticalSectionAndSpinCount
InitializeSListHead
IsDebuggerPresent
IsProcessorFeaturePresent
IsValidCodePage
IsValidLocale
LCMapStringW
LeaveCriticalSection
LoadLibraryExW
MultiByteToWideChar
OutputDebugStringW
QueryPerformanceCounter
RaiseException
ReadConsoleW
ReadFile
RtlCaptureContext
RtlLookupFunctionEntry
RtlVirtualUnwind
SetConsoleCtrlHandler
SetEnvironmentVariableW
SetFilePointerEx
SetLastError
SetStdHandle
SetUnhandledExceptionFilter
TerminateProcess
UnhandledExceptionFilter
VirtualQuery
WideCharToMultiByte
WriteConsoleW
WriteFile

]]#
