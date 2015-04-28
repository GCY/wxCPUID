#include <iostream>
#include <xmmintrin.h>

#include "cpuid.h"

char *ecx_msg[] = {
"SSE3 Extension:",
"Carryless Multiplication:",
"64-bit DS Area:",
"MONITOR/MWAIT:",
"CPL Qualified Debug Store:",
"Virtual Machine Extensions:",
"SaferMode Extensions:",
"Enhanced Intel SpeedStep Technology:",
"Thermal Monitor 2:",
"Supplemental SSSE3 Extensions:",
"L1 Context ID:",
"",
"Fused Multiply Add:",
"CMPXCHG16B:",
"Update Control:",
"Perf/Debug Capability MSR:",
"",
"Process-context Identifiers:",
"Direct Cache Access:",
"SSE4.1:",
"SSE4.2:",
"x2APIC:",
"MOVEBE:",
"POPCNT:",
"TSC-Deadline:",
"AES:",
"XSAVE:",
"OSXSAVE:",
"AVX:",
"",
"",
""
};

char *edx_msg[] = {
"FPU:",
"VME:",
"DE: ",
"PSE:",
"TSC:",
"MSR:",
"PAE:",
"MCE:",
"CMPXCHG8B:",
"APIC:",
"",
"SYSENTER/SYSEXIT:",
"MTRR:",
"PGE:",
"MCA:",
"CMOV:",
"PAT:",
"PSE36:",
"PNS:",
"CLFSH:",
"",
"Debug Store:",
"ACPI:",
"MMX:",
"FXSR:",
"SSE:",
"SSE2:",
"Self snoop:",
"HTT:",
"TM: ",
"",
"PBE:"
};

char *Y_N[] = {"No","Yes"};

	union XMM{
		__m128 m;
		float f[4];
	};

__declspec(align(16)) float data[4] = {1.13f,1.211f,1.111f,1.0f};

float CircleArea(float R)
{
	__asm{
		finit;
		fld R;
		fld R;
		fmul;
		fldpi;
		fmul;
		fstp R;
	}
	return R;
}

float Degree2Radian(float degree)
{
	float radian,PI_div_180 = 0.0174532f;
	__asm{
		fld degree;
		fld PI_div_180;
		fmul;
		fstp radian;
	}
	return radian;
}

float Sin(float radian)
{
	__asm{
		fld radian;
		fsin;
		fstp radian;
	}
	return radian;
}

float Cos(float radian)
{
	__asm{
		fld radian;
		fcos;
		fstp radian;
	}
	return radian;
}

void dot3(XMM &dot,XMM &vec1,XMM &vec2)
{
	__asm{
		mov eax,vec1;
		mov ebx,vec2;
		xor ecx,ecx;
		movaps xmm0,[eax];
		movss xmm0,ecx;
		movaps xmm1,[ebx];
		movss xmm1,ecx;
		mulps xmm0,xmm1;
		haddps xmm0,xmm0;
		haddps xmm0,xmm0;
		mov eax,dot;
		movaps [eax],xmm0;
	}
}


int main(void)
{

	__m128 m0 = _mm_set_ps(1.0f,1.22f,1.23f,1.0f);
	__m128 m1 = _mm_load_ps(data);
	XMM result;
	XMM ref;
	XMM shuf;
	__asm{

		//
		lea eax,m0;
		movaps xmm0,[eax];
		lea ebx,m1;
		movaps xmm1,[ebx];
		movaps [eax],xmm1;
		shufps xmm0,xmm0,1Bh;
		shufps xmm0,xmm0,93h;
		shufps xmm1,xmm1,39h;
		addps xmm0,xmm1;
		lea eax,result;
		haddps xmm0,xmm0;
		haddps xmm0,xmm0;
		movaps [eax],xmm0;

		lea eax,ref;
		movaps [eax],xmm1;
		shufps xmm1,xmm0,61h;
		lea eax,shuf;
		movaps [eax],xmm1;

		/*
		mov ecx,0;
		cvtsi2ss xmm0,ecx;
		cmpeqps xmm0,xmm0;
		xorps xmm0,xmm0;
		movaps [eax],xmm0;
		*/
		/*
		mov ecx,123;
		cvtsi2ss xmm0,ecx;
		xorps xmm1,xmm1;
		mov ecx,321;
		cvtsi2ss xmm1,ecx;
		unpcklps xmm0,xmm1;
		movaps xmm2,xmm0;
		unpcklps xmm0,xmm2;
		movaps [eax],xmm0;
		*/
	}
	_mm_movehl_ps(m0,m1);

	std::cout << CircleArea(7.123) << std::endl
		<< Sin(Degree2Radian(90.0)) << std::endl;
	std::cout << result.f[0] << " " << result.f[1] << " " << result.f[2] << " " << result.f[3] << std::endl
		<< ref.f[0] << " " << ref.f[1] << " " << ref.f[2] << " " << ref.f[3] << std::endl
		<< shuf.f[0] << " " << shuf.f[1] << " " << shuf.f[2] << " " << shuf.f[3] << std::endl;

	XMM dot;
	dot3(dot,ref,result);
	std::cout << dot.f[0] << " " << dot.f[1] << " " << dot.f[2] << " " << dot.f[3] << std::endl;

	std::cout << std::endl;

	Register reg;
	CPUID cpuid;
	cpuid.GetFeature(reg);
	for(int i = 0,j = 1;i < 32;++i){
		std::cout << ecx_msg[i] << Y_N[(bool)(reg.dword[2] & j)] << std::endl;
		j <<= 1;
	}

	for(int i = 0,j = 1;i < 32;++i){
		std::cout << edx_msg[i] << Y_N[(bool)(reg.dword[3] & j)] << std::endl;
		j <<= 1;
	}

	cpuid.GetName(reg);
	std::cout << reg.byte << std::endl;

	getchar();
}

/*

MOVSS XMM0,DWORD PTR DS:[402178]         ; FLOAT 1.000000
MOVSS XMM1,DWORD PTR DS:[402174]         ; FLOAT 1.330000
MOVSS XMM2,DWORD PTR DS:[402170]         ; FLOAT 1.220000
MOVSS XMM3,DWORD PTR DS:[402178]         ; FLOAT 1.000000
UNPCKLPS XMM0,XMM2
UNPCKLPS XMM1,XMM3
UNPCKLPS XMM0,XMM1
MOVAPS DQWORD PTR SS:[LOCAL.6],XMM0
MOVAPS XMM0,DQWORD PTR SS:[LOCAL.6]
MOVAPS DQWORD PTR SS:[LOCAL.10],XMM0

MOVAPS XMM0,DQWORD PTR DS:[403030]
MOVAPS DQWORD PTR SS:[LOCAL.14],XMM0
MOVAPS XMM0,DQWORD PTR SS:[LOCAL.14]
MOVAPS DQWORD PTR SS:[LOCAL.18],XMM0
*/

/*
MOVSS XMM0,DWORD PTR DS:[402140]         ; FLOAT 1.000000
MOVSS XMM2,DWORD PTR DS:[40213C]         ; FLOAT 1.220000
MOVSS XMM3,DWORD PTR DS:[402138]         ; FLOAT 1.330000
MOVSS XMM1,XMM0
UNPCKLPS XMM0,XMM2
UNPCKLPS XMM3,XMM1
UNPCKLPS XMM0,XMM3
MOVAPS DQWORD PTR SS:[LOCAL.8],XMM0

MOVAPS XMM0,DQWORD PTR DS:[403030]
MOVAPS DQWORD PTR SS:[LOCAL.4],XMM0

*/