#include <stdio.h>
#include <stdlib.h>

typedef struct s_map
{
	int		h, w, best, r, c;
	char	empty, ob, full;
	char	*grid;
}	t_map;

static int	error(t_map *m, char *line)
{
	free(line);
	free(m->grid);
	m->grid = NULL;
	printf("Error: invalid map\n");
	return (0);
}

static int	printable(char c)
{
	return (c >= 32 && c <= 126);
}

static int	read_map(FILE *f, t_map *m)
{
	char	*line = NULL;
	size_t	cap = 0;
	int		len;
	int		i = 0;

	len = getline(&line, &cap, f);
	if (len < 5 || line[len - 1] != '\n')
		return (error(m, line));
	while (line[i] >= '0' && line[i] <= '9')
		m->h = m->h * 10 + line[i++] - '0';
	if (m->h <= 0 || i != len - 4)
		return (error(m, line));
	m->empty = line[i];
	m->ob = line[i + 1];
	m->full = line[i + 2];
	if (!printable(m->empty) || !printable(m->ob) || !printable(m->full)
		|| m->empty == m->ob || m->empty == m->full || m->ob == m->full)
		return (error(m, line));
	for (int y = 0; y < m->h; y++)
	{
		len = getline(&line, &cap, f);
		if (len < 2 || line[len - 1] != '\n')
			return (error(m, line));
		line[--len] = 0;
		if (y == 0)
			m->w = len, m->grid = malloc(m->w * m->h);
		if (!m->grid || len != m->w)
			return (error(m, line));
		for (int x = 0; x < m->w; x++)
		{
			if (line[x] != m->empty && line[x] != m->ob)
				return (error(m, line));
			m->grid[y * m->w + x] = line[x];
		}
	}
	free(line);
	return (1);
}

static int	solve(t_map *m)
{
	int	*dp = calloc(m->w, sizeof(int));

	if (!dp)
		return (error(m, NULL));
	for (int y = 0; y < m->h; y++)
	{
		int diag = 0;

		for (int x = 0; x < m->w; x++)
		{
			int top = dp[x];

			if (m->grid[y * m->w + x] == m->ob)
				dp[x] = 0;
			else
			{
				int left = x ? dp[x - 1] : 0;
				int min = top < left ? top : left;

				dp[x] = (diag < min ? diag : min) + 1;
				if (dp[x] > m->best)
					m->best = dp[x], m->r = y - dp[x] + 1, m->c = x - dp[x] + 1;
			}
			diag = top;
		}
	}
	for (int y = m->r; y < m->r + m->best; y++)
		for (int x = m->c; x < m->c + m->best; x++)
			m->grid[y * m->w + x] = m->full;
	free(dp);
	return (1);
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
		for (int y = 0; y < m.h; y++)
			printf("%.*s\n", m.w, m.grid + y * m.w);
		free(m.grid);
	}
	if (ac == 2)
		fclose(f);
	return (0);
}
