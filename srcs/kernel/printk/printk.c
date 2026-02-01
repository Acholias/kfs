#include "printk.h"
#include "libstr.h"

static int32_t	putnbr_base(t_terminal *terminal, int32_t num, int32_t base, int32_t uppercase);
static int32_t	putunbr_base(t_terminal *terminal, uint32_t num, uint32_t base, uint32_t uppercase);
static int32_t	check_format(t_terminal *terminal, va_list *args, char c);

int32_t	printk(t_terminal *terminal, const char *str, ...)
{
	va_list		args;
	int32_t	value = 0;
	int32_t	index = 0;

	if (!str)
		return (-1);

	va_start(args, str);
	while (str[index])
	{
		if (str[index] == '%' && str[index + 1] != '\0')
		{
			++index;
			value += check_format(terminal, &args, str[index]);
		}
		else
		{
			terminal_putchar(terminal, str[index]);
			++value;
		}
		index++;
	}
	va_end(args);

	return (value);
}

static int32_t	putnbr_base(t_terminal *terminal, int32_t num, int32_t base, int32_t uppercase)
{
	char	buffer[10];
	int32_t	value = 0;
	char	*digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

	if (num == 0)
	{
		terminal_putchar(terminal, '0');
		return (1);
	}
	while (num > 0)
	{
		buffer[value++] = digits[num % base];
		num = num / base;
	}
	for (int index = value - 1; index >= 0; --index)
		terminal_putchar(terminal, buffer[index]);

	return (value);
}

static int32_t	putunbr_base(t_terminal *terminal, uint32_t num, uint32_t base, uint32_t uppercase)
{
	char	buffer[10];
	int32_t	value = 0;
	char	*digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

	if (num == 0)
	{
		terminal_putchar(terminal, '0');
		return (1);
	}
	while (num > 0)
	{
		buffer[value++] = digits[num % base];
		num = num / base;
	}
	for (int index = value - 1; index >= 0; --index)
		terminal_putchar(terminal, buffer[index]);

	return (value);
}

static int32_t	check_format(t_terminal *terminal, va_list *args, char c)
{
	int32_t	value = 0;

	switch (c)
	{
		case 'd':
		case 'i':
		{
			int32_t	num = va_arg(*args, int32_t);

			if (num == -2147483648)
			{
				terminal_putstring(terminal, "-2147483648");
				value += 11;
				break;
			}
			else if (num < 0)
			{
				terminal_putchar(terminal, '-');
				++value;
				num = -num;
			}
			value += putnbr_base(terminal, num, 10, false);
			break ;
		}

		case 'u':
			value += putunbr_base(terminal, va_arg(*args, uint32_t), 10, false);
			break ;

		case 'h':
		case 'x':
			value += putunbr_base(terminal, va_arg(*args, uint32_t), 16, false);
			break ;

		case 'H':
		case 'X':
			value += putunbr_base(terminal, va_arg(*args, uint32_t), 16, true);
			break ;

		case 'p':
		{
			uint32_t	ptr = (uint32_t)va_arg(*args, void *);
			if (!ptr)
			{
				terminal_putstring(terminal, "(nil)");
				value += 5;
			}
			else
			{
				terminal_putstring(terminal, "0x");
				value += 2 + putnbr_base(terminal, ptr, 16, 0);
			}
			break ;
		}

		case 's':
		{
			char *str = (char *)va_arg(*args, int32_t *);
			if (!str)
				str = "(null)";
			terminal_putstring(terminal, str);
			value += strlen(str);
			break ;
		}

		case 'c':
			terminal_putchar(terminal, (char)va_arg(*args, int32_t));
			value += 1;
			break ;

		case '%':
			terminal_putchar(terminal, '%');
			value += 1;
			break ;

		default:
			terminal_putchar(terminal, '%');
			terminal_putchar(terminal, c);
			value += 2;
			break ;
	}
	return (value);
}








// static const char	*NUMBER_BASE = "0123456789abcdef";

// static void	print_number(t_terminal *terminal, int32_t number, int32_t base);
// static void	print_unsigned_number(t_terminal *terminal, uint32_t number, uint32_t base);
// static void	print_address(t_terminal *terminal, uint32_t number);

// void	printk(t_terminal *terminal, const char *str, ...)
// {
// 	if (!terminal || !str) // Safety check
// 		return ;

// 	va_list	args;
// 	int		i = 0;

// 	va_start(args, str);
// 	while (str[i])
// 	{
// 		if (str[i] == '%' && str[i + 1] != '\0')
// 		{
// 			i++;
// 			if (str[i] == 'c') // Char
// 				terminal_putchar(terminal, (char)va_arg(args, int32_t));
// 			else if (str[i] == 's') // String
// 				terminal_putstring(terminal, (char *)va_arg(args, int32_t *));
// 			else if (str[i] == 'i') // Integer
// 				print_number(terminal, va_arg(args, int32_t), 10);
// 			else if (str[i] == 'u') // Unsigned integer
// 				print_number(terminal, va_arg(args, uint32_t), 10);
// 			else if (str[i] == 'h') // Integer in base 16
// 				print_unsigned_number(terminal, va_arg(args, uint32_t), 16);
// 			else if (str[i] == 'o') // Integer in base 8
// 				print_unsigned_number(terminal, va_arg(args, uint32_t), 8);
// 			else if (str[i] == 'b') // Interger in base 2
// 				print_unsigned_number(terminal, va_arg(args, uint32_t), 2);
// 			else if (str[i] == 'p') // Address
// 				print_address(terminal, va_arg(args, int));
// 			else
// 			{
// 				terminal_putchar(terminal, '%');
// 				terminal_putchar(terminal, str[i]);
// 			}
// 		}
// 		else
// 			terminal_putchar(terminal, str[i]);
// 		i++;
// 	}
// 	va_end(args);
// }


// static void	print_number(t_terminal *terminal, int32_t number, int32_t base)
// {
// 	// Check base size
// 	if (base < 2)
// 		base = 2;
// 	if (base > 16)
// 		base = 16;

// 	// Safety check of min int32
// 	if (number == -2147483648)
// 	{
// 		terminal_putstring(terminal, "-2147483648");
// 		return ;
// 	}

// 	// Print - if number is negative
// 	if (number < 0)
// 	{
// 		terminal_putchar(terminal, '-');
// 		number *= -1;
// 	}

// 	// If number is greater than base, print previous char before
// 	if (number >= base)
// 		print_number(terminal, number / base, base);

// 	terminal_putchar(terminal, NUMBER_BASE[number % base]);
// }


// static void	print_unsigned_number(t_terminal *terminal, uint32_t number, uint32_t base)
// {
// 	if (base > 16)
// 		base = 16;

// 	if (number >= base)
// 		print_unsigned_number(terminal, number / base, base);

// 	terminal_putchar(terminal, NUMBER_BASE[number % base]);
// }


// static void	print_address(t_terminal *terminal, uint32_t number)
// {
// 	terminal_putstring(terminal, "0x");

// 	uint32_t	number_size = 1;
// 	uint32_t	number_tmp = number;

// 	while (number_tmp >= 16)
// 	{
// 		number_size++;
// 		number_tmp /= 16;
// 	}

// 	for (uint32_t i = number_size; i < 8; i++)
// 		terminal_putchar(terminal, '0');

// 	print_unsigned_number(terminal, number, 16);
// }
