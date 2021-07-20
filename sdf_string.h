/* date = October 29th 2020 3:19 pm */

#ifndef SDF_STRING_H
#define SDF_STRING_H

#include <stdint.h>
#include <stdlib.h>

#ifndef SDF_NO_STDIO
#include <stdio.h>

#ifndef SDF_malloc
#define SDF_malloc malloc
#endif

#ifndef SDF_free
#define SDF_free free
#endif

#ifndef SDF_BOOL
#define SDF_BOOL
typedef unsigned int SdfBool;
#define SDF_TRUE  (0 == 0)
#define SDF_FALSE (0 == 1)
#endif

#endif

#ifndef SDF_ASSERT_H
#define SDF_ASSERT_H

#ifndef SDF_NOASSERT
#define SDF_ASSERT(x) \
if(!(x)) { \
printf("Assert failed %d:%s\n", __LINE__, __FILE__); \
exit(42); \
}
#else
#define SDF_ASSERT(x)
#endif

#endif


#define SDF_SFOR(s) for (uint32_t i = 0, it = s.ptr[0]; \
i < s.len; \
it = s.ptr[++i])

// ===== Definitions =====
typedef struct SdfString
{
    uint32_t len;
    uint32_t capacity;
    char *ptr;
} SdfString;

typedef enum SdfBuildStringError
{
    SdfBuildStringError_None = 0,
    SdfBuildStringError_StringFull = 1,
    SdfBuildStringError_NotEnoughSpace = 2,
} SdfBuildStringError;

typedef struct SdfSubstring
{
    uint32_t offset;
    uint32_t len;
} SdfSubstring;


static inline void
sdf_offset_string(SdfString *s, int32_t offset)
{
    s->ptr += offset;
    s->len -= offset;
    s->capacity -= offset;
}

#ifdef __cplusplus
extern "C" {
#endif
    extern SdfBuildStringError sdf_build_string_from_literal(SdfString *s, char *ls);
    extern SdfBuildStringError sdf_build_string_append_literal(SdfString *s, char *ls);
    
    extern SdfString sdf_next_token(SdfString *s, char separator);
    extern SdfString sdf_get_line(SdfString *s);
    extern SdfString sdf_literal_to_string(char *sl);
    extern SdfString sdf_string_between(SdfString s, SdfString first, SdfString second);
    
    extern SdfSubstring sdf_substring(SdfString s, SdfString sub);
    extern SdfBool sdf_strcmp(SdfString s1, SdfString s2);
    
    extern void sdf_build_string_append(SdfString *s1, SdfString s2);
    extern void sdf_build_string_copy(SdfString *s1, SdfString s2);
    extern void sdf_remove_trailing_spaces(SdfString *s);
    extern void sdf_remove_leading_spaces(SdfString *s);
    extern void sdf_to_cstring(char *buffer, size_t capacity, SdfString string);
    
#ifdef __cplusplus
}
#endif

#endif //DEBONI_STRING_H


#ifdef SDF_STRING_IMPLEMENTATION

// ===== Functions =====

void
sdf_to_cstring(char *buffer, size_t capacity, SdfString string)
{
    int i = 0;
    for (; i < string.len && i < capacity - 1; i++) {
        buffer[i] = string.ptr[i];
    }
    
    buffer[i] = 0;
}

SdfBuildStringError
sdf_build_string_from_literal(SdfString *s, char *ls)
{
    s->len = 0;
    for (int i = 0; *ls != 0; i++, ls++) {
        s->len++;
        s->ptr[i] = *ls;
        
        if (i >= s->capacity) {
            return SdfBuildStringError_StringFull;
        }
    }
    
    return SdfBuildStringError_None;
}

SdfBuildStringError
sdf_build_string_append_literal(SdfString *s, char *ls)
{
    for (int i = s->len; *ls != 0; i++, ls++) {
        s->len++;
        s->ptr[i] = *ls;
        
        if (i >= s->capacity) {
            return SdfBuildStringError_StringFull;
        }
    }
    
    return SdfBuildStringError_None;
}

void
sdf_build_string_append(SdfString *s1, SdfString s2)
{
    SDF_ASSERT(s1->capacity >= (s2.len + s1->len));
    
    SDF_SFOR(s2) {
        s1->ptr[i + s1->len] = it;
    }
    s1->len += s2.len;
}

void
sdf_build_string_copy(SdfString *s1, SdfString s2) {
    SDF_ASSERT(s1->capacity >= s2.len);
    
    s1->len = s2.len;
    SDF_SFOR(s2) {
        s1->ptr[i] = it;
    }
}

SdfString
sdf_next_token(SdfString *s, char separator)
{
    SdfString result = *s;
    
    result.len = 0;
    uint32_t max_len = s->len;
    while ((result.len < max_len) && (*(s->ptr) != separator)) {
        result.len++;
        s->capacity--;
        s->len--;
        s->ptr++;
    }
    
    if (s->len != 0) {
        s->ptr++;
        s->len--;
        s->capacity--;
    }
    
    return result;
}

// NOTE(Samuel): Works with both CRLF and LF
SdfString
sdf_get_line(SdfString *s)
{
    SdfString line = sdf_next_token(s, '\n');
    if (line.ptr[line.len-1] == '\r') line.len--;
    return line;
}

// NOTE(Samuel): Return a SdfString using the memory of a string literal
// You cannot chant its content, i think
SdfString
sdf_literal_to_string(char *sl)
{
    SdfString result = {0};
    result.ptr = (char *)sl;
    while (*(sl++) != 0) {
        result.len++;
        result.capacity++;
    }
    return result;
}

SdfSubstring
sdf_substring(SdfString s, SdfString sub)
{
    SdfSubstring result = {0};
    
    uint32_t i;
    for (i = 0; i < (s.len - sub.len + 1); i++) {
        if (s.ptr[i] == sub.ptr[0]) {
            SdfBool found = SDF_TRUE;
            for (uint32_t j = 0; j < sub.len; j++) {
                if (s.ptr[i + j] != sub.ptr[j]) {
                    found = SDF_FALSE;
                    break;
                }
            }
            if (found) {
                result.offset = i;
                result.len = sub.len;
                break;
            } 
        }
    }
    
    return result;
}

SdfString
sdf_string_between(SdfString s, SdfString first, SdfString second)
{
    SdfString result = s;
    
    SdfSubstring sub = sdf_substring(s, first);
    sdf_offset_string(&result, (int32_t)(sub.offset + sub.len));
    sub = sdf_substring(result, second);
    
    result.len = sub.offset;
    result.capacity = sub.offset;
    
    return result;
}

void
sdf_remove_trailing_spaces(SdfString *s)
{
    while (s->ptr[s->len - 1] == ' ' && s->len > 0) {
        s->len--;
    }
}

void
sdf_remove_leading_spaces(SdfString *s)
{
    while (s->ptr[0] == ' ' && s->len > 0) {
        sdf_offset_string(s, 1);
    }
}

SdfBool
sdf_strcmp(SdfString s1, SdfString s2)
{
    if (s1.len != s2.len) {
        return SDF_FALSE;
    }
    
    if (s1.ptr == s2.ptr) {
        return SDF_TRUE;
    }
    
    SDF_SFOR(s1) {
        if (it != s2.ptr[i]) {
            return SDF_FALSE;
        } 
    }
    
    return SDF_TRUE;
}


#endif