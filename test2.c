
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

const char *IO_POLICY_STRING[] = {
	"TX_IO_POLICY_NORMAL"
,	"TX_IO_POLICY_NORMAL_ALONE"
,	"TX_IO_POLICY_PAUSE"
,	"TX_IO_POLICY_REJECT"
};

int main(int argc, char *argv[])
{
	printf("%s\n", IO_POLICY_STRING[3]);

	return 0;
}

