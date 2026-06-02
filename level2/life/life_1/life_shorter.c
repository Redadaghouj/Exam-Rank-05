#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int	count(char *map, int w, int h, int y, int x)
{
	int	n = 0;

	for (int dy = -1; dy <= 1; dy++)
		for (int dx = -1; dx <= 1; dx++)
			if ((dx || dy) && y + dy >= 0 && y + dy < h
				&& x + dx >= 0 && x + dx < w
				&& map[(y + dy) * w + x + dx] == 'O')
				n++;
	return (n);
}

int	main(int ac, char **av)
{
	if (ac != 4)
		return (1);

	int w = atoi(av[1]);
	int h = atoi(av[2]);
	int iter = atoi(av[3]);

	if (w <= 0 || h <= 0 || iter < 0)
		return (1);

	char *map[2] = {calloc(w * h, 1), calloc(w * h, 1)};
	// if (!map[0] || !map[1]) return (1);
	int x = 0, y = 0, pen = 0;
	char c;

	while (read(0, &c, 1) > 0)
	{
		if (c == 'w' && y > 0)			y--;
		else if (c == 's' && y < h - 1)	y++;
		else if (c == 'a' && x > 0)		x--;
		else if (c == 'd' && x < w - 1)	x++;
		else if (c == 'x')				pen = !pen;
		else 							continue;
		if (pen)
			map[0][y * w + x] = 'O';
	}

	for (int it = 0; it < iter; it++)
	{
		int cur = it % 2;
		int next = (it + 1) % 2;

		for (y = 0; y < h; y++)
		{
			for (x = 0; x < w; x++)
			{
				int n = count(map[cur], w, h, y, x);

				map[next][y * w + x] = (n == 3
					|| (map[cur][y * w + x] == 'O' && n == 2)) ? 'O' : 0;
			}
		}
	}

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
			putchar(map[iter % 2][y * w + x] ? 'O' : ' ');
		putchar('\n');
	}
	free(map[0]);
	free(map[1]);
	return (0);
}
