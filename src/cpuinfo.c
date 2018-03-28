/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@cc.hut.fi, duge at IRCnet>
 */

/******************************************************************************/
/* INCLUDES */
#include "cpuinfo.h"


/******************************************************************************/
/* FUNCTIONS */

static __inline__ int CPU_haveCPUID(void)
{
    int has_CPUID = 0;
#if defined(__GNUC__) && defined(i386)
	__asm__ (
	"        pushfl                      # Get original EFLAGS             \n"
	"        popl    %%eax                                                 \n"
	"        movl    %%eax,%%ecx                                           \n"
	"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
	"        pushl   %%eax               # Save new EFLAGS value on stack  \n"
	"        popfl                       # Replace current EFLAGS value    \n"
	"        pushfl                      # Get new EFLAGS                  \n"
	"        popl    %%eax               # Store new EFLAGS in EAX         \n"
	"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
	"        jz      1f                  # Processor=80486                 \n"
	"        movl    $1,%0               # We have CPUID support           \n"
	"1:                                                                    \n"
	: "=m" (has_CPUID)
	:
	: "%eax", "%ecx"
	);
#elif defined(__GNUC__) && defined(__x86_64__)
	__asm__ (
	"        pushfq                      # Get original EFLAGS             \n"
	"        popq    %%rax                                                 \n"
	"        movq    %%rax,%%rcx                                           \n"
	"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
	"        pushq   %%rax               # Save new EFLAGS value on stack  \n"
	"        popfq                       # Replace current EFLAGS value    \n"
	"        pushfq                      # Get new EFLAGS                  \n"
	"        popq    %%rax               # Store new EFLAGS in EAX         \n"
	"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
	"        jz      1f                  # Processor=80486                 \n"
	"        movl    $1,%0               # We have CPUID support           \n"
	"1:                                                                    \n"
	: "=m" (has_CPUID)
	:
	: "%rax", "%rcx"
	);
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
	__asm
	{
        pushfd                      ; Get original EFLAGS
        pop     eax
        mov     ecx, eax
        xor     eax, 200000h        ; Flip ID bit in EFLAGS
        push    eax                 ; Save new EFLAGS value on stack
        popfd                       ; Replace current EFLAGS value
        pushfd                      ; Get new EFLAGS
        pop     eax                 ; Store new EFLAGS in EAX
        xor     eax, ecx            ; Can not toggle ID bit,
        jz      done                ; Processor=80486
        mov     has_CPUID,1         ; We have CPUID support
done:
	}
#elif defined(__sun) && defined(__i386)
	__asm (
	"       pushfl                 \n"
	"	popl    %eax           \n"
	"	movl    %eax,%ecx      \n"
	"	xorl    $0x200000,%eax \n"
	"	pushl   %eax           \n"
	"	popfl                  \n"
	"	pushfl                 \n"
	"	popl    %eax           \n"
	"	xorl    %ecx,%eax      \n"
	"	jz      1f             \n"
	"	movl    $1,-8(%ebp)    \n"
	"1:                            \n"
	);
#elif defined(__sun) && defined(__amd64)
	__asm (
	"       pushfq                 \n"
	"       popq    %rax           \n"
	"       movq    %rax,%rcx      \n"
	"       xorl    $0x200000,%eax \n"
	"       pushq   %rax           \n"
	"       popfq                  \n"
	"       pushfq                 \n"
	"       popq    %rax           \n"
	"       xorl    %ecx,%eax      \n"
	"       jz      1f             \n"
	"       movl    $1,-8(%rbp)    \n"
	"1:                            \n"
	);
#endif
    return has_CPUID;
}

static __inline__ int CPU_getCPUIDFeatures(void)
{
    int features = 0;
#if defined(__GNUC__) && ( defined(i386) || defined(__x86_64__) )
	__asm__ (
	"        movl    %%ebx,%%edi\n"
	"        xorl    %%eax,%%eax         # Set up for CPUID instruction    \n"
	"        cpuid                       # Get and save vendor ID          \n"
	"        cmpl    $1,%%eax            # Make sure 1 is valid input for CPUID\n"
	"        jl      1f                  # We dont have the CPUID instruction\n"
	"        xorl    %%eax,%%eax                                           \n"
	"        incl    %%eax                                                 \n"
	"        cpuid                       # Get family/model/stepping/features\n"
	"        movl    %%edx,%0                                              \n"
	"1:                                                                    \n"
	"        movl    %%edi,%%ebx\n"
	: "=m" (features)
	:
	: "%eax", "%ecx", "%edx", "%edi"
	);
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
	__asm
	{
        xor     eax, eax            ; Set up for CPUID instruction
        cpuid                       ; Get and save vendor ID
        cmp     eax, 1              ; Make sure 1 is valid input for CPUID
        jl      done                ; We dont have the CPUID instruction
        xor     eax, eax
        inc     eax
        cpuid                       ; Get family/model/stepping/features
        mov     features, edx
done:
	}
#elif defined(__sun) && (defined(__i386) || defined(__amd64))
	    __asm(
	"        movl    %ebx,%edi\n"
	"        xorl    %eax,%eax         \n"
	"        cpuid                     \n"
	"        cmpl    $1,%eax           \n"
	"        jl      1f                \n"
	"        xorl    %eax,%eax         \n"
	"        incl    %eax              \n"
	"        cpuid                     \n"
#ifdef __i386
	"        movl    %edx,-8(%ebp)     \n"
#else
	"        movl    %edx,-8(%rbp)     \n"
#endif
	"1:                                \n"
	"        movl    %edi,%ebx\n" );
#endif
    return features;
}

static __inline__ int CPU_getCPUIDFeaturesExt(void)
{
    int features = 0;
#if defined(__GNUC__) && (defined(i386) || defined (__x86_64__) )
	__asm__ (
	"        movl    %%ebx,%%edi\n"
	"        movl    $0x80000000,%%eax   # Query for extended functions    \n"
	"        cpuid                       # Get extended function limit     \n"
	"        cmpl    $0x80000001,%%eax                                     \n"
	"        jl      1f                  # Nope, we dont have function 800000001h\n"
	"        movl    $0x80000001,%%eax   # Setup extended function 800000001h\n"
	"        cpuid                       # and get the information         \n"
	"        movl    %%edx,%0                                              \n"
	"1:                                                                    \n"
	"        movl    %%edi,%%ebx\n"
	: "=m" (features)
	:
	: "%eax", "%ecx", "%edx", "%edi"
	);
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
	__asm
	{
        mov     eax,80000000h       ; Query for extended functions
        cpuid                       ; Get extended function limit
        cmp     eax,80000001h
        jl      done                ; Nope, we dont have function 800000001h
        mov     eax,80000001h       ; Setup extended function 800000001h
        cpuid                       ; and get the information
        mov     features,edx
done:
	}
#elif defined(__sun) && ( defined(__i386) || defined(__amd64) )
	__asm (
	"        movl    %ebx,%edi\n"
	"        movl    $0x80000000,%eax \n"
	"        cpuid                    \n"
	"        cmpl    $0x80000001,%eax \n"
	"        jl      1f               \n"
	"        movl    $0x80000001,%eax \n"
	"        cpuid                    \n"
#ifdef __i386
	"        movl    %edx,-8(%ebp)   \n"
#else
	"        movl    %edx,-8(%rbp)   \n"
#endif
	"1:                               \n"
	"        movl    %edi,%ebx\n"
	);
#endif
    return features;
}

static __inline__ int CPU_haveRDTSC(void)
{
    if (CPU_haveCPUID())
    {
        return (CPU_getCPUIDFeatures() & 0x00000010);
    }
    return 0;
}

int cpuinfo_has_mmx(void)
{
    if (CPU_haveCPUID())
    {
        return (CPU_getCPUIDFeatures() & 0x00800000);
    }
    return 0;
}

static __inline__ int
CPU_haveMMXExt(void)
{
    if (CPU_haveCPUID())
    {
        return (CPU_getCPUIDFeaturesExt() & 0x00400000);
    }
    return 0;
}

static __inline__ int
CPU_have3DNow(void)
{
    if (CPU_haveCPUID())
    {
        return (CPU_getCPUIDFeaturesExt() & 0x80000000);
    }
    return 0;
}

static __inline__ int
CPU_have3DNowExt(void)
{
    if (CPU_haveCPUID())
    {
        return (CPU_getCPUIDFeaturesExt() & 0x40000000);
    }
    return 0;
}

int cpuinfo_has_sse(void)
{
    if (CPU_haveCPUID())
    {
        return (CPU_getCPUIDFeatures() & 0x02000000);
    }
    return 0;
}

int cpuinfo_has_sse2(void)
{
    if (CPU_haveCPUID())
    {
        return (CPU_getCPUIDFeatures() & 0x04000000);
    }
    return 0;
}

