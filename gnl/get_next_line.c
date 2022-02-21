#include "get_next_line.h"

int	get_next_line(char **line)
{
	//Инициализируем переменные
	int		i;
	int		bytes;
	char	buf_c;

	//Проверяем существование LINE и выделяем под неё память с запасом
	if (line == NULL)
		return (-1);
	*line = malloc(sizeof(char) * 1000000);
	if (*line == NULL)
		return (-1);
	//Наполняем LINE
	i = 0;
	while ((bytes = read(0, &buf_c, 1)) > 0 && buf_c != '\n')
	{
		(*line)[i] = buf_c;
		i++;
	}
	(*line)[i] = '\0';
	//Если не удалось чтение, то помимо возврата "-1" нужно очистить LINE
	if (bytes == -1)
	{
		free(*line);
		*line = NULL;
		return (-1);
	}
	//Если дошли до конца файла, то возвращаем "0"
	if (bytes == 0)
		return (0);
	//Возвращаем "1" по умолчанию
	return (1);
}