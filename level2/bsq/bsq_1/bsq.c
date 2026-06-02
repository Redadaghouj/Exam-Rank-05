#include <stdio.h>
#include <stdlib.h>

typedef struct s_map
{
	int		h, w;
	int		best, r, c;
	char	empty, ob, full;
	char	*grid;
}	t_map;

static int	bad(t_map *m, char *line)
{
	free(line);
	free(m->grid);
	m->grid = NULL;
	printf("Error: invalid map\n");
	return (0);
}

static int	is_print(char c)
{
	return (c >= 32 && c <= 126);
}

static int	parse_header(char *line, int len, t_map *m)
{
	int	i = 0;

	while (line[i] >= '0' && line[i] <= '9')
		m->h = m->h * 10 + line[i++] - '0';
	if (m->h <= 0 || i != len - 4)
		return (0);
	m->empty = line[i];
	m->ob = line[i + 1];
	m->full = line[i + 2];
	if (!is_print(m->empty) || !is_print(m->ob) || !is_print(m->full))
		return (0);
	if (m->empty == m->ob || m->empty == m->full || m->ob == m->full)
		return (0);
	return (1);
}

static int	save_row(t_map *m, char *line, int len, int y)
{
	if (y == 0)
	{
		m->w = len;
		m->grid = malloc((size_t)m->w * m->h);
	}
	if (m->w <= 0 || !m->grid || len != m->w)
		return (0);
	for (int x = 0; x < m->w; x++)
	{
		if (line[x] != m->empty && line[x] != m->ob)
			return (0);
		m->grid[y * m->w + x] = line[x];
	}
	return (1);
}

static int	read_map(FILE *f, t_map *m)
{
	char	*line = NULL;
	size_t	cap = 0;
	int		len;

	len = getline(&line, &cap, f);
	if (len < 5 || line[len - 1] != '\n' || !parse_header(line, len, m))
		return (bad(m, line));
	for (int y = 0; y < m->h; y++)
	{
		len = getline(&line, &cap, f);
		if (len < 2 || line[len - 1] != '\n')
			return (bad(m, line));
		if (!save_row(m, line, len - 1, y))
			return (bad(m, line));
	}
	free(line);
	return (1);
}

static int	min3(int a, int b, int c)
{
	int	min = a;

	if (b < min)
		min = b;
	if (c < min)
		min = c;
	return (min);
}

static void	keep_best(t_map *m, int y, int x, int size)
{
	if (size > m->best)
	{
		m->best = size;
		m->r = y - size + 1;
		m->c = x - size + 1;
	}
}

static int	solve(t_map *m)
{
	int	*dp = calloc(m->w, sizeof(int));

	if (!dp)
		return (bad(m, NULL));
	for (int y = 0; y < m->h; y++)
	{
		int	diag = 0;

		for (int x = 0; x < m->w; x++)
		{
			int	top = dp[x];

			if (m->grid[y * m->w + x] == m->ob)
				dp[x] = 0;
			else
				dp[x] = min3(top, x ? dp[x - 1] : 0, diag) + 1;
			keep_best(m, y, x, dp[x]);
			diag = top;
		}
	}
	free(dp);
	return (1);
}

static void	fill_square(t_map *m)
{
	for (int y = m->r; y < m->r + m->best; y++)
		for (int x = m->c; x < m->c + m->best; x++)
			m->grid[y * m->w + x] = m->full;
}

static void	print_map(t_map *m)
{
	for (int y = 0; y < m->h; y++)
		printf("%.*s\n", m->w, m->grid + y * m->w);
}

int	main(int ac, char **av)
{
	FILE	*f;
	t_map	m = {0};

	if (ac > 2)
		return (printf("Error: invalid map\n"), 1);
	f = ac == 2 ? fopen(av[1], "r") : stdin;
	if (!f)
		return (printf("Error: invalid map\n"), 1);
	if (read_map(f, &m) && solve(&m))
	{
		fill_square(&m);
		print_map(&m);
	}
	free(m.grid);
	if (ac == 2)
		fclose(f);
	return (0);
}
