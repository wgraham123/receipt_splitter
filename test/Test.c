#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <StringFunctions.h>
#include <HelperFunctions.h>
#include <Receipt.h>

#define MIN_FLOAT_DIFF 0.005

void TestStringLength()
{
    char *a = "";
    assert(StringLength(a) == 0);

    a = "\0  \0";
    assert(StringLength(a) == 0);

    a = "abc";
    assert(StringLength(a) == 3);

    a = "123456789";
    assert(StringLength(a) == 9);

    a = "\n\n\n\x80\x80\x80";
    assert(StringLength(a) == 6);

    a = "   123   abc   \n\n\n   \0\0\0";
    assert(StringLength(a) == 21);

    return;
}
void TestStringEquals()
{
    return;
}
void TestIsCleanChar()
{
    return;
}
void TestCleanString()
{
    return;
}
void TestParseInt()
{
    assert(ParseInt("123456789") == 123456789);
    assert(ParseInt("-123") == 123);
    assert(ParseInt("0") == 0);

    assert(ParseInt("abc123abc") == 123);
    assert(ParseInt("") == -1);
}

void TestParseFloat()
{
    assert(abs(ParseFloat("123.123") - 123.123) <= MIN_FLOAT_DIFF);
    assert(abs(ParseFloat("0.5") - 0.5) <= MIN_FLOAT_DIFF);
    assert(abs(ParseFloat("0") - -1) <= MIN_FLOAT_DIFF);

    assert(abs(ParseFloat("abc123.123abc") - 123.123) <= MIN_FLOAT_DIFF);
}

int main()
{
    TestStringLength();
    TestParseInt();
    TestParseFloat();

    printf("All tests passed!\n");
    return 0;
}