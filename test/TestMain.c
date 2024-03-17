#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <Receipt.h>

void TestStringEquals()
{
    assert(StringEquals("abc", "abc") == true);
    assert(StringEquals("!@£$^&*()_+", "~<>?:|{}") == false);

    assert(StringEquals("", "") == true);
    return;
}

void TestIsStinkyChar()
{
    assert(IsStinkyChar('\n') == true);
    assert(IsStinkyChar('\xE2') == true);
    assert(IsStinkyChar('\x80') == true);
    assert(IsStinkyChar('\xA0') == true);
    assert(IsStinkyChar('\0') == true);

    char test_char;

    for (int i = 32; i < 127; i++)
    {
        test_char = i;
        assert(IsStinkyChar(test_char) == false);
    }

    test_char = -1;
    assert(IsStinkyChar(test_char) == true);
    test_char = 128;
    assert(IsStinkyChar(test_char) == true);
    return;
}

void TestCleanLine()
{
    char a[] = "abc";
    CleanLine(a);
    assert(StringEquals(a, "abc"));

    char b[] = "\n a \xE2 b \x80 c \xA0";
    CleanLine(b);
    assert(StringEquals(b, " a  b  c "));

    char c[] = "";
    CleanLine(c);
    assert(StringEquals(c, ""));
    return;
}

int main()
{
    TestStringEquals();
    TestIsStinkyChar();
    TestCleanLine();

    printf("All tests passed!\n");

    return 0;
}