#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <ReceiptLoader.h>

void TestStreq()
{
    assert(Streq("abc", "abc") == true);
    assert(Streq("!@£$^&*()_+", "~<>?:|{}") == false);

    assert(Streq("", "") == true);
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
    assert(Streq(CleanLine("abc123"), "abc123"));
    assert(Streq(CleanLine("\n a \xE2 b \x80 c \xA0"), " a  b  c "));

    assert(Streq(CleanLine(""), ""));
    return;
}

int main()
{
    TestStreq();
    TestIsStinkyChar();
    TestCleanLine();

    printf("All tests passed!\n");

    return 0;
}