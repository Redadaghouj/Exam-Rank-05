#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int		w, h, iter, stride;
static char*	map[2];

static int	count(char *map, int p)
{
	return (map[p - 1] + map[p + 1]
			+ map[p - stride] + map[p + stride]
			+ map[p - stride - 1] + map[p + stride + 1]
			+ map[p - stride + 1] + map[p + stride - 1]);
}

int	main(int ac, char **av)
{
	if (ac != 4)
		return (1);

	w = atoi(av[1]);
	h = atoi(av[2]);
	iter = atoi(av[3]);
	if (w <= 0 || h <= 0 || iter < 0)
		return (1);

	stride = w + 2;
	map[0] = calloc((w + 2) * (h + 2), 1);
	map[1] = calloc((w + 2) * (h + 2), 1);
	if (!map[0] || !map[1])
		return (1);

	int x = 1, y = 1, p = stride + 1, pen = 0;
	char c;

	while (read(0, &c, 1) > 0)
	{
		if (c == 'w' && y > 1)		y--, p -= stride;
		else if (c == 's' && y < h)	y++, p += stride;
		else if (c == 'a' && x > 1)	x--, p--;
		else if (c == 'd' && x < w)	x++, p++;
		else if (c == 'x')			pen = !pen;
		else						continue;
		if (pen)
			map[0][p] = 1;
	}

	for (int it = 0; it < iter; it++)
		for (y = 1; y <= h; y++)
			for (x = 1; x <= w; x++)
			{
				p = y * stride + x;
				int n = count(map[it % 2], p);
				map[(it + 1) % 2][p] = (n == 3 || (map[it % 2][p] && n == 2));
			}

	for (y = 1; y <= h; y++)
	{
		for (x = 1; x <= w; x++)
			putchar(map[iter % 2][y * stride + x] ? 'O' : ' ');
		putchar('\n');
	}
	free(map[0]);
	free(map[1]);
	return (0);
}
