#include "../headers/defines.h"

#include "../headers/plot.h"
#include "../headers/testing.h"
#include "../headers/util_funcs.h"


int clear_buffer()
{

    int ch = 0, n_cleared_chars = 0;

    while (ch != EOF && ch != '\n')
    {
        ch = getchar();
        ++n_cleared_chars;
    }
    return n_cleared_chars;
}


int non_space_symbols_in_str(char *str)
{
    if (strlen(str) == 0)
        return TRUE;

    int ch = str[0], iter = 0, answer = 1;

    while (ch != '\0')
    {
        if (!isspace(ch))
            answer = 0;
        ch = str[++iter];
    }
    return answer;
}

void print_ascii_cat() {
    printf(""
           "     /\\__/\\ " ENDL 
           "    /`    '\\ " ENDL 
           "  === 0  0 === " ENDL
           "    \\  --  / " ENDL 
           "   /        \\ " ENDL 
           "  /          \\ " ENDL
           " |            | " ENDL 
           "  \\  ||  ||  / " ENDL
           "   \\_oo__oo_/#######o" ENDL);
}

int n_spaces_in_next_input()
{
    int ch = 0, n_non_space_symbols = 0;

    while (ch != EOF && ch != '\n')
    {
        ch = getchar();
        if (!isspace(ch))
            ++n_non_space_symbols;
    }
    if (!n_non_space_symbols)
        return TRUE;
    return FALSE;
}

bool is_double_zero(double x)
{
    if (fabs(x) < epsilon)
        return TRUE; //TODO: bool
    return FALSE;
}

bool custom_isinf(double x)
{
    unsigned char *p = (unsigned char *)(&x + 1);
    unsigned char *p1 = p - 1;
    unsigned char *p2 = p - 2;

    if ((*p1 == 0x7f || *p1 == 0xff) && *p2 == 0xf0)
        return TRUE;
    return FALSE;
}

bool custom_isnan(double x)
{
    unsigned char *p = (unsigned char *)(&x + 1); // to the end of number
    unsigned char *p1 = p - 1;                    // to the first byte
    unsigned char *p2 = p - 2;                    // to the second byte

    if (*p1 == 0x7f && *p2 > 0xf0)
        return TRUE;
    return FALSE;
}

bool custom_isfinite(double x) {
    if (custom_isinf(x) || custom_isnan(x))
        return FALSE;
    return TRUE;
}
// todo plot in console - done
// todo read about split files - done

void replace_newline_with_null_terminator(char str[MAX_STR_LEN])
{
    char *idx = strchr(str, '\n');
    *idx = '\0';
}

bool is_doubles_equal(double x, double y) {

	if (fabs(x - y) < epsilon)
		return TRUE;
	return FALSE;
}


int custom_sgets(char str_dest[MAX_STR_LEN], char str_source[MAX_STR_LEN]) { // reads \n at the end
	int iter = 0;
	char ch = 0;

	while (ch != '\n' && iter < MAX_STR_LEN) {
		ch = str_source[iter];
        str_dest[iter] = ch;
		++iter;
	}
	return iter;
}