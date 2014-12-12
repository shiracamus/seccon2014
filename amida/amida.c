/*
  For the SECCON CTF 2014 summer online qualifications
  Title : Let's disassemble
  Genre : Programming
  Points: 300
  Author: @shiracamus
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <openssl/sha.h>	// need option "-lcrypto" to compile

#define NUM 1000

#define LABEL "1 2 3 4 5 6 7 8"
#define SPACE "               "
#define DISPLAY_WIDTH (sizeof SPACE)
#define DISPLAY_HEIGHT (SHA_DIGEST_LENGTH + 1)

#define RESULT_OK	(0)
#define RESULT_FLAG	(1)
#define RESULT_WRONG	(2)
#define RESULT_TOO_SLOW	(3)
#define RESULT_DEBUG	(4)

typedef unsigned char BYTE;

BYTE amidas[][SHA_DIGEST_LENGTH] = {
#include "amida.hash"
};

int check_amida(const BYTE *amida)
{
	int i;
	BYTE prev = 0, next;

	for (i = 0; i < SHA_DIGEST_LENGTH - 1; i++) {
		next = amida[i];
		if ((prev & next) != 0
		||  (next & (next << 1)) != 0) {
			return RESULT_WRONG;
		}
		prev = next;
	}
	if (amida[i] < 1 || amida[i] > 8) {
		return RESULT_WRONG;
	}
	return RESULT_OK;
}

#define LEVEL_MAGIC (1337)
#define LEVEL0 (LEVEL_MAGIC *  9)	// normal
#define LEVEL1 (LEVEL_MAGIC * 19)	// reverse horizontal
#define LEVEL2 (LEVEL_MAGIC * 29)	// reverse vertical
#define LEVEL3 (LEVEL_MAGIC * 39)	// reverse vertical and horizontal
#define LEVEL4 (LEVEL_MAGIC * 49)	// random width
#define LEVEL5 (LEVEL_MAGIC * 59)	// rotate 90
#define LEVEL6 (LEVEL_MAGIC * 69)	// reverse horizontal
#define LEVEL7 (LEVEL_MAGIC * 79)	// reverse vertical
#define LEVEL8 (LEVEL_MAGIC * 89)	// reverse vertical and horizontal
#define LEVEL9 (LEVEL_MAGIC * 99)	// random width

static inline __attribute__((always_inline))
void print_rotate90(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	int x, y;

	for (x = 0; x < DISPLAY_WIDTH - 1; x++) {
		for (y = 0; y < DISPLAY_HEIGHT; y++) {
			char c = display[y][x];
			if (c == '-') {
				c = '|';
			} else if (c == '|') {
				c = '-';
			}
			putchar(c);
		}
		putchar('\n');
	}
}

static inline __attribute__((always_inline))
void swap_horizontal(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	int x, y;

	for (y = 0; y < DISPLAY_HEIGHT; y++) {
		for (x = 0; x < (DISPLAY_WIDTH - 1) / 2; x++) {
			char save = display[y][x];
			display[y][x] = display[y][DISPLAY_WIDTH - x - 2];
			display[y][DISPLAY_WIDTH - x - 2] = save;
		}
	}
}

static inline __attribute__((always_inline))
void swap_vertical(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	int x, y;

	for (y = 0; y < DISPLAY_HEIGHT / 2; y++) {
		for (x = 0; x < DISPLAY_WIDTH; x++) {
			char save = display[y][x];
			display[y][x] = display[DISPLAY_HEIGHT - y - 1][x];
			display[DISPLAY_HEIGHT - y - 1][x] = save;
		}
	}
}

static inline __attribute__((always_inline))
void print_level9(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	int r, x, y, z;
	int w[DISPLAY_WIDTH];

	r = (rand() >> 2) & 3;
	if (r&1) {
		swap_horizontal(display);
	}
	if (r&2) {
		swap_vertical(display);
	}
	for (x = 0; x < DISPLAY_WIDTH; x++) {
		if (x & 1) {
			w[x] = ((rand() >> 4) & 3) + 1;
		} else {
			w[x] = 1;
		}
	}
	for (x = 0; x < DISPLAY_WIDTH - 1; x++) {
		for (z = 0; z < w[x]; z++) {
			for (y = 0; y < DISPLAY_HEIGHT; y++) {
				char c = display[y][x];
				if (c == '-') {
					c = '|';
				} else if (c == '|') {
					c = '-';
				}
				putchar(c);
			}
			putchar('\n');
		}
	}
}

static inline __attribute__((always_inline))
void print_level8(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	swap_horizontal(display);
	swap_vertical(display);
	print_rotate90(display);
}

static inline __attribute__((always_inline))
void print_level7(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	swap_vertical(display);
	print_rotate90(display);
}

static inline __attribute__((always_inline))
void print_level6(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	swap_horizontal(display);
	print_rotate90(display);
}

static inline __attribute__((always_inline))
void print_level5(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	print_rotate90(display);
}

static inline __attribute__((always_inline))
void print_level4(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	int r, x, y, z;
	int w[DISPLAY_WIDTH];

	r = (rand() >> 6) & 3;
	if (r&1) {
		swap_horizontal(display);
	}
	if (r&2) {
		swap_vertical(display);
	}
	for (x = 0; x < DISPLAY_WIDTH; x++) {
		if (x & 1) {
			w[x] = ((rand() >> 8) & 3) + 1;
		} else {
			w[x] = 1;
		}
	}
	for (y = 0; y < DISPLAY_HEIGHT; y++) {
		for (x = 0; x < DISPLAY_WIDTH; x++) {
			for (z = w[x]; --z >= 0; ) {
				putchar(display[y][x]);
			}
		}
		putchar('\n');
	}
}

static inline __attribute__((always_inline))
void print_level3(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	int x, y;

	for (y = DISPLAY_HEIGHT; --y >= 0; ) {
		for (x = DISPLAY_WIDTH; --x >= 0; ) {
			putchar(display[y][x]);
		}
		putchar('\n');
	}
}

static inline __attribute__((always_inline))
void print_level2(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	int y;

	for (y = DISPLAY_HEIGHT; --y >= 0; ) {
		printf("%s\n", display[y]);
	}
}

static inline __attribute__((always_inline))
void print_level1(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	int x, y;

	for (y = 0; y < DISPLAY_HEIGHT; y++) {
		for (x = DISPLAY_WIDTH - 1; --x >= 0; ) {
			putchar(display[y][x]);
		}
		putchar('\n');
	}
}

static inline __attribute__((always_inline))
void print_level0(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH])
{
	int y;

	for (y = 0; y < DISPLAY_HEIGHT; y++) {
		printf("%s\n", display[y]);
	}
}

static inline __attribute__((always_inline))
void print_level(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH], unsigned int level)
{
	if (LEVEL4 < level && level <= LEVEL5) {
		print_level5(display);
	} else if (LEVEL2 < level && level <= LEVEL3) {
		print_level3(display);
	} else if (LEVEL7 < level && level <= LEVEL8) {
		print_level8(display);
	} else if (LEVEL8 < level && level <= LEVEL9) {
		print_level9(display);
	} else if (LEVEL1 < level && level <= LEVEL2) {
		print_level2(display);
	} else if (level <= LEVEL0) {
		print_level0(display);
	} else if (LEVEL0 < level && level <= LEVEL1) {
		print_level1(display);
	} else if (LEVEL5 < level && level <= LEVEL6) {
		print_level6(display);
	} else if (LEVEL3 < level && level <= LEVEL4) {
		print_level4(display);
	} else if (LEVEL6 < level && level <= LEVEL7) {
		print_level7(display);
	} else {
		print_level(display, rand() % (LEVEL9 + 1));
	}
}

void print_amida(const BYTE *amida, int n)
{
	int x, y;
	char display[DISPLAY_HEIGHT][DISPLAY_WIDTH], *d;

	sprintf(display[0], LABEL);
	for (y = 0; y < SHA_DIGEST_LENGTH - 1; y++) {
		d = display[y+1];
		for (x = 0; x < 7; x++) {
			*d++ = '|';
			*d++ = amida[y] & (0x80 >> x) ? '-' : ' ';
		}
		*d++ = '|';
		*d = '\0';
	}
	d = display[y+1];
	sprintf(d, SPACE);
	d[(amida[y] - 1) * 2] = '*';

	printf("No.%d\n", n + 1);
	print_level(display, n * LEVEL_MAGIC);
}

void print_hash(const BYTE *label, const BYTE *hash)
{
	int i;

	printf("%s: ", label);
	for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
		printf("%02x", hash[i]);
	}
	printf("\n");
}

int do_amida(BYTE *answers, int n)
{
	int i, result;
	SHA_CTX sha;
	BYTE *amida, xor[SHA_DIGEST_LENGTH];
	time_t start, end;

	SHA1_Init(&sha);
	SHA1_Update(&sha, answers, n);
	SHA1_Final(xor, &sha);

	amida = amidas[n];
	for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
		amida[i] ^= xor[i];
	}

	result = check_amida(amida);
	if (result != RESULT_OK) {
		return result;
	}

	if (n == NUM) {
		return RESULT_FLAG;
	}

	print_amida(amida, n);
	amida++;

	printf("? ");
	fflush(stdout);
	time(&start);
	if (scanf("%7s", answers + n) != 1) {
		return RESULT_WRONG;
	}
	time(&end);
	if (end - start > 2) {
		return RESULT_TOO_SLOW;
	}
	usleep(300000);
	return RESULT_OK;
}

int main(int argc, char *argv[])
{
	int i, n, result;
	SHA_CTX sha;
	BYTE answers[NUM + 8], flag[SHA_DIGEST_LENGTH], c;
	static BYTE FLAG[] = {'F'^'}', 'L'^'F', 'A'^'L', 'G'^'A', '{'^'G'};

#ifdef RESULT_DEBUG
	if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
		return 1;
	}
#endif

	srand(getpid() + time(NULL));

	n = 0;
	while ((result = do_amida(answers, n)) == RESULT_OK) {
		n++;
	}

	if (result == RESULT_WRONG) {
		printf("Wrong.\n");
		return 1;
	}
	if (result == RESULT_TOO_SLOW) {
		printf("Too slow.\n");
		return 1;
	}
	if (result != RESULT_FLAG) {
		return 1;
	}

	// print "FLAG{" + sha1(answer characters) + "}"
	SHA1_Init(&sha);
	SHA1_Update(&sha, answers, n);
	SHA1_Final(flag, &sha);

	c = '}';
	for (i = 0; i < sizeof FLAG; i++) {
		c ^= FLAG[i];
		printf("%c", c);
	}
	for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
		printf("%02x", flag[i]);
	}
	printf("}\n");

	return 0;
}
