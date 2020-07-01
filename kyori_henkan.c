/*
b1018194 Ito Hajime
二値二次元配列を距離変換し、骨格を取り出す画像処理プログラム
*/
#include <stdio.h>
#include <stdlib.h>

#define ILAND 1
#define SPACE 0
#define OUTSPACE -1
#define MAX_SIZE 400
#define SEARCH_TABLE_SIZE 2
#define SKELETON_TABLE_SIZE 4

typedef struct map
{
    int **map;
    int row_size;
    int col_size;
} MAP;

MAP init_map(int height, int width);
void set_iland(MAP *m, char *s, int row);
void print_map(MAP m);
int minElement(const int *array, int size);
void distance(MAP *m);
void skeleton(MAP *m);
void free_map(MAP *m);

int main(void)
{
    char buf[MAX_SIZE], s[MAX_SIZE];
    int h, w, lookup_table[MAX_SIZE][1], max_label_num = 0;
    MAP m;

    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%d %d\n", &h, &w);

    m = init_map(h, w);

    for (int i = 0; i < h; i++)
    {
        fgets(buf, sizeof(buf), stdin);
        sscanf(buf, "%s\n", s);
        set_iland(&m, s, i);
    }
    distance(&m);
    skeleton(&m);
    print_map(m);
    free_map(&m);
    return 0;
}

MAP init_map(int height, int width)
{
    MAP m;
    m.row_size = height + 2;
    m.col_size = width + 2;

    m.map = (int **)malloc(m.row_size * sizeof(int *));
    for (int i = 0; i < m.row_size; i++)
        m.map[i] = (int *)malloc(m.col_size * sizeof(int));

    for (int i = 0; i < m.row_size; i++)
        for (int j = 0; j < m.col_size; j++)
            m.map[i][j] = OUTSPACE;

    return m;
}

void free_map(MAP *m)
{
    for (int i = 0; i < m->col_size; i++)
    {
        free(m->map[i]);
    }
    free(m->map);
}

void set_iland(MAP *m, char s[MAX_SIZE], int row)
{
    int i = 0;

    while (i < m->col_size - 2)
    {
        if (s[i] == '.')
            m->map[row + 1][i + 1] = SPACE;
        else if (s[i] == '#')
            m->map[row + 1][i + 1] = ILAND;
        else
            m->map[row + 1][i + 1] = SPACE;
        i++;
    }
}

void print_map(MAP m)
{
    for (int i = 0; i < m.row_size; i++)
    {
        if (i != 0)
            printf("\n");

        for (int j = 0; j < m.col_size; j++)
        {
            if (m.map[i][j] == OUTSPACE)
            {
                printf("*");
            }
            else
            {
                printf("%d", m.map[i][j]);
            }
        }
    }
}

int minElement(const int *array, int size)
{
    int min = array[0];
    for (int i = 1; i < size; ++i)
    {
        if (min > array[i])
        {
            min = array[i];
        }
    }

    return min;
}

void distance(MAP *m)
{
    int search_table[SEARCH_TABLE_SIZE];

    /*ラスタスキャン*/
    for (int i = 0; i < m->row_size; i++)
    {
        for (int j = 0; j < m->col_size; j++)
        {
            /*ILAND以外ならスキップ*/
            if (m->map[i][j] < ILAND)
                continue;

            search_table[0] = m->map[i][j - 1];
            search_table[1] = m->map[i - 1][j];

            /*OUTSPACEはSPACEとして扱う*/
            for (int n = 0; n < SEARCH_TABLE_SIZE; n++)
            {
                if (search_table[n] == OUTSPACE)
                    search_table[n] = SPACE;
            }

            search_table[0]++;
            search_table[1]++;

            int min = minElement(search_table, SEARCH_TABLE_SIZE);
            m->map[i][j] = min;
        }
    }

    /*逆ラスタスキャン*/
    for (int i = m->row_size - 1; i > 0; i--)
    {
        for (int j = m->col_size - 1; j > 0; j--)
        {
            /*ILAND以外ならスキップ*/
            if (m->map[i][j] < ILAND)
                continue;

            search_table[0] = m->map[i][j + 1];
            search_table[1] = m->map[i + 1][j];

            /*OUTSPACEはSPACEとして扱う*/
            for (int n = 0; n < SEARCH_TABLE_SIZE; n++)
            {
                if (search_table[n] == OUTSPACE)
                    search_table[n] = SPACE;
            }

            search_table[0]++;
            search_table[1]++;

            int min = minElement(search_table, SEARCH_TABLE_SIZE);
            if (min > m->map[i][j])
                continue;
            m->map[i][j] = min;
        }
    }
}

void skeleton(MAP *m)
{
    int skeleton_table[SKELETON_TABLE_SIZE];

    for (int i = 0; i < m->row_size; i++)
    {
        for (int j = 0; j < m->col_size; j++)
        {
            /*ILAND以外ならスキップ*/
            if (m->map[i][j] < ILAND)
                continue;

            int flag = 1;
            skeleton_table[0] = m->map[i][j - 1];
            skeleton_table[1] = m->map[i - 1][j];
            skeleton_table[2] = m->map[i][j + 1];
            skeleton_table[3] = m->map[i + 1][j];

            /*OUTSPACEはSPACEとして扱う*/
            for (int n = 0; n < SKELETON_TABLE_SIZE; n++)
            {
                if (m->map[i][j] < skeleton_table[n])
                {
                    flag = 0;
                    continue;
                }
            }

            if (flag == 0)
                continue;

            printf("[r = %c, c = %d] v=%d\n", i + 96, j, m->map[i][j]);
        }
    }
}