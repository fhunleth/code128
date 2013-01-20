/* Unit tests for code128.c */

#include "code128.h"
#include "clar.h"

/* Scans the encoding for count bar/space weights and then returns it 
   decimally encoded. I.e. Code 0 = 212222, etc.
 */
static const char *bits_to_barspace(const char *encoding, int count, int *barspace)
{
    // Scan for the first one
    while (*encoding == 0)
	encoding++;

    int bs = 0;

    int c = 0;
    int on = 1;
    while (count) {
	while (!!*encoding == on) {
	    encoding++;
	    c++;
	}
	on = !on;
	bs = (bs * 10) + c;
	c = 0;
	count--;
    }
    *barspace = bs;
    return encoding;
}

static int code128a_to_barspace(char value)
{
	switch (value) {
	case ' ': return 212222;
	case '!': return 222122;
	case '\"': return 222221;
	case '#': return 121223;
	case '$': return 121322;
	case '%': return 131222;
	case '&': return 122213;
	case '\'': return 122312;
	case '(': return 132212;
	case ')': return 221213;
	case '*': return 221312;
	case '+': return 231212;
	case ',': return 112232;
	case '-': return 122132;
	case '.': return 122231;
	case '/': return 113222;
	case '0': return 123122;
	case '1': return 123221;
	case '2': return 223211;
	case '3': return 221132;
	case '4': return 221231;
	case '5': return 213212;
	case '6': return 223112;
	case '7': return 312131;
	case '8': return 311222;
	case '9': return 321122;
	case ':': return 321221;
    case ';': return 312212;
	case '<': return 322112;
	case '=': return 322211;
	case '>': return 212123;
	case '?': return 212321;
	case '@': return 232121;
	case 'A': return 111323;
    case 'B': return 131123;
    case 'C': return 131321;
    case 'D': return 112313;
    case 'E': return 132113;
    case 'F': return 132311;
    case 'G': return 211313;
    case 'H': return 231113;
    case 'I': return 231311;
    case 'J': return 112133;
    case 'K': return 112331;
    case 'L': return 132131;
    case 'M': return 113123;
    case 'N': return 113321;
    case 'O': return 133121;
    case 'P': return 313121;
    case 'Q': return 211331;
    case 'R': return 231131;
    case 'S': return 213113;
    case 'T': return 213311;
    case 'U': return 213131;
    case 'V': return 311123;
    case 'W': return 311321;
    case 'X': return 331121;
    case 'Y': return 312113;
    case 'Z': return 312311;
    case '[': return 332111;
    case '\\': return 314111;
    case ']': return 221411;
    case '^': return 431111;
    case '_': return 111224;
    //case 0: return 111422; /* Currently null characters aren't supported */
    case 1: return 121124;
    case 2: return 121421;
    case 3: return 141122;
    case 4: return 141221;
    case 5: return 112214;
    case 6: return 112412;
    case 7: return 122114;
    case 8: return 122411;
    case 9: return 142112;
    case 10: return 142211;
    case 11: return 241211;
    case 12: return 221114;
    case 13: return 413111;
    case 14: return 241112;
    case 15: return 134111;
    case 16: return 111242;
    case 17: return 121142;
    case 18: return 121241;
    case 19: return 114212;
    case 20: return 124112;
    case 21: return 124211;
    case 22: return 411212;
    case 23: return 421112;
    case 24: return 421211;
    case 25: return 212141;
    case 26: return 214121;
    case 27: return 412121;
    case 28: return 111143;
    case 29: return 111341;
    case 30: return 131141;
    case 31: return 114113;
    default: return 0;
	}
}

static void test_one_code128a_point(char value)
{
	char input[2];
	input[0] = value;
	input[1] = 0;

	char buffer[256];
	size_t len = code128a_encode(input, buffer, sizeof(buffer));    
	cl_assert(len > 0);

	const char *p = buffer;
	int barspace;
	p = bits_to_barspace(p, 6, &barspace);
	cl_assert_(barspace == 211412, "start code A incorrect"); // Start code A (103)

	int expected_barspace = code128a_to_barspace(value);
	p = bits_to_barspace(p, 6, &barspace);
	cl_assert(barspace == expected_barspace); // '1' (17)

	p = bits_to_barspace(p, 6, &barspace);
	cl_assert(barspace == expected_barspace); // Checkcode (17)

	p = bits_to_barspace(p, 7, &barspace);
	cl_assert(barspace == 2331112); // Stop code

}

void test_encode__all_code128a_points(void)
{
	char c;
    for (c = 0; c < 127; c++) {
        if (code128a_to_barspace(c) != 0)
            test_one_code128a_point(c);
    }
}

