#pragma once

typedef struct CSV CSV;

CSV* csvRead(const char* filename);
const char* csvGet(CSV* csv, int row, const char* column);
void csvFree(CSV* csv);
