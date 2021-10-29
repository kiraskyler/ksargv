#include "ksargv.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct
{
    bool select;            // is elem select
} s_ksargv_elems_status;

typedef struct
{
    s_values* next;/* data */
    char* val;
}s_values;

typedef struct
{
    s_options* next;
    s_options* back;
    s_values* values;

    char*   argv;
}s_options;

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
int argv_get_elem_index(char* args, s_ksargv_elems_status* status, s_ksargv_elems* elems, unsigned int elems_count)
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

int argv_get_int(char* mess, bool* res)
{
    int a = atoi(mess);
    if(a == 0 && mess[0] != '0')
        *res = false;
    return a;
}

double argv_get_double(char* mess, bool* res)
{
    char* endpoint;
    double num = strtod(mess, &endpoint);
    if(endpoint == mess)
        *res = false;
    return num;
}

bool argv_get_bool(char* mess, bool* res)
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

s_options* argc_get_options(char** argv)
{
    s_options* opts = NULL;
    s_values* val = NULL;

    unsigned int i = 0;

    while (argv[i++] != NULL)
    {
        if (argv[i][0] == '-')
        {
            /** new opts */
            if (opts == NULL)
                opts = KSARGV_MALLOC(sizeof(s_options));
            else
            {
                s_options* opt_buff = KSARGV_MALLOC(sizeof(s_options));
                opts->next = opt_buff;
                opts = opt_buff;
            }

            KSARGV_LOG_DEBUG("new options: %s", argv[i]);
            opts->values = NULL;
            opts->argv = argv[i];
            val = NULL;
        }
        else
        {
            if (val == NULL)
            {
                opts->values = KSARGV_MALLOC(sizeof(s_options));
                val = opts->values;

            }
            else
            {
               s_values* val_buff = KSARGV_MALLOC(sizeof(s_options));
               val->next = val_buff;
               val = val_buff;
            }
            
            KSARGV_LOG_DEBUG("new values: %s:%s", opts->argv, argv[i]);
            val->next = NULL;
            val->val = argv[i];
        }
    }
}

int ksargv_parse_argv(char** argv, s_ksargv_elems* elems, size_t elems_count)
{
    if(argv[0] == NULL || argv[1] == NULL || elems_count == 0)
        return 0;
    
    s_ksargv_elems_status* status = KSARGV_MALLOC(sizeof(s_ksargv_elems_status) * elems_count);
    if(status == NULL)
    {
        errno = ENOMEM;
        return -1;
    }

    s_options* opts = argc_get_options(argv[1]);

    if (opts == NULL)
        return -1;

    while (opts != NULL)
    {
        
        
        s_options* opts_buff = opts->next;
        KSARGV_FREE(opts);
        opts = opts_buff;
    }
    
    {
        int elem_index;
        if((elem_index = argv_get_elem_index(argv[argv_index], status, elems, elems_count)) >= 0)
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
                            values[arg_index].value.num_i = argv_get_int(argv[++argv_index], &res);
                            break;

                        case ARGV_BOOL:
                            if (argv[argv_index + 1] == NULL || argv[argv_index + 1][0] == '-')             // bool 后面可以不跟参数表示True
                                values[arg_index].value.num_b = argv_get_bool(NULL, &res);
                            else
                                values[arg_index].value.num_b = argv_get_bool(argv[++argv_index], &res);
                            break;

                        case ARGV_DOUBLE:
                            values[arg_index].value.num_d = argv_get_double(argv[++argv_index], &res);
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

