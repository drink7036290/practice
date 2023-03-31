
#include "lib/helpers.h"

/**
 * The read4 API is defined in the parent class Reader4.
 *     int read4(char *buf4);
 */

int read4(char *buf4);

#define READ4_FULL_LEN 4

typedef struct {
    char tmp[READ4_FULL_LEN + 1];
    
    int index;
    int len;
} Solution;

/** initialize your data structure here. */
Solution* solutionCreate() {
    return calloc(1, sizeof(Solution));
}

int CopyFromTmp(Solution* obj, char* buf, int *len)
{
    int i = 0;
    int cn = Min(obj->len, *len);

    for (i = 0; i < cn; ++i)
    {
        buf[i] = obj->tmp[obj->index];

        ++obj->index;
    }
                
    obj->len -= cn;
    *len -= cn;
    
    return cn;
}

/**
 * @param buf Destination buffer
 * @param n   Number of characters to read
 * @return    The number of actual characters read
 */
int _read(Solution* obj, char* buf, int len) {
   
	int org_len = len, cn = 0;
    
	if (len <= 0)
		return 0;

	while (len > 0)
	{   
        if (obj->len > 0)
        {
            cn = CopyFromTmp(obj, buf, &len);
        
            if (len == 0)
                break;
        
            buf += cn;
        }
        
		if (len < READ4_FULL_LEN)
		{
            obj->len = read4(obj->tmp);
            obj->index = 0;
            
            CopyFromTmp(obj, buf, &len);
            
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
