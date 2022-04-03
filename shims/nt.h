#define _NTDEF_
#include <minwindef.h>
#include <minwinbase.h>
#include <winnt.h>

typedef LONG NTSTATUS;
/*lint -save -e624 */  // Don't complain about different typedefs.
typedef NTSTATUS *PNTSTATUS;

#include <winternl.h>

#define STATUS_NOT_FOUND                 ((NTSTATUS)0xC0000225L)
#define FLG_APPLICATION_VERIFIER        0x00000100      // user mode only


#define NtCurrentPeb() ((struct _PEB64*)((NtCurrentTeb())->ProcessEnvironmentBlock))


#define PEBTEB_PRIVATE_PASTE(x,y)       x##y
#define PEBTEB_PASTE(x,y)               PEBTEB_PRIVATE_PASTE(x,y)

#define PEBTEB_BITS 64
#if defined(PEBTEB_BITS) /* This is defined by wow64t.h. */

#if PEBTEB_BITS == 32

#define PEBTEB_STRUCT(x)    PEBTEB_PASTE(x, 32) /* FOO32 */
#define PEBTEB_POINTER(x)   TYPE32(x) /* ULONG, defined in wow64t.h */

#elif PEBTEB_BITS == 64

#define PEBTEB_STRUCT(x)    PEBTEB_PASTE(x, 64) /* FOO64 */
#define PEBTEB_POINTER(x)   ULONGLONG /* ULONGLONG, defined in wow64t.h */

#else

#error Unknown value for pebteb_bits (PEBTEB_BITS).

#endif

#else

//
// Declare and use regular native types.
//
#define PEBTEB_POINTER(x) x
#define PEBTEB_STRUCT(x)  x

#endif

#define GDI_HANDLE_BUFFER_SIZE32  34
#define GDI_HANDLE_BUFFER_SIZE64  60

#if !defined(_AMD64_)
#define GDI_HANDLE_BUFFER_SIZE      GDI_HANDLE_BUFFER_SIZE32
#else
#define GDI_HANDLE_BUFFER_SIZE      GDI_HANDLE_BUFFER_SIZE64
#endif

typedef ULONG GDI_HANDLE_BUFFER32[GDI_HANDLE_BUFFER_SIZE32];
typedef ULONG GDI_HANDLE_BUFFER64[GDI_HANDLE_BUFFER_SIZE64];
typedef ULONG GDI_HANDLE_BUFFER  [GDI_HANDLE_BUFFER_SIZE  ];

/* for searching
typedef struct _PEB
typedef struct _PEB32
typedef struct _PEB64
*/
typedef struct PEBTEB_STRUCT(_PEB) {
    BOOLEAN InheritedAddressSpace;      // These four fields cannot change unless the
    BOOLEAN ReadImageFileExecOptions;   //
    BOOLEAN BeingDebugged;              //
    union {
        BOOLEAN BitField;                  //
        struct {
            BOOLEAN ImageUsesLargePages : 1;
            BOOLEAN SpareBits : 7;
         };
    };
    PEBTEB_POINTER(HANDLE) Mutant;      // INITIAL_PEB structure is also updated.

    PEBTEB_POINTER(PVOID) ImageBaseAddress;
    PEBTEB_POINTER(PPEB_LDR_DATA) Ldr;
    PEBTEB_POINTER(struct _RTL_USER_PROCESS_PARAMETERS*) ProcessParameters;
    PEBTEB_POINTER(PVOID) SubSystemData;
    PEBTEB_POINTER(PVOID) ProcessHeap;
    PEBTEB_POINTER(struct _RTL_CRITICAL_SECTION*) FastPebLock;
    PEBTEB_POINTER(PVOID) AtlThunkSListPtr;     // Used only for AMD64
    PEBTEB_POINTER(PVOID) SparePtr2;
    ULONG EnvironmentUpdateCount;
    PEBTEB_POINTER(PVOID) KernelCallbackTable;
    ULONG SystemReserved[1];
    ULONG SpareUlong;
    PEBTEB_POINTER(PPEB_FREE_BLOCK) FreeList;
    ULONG TlsExpansionCounter;
    PEBTEB_POINTER(PVOID) TlsBitmap;
    ULONG TlsBitmapBits[2];         // TLS_MINIMUM_AVAILABLE bits
    PEBTEB_POINTER(PVOID) ReadOnlySharedMemoryBase;
    PEBTEB_POINTER(PVOID) ReadOnlySharedMemoryHeap;
    PEBTEB_POINTER(PPVOID) ReadOnlyStaticServerData;
    PEBTEB_POINTER(PVOID) AnsiCodePageData;
    PEBTEB_POINTER(PVOID) OemCodePageData;
    PEBTEB_POINTER(PVOID) UnicodeCaseTableData;

    //
    // Useful information for LdrpInitialize
    ULONG NumberOfProcessors;
    ULONG NtGlobalFlag;

    //
    // Passed up from MmCreatePeb from Session Manager registry key
    //

    LARGE_INTEGER CriticalSectionTimeout;
    PEBTEB_POINTER(SIZE_T) HeapSegmentReserve;
    PEBTEB_POINTER(SIZE_T) HeapSegmentCommit;
    PEBTEB_POINTER(SIZE_T) HeapDeCommitTotalFreeThreshold;
    PEBTEB_POINTER(SIZE_T) HeapDeCommitFreeBlockThreshold;

    //
    // Where heap manager keeps track of all heaps created for a process
    // Fields initialized by MmCreatePeb.  ProcessHeaps is initialized
    // to point to the first free byte after the PEB and MaximumNumberOfHeaps
    // is computed from the page size used to hold the PEB, less the fixed
    // size of this data structure.
    //

    ULONG NumberOfHeaps;
    ULONG MaximumNumberOfHeaps;
    PEBTEB_POINTER(PPVOID) ProcessHeaps;

    //
    //
    PEBTEB_POINTER(PVOID) GdiSharedHandleTable;
    PEBTEB_POINTER(PVOID) ProcessStarterHelper;
    ULONG GdiDCAttributeList;
    PEBTEB_POINTER(struct _RTL_CRITICAL_SECTION*) LoaderLock;

    //
    // Following fields filled in by MmCreatePeb from system values and/or
    // image header.
    //

    ULONG OSMajorVersion;
    ULONG OSMinorVersion;
    USHORT OSBuildNumber;
    USHORT OSCSDVersion;
    ULONG OSPlatformId;
    ULONG ImageSubsystem;
    ULONG ImageSubsystemMajorVersion;
    ULONG ImageSubsystemMinorVersion;
    PEBTEB_POINTER(ULONG_PTR) ImageProcessAffinityMask;
    PEBTEB_STRUCT(GDI_HANDLE_BUFFER) GdiHandleBuffer;
    PEBTEB_POINTER(PPS_POST_PROCESS_INIT_ROUTINE) PostProcessInitRoutine;

    PEBTEB_POINTER(PVOID) TlsExpansionBitmap;
    ULONG TlsExpansionBitmapBits[32];   // TLS_EXPANSION_SLOTS bits

    //
    // Id of the Hydra session in which this process is running
    //
    ULONG SessionId;

    //
    // Filled in by LdrpInstallAppcompatBackend
    //
    ULARGE_INTEGER AppCompatFlags;

    //
    // ntuser appcompat flags
    //
    ULARGE_INTEGER AppCompatFlagsUser;

    //
    // Filled in by LdrpInstallAppcompatBackend
    //
    PEBTEB_POINTER(PVOID) pShimData;

    //
    // Filled in by LdrQueryImageFileExecutionOptions
    //
    PEBTEB_POINTER(PVOID) AppCompatInfo;

    //
    // Used by GetVersionExW as the szCSDVersion string
    //
    UNICODE_STRING CSDVersion;

    //
    // Fusion stuff
    //
    PEBTEB_POINTER(const struct _ACTIVATION_CONTEXT_DATA *) ActivationContextData;
    PEBTEB_POINTER(struct _ASSEMBLY_STORAGE_MAP *) ProcessAssemblyStorageMap;
    PEBTEB_POINTER(const struct _ACTIVATION_CONTEXT_DATA *) SystemDefaultActivationContextData;
    PEBTEB_POINTER(struct _ASSEMBLY_STORAGE_MAP *) SystemAssemblyStorageMap;
    
    //
    // Enforced minimum initial commit stack
    //
    PEBTEB_POINTER(SIZE_T) MinimumStackCommit;

    //
    // Fiber local storage.
    //

    PEBTEB_POINTER(PPVOID) FlsCallback;
    PEBTEB_STRUCT(LIST_ENTRY) FlsListHead;
    PEBTEB_POINTER(PVOID) FlsBitmap;
    ULONG FlsBitmapBits[FLS_MAXIMUM_AVAILABLE / (sizeof(ULONG) * 8)];
    ULONG FlsHighIndex;
} PEBTEB_STRUCT(PEB), * PEBTEB_STRUCT(PPEB);
