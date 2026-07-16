#ifndef CSV_PRINTER_H
#define CSV_PRINTER_H

#include <stddef.h>

#ifndef csvprint_OUTPUT_PATH
#define csvprint_OUTPUT_PATH "/Users/bogdan/ct-dance/gpu/workspaces/matrix-multiply/tables/"
#endif

#define csvprint_DEFAULT_FILENAME "csv_output.csv"

void csvSetPath(const char* path);

int csvClearFile(void);

int csvAppendLine(const char* line);

int csvAppendFloatRow(const char* label, const float* values, size_t count);

int csvAppendRow(const char* label, const int* values, size_t count);

int csvAppendFloat(float value);
int csvNewLine(void);

#endif
