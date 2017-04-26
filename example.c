#include <libepc.h>
#include <limits.h>

#define LOW64(x) ((x) & 0xFFFFFFFFFFFFFFFF)

void llmultiply(unsigned long long int l1,
                unsigned long long int l2,
                unsigned char *result);

unsigned long int lower32(unsigned long long int n);
unsigned long int higher32(unsigned long long int n);

void llmultiply2(unsigned long long int l1,
                unsigned long long int l2,
                unsigned char *result);

struct test_case {
  unsigned long long int a;
  unsigned long long int b;
  unsigned long long int rh;
  unsigned long long int rl;
};

struct test_case cases[6] = {

  { 0x0000111122223333ULL, 0x0000555566667777ULL,
    0x0000000005B061D9ULL, 0x58BF0ECA7C0481B5ULL },

  { 0x3456FEDCAAAA1000ULL, 0xEDBA00112233FF01ULL,
    0x309A912AF7188C57ULL, 0xE62072DD409A1000ULL },

  { 0xFFFFEEEEDDDDCCCCULL, 0xBBBBAAAA99998888ULL,
    0xBBBB9E2692C5DDDCULL, 0xC28F7531048D2C60ULL },

  { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL,
    0xFFFFFFFFFFFFFFFEULL, 0x0000000000000001ULL },

  { 0x00000001FFFFFFFFULL, 0x00000001FFFFFFFFULL,
    0x0000000000000003ULL, 0xFFFFFFFC00000001ULL },

  { 0xFFFEFFFFFFFFFFFFULL, 0xFFFF0001FFFFFFFFULL,
    0xFFFE0002FFFDFFFEULL, 0x0001FFFE00000001ULL }
};

void PutUnsignedLongLong(unsigned long long int* ulli)
{
  unsigned long int* uli = (unsigned long int*)ulli;
  PutUnsigned(uli[1], 16, 8);
  PutUnsigned(uli[0], 16, 8);
}

int main(int argc, char *argv[])
{
  unsigned char result[16], result2[16];
  int i;

  ClearScreen(0x07);
  SetCursorPosition(0, 0);

  for (i = 0; i < 6; ++i)
  {
    PutString("Test : ");
    PutUnsignedLongLong(&cases[i].a);
    PutString(" * ");
    PutUnsignedLongLong(&cases[i].b);
    PutString("\r\n");

    PutString("    == ");
    PutUnsignedLongLong(&cases[i].rh);
    PutUnsignedLongLong(&cases[i].rl);
    PutString("\r\n");

    llmultiply(cases[i].a, cases[i].b, result);

    PutString("Result1 ");
    PutUnsignedLongLong(&result[8]);
    PutUnsignedLongLong(&result[0]);
    PutString("\r\n");

    llmultiply2(cases[i].a, cases[i].b, result2);

    PutString("Result2 ");
    PutUnsignedLongLong(&result2[8]);
    PutUnsignedLongLong(&result2[0]);
    PutString("\r\n");

    PutString("\r\n");
  }

  /*
  unsigned int *r = result;
  r[0] = 0x13;
  r[1] = 0x0;
  r[2] = 0x0;
  r[3] = 0x0;
  PutUnsigned(Milliseconds(), 10, 8);
  PutString("\r\n");
  unsigned long long int a, b;
  for (i=0; i<1000000000; i++) {
  	llmultiply2(cases[0].a, (((unsigned int) result[2])) + (unsigned int) result[0], result);
  }
  PutUnsigned(Milliseconds(), 10, 8);
  */


  return 0;
}

unsigned long int lower32(unsigned long long int n) {
	return (n & 0xFFFFFFFF);
}

unsigned long int higher32(unsigned long long int n) {
	return ((n >> 32) & 0xFFFFFFFF);
}

void llmultiply2(unsigned long long int l1, unsigned long long int l2, unsigned char *result) {
	unsigned int *res = (unsigned int *) result;
	unsigned long long int l1l = lower32(l1);
	unsigned long long int l1h = higher32(l1);
	unsigned long long int l2l = lower32(l2);
	unsigned long long int l2h = higher32(l2);
	unsigned long long int tmp;

	int i;
	for (i=0; i<4; i++) res[i] = 0;

	tmp = l1l * l2l;
	res[0] = lower32(tmp);
	res[1] = higher32(tmp);

	tmp = l1l * l2h;
	if (ULONG_MAX - res[1] < lower32(tmp)) res[2]++;
	res[1] += lower32(tmp);
	if (ULONG_MAX - res[2] < higher32(tmp)) res[3]++;
	res[2] += higher32(tmp);

	tmp = l1h * l2l;
	if (ULONG_MAX - res[1] < lower32(tmp)) res[2]++;
	res[1] += lower32(tmp);
	if (ULONG_MAX - res[2] < higher32(tmp)) res[3]++;
	res[2] += higher32(tmp);

	tmp = l1h * l2h;
	if (ULONG_MAX - res[2] < lower32(tmp)) res[3]++;
	res[2] += lower32(tmp);
	res[3] += higher32(tmp);


}
