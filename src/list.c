// SPDX-License-Identifier: GPL-2.0+

/*
 * list.c - Linux kernel list API
 *
 * TODO 1/0: Fill in name / email
 * Author: Lucas Lăzăroiu <lucas.lazaroiu.stud.acs.upb.ro>
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>

#define PROCFS_MAX_SIZE		512
#define COMMAND_LENGTH 		4

#define procfs_dir_name		"list"
#define procfs_file_read	"preview"
#define procfs_file_write	"management"

#define add_first		"addf"
#define add_end			"adde"
#define delete_first	"delf"
#define delete_all		"dela"

struct proc_dir_entry *proc_list;
struct proc_dir_entry *proc_list_read;
struct proc_dir_entry *proc_list_write;

/* TODO 2: define your list! */

struct string_data {
	char *string;
	struct list_head list;
};

static struct list_head head;

DEFINE_RWLOCK(lock);

static struct string_data *string_data_alloc(char *string) {
	struct string_data *sd;

	sd = kmalloc(sizeof(*sd), GFP_KERNEL);
	if (sd == NULL) {
		return NULL;
	}
	sd->string = kmalloc(sizeof(char) * strlen(string) + 1, GFP_KERNEL);
	if (sd->string == NULL) {
		return NULL;
	}
	strcpy(sd->string, string);

	return sd;
}

static void string_data_add_to_list(char *string, int end) {
	struct string_data *sd;

	sd = string_data_alloc(string);
	if (sd == NULL) {
		return;
	}
	write_lock(&lock);
	if (end == 0) {
		list_add(&sd->list, &head);
	} else {
		struct list_head *last = head.prev;
		list_add(&sd->list, last);
	}
	write_unlock(&lock);
}

static int list_proc_show(struct seq_file *m, void *v)
{
	/* TODO 3: print your list. One element / line. */

	struct list_head *p;
	struct string_data *sd;

	read_lock(&lock);
	list_for_each(p, &head) {
		sd = list_entry(p, struct string_data, list);
		seq_puts(m, sd->string);
		seq_putc(m, '\n');
	}
	read_unlock(&lock);
	return 0;
}

void string_data_delete(char *string, int all) {
	struct list_head *p, *q;
	struct string_data *sd;

	write_lock(&lock);
	list_for_each_safe(p, q, &head) {
		sd = list_entry(p, struct string_data, list);
		if (strcmp(sd -> string, string) == 0) {
			list_del(p);
			kfree(sd->string);
			kfree(sd);
			if (all == 0) {
				write_unlock(&lock);
				return;
			}
		}
	}
	write_unlock(&lock);
}

static int list_read_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static int list_write_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static ssize_t list_write(struct file *file, const char __user *buffer,
			  size_t count, loff_t *offs)
{
	char local_buffer[PROCFS_MAX_SIZE];
	unsigned long local_buffer_size = 0;
	char *command, *string_for_list;

	local_buffer_size = count;
	if (local_buffer_size > PROCFS_MAX_SIZE)
		local_buffer_size = PROCFS_MAX_SIZE;

	memset(local_buffer, 0, PROCFS_MAX_SIZE);
	if (copy_from_user(local_buffer, buffer, local_buffer_size))
		return -EFAULT;

	/* local_buffer contains your command written in /proc/list/management
	 * TODO 4/0: parse the command and add/delete elements.
	 */

	command = kmalloc(sizeof(char) * COMMAND_LENGTH, GFP_KERNEL);
	string_for_list = local_buffer + COMMAND_LENGTH + 1;
	strncpy(command, local_buffer, COMMAND_LENGTH);

	if (strcmp(command, add_first) == 0) {
		string_data_add_to_list(string_for_list, 0);
	} else if (strcmp(command, add_end) == 0) {
		string_data_add_to_list(string_for_list, 1);
	} else if (strcmp(command, delete_first) == 0) {
		string_data_delete(string_for_list, 0);
	} else if (strcmp(command, delete_all) == 0) {
		string_data_delete(string_for_list, 1);
	}
	kfree(command);
	return local_buffer_size;
}

static const struct proc_ops r_pops = {
	.proc_open		= list_read_open,
	.proc_read		= seq_read,
	.proc_release	= single_release,
};

static const struct proc_ops w_pops = {
	.proc_open		= list_write_open,
	.proc_write		= list_write,
	.proc_release	= single_release,
};

static int list_init(void)
{
	proc_list = proc_mkdir(procfs_dir_name, NULL);
	if (!proc_list)
		return -ENOMEM;

	proc_list_read = proc_create(procfs_file_read, 0000, proc_list,
				     &r_pops);
	if (!proc_list_read)
		goto proc_list_cleanup;

	proc_list_write = proc_create(procfs_file_write, 0000, proc_list,
				      &w_pops);
	if (!proc_list_write)
		goto proc_list_read_cleanup;

	return 0;

proc_list_read_cleanup:
	proc_remove(proc_list_read);
proc_list_cleanup:
	proc_remove(proc_list);
	return -ENOMEM;
}

static void list_exit(void)
{
	proc_remove(proc_list);
}

module_init(list_init);
module_exit(list_exit);

MODULE_DESCRIPTION("Linux kernel list API");
/* TODO 5: Fill in your name / email address */
MODULE_AUTHOR("Lucas Lăzăroiu <lucas.lazaroiu@stud.acs.pub.ro>");
MODULE_LICENSE("GPL v2");
