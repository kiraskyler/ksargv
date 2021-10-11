#include "ksargv.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct
{
    bool select;            // is elem select
} s_ksargv_elems_status;

#if 0
    static s_ksargv_value* cache_values;
    static unsigned int cache_value_count = 0;
    static unsigned int cache_value_size = 0;
#endif

/*********************************** dbg malloc *******************************************/
static int dbg_malloc_time = 0;

void* dbg_malloc(size_t size, const char* file, const char* func, int line)
{
    dbg_printf("%s,\t%s,%d", file, func, line);
    dbg_malloc_time++;
    return malloc(size);
}

void* dbg_realloc(void* mem, size_t size, const char* file, const char* func, int line)
{
    dbg_printf("%s,\t%s,%d", file, func, line);
    return realloc(mem, size);
}

void dbg_free(void* mem, const char* file, const char* func, int line)
{
    dbg_printf("%s,\t%s,%d", file, func, line);
    dbg_malloc_time--;
    free(mem);
}

void dbg_print_mem(void)
{
    dbg_printf("mem all alloc time = %d\n", dbg_malloc_time);
}

/*************************************** parse slow **************************************************/
int ksargv_parse_argv_get_elem_index(char* args, s_ksargv_elems_status* status, s_ksargv_elems* elems, unsigned int elems_count)
{
    if(args == NULL)
        return -1;

    for(int j = 0; j < elems_count; j++)
    {
        if(status[j].select == true)
            continue;

        for(int option_index = 0; elems[j].option[option_index] != NULL; option_index++)
        {
            /* option select */
            if(strcmp(elems[j].option[option_index], args) == 0)
                return j;
        }
    }
    return -1;
}

int ksargv_parse_argv_get_elem_argv_count(e_argv_type* args)
{
    int res = 0;
    while(args[res] < ARGV_END)
        res++;
    return res;
}

int ksargv_parse_argv_get_int(char* mess, bool* res)
{
    int a = atoi(mess);
    if(a == 0 && mess[0] != '0')
        *res = false;
    return a;
}

double ksargv_parse_argv_get_double(char* mess, bool* res)
{
    char* endpoint;
    double num = strtod(mess, &endpoint);
    if(endpoint == mess)
        *res = false;
    return num;
}

bool ksargv_parse_argv_get_bool(char* mess, bool* res)
{
    *res = true;
    
    if (mess == NULL)
        return true;
    if(strcmp(mess, "false") == 0 || strcmp(mess, "False") == 0)
        return false;
    else if(strcmp(mess, "true") == 0 || strcmp(mess, "True") == 0)
        return true;
    else
        return false;
}

#if 0
/*  */
int ksargv_value_cache_pull(s_ksargv_value* value)
{
    if(cache_value_size == 0)
    {
        cache_values = KSARGV_MALLOC(sizeof(s_ksargv_value) * 20);
        if(cache_values == NULL)
        {
            errno = ENOMEM;
            return -1;
        }
        cache_value_size = 20;
    }
    if(cache_value_count == cache_value_size)
    {
        s_ksargv_value* cache_values_buff = KSARGV_REALLOC(cache_values, sizeof(s_ksargv_value) * cache_value_size * 2);
        if(cache_values_buff == NULL)
        {
            errno = ENOMEM;
            return -1;
        }
        cache_values = cache_values_buff;
        cache_value_size = cache_value_size * 2;
    }
    if(cache_value_count < cache_value_size)
    {
        memcpy(&cache_values[cache_value_count], value, sizeof(s_ksargv_value));
        cache_value_count++;
        return cache_value_count - 1;
    }
    return -1;
}

s_ksargv_value* ksargv_value_cache_pop(int index)
{
    if(index < cache_value_count)
        return &cache_values[index];
    else
        return NULL;
}

void ksargv_value_cache_clean(void)
{
    KSARGV_FREE(cache_values);
    cache_value_count = 0;
    cache_value_size = 0;
}

#endif

int ksargv_parse_argv(char** argv, s_ksargv_elems* elems, unsigned int elems_count)
{
    if(argv[0] == NULL || argv[1] == NULL || elems_count == 0)
        return 0;
    
    s_ksargv_elems_status* status = KSARGV_MALLOC(sizeof(s_ksargv_elems_status) * elems_count);
    if(status == NULL)
    {
        errno = ENOMEM;
        return -1;
    }
    
    memset(status, 0, sizeof(s_ksargv_elems_status) * elems_count);
    for(int argv_index = 1; argv[argv_index] != NULL; argv_index++)
    {
        int elem_index;
        if((elem_index = ksargv_parse_argv_get_elem_index(argv[argv_index], status, elems, elems_count)) >= 0)
        {
            status[elem_index].select = true;
            int args_count = ksargv_parse_argv_get_elem_argv_count(elems[elem_index].args);
            
            if(args_count == 0)              /* this option do not need args */
                elems[elem_index].function(elems[elem_index].args, NULL, 0, ARGV_ERRO_NONE);
            else
            {
                /* ready */
                s_ksargv_value* values = KSARGV_MALLOC(sizeof(s_ksargv_value) * args_count);
                if(values == NULL)
                {
                    elems[elem_index].function(elems[elem_index].args, NULL, 0, ARGV_ERRO_LESS_ARGS);
                    errno = ENOMEM;
                    goto fall;
                }
                
                /* loop, convert argv */
                for(int arg_index = 0; arg_index < args_count; arg_index++)
                {
                    if(elems[elem_index].args[arg_index] != ARGV_BOOL && argv[argv_index + 1] == NULL)  // bool类型后面不跟，表示true
                    {
                        KSARGV_FREE(values);
                        elems[elem_index].function(elems[elem_index].args, NULL, 0, ARGV_ERRO_LESS_ARGS);
                        errno = ERESTART;
                        goto fall;
                    }
                    
                    bool res = true;
                    values[arg_index].type = elems[elem_index].args[arg_index];
                    switch(elems[elem_index].args[arg_index])
                    {
                        case ARGV_STRING:
                            values[arg_index].value.str = argv[++argv_index];
                            break;

                        case ARGV_INT:
                            values[arg_index].value.num_i = ksargv_parse_argv_get_int(argv[++argv_index], &res);
                            break;

                        case ARGV_BOOL:
                            if (argv[argv_index + 1] == NULL || argv[argv_index + 1][0] == '-')             // bool 后面可以不跟参数表示True
                                values[arg_index].value.num_b = ksargv_parse_argv_get_bool(NULL, &res);
                            else
                                values[arg_index].value.num_b = ksargv_parse_argv_get_bool(argv[++argv_index], &res);
                            break;

                        case ARGV_DOUBLE:
                            values[arg_index].value.num_d = ksargv_parse_argv_get_double(argv[++argv_index], &res);
                            break;

                        default:
                            break;
                    }

                    if(res == false)
                    {
                        KSARGV_FREE(values);
                        elems[elem_index].function(elems[elem_index].args, NULL, 0, ARGV_ERRO_PARSE);
                        errno = ERESTART;
                        goto fall;
                    }
                }

                /* done, send */
                switch (elems[elem_index].parse_tpye)
                {
                case ATGV_PARSE_FUNC:
                    elems[elem_index].function(elems[elem_index].args, values, args_count, ARGV_ERRO_NONE);
                    break;

                 case ATGV_PARSE_VALS:
                    for (int val_index = 0; val_index < args_count; val_index++)
                        switch(elems[elem_index].args[val_index])
                        {
                            case ARGV_STRING:
                                sprintf(elems[elem_index].vals[val_index], values[val_index].value.str);
                                break;

                            case ARGV_INT:
                                *(int* )elems[elem_index].vals[val_index] = values[val_index].value.num_i;
                                break;

                            case ARGV_BOOL:
                                *(bool* )elems[elem_index].vals[val_index] = values[val_index].value.num_b;
                                break;

                            case ARGV_DOUBLE:
                                *(double* )elems[elem_index].vals[val_index] = values[val_index].value.num_d;
                                break;

                            default:
                                break;
                        }
                        
                    break;
                
                default:
                    break;
                }
                KSARGV_FREE(values);
            }
        }
    }
    KSARGV_FREE(status);
    return 0;

fall:
    KSARGV_FREE(status);
    return -1;
}

