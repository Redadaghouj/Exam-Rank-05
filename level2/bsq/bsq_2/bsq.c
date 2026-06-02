#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int h, w, size, r, c;
    char empty, ob, full;
    char **grid;
} t_map;

int err(void) {
    printf("Error: invalid map\n");
    return 0;
}

int solve(t_map *m) {
    int *dp = calloc(m->w, sizeof(int));
    if (!dp) return err();
    for (int i = 0; i < m->h; i++) {
        int diag = 0;
        for (int j = 0; j < m->w; j++) {
            int top = dp[j];
            if (m->grid[i][j] == m->ob) dp[j] = 0;
            else if (m->grid[i][j] == m->empty) {
                int left = j > 0 ? dp[j - 1] : 0;
                int min = top < left ? top : left;
                dp[j] = (diag < min ? diag : min) + 1;
                if (dp[j] > m->size)
				{
                    m->size = dp[j];
					m->r = i - dp[j] + 1;
					m->c = j - dp[j] + 1;
				}
            } else return err();
            diag = top;
        }
    }
    for (int i = m->r; i < m->r + m->size; i++)
        for (int j = m->c; j < m->c + m->size; j++)
            m->grid[i][j] = m->full;
    free(dp);
    return 1;
}

int parse(FILE *f, t_map *m) {
    char *line = NULL;
    size_t cap = 0;
    int len = getline(&line, &cap, f);
    if (len < 5 || line[len - 1] != '\n') return err();

    int i = 0;
    while (line[i] >= '0' && line[i] <= '9')
		m->h = m->h * 10 + (line[i++] - '0');
    if (m->h <= 0 || i != len - 4) return err();

    m->empty = line[i];
	m->ob = line[i + 1];
	m->full = line[i + 2];
    if (m->empty == m->ob || m->empty == m->full || m->ob == m->full) return err();
    if (m->empty < 32 || m->ob < 32 || m->full < 32) return err();

    m->grid = malloc(sizeof(char *) * m->h);
    if (!m->grid) return err();
    cap = 0;
    for (int r = 0; r < m->h; r++) {
        m->grid[r] = NULL;
        len = getline(&m->grid[r], &cap, f);
        if (len < 2 || m->grid[r][len - 1] != '\n') return err();
        m->grid[r][--len] = '\0';
        if (r == 0) m->w = len;
        if (len != m->w) return err();
        for (int x = 0; x < m->w; x++)
            if (m->grid[r][x] != m->empty && m->grid[r][x] != m->ob)
                return err();
    }
    return 1;
}

int main(int ac, char **av) {
    FILE *f = ac > 1 ? fopen(av[1], "r") : stdin;
    if (!f) return err();

    t_map m = {0};
    if (parse(f, &m) && solve(&m))
        for (int r = 0; r < m.h; r++)
			printf("%s\n", m.grid[r]);
    if (ac > 1) fclose(f);
    return 0;
}