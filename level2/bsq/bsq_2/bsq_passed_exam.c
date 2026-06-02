#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    int height, width;
    char free,ob,full;
    char **grid;
} t_map;

void free_map(t_map *map) {
    if (!map->grid) return;
    for (int i =0; i < map->height; i++) 
        free(map->grid[i]);
    free(map->grid);
    map->grid = NULL;
}

int notprintable (int c) {
    return (c < 32 || c >126);
}

void print_map(t_map *map) {
    for (int i =0; i < map->height; i++) 
        printf("%s\n", map->grid[i]);
}

int read_map(FILE *file, t_map *map) {
    char *line = NULL;
    size_t cap = 0;
    
    int len = getline(&line, &cap, file);
    if (len < 5 || line[len - 1] != '\n') 
    {
        free(line);
        return 1;
    }
    int i = 0;
    map->height = 0;
    while (i < len - 4 && line[i] >= '0' && line[i] <= '9') 
    {
        map->height = (map->height * 10) + line[i] - '0';
        i++;
    }
    if (i != len - 4)
    {
        free(line); 
        return 1;
    }
    map->free = line[i++];
    map->ob = line[i++];
    map->full = line[i++];
    if (map->height < 1 || map->free == map->ob || map->free == map->full
						|| map->full == map->ob || notprintable(map->free)
						|| notprintable(map->full) || notprintable(map->ob)) 
        return 1;
    map->grid = calloc(map->height, sizeof(char *));
    if (!map->grid)
		return 1;
        map->width = 0;
    for (int i = 0; i < map->height; i++) 
    {
        int len = getline(&line, &cap, file);
        if (len < 2 || line[len - 1] != '\n') 
        {
            free(line); 
            return 1;
        }
        --len;
        if (i == 0) 
            map->width = len;
        else if(map->width != len)
		{
			free(line);
			return 1;
		}
        for (int j = 0; j < map->width; j++) {
            if (line[j] != map->free && line[j] != map->ob) {
                free(line);
                return 1;
            }
        }
        map->grid[i] = calloc(map->width + 1, sizeof(char));
        if (!map->grid[i])
		{
			free(line);
			return 1;
		}
        for (int j = 0; j < map->width; j++) {
            map->grid[i][j] = line[j];
        }
        map->grid[i][map->width] = '\0';
    }
    free(line);
    return 0;
}

void solve(t_map *map) {
    int *row = calloc(map->width, sizeof(int));
    int bs = 0, br = 0, bc = 0;
    for (int i = 0; i < map->height; i++) {
        int diag = 0;
        for (int j = 0; j < map->width; j++) {
            int top = row[j];
            int size;
            if (map->grid[i][j] == map->ob) size = 0;
            else if (i == 0 || j == 0) size = 1;
            else {
                size = row[j - 1];
                if (top < size) size = top;
                if (diag < size) size = diag;
                size++;
            }
            row[j] = size;
            diag = top;
            if (size > bs) {
                bs = size;
                br = i - size + 1;
                bc = j - size + 1;
            }
        }
    }
    for (int i = br; i < br + bs; i++) {
        for (int j = bc; j < bc + bs; j++) {
            map->grid[i][j] = map->full;
        }
    }
    free(row);
}

void process (FILE *file) {
    t_map map = {0};
	if (read_map(file, &map))
        printf("Error: invalid map\n");
	else
	{
        solve(&map);
        print_map(&map);
    }
    free_map(&map);
}

int main(int argc, char **argv) {
    if (argc == 1)
		process(stdin);
    else
	{
        FILE *file;
        for (int i = 1; i < argc; i++) {
            file = fopen(argv[i], "r");
            if (!file) {printf("Error: invalid map\n"); continue;}
            process(file);
            fclose(file);
		}
    }
    return 0;
}
