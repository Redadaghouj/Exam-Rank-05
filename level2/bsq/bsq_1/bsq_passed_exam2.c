#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

enum { EMPTY, OBSTACLE, FULL };

typedef struct {
    int x, y, size;
} t_bsq;

typedef struct {
    int x, y;
    char empty, obstacle, full;
    int map[MAX][MAX];
    t_bsq bsq;
} t_data;

int min3(int a, int b, int c) {
    int m = a < b ? a : b;
    return m < c ? m : c;
}

/* ---------- PARSING ---------- */

int parse_header(char *line, t_data *data) {
    int i = 0;

    if (!(line[i] >= '0' && line[i] <= '9'))
        return 0;

    data->y = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        data->y = data->y * 10 + (line[i] - '0');
        i++;
    }

    if (data->y <= 0)
        return 0;

    if (!line[i] || !line[i+1] || !line[i+2])
        return 0;

    data->empty = line[i++];
    data->obstacle = line[i++];
    data->full = line[i++];

    if (data->empty == data->obstacle ||
        data->empty == data->full ||
        data->obstacle == data->full)
        return 0;

    if (line[i] != '\n' && line[i] != '\0')
        return 0;

    return 1;
}

int read_map(FILE *file, t_data *data) {
    char *line = NULL;
    size_t len = 0;
    int y = 0;

    if (getline(&line, &len, file) == -1)
        return 0;

    if (!parse_header(line, data)) {
        free(line);
        return 0;
    }

    while (y < data->y && getline(&line, &len, file) != -1) {
        int x = 0;

        while (line[x] && line[x] != '\n')
            x++;

        if (x == 0 || x > MAX) {
            free(line);
            return 0;
        }

        if (y == 0)
            data->x = x;
        else if (x != data->x) {
            free(line);
            return 0;
        }

        for (int i = 0; i < x; i++) {
            if (line[i] == data->empty)
                data->map[y][i] = EMPTY;
            else if (line[i] == data->obstacle)
                data->map[y][i] = OBSTACLE;
            else {
                free(line);
                return 0;
            }
        }
        y++;
    }

    free(line);

    if (y != data->y)
        return 0;

    return 1;
}

/* ---------- BSQ ---------- */

void solve_bsq(t_data *data) {
    int dp[MAX][MAX] = {0};

    for (int y = 0; y < data->y; y++) {
        for (int x = 0; x < data->x; x++) {
            if (data->map[y][x] == OBSTACLE)
                dp[y][x] = 0;
            else if (x == 0 || y == 0)
                dp[y][x] = 1;
            else
                dp[y][x] = 1 + min3(dp[y-1][x], dp[y][x-1], dp[y-1][x-1]);

            if (dp[y][x] > data->bsq.size) {
                data->bsq.size = dp[y][x];
                data->bsq.x = x - dp[y][x] + 1;
                data->bsq.y = y - dp[y][x] + 1;
            }
        }
    }

    for (int y = 0; y < data->bsq.size; y++)
        for (int x = 0; x < data->bsq.size; x++)
            data->map[data->bsq.y + y][data->bsq.x + x] = FULL;
}

/* ---------- OUTPUT ---------- */

void print_map(t_data *data) {
    for (int y = 0; y < data->y; y++) {
        for (int x = 0; x < data->x; x++) {
            if (data->map[y][x] == EMPTY)
                fputc(data->empty, stdout);
            else if (data->map[y][x] == OBSTACLE)
                fputc(data->obstacle, stdout);
            else
                fputc(data->full, stdout);
        }
        fputc('\n', stdout);
    }
}

/* ---------- DRIVER ---------- */

void process(FILE *file) {
    t_data data = {0};

    if (!read_map(file, &data)) {
        printf("Error: invalid map\n");
        return;
    }

    solve_bsq(&data);
    print_map(&data);
}

int main(int ac, char **av) {
    if (ac == 1) {
        process(stdin);
    } else if (ac == 2) {
        FILE *f = fopen(av[1], "r");
        if (!f) {
            printf("Error: file not found\n");
            return 1;
        }
        process(f);
        fclose(f);
    }
    return 0;
}