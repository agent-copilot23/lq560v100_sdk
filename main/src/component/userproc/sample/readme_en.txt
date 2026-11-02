Purpose: to demonstrate the user proc.
Command: ./sample_userproc
Note:
	Invoke API 'ot_proc_dir_add' and 'ot_proc_dir_remove' to create or remove a proc directory, the dir will be added in /proc/oritek/, for multi-process supporting, its name will be ${NAME_INPUT}_${PID}.
	Invoke API 'ot_proc_entry_create' and 'ot_proc_entry_destroy' to create or remove a proc entry.
	You can register a call back function 'ot_proc_show_func' to support read command as 'cat /proc/oritek/your_dir_pid/your_entry',
	and register function 'ot_proc_cmd_func' to support write command as 'echo params [...] > /proc/oritek/your_dir_pid/your_entry'.
	Invoke API 'ot_proc_entry_show' in your 'ot_proc_show_func' to print info to proc entry.
	You can invoke it repeatedly, but the total amount of data must be less than 4KB.