#ifndef BSQ_H
#define BSQ_H

typedef struct s_map
{
	int rows;
	int cols;
	char empty;
	char obstacle;
	char full;
	char **grid;
} t_map;

int ft_strlen(char *str)
{
	int i;
	i = 0;
	while (str[i])
		i++;
	return(i);
}
#endif