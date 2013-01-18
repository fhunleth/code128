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
    printf("barspace = %d\n", bs);
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
	case ';': return 321212;
	case '<': return 322112;
	case '=': return 322211;
	case '>': return 212123;
	case '?': return 212321;
	case '@': return 232121;
	case 'A': return 111323;
	case 'B': return 212222;//
	case 'C': return 212222;
	case 'D': return 212222;
	case 'E': return 212222;
	case 'F': return 212222;
	case 'G': return 212222;
	case 'H': return 212222;
	case 'I': return 212222;
	case 'J': return 212222;
	case 'K': return 212222;
	case 'L': return 212222;
	case 'M': return 212222;
	case 'N': return 212222;
	case 'O': return 212222;
	case 'P': return 212222;
	case 'Q': return 212222;
	case 'R': return 212222;
	case 'S': return 212222;
	case 'T': return 212222;
	case 'U': return 212222;
	case 'V': return 212222;
	case 'W': return 212222;
	case 'X': return 212222;
	case 'Y': return 212222;
	case 'Z': return 212222;
	case '[': return 212222;
	case '\\': return 212222;
	case ']': return 212222;
	case '^': return 212222;
	case '_': return 212222;
	default: return 0;
	}
}

static void test_one_code128a_point(char value)
{
    printf("value = '%c'\n", value);
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
	for (c = ' '; c <= '9'; c++)
		test_one_code128a_point(c);
}

