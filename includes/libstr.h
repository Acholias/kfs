#ifndef LIBSTR_H
# define LIBSTR_H

# include "types.h"

extern uint32_t	strlen(const char *str);

uint32_t	line_len(const char *line);
bool		is_whitespace(const char c);
bool		is_str_equals(const char *str1, const char *str2);

#endif
