#ifndef __CPUID__
#define __CPUID__

union Register
{
	int dword[4];	//eax,ebx,ecx,edx
	char byte[16];
};

class CPUID
{
public:
	CPUID(){}
	~CPUID(){}
	void GetFeature(Register&);	//eax = 0x00000001
	void GetName(Register&);	//eax = 0x80000002 ~ 0x80000004
};

#endif