#include "cpuid.h"

void CPUID::GetFeature(Register &reg)
{
	__asm{
		mov esi,reg;
		mov eax,1h;
		cpuid;
		mov dword ptr [esi],eax;
		mov dword ptr [esi+4],ebx;
		mov dword ptr [esi+8],ecx;
		mov dword ptr [esi+12],edx;
		
	}
}

void CPUID::GetName(Register &name)
{
	__asm{;
		mov esi,name;
		mov eax,80000002h;

		_loop:

		push eax;
		cpuid;
		mov dword ptr [esi],eax;
		mov dword ptr [esi+4],ebx;
		mov dword ptr [esi+8],ecx;
		mov dword ptr [esi+12],edx;
		pop eax;
		add eax,1h;
		add esi,10h;
		cmp eax,80000005h;

		jnz _loop;

	}
}