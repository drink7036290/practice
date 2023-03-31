
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int IsNumber(char c)
{
	return ((c > '0') && (c - '0') <= 9);
}

double GetOperand(char **c_ref)
{
	char *c = *c_ref;
	double val = 0;

	while (*c != '\0')
	{
		if (! IsNumber(*c))
			break;

		val = val * 10 + ((*c) - '0');

		++c;
	}

	*c_ref = c;

	return val;
}

double Calculate(char *equ)
{
	char *c = equ, op = '\0';
	double left = 0;

	if (*c == '\0')
		return 0;

	left = GetOperand(&c);
	if (*c == '\0')
		return left;

	op = *c;
	++c;

	while ((op == '*') || (op == '/'))
	{
		if (op == '*')
			left *= GetOperand(&c);
		else
			left /= GetOperand(&c);

		if (*c == '\0')
			return left;

		op = *c;
		++c;
	}

	if (op == '+')
		return left + Calculate(c);

	// op == '-'
	return left - Calculate(c);
}

int main(int argc, char *argv[])
{
	char equ[] = "2*3+5/6*3+15";
//	char equ[] = "2*3";

	printf("%.1lf\n", Calculate(equ));

	return 0;
}

