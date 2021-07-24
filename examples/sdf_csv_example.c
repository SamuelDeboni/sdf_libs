
#define SDF_CSV_IMPLEMENTATION
#define SDF_PRINT_ERRORS
#include "../sdf_csv.h"

#define SDF_STRING_IMPLEMENTATION
#include "../sdf_string.h"

#include <stdio.h>
#include <stdlib.h>

char s_buffer[512];

static void
print_csv(SdfCsv csv)
{
    printf("cell_count   = %ld\n", csv.cell_count);
    printf("row_count    = %ld\n", csv.row_count);
    printf("column_count = %ld\n\n", csv.column_count);
    
    
    for (int i = 0; i < csv.row_count; i++) {
        printf("Row %d:\n", i);
        
        SdfRow row = sdf_csv_get_row(csv, i);
        
        for (int j = 0; j < row.count; j++) {
            sdf_to_cstring(&s_buffer[0], 512, row.cells[j]);
            printf("\tcolumn %d = %s\n", j, s_buffer);
        }
    }
}

int
main()
{
    // NOTE(samuel): One function parse
    {
        printf("==== One Function ====\n\n");
        SdfCsv csv = sdf_csv_open_parse("test.csv", (SdfCsvOptions){';'});
        print_csv(csv);
        free(csv.mem);
        free(csv.cells);
    }
    
    // NOTE(samuel): Parse from memory
    {
        printf("\n==== From Memory ====\n\n");
        SdfCsv csv = {};
        
        uint64_t data_size;
        
        // NOTE(samuel): You can read as binary, the parser handles CR/CRLR
        FILE *file = fopen("test.csv", "rb");
        
        fseek(file, 0, SEEK_END);
        data_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        uint8_t *data = (uint8_t *)malloc(data_size);
        fread(data, 1, data_size, file);
        
        csv = sdf_csv_parse(data, data_size, (SdfCsvOptions){';'});
        
        print_csv(csv);
        free(csv.mem);
        free(csv.cells);
    }
    
    // NOTE(samuel): No internal allocations, if SDF_NO_CRT is defined this is the only method available
    {
        printf("\n==== No internal allocations ====\n\n");
        
        uint64_t data_size;
        
        // NOTE(samuel): You can read as binary, the parser handles CR/CRLR
        FILE *file = fopen("test.csv", "rb");
        
        fseek(file, 0, SEEK_END);
        data_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        uint8_t *data = (uint8_t *)malloc(data_size);
        fread(data, 1, data_size, file);
        
        SdfCsv csv = sdf_csv_get_size(data, data_size, (SdfCsvOptions){';'});
        csv.cells = (SdfString *)malloc(csv.cell_count * sizeof(SdfString));
        sdf_parse_with_known_size(&csv);
        
        print_csv(csv);
    }
    
}