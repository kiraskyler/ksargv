#include "template.h"

#include <stdio.h>

#include "ksargv.h"

void template_version(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro);
char* version_option[] = {"-v", "-version", NULL};
e_argv_type version_type[] = {ARGV_STRING, ARGV_END};

void template_ip_port(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro);
char* ip_port_option[] = {"-ip", NULL};
e_argv_type ip_port_type[] = {ARGV_STRING, ARGV_INT, ARGV_END};

void template_am_i_great(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro);
char* am_i_great_option[] = {"-g", "-great", NULL};
e_argv_type am_i_great_type[] = {ARGV_BOOL, ARGV_END};

void template_score(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro);
char* score_option[] = {"-s", "-score", NULL};
e_argv_type score_type[] = {ARGV_DOUBLE, ARGV_END};

void template_keep(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro);
char* keep_option[] = {"-k", "-keep", NULL};
e_argv_type keep_type[] = {ARGV_END};


s_ksargv_elems argv_options[] =
{
    {
        .option = version_option,
        .args = version_type,
        .function = template_version,
    },
    {
        .option = ip_port_option,
        .args = ip_port_type,
        .function = template_ip_port,
    },
    {
        .option = am_i_great_option,
        .args = am_i_great_type,
        .function = template_am_i_great,
    },
    {
        .option = score_option,
        .args = score_type,
        .function = template_score,
    },
    {
        .option = keep_option,
        .args = keep_type,
        .function = template_keep,
    },
};

void template_display_erro(char* function_name, e_argv_erro erro)
{
    switch(erro)
    {
        case ARGV_ERRO_NONE:
            printf("%s : i'm not erro, why code run here\n", function_name);
            break;
        case ARGV_ERRO_LESS_ARGS:
            printf("%s : input less args in cmd, check stdin\n", function_name);
            break;
        case ARGV_ERRO:
            printf("%s : this erro can not describe esay, please check the ksargv source code\n", function_name);
            break;
        default:
            break;
    }
}

void template_display_all_argv(char* function_name, s_ksargv_value* values, unsigned int values_count)
{
    for(int i = 0; i < values_count; i++)
    {
        switch(values[i].type)
        {
            case ARGV_STRING:
                printf("%s: %s\n", function_name, values[i].value.str);
                break;
            case ARGV_INT:
                printf("%s: %d\n", function_name, values[i].value.num_i);
                break;
            case ARGV_BOOL:
                printf("%s: %s\n", function_name, values[i].value.num_b == 0 ? "false" : "true");
                break;
            case ARGV_DOUBLE:
                printf("%s: %f\n", function_name, values[i].value.num_d);
                break;
            default:
                break;
        }
    }
}

void template_version(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro)
{
    if(erro == 0)
        template_display_all_argv("version", values, values_count);
    else
        template_display_erro("version", erro);
}

void template_ip_port(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro)
{
    if(erro == 0 && values_count == 2)
    {
        printf("ip = %s\n", values[0].value.str);
        printf("port = %d\n", values[1].value.num_i);
    }
    else
        template_display_erro("ip_port", erro);
}

void template_am_i_great(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro)
{
    if(erro == 0 && values_count == 1)
    {
        if(values[0].value.num_b == true)
            printf("greate, thank you\n");
        else
            printf("fxxx!\n");
    }
    else
        template_display_erro("am_i_great", erro);
}

void template_score(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro)
{
    if(erro == 0 && values_count == 1)
    {
        if(values[0].value.num_d > 6.0)
            printf("thanks your grade %f\n", values[0].value.num_d);
        else
            printf("fxxx!\n");
    }
    else
        template_display_erro("score", erro);
}

void template_keep(e_argv_type* argv,  s_ksargv_value* values, unsigned int values_count, e_argv_erro erro)
{
    if(erro == 0)
        printf("thanks you, i will keep that\n");
    else
        template_display_erro("keep", erro);
}

int main(int argc, char** argv)
{
    ksargv_parse_argv(argv, argv_options, sizeof(argv_options) / sizeof(s_ksargv_elems));
    dbg_print_mem();
}
