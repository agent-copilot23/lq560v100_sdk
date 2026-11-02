#ifndef EMMC_CMD_H_
#define EMMC_CMD_H_

struct mmc_data {
    unsigned int  blk_cnt;       /* number of blocks */
    unsigned short blk_sz;        /* data block size */
    unsigned char flags;         /* read or write flags. read: 0x1; write: 0x0 */
    unsigned char rsvd;
    unsigned int *data_buf;     /* buffer containing data to be read into or written */
};

struct mmc_command {
    unsigned int opcode;         /* cmd index */
    unsigned int arg;            /* cmd argument */
    unsigned char flags;          /* expected response type */
    unsigned char rsvd[3];
    unsigned int resp[4];        /* cmd respense */

    struct mmc_data *data; /* data segment associated with cmd */
};
#endif /* EMMC_CMD_H_ */
