
#define SDF_CSV_IMPLEMENTATION
#define SDF_PRINT_ERRORS
#include "sdf_csv.h"

#define SDF_STRING_IMPLEMENTATION
#include "sdf_string.h"

#include <stdio.h>

char s_buffer[512];

int
main()
{
    SdfCsv csv = sdf_csv_open_parse("test.csv", (SdfCsvOptions){','});
    
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