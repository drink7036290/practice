
#include "lib/helpers.h"

/**
 * The read4 API is defined in the parent class Reader4.
 *     int read4(char *buf4);
 */

int read4(char *buf4);

#define READ4_FULL_LEN 4

/**
 * @param buf Destination buffer
 * @param n   Number of characters to read
 * @return    The number of actual characters read
 */
int _read(char* buf, int len) {
	int org_len = len, cn = 0;

	if (len <= 0)
		return 0;

	while (len > 0)
	{   
		if (len < READ4_FULL_LEN)
		{
			char *tmp = calloc(READ4_FULL_LEN + 1, sizeof(char));
			int tmp_len = read4(tmp), i = 0;

			cn = Min(tmp_len, len);

			for (i = 0; i < cn; ++i)
				buf[i] = tmp[i];

			len -= cn;

			free(tmp);
			break;
		}

		cn = read4(buf);

		buf += cn;
		len -= cn;

		if (cn < READ4_FULL_LEN)
			break;
	}

	return org_len - len;
}
