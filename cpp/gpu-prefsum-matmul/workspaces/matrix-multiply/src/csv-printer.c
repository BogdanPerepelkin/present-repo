#include "csv-printer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define csvprint_mkdir(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
static int csvprint_mkdir(const char* path) {
    return mkdir(path, 0755);
}
#endif

static char* csvprint_runtime_name = NULL;
static char* csvprint_full_path = NULL;
static int csvprint_at_line_start = 1;

static int csvprint_make_parent_dirs(const char* path) {
    if (!path) {
        return -1;
    }

    char* dir = strdup(path);
    if (!dir) {
        return -1;
    }

    char* last_slash = strrchr(dir, '/');
    char* last_back = strrchr(dir, '\\');
    char* last = last_back > last_slash ? last_back : last_slash;

    if (last) {
        *last = '\0';
        for (char* p = dir + 1; *p; ++p) {
            if (*p == '/' || *p == '\\') {
                char saved = *p;
                *p = '\0';
                csvprint_mkdir(dir);
                *p = saved;
            }
        }
        csvprint_mkdir(dir);
    }

    free(dir);
    return 0;
}

static void csvprint_build_full_path(void) {
    if (csvprint_full_path) {
        free(csvprint_full_path);
        csvprint_full_path = NULL;
    }

    const char* base = csvprint_OUTPUT_PATH;
    const char* raw = csvprint_runtime_name ? csvprint_runtime_name : csvprint_DEFAULT_FILENAME;

    int need_ext = (strchr(raw, '.') == NULL);
    size_t len = strlen(base) + strlen(raw) + (need_ext ? 4 : 0) + 1;

    csvprint_full_path = (char*) malloc(len);
    if (!csvprint_full_path) {
        return;
    }

    if (need_ext) {
        snprintf(csvprint_full_path, len, "%s%s.csv", base, raw);
    } else {
        snprintf(csvprint_full_path, len, "%s%s", base, raw);
    }

    csvprint_make_parent_dirs(csvprint_full_path);
}

static const char* csvPrintGetPath(void) {
    if (!csvprint_full_path) {
        csvprint_build_full_path();
    }
    return csvprint_full_path ? csvprint_full_path : csvprint_OUTPUT_PATH;
}

void csvSetPath(const char* path) {
    if (csvprint_runtime_name) {
        free(csvprint_runtime_name);
        csvprint_runtime_name = NULL;
    }

    if (path == NULL) {
        csvprint_build_full_path();
        return;
    }

    csvprint_runtime_name = strdup(path);
    csvprint_build_full_path();
}

int csvClearFile(void) {
    const char* path = csvPrintGetPath();
    FILE* f = fopen(path, "w");
    if (!f) {
        return -1;
    }
    fclose(f);
    csvprint_at_line_start = 1;
    return 0;
}

int csvAppendLine(const char* line) {
    if (line == NULL) {
        return -1;
    }
    const char* path = csvPrintGetPath();
    FILE* f = fopen(path, "a");
    if (!f) {
        return -1;
    }
    int res = fprintf(f, "%s\n", line);
    fclose(f);
    csvprint_at_line_start = 1;
    return (res < 0) ? -1 : 0;
}

int csvAppendFloatRow(const char* label, const float* values, size_t count) {
    const char* path = csvPrintGetPath();
    FILE* f = fopen(path, "a");
    if (!f) {
        return -1;
    }
    int ok = 0;
    if (label) {
        ok = fprintf(f, "%s", label);
    }
    for (size_t i = 0; i < count; ++i) {
        if (i == 0 && label == NULL) {
            ok = fprintf(f, "%f", values[i]);
        } else {
            ok = fprintf(f, ",%f", values[i]);
        }
        if (ok < 0) {
            fclose(f);
            return -1;
        }
    }
    fprintf(f, "\n");
    fclose(f);
    csvprint_at_line_start = 1;
    return 0;
}

int csvAppendRow(const char* label, const int* values, size_t count) {
    const char* path = csvPrintGetPath();
    FILE* f = fopen(path, "a");
    if (!f) {
        return -1;
    }
    int ok = 0;
    if (label) {
        ok = fprintf(f, "%s", label);
    }
    for (size_t i = 0; i < count; ++i) {
        if (i == 0 && label == NULL) {
            ok = fprintf(f, "%d", values[i]);
        } else {
            ok = fprintf(f, ",%d", values[i]);
        }
        if (ok < 0) {
            fclose(f);
            return -1;
        }
    }
    fprintf(f, "\n");
    fclose(f);
    csvprint_at_line_start = 1;
    return 0;
}

int csvAppendFloat(float value) {
    const char* path = csvPrintGetPath();
    FILE* f = fopen(path, "a");
    if (!f) {
        return -1;
    }
    int res;
    if (csvprint_at_line_start) {
        res = fprintf(f, "%f", value);
        csvprint_at_line_start = 0;
    } else {
        res = fprintf(f, ",%f", value);
    }
    fclose(f);
    return (res < 0) ? -1 : 0;
}

int csvNewLine(void) {
    const char* path = csvPrintGetPath();
    FILE* f = fopen(path, "a");
    if (!f) {
        return -1;
    }
    int res = fprintf(f, "\n");
    fclose(f);
    csvprint_at_line_start = 1;
    return (res < 0) ? -1 : 0;
}

