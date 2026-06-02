#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct s_game {
    int width;
    int height;
    int iteration;
    char **map;
} t_game;

static void print_map(t_game game)
{
    for (int y = 0; y < game.height; y++)
    {
        for (int x = 0; x < game.width; x++)
            putchar(game.map[y][x]);
        putchar('\n');
    }
}

static void free_map(t_game *game)
{
    if (!game->map)
        return;
    for (int y = 0; y < game->height; y++)
        free(game->map[y]);
    free(game->map);
}

static void fill_map(t_game *game)
{
    int x = 0, y = 0;
    bool draw = false;
    char buffer;

    while (read(STDIN_FILENO, &buffer, 1) > 0)
    {
        switch (buffer)
        {
            case 'w': if (y > 0) y--; break;
            case 'a': if (x > 0) x--; break;
            case 's': if (y < game->height - 1) y++; break;
            case 'd': if (x < game->width - 1) x++; break;
            case 'x': draw = !draw; break;
            default: continue;
        }
        if (draw)
            game->map[y][x] = 'O';
    }
}

static int count_neighbors(t_game game, int y, int x)
{
    int count = 0;
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < game.width && ny >= 0 && ny < game.height)
                if (game.map[ny][nx] == 'O')
                    count++;
        }
    return count;
}

static void play_game(t_game *game)
{
    char **new_map = malloc(game->height * sizeof(char *));
    for (int y = 0; y < game->height; y++)
    {
        new_map[y] = malloc(game->width * sizeof(char));
        for (int x = 0; x < game->width; x++)
            new_map[y][x] = ' ';
    }

    for (int y = 0; y < game->height; y++)
        for (int x = 0; x < game->width; x++)
        {
            int n = count_neighbors(*game, y, x);
            if (game->map[y][x] == 'O')
                new_map[y][x] = (n == 2 || n == 3) ? 'O' : ' ';
            else
                new_map[y][x] = (n == 3) ? 'O' : ' ';
        }

    free_map(game);
    game->map = new_map;
}

int main(int argc, char **argv)
{
    if (argc != 4)
        return 1;

    t_game game;
    game.width = atoi(argv[1]);
    game.height = atoi(argv[2]);
    game.iteration = atoi(argv[3]);

    if (game.width <= 0 || game.height <= 0 || game.iteration < 0)
        return 1;

    game.map = malloc(game.height * sizeof(char *));
    for (int y = 0; y < game.height; y++)
    {
        game.map[y] = malloc(game.width * sizeof(char));
        for (int x = 0; x < game.width; x++)
            game.map[y][x] = ' ';
    }

    fill_map(&game);

    for (int i = 0; i < game.iteration; i++)
        play_game(&game);

    print_map(game);
    free_map(&game);
    return 0;
}
