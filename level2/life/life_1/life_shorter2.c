#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_life
{
	int		w, h, iter;
	char	*map[2];
}	t_life;

static int	inside(t_life *g, int y, int x)
{
	return (y >= 0 && y < g->h && x >= 0 && x < g->w);
}

static int	move_pen(t_life *g, char c, int *y, int *x, int *pen)
{
	if (c == 'w' && *y > 0)
		(*y)--;
	else if (c == 's' && *y < g->h - 1)
		(*y)++;
	else if (c == 'a' && *x > 0)
		(*x)--;
	else if (c == 'd' && *x < g->w - 1)
		(*x)++;
	else if (c == 'x')
		*pen = !*pen;
	else
		return (0);
	return (1);
}

static void	draw_input(t_life *g)
{
	int		x = 0;
	int		y = 0;
	int		pen = 0;
	char	c;

	while (read(0, &c, 1) > 0)
	{
		if (!move_pen(g, c, &y, &x, &pen))
			continue;
		if (pen)
			g->map[0][y * g->w + x] = 'O';
	}
}

static int	count_neighbors(t_life *g, char *map, int y, int x)
{
	int	n = 0;

	for (int dy = -1; dy <= 1; dy++)
		for (int dx = -1; dx <= 1; dx++)
			if ((dx || dy) && inside(g, y + dy, x + dx)
				&& map[(y + dy) * g->w + x + dx] == 'O')
				n++;
	return (n);
}

static char	next_cell(t_life *g, char *map, int y, int x)
{
	int	n = count_neighbors(g, map, y, x);

	if (n == 3 || (map[y * g->w + x] == 'O' && n == 2))
		return ('O');
	return (0);
}

static void	step(t_life *g, int cur, int next)
{
	for (int y = 0; y < g->h; y++)
		for (int x = 0; x < g->w; x++)
			g->map[next][y * g->w + x] = next_cell(g, g->map[cur], y, x);
}

static void	play(t_life *g)
{
	for (int i = 0; i < g->iter; i++)
		step(g, i % 2, (i + 1) % 2);
}

static void	print_map(t_life *g)
{
	char	*map = g->map[g->iter % 2];

	for (int y = 0; y < g->h; y++)
	{
		for (int x = 0; x < g->w; x++)
			putchar(map[y * g->w + x] ? 'O' : ' ');
		putchar('\n');
	}
}

static void	free_map(t_life *g)
{
	free(g->map[0]);
	free(g->map[1]);
}

int	main(int ac, char **av)
{
	t_life	g;

	if (ac != 4)
		return (1);
	g.w = atoi(av[1]);
	g.h = atoi(av[2]);
	g.iter = atoi(av[3]);
	if (g.w <= 0 || g.h <= 0 || g.iter < 0)
		return (1);
	g.map[0] = calloc(g.w * g.h, 1);
	g.map[1] = calloc(g.w * g.h, 1);
	if (!g.map[0] || !g.map[1])
		return (free_map(&g), 1);
	draw_input(&g);
	play(&g);
	print_map(&g);
	free_map(&g);
	return (0);
}
