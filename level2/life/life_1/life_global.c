#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int	g_w;
static int	g_h;
static int	g_iter;
static char	*g_map[2];

static int	inside(int y, int x)
{
	return (y >= 0 && y < g_h && x >= 0 && x < g_w);
}

static int	move_pen(char c, int *y, int *x, int *pen)
{
	if (c == 'w' && *y > 0)
		(*y)--;
	else if (c == 's' && *y < g_h - 1)
		(*y)++;
	else if (c == 'a' && *x > 0)
		(*x)--;
	else if (c == 'd' && *x < g_w - 1)
		(*x)++;
	else if (c == 'x')
		*pen = !*pen;
	else
		return (0);
	return (1);
}

static void	draw_input(void)
{
	int		x = 0;
	int		y = 0;
	int		pen = 0;
	char	c;

	while (read(0, &c, 1) > 0)
	{
		if (!move_pen(c, &y, &x, &pen))
			continue;
		if (pen)
			g_map[0][y * g_w + x] = 'O';
	}
}

static int	count_neighbors(char *map, int y, int x)
{
	int	n = 0;

	for (int dy = -1; dy <= 1; dy++)
		for (int dx = -1; dx <= 1; dx++)
			if ((dx || dy) && inside(y + dy, x + dx)
				&& map[(y + dy) * g_w + x + dx] == 'O')
				n++;
	return (n);
}

static char	next_cell(char *map, int y, int x)
{
	int	n = count_neighbors(map, y, x);

	if (n == 3 || (map[y * g_w + x] == 'O' && n == 2))
		return ('O');
	return (0);
}

static void	step(int cur, int next)
{
	for (int y = 0; y < g_h; y++)
		for (int x = 0; x < g_w; x++)
			g_map[next][y * g_w + x] = next_cell(g_map[cur], y, x);
}

static void	play(void)
{
	for (int i = 0; i < g_iter; i++)
		step(i % 2, (i + 1) % 2);
}

static void	print_map(void)
{
	char	*map = g_map[g_iter % 2];

	for (int y = 0; y < g_h; y++)
	{
		for (int x = 0; x < g_w; x++)
			putchar(map[y * g_w + x] ? 'O' : ' ');
		putchar('\n');
	}
}

static void	free_map(void)
{
	free(g_map[0]);
	free(g_map[1]);
}

int	main(int ac, char **av)
{
	if (ac != 4)
		return (1);
	g_w = atoi(av[1]);
	g_h = atoi(av[2]);
	g_iter = atoi(av[3]);
	if (g_w <= 0 || g_h <= 0 || g_iter < 0)
		return (1);
	g_map[0] = calloc(g_w * g_h, 1);
	g_map[1] = calloc(g_w * g_h, 1);
	if (!g_map[0] || !g_map[1])
		return (free_map(), 1);
	draw_input();
	play();
	print_map();
	free_map();
	return (0);
}
