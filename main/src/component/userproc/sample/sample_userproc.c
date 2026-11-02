#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

#include "ot_eis_type.h"
#include "ot_eis_errno.h"

#include "ot_proc.h"

#define DIRNAME     "sample"
#define ENTRYNAME1   "help"
#define ENTRYNAME2   "testproc2"
#define ENTRYNAME3   "testproc3"

#define SAMPLE_PRINT printf

ot_s32 Proc_Test_Show1(ot_proc_show_buffer_attr * buffer, ot_void *priv_data)
{
    SAMPLE_PRINT("Print in process %d, %s\n", (ot_s8)getpid(), __FUNCTION__);
    ot_proc_entry_show(buffer, "Print in process %d, %s\n", (ot_s8)getpid(), __FUNCTION__);
    ot_proc_entry_show(buffer, "Invoke API \'ot_proc_dir_add\' and \'ot_proc_dir_remove\' to create or remove a proc directory, the dir will be added in /proc/umap/ default\n");
    ot_proc_entry_show(buffer, "invoke API \'ot_proc_entry_create\' and \'ot_proc_entry_destroy\' to create or remove a proc entry,\n");
    ot_proc_entry_show(buffer, "you can register a call back function \'ot_proc_show_func\' to support read command as \'cat /proc/umap/your_dir_pid/your_entry\',\n");
    ot_proc_entry_show(buffer, "and register function \'ot_proc_cmd_func\' to support write command as \'echo params [...] > /proc/umap/your_dir_pid/your_entry\'.\n");
    ot_proc_entry_show(buffer, "Invoke API \'ot_proc_entry_show\' in your \'ot_proc_show_func\' to print info to proc entry.\n");
    ot_proc_entry_show(buffer, "You can invoke it repeatedly, but the total amount of data must be less than 4KB. \n");
    return OT_SUCCESS;
}

ot_s32 Proc_Test_Show2(ot_proc_show_buffer_attr * buffer, ot_void *priv_data)
{
    SAMPLE_PRINT("Print in process %d, %s\n", (ot_s8)getpid(), __FUNCTION__);
    ot_proc_entry_show(buffer, "Print in process %d, %s\n", (ot_s8)getpid(), __FUNCTION__);
    return OT_SUCCESS;
}

ot_s32 Proc_Test_Show3(ot_proc_show_buffer_attr * buffer, ot_void *priv_data)
{
    SAMPLE_PRINT("Print in process3 %d, %s\n", (ot_s8)getpid(), __FUNCTION__);
    ot_proc_entry_show(buffer, "Print in process3 %d, %s\n", (ot_s8)getpid(), __FUNCTION__);
    return OT_SUCCESS;
}

ot_s32 Proc_Test_Cmd1(ot_proc_show_buffer_attr * buffer, ot_u32 args, ot_u8 *argv[], ot_void *priv_data)
{
    ot_s8 i;

    SAMPLE_PRINT("Print in process %d, %s\n", (ot_s8)getpid(), __FUNCTION__);
    SAMPLE_PRINT("Argc: %d \n", args);
    for (i=0; i<args; i++) {
        SAMPLE_PRINT("\tArgv[%d]: %s \n", i, (ot_s8*)argv[i]);
    }
    
    /* echo info to current teriminal */
    ot_proc_entry_show(buffer, "Get help info from %s.\n", __func__);
    return OT_SUCCESS;
}

ot_s32 Proc_Test_Cmd2(ot_proc_show_buffer_attr * buffer, ot_u32 args, ot_u8 *argv[], ot_void *priv_data)
{
    ot_s8 i;

    SAMPLE_PRINT("Print in process %d, %s\n", (ot_s8)getpid(), __FUNCTION__);
    SAMPLE_PRINT("Argc: %d \n", args);
    for (i=0; i<args; i++) {
        SAMPLE_PRINT("\tArgv[%d]: %s \n", i, (ot_s8*)argv[i]);
    }
    
    return OT_SUCCESS;
}

ot_s32 main(ot_void)
{
    ot_s32 ret;
    ot_proc_entry_attr entry1;
    ot_proc_entry_attr entry2;
    ot_proc_entry_attr entry3;
    ot_s8 input_cmd[16];

    ret = ot_proc_init();
    if (OT_SUCCESS != ret) {
        SAMPLE_PRINT("ot_proc_init failure: %d\n", ret);
        goto sys_deinit;
    }

    /* Will be added at /proc/umap/ directory */
    ret = ot_proc_dir_add(DIRNAME);
    if (OT_SUCCESS != ret) {
        SAMPLE_PRINT("ot_proc_dir_add(\"%s\") return %d\n", DIRNAME, ret);
        goto sys_deinit;
    }
    SAMPLE_PRINT("Add proc dir /proc/umap/"DIRNAME" successfully.\n");

    /* Will be added at /proc/umap/${DIRNAME} directory */
    entry1.dir_name = OT_NULL;
    entry1.name = ENTRYNAME1;
    entry1.proc_show = Proc_Test_Show1;
    entry1.proc_cmd = Proc_Test_Cmd1;
    entry1.priv_data = OT_NULL;
    ret = ot_proc_entry_create( &entry1);
    if (OT_SUCCESS != ret) {
        SAMPLE_PRINT("ot_proc_entry_create(\"%s\") return %d\n", ENTRYNAME1, ret);
        goto remove_dir;
    }
    SAMPLE_PRINT("Add proc entry /proc/umap/"DIRNAME"/"ENTRYNAME1" successfully.\n");

    /* Will be added at /proc/umap/${DIRNAME} directory */
    entry2.dir_name = DIRNAME;
    entry2.name = ENTRYNAME2;
    entry2.proc_show = Proc_Test_Show2;
    entry2.proc_cmd = Proc_Test_Cmd2;
    entry2.priv_data = OT_NULL;
    ret = ot_proc_entry_create(&entry2);
    if (OT_SUCCESS != ret) {
        SAMPLE_PRINT("ot_proc_entry_create(\"%s\") return %d\n", ENTRYNAME2, ret);
        goto remove_entry1;
    }
    SAMPLE_PRINT("Add proc entry /proc/umap/"DIRNAME"/"ENTRYNAME2" successfully.\n");

    /* Only support cat, Will be added at /proc/umap/${DIRNAME} directory */
    entry3.dir_name = OT_NULL;
    entry3.name = ENTRYNAME3;
    entry3.proc_show = Proc_Test_Show3;
    entry3.proc_cmd = OT_NULL;
    entry3.priv_data = OT_NULL;
    ret = ot_proc_entry_create(&entry3);
    if (OT_SUCCESS != ret) {
        SAMPLE_PRINT("ot_proc_entry_create(\"%s\") return %d\n", ENTRYNAME3, ret);
        goto remove_entry2;
    }
    SAMPLE_PRINT("Add proc entry /proc/umap/"DIRNAME"/"ENTRYNAME3" successfully.\n");

    SAMPLE_PRINT("Telnet to board and try to control the proc entry:\n");
    SAMPLE_PRINT("    cat /proc/umap/"DIRNAME"/"ENTRYNAME1"\n");
    SAMPLE_PRINT("    echo 1 2 3 > /proc/umap/"DIRNAME"/"ENTRYNAME2"\n");
    SAMPLE_PRINT("Press q to quit.\n");

    while (1) {
        fgets((char *)(input_cmd), (sizeof(input_cmd) - 1), stdin);
        if ('q' == input_cmd[0]) {
            break;
        }
        
        sleep(1);
    }

    ot_proc_entry_destroy(&entry3);
    SAMPLE_PRINT("Remove proc entry /proc/umap/"DIRNAME"/"ENTRYNAME3" successfully.\n");

remove_entry2:
    ot_proc_entry_destroy(&entry2);
    SAMPLE_PRINT("Remove proc entry /proc/umap/"DIRNAME"/"ENTRYNAME2" successfully.\n");

remove_entry1:
    ot_proc_entry_destroy(&entry1);
    SAMPLE_PRINT("Remove proc entry /proc/umap/"DIRNAME"/"ENTRYNAME1" successfully.\n");

remove_dir:
    ot_proc_dir_remove(DIRNAME);
    SAMPLE_PRINT("Remove proc dir /proc/umap/"DIRNAME" successfully.\n");

sys_deinit:
    ot_proc_deinit();
    return 0;
}
