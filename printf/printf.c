# include <stdio.h>
# include <unistd.h>
# include <stdarg.h>

//Структура флагов
typedef struct s_flags
{
	int		width;
	int		dot;
	int		type;
	int		sign;
}			t_flags;

//Замер длин переменных
static size_t	pf_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
static size_t	pf_intlen(long nbr)
{
	size_t	i;

	i = 1;
	if (nbr < 0)
		nbr = -nbr;
	while (nbr > 9)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}
static size_t	pf_hexlen(unsigned int nbr)
{
	size_t	i;

	i = 1;
	while (nbr > 15)
	{
		nbr /= 16;
		i++;
	}
	return (i);
}
//Назначение флагов
static int	pf_flag_dot(const char *str, t_flags *flags)
{
	int	i;

	flags->dot = 0;
	i = 1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		flags->dot = flags->dot * 10 + str[i] - '0';
		i++;
	}
	return (i);
}
static void	pf_flag_width(int c, t_flags *flags)
{
	flags->width = flags->width * 10 + c - '0';
}
//Распечатка символа
static void	pf_putchar(int c, size_t *len)
{
	write(1, &c, 1);
	*len += 1;
}
//Распечатка строки
static void	pf_putstr(char *str, size_t *len)
{
	int	i;

	i = 0;
	while (str[i])
	{
		pf_putchar(str[i], len);
		i++;
	}
}
//Распечатка недостоющей длины
static void	pf_putwidth(int width, int length, int zero, size_t *len)
{
	while (width - length > 0)
	{
		if (zero)
			pf_putchar('0', len);
		else
			pf_putchar(' ', len);
		length++;
	}
}
//Распечатка строки (S)
static void	pf_putlstr(char *str, int l, size_t *len)
{
	int	i;

	i = 0;
	while (str[i] && i < l)
	{
		pf_putchar(str[i], len);
		i++;
	}
}
static void	pf_putvar_str(char *str, t_flags flags, size_t *len)
{
	size_t	strlen;

	if (str == NULL)
		str = "(null)";
	strlen = pf_strlen(str);
	if (flags.dot < 0 || (flags.dot >= 0 && (size_t)flags.dot > strlen))
		flags.dot = strlen;
	pf_putwidth(flags.width, flags.dot, 0, len);
	pf_putlstr(str, flags.dot, len);
}
//Распечатка числа (D)
static void	pf_putnbr(long n, size_t *len)
{
	if (n < 0)
		pf_putnbr(-n, len);
	else if (n > 9)
	{
		pf_putnbr(n / 10, len);
		pf_putnbr(n % 10, len);
	}
	else
		pf_putchar(n + '0', len);
}
static void	pf_putvar_nbr(int n, t_flags flags, size_t *len)
{
	size_t	intlen;
	int		if_sign;

	if (n == 0 && flags.dot == 0)
		pf_putwidth(flags.width, 0, 0, len);
	else
	{
		if_sign = 0;
		if (n < 0)
			if_sign = 1;
		intlen = pf_intlen(n);
		if (flags.dot < 0 || (flags.dot >= 0 && (size_t)flags.dot < intlen))
			flags.dot = intlen;
		pf_putwidth(flags.width, (flags.dot + if_sign), 0, len);
		if (n < 0)
			pf_putchar('-', len);
		pf_putwidth(flags.dot, intlen, 1, len);
		pf_putnbr(n, len);
	}
}
//Распечатка хекса (X)
static void	pf_puthex(unsigned int n, char *alpha, size_t *len)
{
	char	buffer[50];
	char	*ptr;

	ptr = &buffer[49];
	*ptr = '\0';
	if (n == 0)
	{
		ptr--;
		*ptr = '0';
	}
	while (n != 0)
	{
		ptr--;
		*ptr = alpha[n % 16];
		n /= 16;
	}
	pf_putstr(ptr, len);
}
static void	pf_putvar_hex(unsigned int n, t_flags flags, char *alpha, size_t *len)
{
	size_t	convlen;

	if (n == 0 && flags.dot == 0)
		pf_putwidth(flags.width, 0, 0, len);
	else
	{
		convlen = pf_hexlen(n);
		if (flags.dot < 0 || (flags.dot >= 0 && (size_t)flags.dot < convlen))
			flags.dot = convlen;
		pf_putwidth(flags.width, flags.dot, 0, len);
		pf_putwidth(flags.dot, convlen, 1, len);
		pf_puthex(n, alpha, len);
	}
}
//Инициализация флагов
static t_flags	pf_flags_init(void)
{
	t_flags	flags;

	flags.width = 0;
	flags.dot = -1;
	flags.type = 0;
	return (flags);
}
//Разбор строки PrintF
static int	pf_parser(const char *str, t_flags *flags, int i)
{
	while (str[i] && ((str[i] >= '0' && str[i] <= '9') || str[i] == 's' || str[i] == 'd' || str[i] == 'x' || str[i] == '.'))
	{
		if (str[i] == '.')
			i += pf_flag_dot(&str[i], flags);
		if (str[i] >= '0' && str[i] <= '9')
			pf_flag_width(str[i], flags);
		if (str[i] == 's' || str[i] == 'd' || str[i] == 'x')
		{
			flags->type = str[i];
			break ;
		}
		i++;
	}
	return (i);
}
//Варианты вывода данных
static void	pf_put_variants(int c, t_flags *flags, va_list args, size_t *len)
{
	if (c == 's')
		pf_putvar_str(va_arg(args, char *), *flags, len);
	else if (c == 'd')
		pf_putvar_nbr(va_arg(args, int), *flags, len);
	else if (c == 'x')
		pf_putvar_hex(va_arg(args, unsigned int), *flags, "0123456789abcdef", len);
}
//Запуск функции PrintF
static int	pf_start(const char *str, va_list args)
{
	size_t	i;
	t_flags	flags;
	size_t len;

	i = 0;
	len = 0;
	while (str[i])
	{
		flags = pf_flags_init();
		while (str[i] && str[i] != '%')
		{
			pf_putchar(str[i], &len);
			i++;
		}
		if (str[i] == '%' && str[i + 1])
		{
			i = i + pf_parser(&str[i + 1], &flags, 0) + 2;
			pf_put_variants(flags.type, &flags, args, &len);
		}
		else if (str[i + 1] == '\0')
		{
			i++;
		}
	}
	return (len);
}
//Собственно функция PrintF
int	ft_printf(const char *string, ...)
{
	va_list	args;
	int		len;

	va_start(args, string);
	len = pf_start(string, args);
	va_end(args);
	return (len);
}

//ТЕСТИРОВАНИЕ
int main() 
{
	int			number = 2147483648;
	unsigned	unumber = 2147483648;
	char		*string = "mama-myla-ramu\0";

	ft_printf("number = %15.4d; hex = %18.6x; string = %12.10s;\n", number, unumber, string);
	   printf("number = %15.4d; hex = %18.6x; string = %12.10s;\n", number, unumber, string);

    return 0;
}