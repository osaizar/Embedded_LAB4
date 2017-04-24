#include <libepc.h>

void llmultiply(unsigned long long int l1,
                unsigned long long int l2,
                unsigned char *result);

void llmultiply2(unsigned long long int l1,
                unsigned long long int l2,
                unsigned char *result);

unsigned long int high (unsigned long long int num);

unsigned long int low (unsigned long long int num);

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

unsigned long int low (unsigned long long int num) {
	return (((1L << 32) -1) & num);
}

unsigned long int high (unsigned long long int num) {
	return (num >> 32);
}

void llmultiply2(unsigned long long int l1, unsigned long long int l2, unsigned char *result) {

    unsigned long int s0, s1, s2, s3;
    unsigned long int num = low(l1) * low(l2);
    unsigned long long res, carry;

    s0 = low(num);

    num = high(l1) * low(l2) + high(num);
    s1 = low(num);
    s2 = high(num);

    num = s1 + low(l1) * high(l2);
    s1 = low(num);

    num = s2 + high(l1) * high(l2) + high(num);
    s2 = low(num);
    s3 = high(num);

    PutUnsignedLongLong(&s3);
    PutString(" ");
    PutUnsignedLongLong(&s2);
    PutString(" ");
    PutUnsignedLongLong(&s1);
    PutString(" ");
    PutUnsignedLongLong(&s0);

    res = s1 << 8 | s0;
    carry = s3 << 8 | s2;

    PutString("\r\n");
    PutString("Result: ");
    PutUnsignedLongLong(res);
    PutUnsignedLongLong(carry);
    PutString("\r\n");
}

int main(int argc, char *argv[])
{
  unsigned char result[16];
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

    llmultiply2(cases[i].a, cases[i].b, result);
    /*
    PutString("Result ");
    PutUnsignedLongLong(&result[8]);
    PutUnsignedLongLong(&result[0]);
    PutString("\r\n");
    */
    PutString("\r\n");
  }

  return 0;
}
