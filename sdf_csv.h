/* date = July 20th 2021 11:17 am */

#ifndef SDF_CSV_H
#define SDF_CSV_H

#include "sdf_string.h"
#include <stdint.h>

#ifndef SDF_print

#if (defined(SDF_PRINT_ERRORS) && !defined(SDF_NO_CRT))
#include <stdio.h>
#define SDF_print printf
#else
#define SDF_print(...)
#endif

#endif // SDF_print

#ifndef SDF_NO_CRT
#include <stdlib.h>

#ifndef SDF_malloc
#define SDF_malloc malloc
#endif

#ifndef SDF_free
#define SDF_free free
#endif

#endif // SDF_NO_CRT

#ifndef SDF_BOOL
#define SDF_BOOL
typedef unsigned int SdfBool;
#define SDF_TRUE  (0 == 0)
#define SDF_FALSE (0 == 1)
#endif

typedef struct {
    char separator;
    int no_header;
    int traling_comma;
} SdfCsvOptions;


typedef struct {
    SdfString *cells;
    uint64_t cell_count;
    
    uint64_t column_count;
    uint64_t row_count;
    
    uint8_t *mem;
    uint64_t mem_size;
    
    SdfCsvOptions options;
} SdfCsv;

typedef struct {
    SdfString *cells;
    uint64_t count;
} SdfRow;

#ifdef __cplusplus
extern "C" {
#endif
    
#ifndef SDF_NO_CRT
    SdfCsv sdf_csv_open_parse(char *file_path, SdfCsvOptions options);
    SdfCsv sdf_csv_parse(uint8_t *data, uint64_t data_size,SdfCsvOptions options);
#endif
    SdfCsv sdf_csv_get_size(uint8_t *data, uint64_t data_size,SdfCsvOptions options);
    void sdf_parse_with_known_size(SdfCsv *csv);
    SdfString sdf_csv_get_cell(SdfCsv csv, uint64_t column, uint64_t row);
    SdfRow sdf_csv_get_row(SdfCsv csv, uint64_t row);
#ifdef __cplusplus
}
#endif


#endif //SDF_CSV_H


#ifdef SDF_CSV_IMPLEMENTATION



#ifndef SDF_NO_CRT


static uint8_t *
sdf_read_file_all(char *path, uint64_t *size)
{
    if (size == 0) return 0;
    
    FILE *file = fopen(path, "rb");
    
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    
    uint8_t *result = (uint8_t *)SDF_malloc(*size);
    
    if (fread(result, 1, *size, file)) {
        return result;
    }
    
    SDF_free(result);
    return 0;
}

SdfCsv
sdf_csv_open_parse(char *file_path, SdfCsvOptions options)
{
    SdfCsv result = {};
    uint64_t data_size = 0; 
    
    uint8_t *data = sdf_read_file_all(file_path, &data_size);
    if (data == 0) {
        return result;
    }
    
    result = sdf_csv_parse(data, data_size, options);
    if (!result.mem) {
        SDF_free(data);
    }
    
    return result;
}


SdfCsv
sdf_csv_parse(uint8_t *data, uint64_t data_size, SdfCsvOptions options)
{
    SdfCsv result = sdf_csv_get_size(data, data_size, options);
    if (result.mem) {
        result.cells = (SdfString *)SDF_malloc(result.cell_count * sizeof(SdfString));
        sdf_parse_with_known_size(&result);
    }
    return result;
}


#endif


SdfCsv
sdf_csv_get_size(uint8_t *data, uint64_t data_size,SdfCsvOptions options)
{
    SdfCsv result = {};
    
    result.options = options;
    
    result.mem = data;
    result.mem_size = data_size;
    
    int first_line = 0;
    for (uint64_t i = 0; i < result.mem_size; i++) {
        char c = result.mem[i];
        
        if (c == options.separator) {
            result.cell_count++;
        } else if (c == '\n') {
            if (!options.traling_comma) {
                result.cell_count++;
            }
            
            result.row_count++;
            
            if (first_line == 0) {
                result.column_count = result.cell_count;
                first_line = 1;
            }
            
            
            if (result.row_count * result.column_count != result.cell_count) {
                
                SDF_print("Error, line %ld has does not have the correct column count\n", result.row_count);
                return (SdfCsv){};
            }
            
        }
    }
    
    // NOTE(samuel): if the file does not end on /n or ,
    {
        if (result.mem[result.mem_size - 1] != '\n') {
            if (!options.traling_comma && result.row_count == 0) {
                result.column_count++;
            }
            
            result.cell_count++;
            result.row_count++;
        }
    }
    
    if (result.row_count * result.column_count != result.cell_count) {
        SDF_print("Error, last line has does not have the correct column count\n");
        return (SdfCsv){};
    }
    
    return result;
}

void
sdf_parse_with_known_size(SdfCsv *csv)
{
    SdfString tmp = { csv->mem_size, (char *)csv->mem };
    SdfString *cell = csv->cells;
    for (uint64_t ri = 0; ri < csv->row_count; ri++) {
        
        SdfString line = sdf_get_line(&tmp);
        for (uint64_t ci = 0; ci < csv->column_count; ci++) {
            *cell = sdf_next_token(&line, csv->options.separator);
            cell++;
        }
    }
}


SdfString
sdf_csv_get_cell(SdfCsv csv, uint64_t column, uint64_t row)
{
    SdfString result = {};
    
    if (column < csv.column_count && row < csv.row_count) {
        result = csv.cells[csv.column_count * row + column];
    }
    
    return result;
}


SdfRow
sdf_csv_get_row(SdfCsv csv, uint64_t row)
{
    SdfRow result = {};
    
    if (row < csv.row_count) {
        result.cells = csv.cells + (csv.column_count * row);
    }
    
    result.count = csv.column_count;
    
    return result;
}

#endif // SDF_CSV_IMPLEMENTATION
