#include "libstr.h"
#include "terminal.h"

uint32_t	line_len(const char *line)
{
	if (!line)
		return (0);

	uint32_t	len = 0;

	while (len < TERMINAL_WIDTH && line[len] != '\n' && line[len] != '\0')
		len++;

	return (len);
}


bool	is_whitespace(const char c)
{
	return (c <= 32);
}


bool	is_str_equals(const char *str1, const char *str2)
{
	uint32_t str1_length = strlen(str1);

	if (str1_length != strlen(str2))
		return (false);

	for (uint32_t i = 0; i < str1_length; i++)
	{
		if (str1[i] != str2[i])
			return (false);
	}

	return (true);
}
