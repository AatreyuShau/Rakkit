#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char** keys;
    char** values;
    int count;
} Row;

struct CSV {
    Row* rows;
    int row_count;
    int col_count;
};

static char* trim(char* s) {
    while (*s == '\n' || *s == '\t' || *s == '\r' || *s == '\f' || *s == '\v') s++;

    char* end = s + strlen(s) - 1;
    while (end > s && (*end == '\r' || *end == '\n' || *end == '\f' || *end == '\v' || *end == '\t'))
        *end-- = '\0';

    return s;
}

CSV* csvRead(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return NULL;

    CSV* csv = calloc(1, sizeof(CSV));

    char line[4096];
    char* headers[128];
    int cols = 0;

    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        free(csv);
        return NULL;
    }

    char* tok = strtok(line, ",\n");
    while (tok) {
        headers[cols++] = strdup(trim(tok));
        tok = strtok(NULL, ",\n");
    }

    csv->rows = malloc(sizeof(Row) * 1024);
    csv->col_count = cols;

    while (fgets(line, sizeof(line), f)) {
        Row* r = &csv->rows[csv->row_count++];
        r->count = cols;
        r->keys = malloc(sizeof(char*) * cols);
        r->values = malloc(sizeof(char*) * cols);

        for (int i = 0; i < cols; i++)
            r->keys[i] = headers[i];

        tok = strtok(line, ",\n");
        for (int i = 0; i < cols && tok; i++) {
            r->values[i] = strdup(trim(tok));
            tok = strtok(NULL, ",\n");
        }
    }

    fclose(f);
    return csv;
}

const char* csvGet(CSV* csv, int row, const char* key) {
    if (!csv) return "";
    if (row < 0 || row >= csv->row_count) return "";

    Row* r = &csv->rows[row];
    for (int i = 0; i < r->count; i++) {
        if (strcmp(r->keys[i], key) == 0)
            return r->values[i];
    }
    return "";
}

void csvFree(CSV* csv) {
    if (!csv) return;

    for (int r = 0; r < csv->row_count; r++) {
        for (int c = 0; c < csv->col_count; c++) {
            free(csv->rows[r].values[c]);
        }
        free(csv->rows[r].values);
        free(csv->rows[r].keys);
    }

    free(csv->rows);
    free(csv);
}
