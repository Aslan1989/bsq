#include "bsq.h"
#include <stdio.h>
#include <stdlib.h>

int validate_map(t_map *map, int line)
{
	int i;
	int j;
	i = 0;
	if (line != map->rows)
		return 0;
	if (map->rows <= 0 || map->cols <= 0)
		return 0;	
	if (map->empty == map->obstacle ||
		map->empty == map->full || map->obstacle == map->full)
		{
			return 0;
		}
	while (i < map->rows)
	{
		if (ft_strlen(map->grid[i]) != map->cols)
			return 0;
		j = 0;
		while (j < map->cols)
		{
			if (map->grid[i][j] != map->empty && map->grid[i][j] != map->obstacle)
				return 0;
			j++;	
		}	
		i++;
	}
	return 1;
}
void free_map(t_map *map)
{
	int i;
	if (!map->grid)
		return ;
	i = 0;
	while (i < map->rows)
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
}

void free_dp(int **dp, int rows)
{
	int i;
	if(!dp)
		return;
	i = 0;
	while (i < rows)
	{
		free(dp[i]);
		i++;
	}
	free(dp);	
}
int **create_dp(int rows, int cols)
{
	int **dp;
	int i;

	dp = calloc(rows, sizeof(int *));
	if (!dp)
		return NULL;
	i = 0;
	while (i < rows)
	{
		dp[i] = calloc(cols, sizeof(int));
		if (!dp[i])
		{
			free_dp(dp, rows);
			return NULL;
		}
		i++;
	}
	return dp;
}
int min3(int a, int b, int c)
{
	int min;
	min = a;
	if (b < min)
		min = b;
	if (c < min)
		min = c;
	return min;		
}

void fill_square(t_map *map, int max_size, int max_row, int max_col)
{
	int start_row;
	int start_col;
	int i, j;

	start_row = max_row - max_size + 1;
	start_col = max_col - max_size + 1;

	i = start_row;
	while (i <= max_row)
	{
		j = start_col;
		while (j <= max_col)
		{
			map->grid[i][j] = map->full;
			j++;
		}
		i++;
	}
}

void solve_bsq(t_map *map)
{
	int **dp;
	int i;
	int j;
	
	int	max_size;
	int	max_row;
	int	max_col;

	max_size = 0;
	max_row = 0;
	max_col = 0;

	dp = create_dp(map->rows, map->cols);
	if (!dp)
		return ;
	i = 0;
	while (i < map->rows)
	{
		j = 0;
		while (j < map->cols)
		{
			if (map->grid[i][j] == map->obstacle)
				dp[i][j] = 0;
			else
			{
				if (i == 0 || j == 0)
					dp[i][j] = 1;
				else
					dp[i][j] = min3(
						dp[i - 1][j],
						dp[i][j - 1],
						dp[i - 1][j - 1]
					) + 1;	
			}
			if (dp[i][j] > max_size)
			{
				max_size = dp[i][j];
				max_row = i;
				max_col = j;
			}
			j++;
		}
		i++;
	}	
	fill_square(map, max_size, max_row, max_col);
	free_dp(dp, map->rows);
}


int main(int argc, char **argv)
{
	FILE *file;
	t_map map;

	map.grid = NULL;
	map.rows = 0;
	map.cols = 0;
	if (argc != 2)
		return 1;
	file = fopen(argv[1], "r");
	if (!file)
		return 1;
	if (fscanf(file, "%d %c %c %c\n", &map.rows, 
			&map.empty,
			&map.obstacle,
			&map.full) != 4)
	{
		fclose(file);
		return (1);
	}

	map.grid = calloc(map.rows, sizeof(char *));
	if (!map.grid)
	{
		fclose(file);
		return (1);
	}

	char	*line;
	size_t	len;
	int		i;

	line = NULL;
	len = 0;
	i = 0;
	while (i < map.rows && getline(&line, &len, file) != -1)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (i == 0)
			map.cols = ft_strlen(line);	
		map.grid[i] = line;
		i++;
		line = NULL;
		len = 0;
	}

	if (!validate_map(&map, i))
	{
		fprintf(stderr, "map error\n");
		free_map(&map);
		fclose(file);
		return (1);
	}

	solve_bsq(&map);
	i = 0;
	while (i < map.rows)
	{
		fprintf(stdout, "%s\n", map.grid[i]);
		i++;
	}
	free_map(&map);
	free(line);
	fclose(file);
	return 0;
}