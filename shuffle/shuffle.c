/*
  For the SECCON CTF 2014 winter online qualifications
  Title : shuffle
  Genre : Binary
  Points: 100
  Author: @shiracamus
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

/*            0         1         2         3         */
/*            012345678901234567890123456789012345678 */
#define FLAG "SECCON{Welcome to the SECCON 2014 CTF!}"

int
main(int argc, char *argv[])
{
	int i;
	char buf[sizeof(FLAG)];

	buf[ 0] = FLAG[ 0];
	buf[ 1] = FLAG[ 1];
	buf[ 2] = FLAG[ 2];
	buf[ 3] = FLAG[ 3];
	buf[ 4] = FLAG[ 4];
	buf[ 5] = FLAG[ 5];
	buf[ 6] = FLAG[ 6];
	buf[ 7] = FLAG[ 7];
	buf[ 8] = FLAG[ 8];
	buf[ 9] = FLAG[ 9];
	buf[10] = FLAG[10];
	buf[11] = FLAG[11];
	buf[12] = FLAG[12];
	buf[13] = FLAG[13];
	buf[14] = FLAG[14];
	buf[15] = FLAG[15];
	buf[16] = FLAG[16];
	buf[17] = FLAG[17];
	buf[18] = FLAG[18];
	buf[19] = FLAG[19];
	buf[20] = FLAG[20];
	buf[21] = FLAG[21];
	buf[22] = FLAG[22];
	buf[23] = FLAG[23];
	buf[24] = FLAG[24];
	buf[25] = FLAG[25];
	buf[26] = FLAG[26];
	buf[27] = FLAG[27];
	buf[28] = FLAG[28];
	buf[29] = FLAG[29];
	buf[30] = FLAG[30];
	buf[31] = FLAG[31];
	buf[32] = FLAG[32];
	buf[33] = FLAG[33];
	buf[34] = FLAG[34];
	buf[35] = FLAG[35];
	buf[36] = FLAG[36];
	buf[37] = FLAG[37];
	buf[38] = FLAG[38];
	buf[39] = FLAG[39];
	buf[34] = FLAG[34];
	buf[35] = FLAG[35];
	buf[36] = FLAG[36];

	srand(time(NULL) + getpid());
	for (i = 0; i < 100; i++) {
		int r1 = rand() % sizeof(buf);
		int r2 = rand() % sizeof(buf);
		int tmp = buf[r1];
		buf[r1] = buf[r2];
		buf[r2] = tmp;
        }
	printf("%s\n", buf);
	return 0;
}
