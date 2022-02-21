#include <unistd.h>

void    ft_putchar(char c)
{
    write(1, &c, 1);
}

int	main(int argc, char **argv)
{
	int	symbols[256];
	int	i;

	if (argc == 3)
	{
		//Подготавливаем массив всех возможных символов
		i = 0;
		while (i < 256)
		{
			symbols[i] = 0;
			i++;
		}
		//Распечатываем символы 1-й строки
		i = 0;
		while (argv[1][i])
		{
			if (symbols[(unsigned char)argv[1][i]] == 0)
			{
				ft_putchar(argv[1][i]);
				symbols[(unsigned char)argv[1][i]] = 1; //Исключение повторов
			}
			i++;
		}
		//Распечатываем символы 2-й строки
		i = 0;
		while (argv[2][i])
		{
			if (symbols[(unsigned char)argv[2][i]] == 0)
			{
				ft_putchar(argv[2][i]);
				symbols[(unsigned char)argv[2][i]] = 1; //Исключение повторов
			}
			i++;
		}
	}
	//Перенос строки по умолчанию
	ft_putchar('\n');
	return (0);
}