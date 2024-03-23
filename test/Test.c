#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <StringFunctions.h>
#include <HelperFunctions.h>
#include <Receipt.h>

#define FLOAT_TOL 0.001

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
    assert(ParseInt("0") == 0);
    assert(ParseInt("1") == 1);
    assert(ParseInt("10") == 10);
    assert(ParseInt("100") == 100);
    assert(ParseInt("101") == 101);
    assert(ParseInt("123456789") == 123456789);

    assert(ParseInt("010") == 10);
    assert(ParseInt("0001") == 1);

    assert(ParseInt("-0") == -1);
    assert(ParseInt("-123") == -1);
    assert(ParseInt("abc123abc") == -1);
    assert(ParseInt("") == -1);
}

void TestParseFloat()
{
    assert(Abs(ParseFloat("0.0") - 0) <= FLOAT_TOL);
    assert(Abs(ParseFloat("1.0") - 1) <= FLOAT_TOL);
    assert(Abs(ParseFloat("0.1") - 0.1) <= FLOAT_TOL);
    assert(Abs(ParseFloat("1.1") - 1.1) <= FLOAT_TOL);
    assert(Abs(ParseFloat("01.01") - 1.01) <= FLOAT_TOL);
    assert(Abs(ParseFloat("000.001") - 0.001) <= FLOAT_TOL);
    assert(Abs(ParseFloat("123987.987123") - 123987.987123) <= FLOAT_TOL);
    assert(Abs(ParseFloat("501.105") - 501.105) <= FLOAT_TOL);

    assert(Abs(ParseFloat("0") - -1) <= FLOAT_TOL);
    assert(Abs(ParseFloat("-1") - -1) <= FLOAT_TOL);
    assert(Abs(ParseFloat("") - -1) <= FLOAT_TOL);

    assert(abs(ParseFloat("abc123.123abc") - -1) <= FLOAT_TOL);
}

int main()
{
    TestStringLength();
    TestParseInt();
    TestParseFloat();

    printf("All tests passed!\n");
    return 0;
}