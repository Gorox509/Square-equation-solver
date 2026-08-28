/**
 * \file
 * 
 * \brief File containing small functions primarily used in other functions
 */


#include "../headers/defines.h"

#include "../headers/plot.h"
#include "../headers/testing.h"
#include "../headers/util_funcs.h"


/**
 * \defgroup UtilFuncs Useful functions
 * \brief Small functions primarily used in other functions
 */


/**
 * \ingroup UtilFuncs
 * 
 * \brief Clears input buffer
 * 
 * Clears all next symbols from standard input stream
 * 
 * \return Amount of symbols cleared
 */
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

/**
 * \ingroup UtilFuncs
 * 
 * \brief Checks if string contains only space symbols
 * 
 * \return TRUE if string has only space symbols, FALSE otherwise 
 * 
 * \param [in] str string that is being checked
 */
bool is_str_all_space(char *str)
{
    if (strlen(str) == 0)
        return TRUE;

    int ch = str[0], iter = 0;

    while (ch != '\0')
    {
        if (!isspace(ch))
            return FALSE;
        ch = str[++iter];
    }
    return TRUE;
}


/**
 * \ingroup UtilFuncs
 * 
 * \brief Prints ASCII cat image in standard output stream
 */
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


/**
 * \ingroup UtilFuncs
 * 
 * \brief Checks if all next symbols in standard input stream are spaces
 * 
 * \return TRUE if all symbols in stdin are spaces, FALSE otherwise
 */
bool is_next_input_all_space()
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


/**
 * \ingroup UtilFuncs
 * 
 * \brief Checks if given double number is equal to zero
 * 
 * Equality check is based on global epsilon constant
 * 
 * \param x number that is compared to zero
 * 
 * \return TRUE if number absolute value is lower than epsilon constant, FALSE otherwise
 */
bool is_double_zero(double x)
{
    if (fabs(x) < epsilon)
        return TRUE; //TODO: bool - done
    return FALSE;
}


/**
 * \ingroup UtilFuncs
 * 
 * \brief Checks if given double number is infinite
 * 
 * Values that considered infinite are inf and -inf
 * 
 * \param x number that is being checked on being infinite
 * 
 * \return TRUE if given value is infinite, FALSE otherwise
 */
bool custom_isinf(double x)
{
    unsigned char *p = (unsigned char *)(&x + 1);
    unsigned char *p1 = p - 1;
    unsigned char *p2 = p - 2;

    if ((*p1 == 0x7f || *p1 == 0xff) && *p2 == 0xf0)
        return TRUE;
    return FALSE;
}


/**
 * \ingroup UtilFuncs
 * 
 * \brief Checks if given double number is Not-a-Number
 * 
 * \param x number that is being checked on being NAN
 * 
 * \return TRUE if given value is NAN, FALSE otherwise
 */
bool custom_isnan(double x)
{
    unsigned char *p = (unsigned char *)(&x + 1); // to the end of number
    unsigned char *p1 = p - 1;                    // to the first byte
    unsigned char *p2 = p - 2;                    // to the second byte

    if (*p1 == 0x7f && *p2 > 0xf0)
        return TRUE;
    return FALSE;
}


/**
 * \ingroup UtilFuncs
 * 
 * \brief Checks if given double number is finite
 * 
 * Finite values should not be infinite or Not-a-Number
 * 
 * \param x number that is being checked on being finite
 * 
 * \return TRUE if given value is finite, FALSE otherwise
 */
bool custom_isfinite(double x) {
    if (custom_isinf(x) || custom_isnan(x))
        return FALSE;
    return TRUE;
}
// todo plot in console - done
// todo read about split files - done


/**
 * \ingroup UtilFuncs
 * 
 * \brief Replaces first newline symbol in string with null terminator symbol, thus making it end there
 * 
 * If there's no newline symbols in given string, nothing happens
 * 
 * \param [out] str string that is being changed
 */
void replace_newline_with_null_terminator(char str[MAX_STR_LEN])
{
    char *idx = strchr(str, '\n');
    if (idx != NULL)
        *idx = '\0';
}


/**
 * \ingroup UtilFuncs
 * 
 * \brief Checks if two given double numbers are equal
 * 
 * Equality check is based on global epsilon constant
 * 
 * \param x first given double number
 * \param y second given double number
 * 
 * \return TRUE if given numbers are no more than epsilon apart, FALSE otherwise
 */
bool is_doubles_equal(double x, double y) {

	if (fabs(x - y) < epsilon)
		return TRUE;
	return FALSE;
}


/**
 * \ingroup UtilFuncs
 * 
 * \brief Reads one string and writes read line in other string
 * 
 * All symbols in source string before newline symbol are written in destination string, 
 * then in destination string newline symbol and null terminator are added to the end
 * 
 * \param [in] str_source string from where reading occurs
 * \param [out] str_dest string that being written to
 * 
 * \return Amount of symbols written (including newline and not including null terminator)
 */
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


/**
 * \ingroup UtilFuncs
 * 
 * \brief Adds newline symbol to the end of the string if there's no such
 * 
 * If there's no newline symbol at the end, it is added at position of null terminator, and null terminator is written in next cell, 
 * thus increasing string length by 1
 * 
 * \param [in, out] str string that is treated
 */
void add_newline_symbol_to_str_end_if_theres_not(char str[MAX_STR_LEN]) {
    char ch = -1;
    int iter = -1;

    while(ch != '\0') {
        ++iter;
        ch = str[iter];
    }

    if (iter == 0) {
        str[iter] = '\n';
        str[iter + 1] = '\0';
    }
    else if (iter < MAX_STR_LEN && str[iter - 1] != '\n') {
        str[iter] = '\n';
        str[iter + 1] = '\0';
    }
}