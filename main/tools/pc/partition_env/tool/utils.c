/*
  Copyright (c), 2021-2022, Oritek Tech. Co., Ltd.
 */
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "partition_env_tool_priv.h"

static inline int isdigit(int c)
{
	return '0' <= c && c <= '9';
}

static inline int isxdigit(int c)
{
	return ('0' <= c && c <= '9') ||
            ('a' <= c && c <= 'f') ||
            ('A' <= c && c <= 'F');
}

/* Works only for digits and letters, but small and fast */
#define TOLOWER(x) ((x) | 0x20)

static unsigned int simple_guess_base(const char *cp)
{
	if (cp[0] == '0') {
		if (TOLOWER(cp[1]) == 'x' && isxdigit(cp[2]))
			return 16;
		else
			return 8;
	} else {
		return 10;
	}
}

static unsigned long long simple_strtoull(const char *cp, char **endp, unsigned int base)
{
	unsigned long long result = 0;

	if (!base)
		base = simple_guess_base(cp);

	if (base == 16 && cp[0] == '0' && TOLOWER(cp[1]) == 'x')
		cp += 2;

	while (isxdigit(*cp)) {
		unsigned int value;

		value = isdigit(*cp) ? *cp - '0' : TOLOWER(*cp) - 'a' + 10;
		if (value >= base)
			break;
		result = result * base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;

	return result;
}

unsigned long long memparse(const char *ptr, char **retptr)
{
	char *endptr;	/* local pointer to end of parsed string */

	unsigned long long ret = simple_strtoull(ptr, &endptr, 0);

	switch (*endptr) {
	case 'E':
	case 'e':
		ret <<= 10;
		/* fall through */
	case 'P':
	case 'p':
		ret <<= 10;
		/* fall through */
	case 'T':
	case 't':
		ret <<= 10;
		/* fall through */
	case 'G':
	case 'g':
		ret <<= 10;
		/* fall through */
	case 'M':
	case 'm':
		ret <<= 10;
		/* fall through */
	case 'K':
	case 'k':
		ret <<= 10;
		endptr++;
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}

char* read_file(const char *filename)
{
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;

    /* open in read binary mode */
    file = fopen(filename, "rb");
    if (file == NULL){
        goto cleanup;
    }

    /* get the length */
    if (fseek(file, 0, SEEK_END) != 0){
        goto cleanup;
    }
    length = ftell(file);
    if (length < 0){
        goto cleanup;
    }
    if (fseek(file, 0, SEEK_SET) != 0){
        goto cleanup;
    }

    /* allocate content buffer */
    content = (char*)malloc((size_t)length + sizeof(""));
    if (content == NULL){
        goto cleanup;
    }

    /* read the file into memory */
    read_chars = fread(content, sizeof(char), (size_t)length, file);
    if ((long)read_chars != length){
        free(content);
        content = NULL;
        goto cleanup;
    }
    content[read_chars] = '\0';


cleanup:
    if (file != NULL){
        fclose(file);
    }

    return content;
}

void save_bin(const char *path, uint8_t *content, uint32_t size)
{
    FILE *file = NULL;
    uint32_t writed = 0;
	pt_trace("begin\n");
    /* open in read binary mode */
    file = fopen(path, "wb");
    if (file == NULL){
        pt_error("fopen %s failed:%s \n",path, strerror(errno));
        goto cleanup;
    }

    if (content == NULL || (size == 0)){
        goto cleanup;
    }

    do{
        writed += fwrite(content + writed, 1, size-writed, file);
        pt_trace("writed:%d\n", writed);
    }while(writed < size);

cleanup:
    if (file != NULL){
		/*fclose = fflush and close fd*/
        fclose(file);
    }
	pt_trace("end\n");
}

void save_env(const char *path, const char *content)
{
    int fd;
	pt_trace("begin\n");

    fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd == -1){
        pt_error("open %s failed:%s \n",path, strerror(errno));
        goto cleanup;
    }

    if (content == NULL){
        goto cleanup;
    }
    write(fd, content, strlen(content));

cleanup:
    fsync(fd);
    close(fd);
    pt_trace("end\n");
}
