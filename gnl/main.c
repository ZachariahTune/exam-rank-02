# include <stdio.h>
#include "get_next_line.h"

int main (void)
{
	char	*line;
	int		gnl_result;

	line = "Hello World";
	while ((gnl_result = get_next_line(&line)) > 0)
	{
		printf("result = %d; line = %s\n", gnl_result, line);
	}	printf("result = %d; line = %s\n", gnl_result, line);

	return (0);
}