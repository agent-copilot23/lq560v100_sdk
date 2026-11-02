#include "opt_args.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static const char arguments[] = {
    'm','i','o','p','d','r','f','c',
};

bool is_positive_number(char* str)
{
	if (0 == strlen(str)) {
		return false;
	}
	bool are_all_digits_zeros = true;
	for (size_t idx = 0; idx < strlen(str); idx++) {
		if (0 == isdigit(str[idx])) {
			return false;
		}
		else {
			if (str[idx] != '0') {
				are_all_digits_zeros = false;
			}
		}
	}
	if (are_all_digits_zeros) {
		return false;
	}
	return true;
}

bool is_non_negative_number(char* str)
{
	if (0 == strlen(str)) {
		return false;
	}
	for (size_t idx = 0; idx < strlen(str); idx++) {
		if (0 == isdigit(str[idx])) {
			return false;
		}
	}
	return true;
}

int parse_optarg(int argc, char** argv, arg_config* opt_arg)
{
    for (int i = 0; i < argc; i++) {
        char* argument = argv[i];
        char* pos = strchr(argument, '-');
        int arg_nums = sizeof(arguments);
        if (pos && strlen(argument) == 2) {
            char val = *(pos + 1);
            int valid = -1;
            for (int j = 0; j < arg_nums; j++) {
                if (arguments[j] == val) {
                    valid = 0;
                    break;
                }
                valid = -1;
            }
            if (-1 == valid) {
                fprintf(stderr, "not support argument:%c, ref: ortm_run_sim_model -m xxx.ortm -i xxx.bin\n", val);
                return -1;
            }
        }
        char* content = argv[i+1];
        const int argument_max_len = 2;
        if (0 == strncmp("-m", argument, argument_max_len)) {
            opt_arg->model_path = content;
        } else if (0 == strncmp("-i", argument, argument_max_len)) {
            if (NULL == content || 0 == strlen(content)) {
                fprintf(stderr, "please do not specify an empty input path.\n");
                return -1;
	        }
            opt_arg->input_path = content;
        } else if (0 == strncmp("-o", argument, argument_max_len)) {
            opt_arg->output_path = content;
        } else if (0 == strncmp("-p", argument, argument_max_len)) {
            opt_arg->perf_mode = true;
        } else if (0 == strncmp("-d", argument, argument_max_len)) {
            opt_arg->dump_layer = true;
        } else if (0 == strncmp("-r", argument, argument_max_len)) {
            if (!is_positive_number(content)) {
                fprintf(stderr, "repeat must be a positive number but got %s\n", content);
                return -1;
            }
            opt_arg->repeat = atoi(content);
        } else if (0 == strncmp("-f", argument, argument_max_len)) {
            opt_arg->is_folder = true;
        } else if (0 == strncmp("-c", argument, argument_max_len)) {
            if (!is_non_negative_number(content)) {
                fprintf(stderr, "core id must be a non negative number but got %s\n", content);
                return -1;
            }
            opt_arg->core_ids = atoi(content);
        }
    }
    if (opt_arg->perf_mode && opt_arg->repeat > 1) {
        fprintf(stdout, "perf mode can not set -r!\n");
        opt_arg->repeat = 1;
    }
    return 0;
}