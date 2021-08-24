#include<stdio.h>
#include<stdint.h>

#define SDF_STRING_IMPLEMENTATION
#include "../sdf_string.h"

char string_buffer[512];

static void
print_string(SdfString string) {
    sdf_to_cstring(string_buffer, 512, string);
    printf("%s\n", string_buffer);
}

int
main(int arg_count, char **arg_vector)
{
    char buffer[256];
    SdfString string = {
        .len = 0,
        .ptr = buffer,
    };
    
    printf("sting len = %d\n", string.len);
    
    print_string(string);
    
    printf("\nAppending yay\n");
    print_string(string);
    
    SdfString string2 = string;
    printf("\nAppending yay until error\n");
    
    printf("\nString\n");
    print_string(string);
    
    printf("\nString 2\n");
    print_string(string2);
    
    printf("\nString\n");
    print_string(string);
    
    string.len = 16;
    string2.len = 0;
    char buffer2[256];
    string2.ptr = buffer2;
    
    printf("\nString\n");
    print_string(string);
    
    
    printf("\nString 2\n");
    print_string(string2);
    
    
    printf("\nString 2\n");
    print_string(string2);
    
    SdfString tok;
    do {
        tok = sdf_next_token(&string2, ' ');
        print_string(tok);
    } while (string2.len > 0);
    
    SdfSubstring sub_string = sdf_substring(string2, sdf_literal_to_string("batata"));
    
    printf("Substring = [%d : %d]\n", sub_string.offset, sub_string.len);
    
    SdfString string3 = sdf_string_between(string2,
                                           sdf_literal_to_string("batata"),
                                           sdf_literal_to_string("yay"));
    print_string(string3);
    printf("s3 == 'potato' = %s\n", 
           sdf_strcmp(string3 , sdf_literal_to_string("potato")) ? "true" : "false");
    
    sdf_remove_trailing_spaces(&string3);
    print_string(string3);
    printf("s3 == 'potato' = %s\n",
           sdf_strcmp(string3 , sdf_literal_to_string("potato")) ? "true" : "false");
    
    sdf_remove_leading_spaces(&string3);
    print_string(string3);
    printf("s3 == 'potato' = %s\n",
           sdf_strcmp(string3 , sdf_literal_to_string("potato")) ? "true" : "false");
    
}
