#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/uaccess.h>


static unsigned int variable;
static struct proc_dir_entry *test_dir, *test_entry;

static noinline void kill_moretime(void)
{
	mdelay(2);
}


static noinline void kill_time(void)
{
	mdelay(2);
	kill_moretime();
}

static int test_proc_show(struct seq_file *seq, void *v)
{
	unsigned int *ptr_var = seq->private;
	kill_time();
	seq_printf(seq, "%u\n", *ptr_var);
	return 0;
}


static ssize_t test_proc_write(struct file *file, const char __user *buffer,
						size_t count, loff_t *ppos)
{
	struct seq_file *seq = file->private_data;
	unsigned int *ptr_var = seq->private;
	int err;
	char *kbuffer;

        if (!buffer || count > PAGE_SIZE - 1)
                return -EINVAL;

	kbuffer = (char *)__get_free_page(GFP_KERNEL);

	if (!kbuffer)
		return -ENOMEM;
	err = -EFAULT;
	if (copy_from_user(kbuffer, buffer, count))
		goto out;
	kbuffer[count] = '\0';
	*ptr_var = simple_strtoul(kbuffer, NULL, 10);
	return count;
out:
	free_page((unsigned long)buffer);
	return err;
}


static int test_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, test_proc_show, PDE_DATA(inode));
}


static const struct proc_ops test_proc_fops =
{
	.proc_open = test_proc_open,
	.proc_read = seq_read,
	.proc_write = test_proc_write,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};


static __init int test_proc_init(void)
{

	printk("------proc init------\n");
	test_dir = proc_mkdir("test_dir", NULL);
	if (test_dir) {
		test_entry = proc_create_data("test_rw",0666, test_dir, &test_proc_fops, &variable);
		if (test_entry)
			return 0;
	}
	return -ENOMEM;
}
module_init(test_proc_init);

static __exit void test_proc_cleanup(void)
{
	remove_proc_entry("test_rw", test_dir);
	remove_proc_entry("test_dir", NULL);
}
module_exit(test_proc_cleanup);



MODULE_DESCRIPTION("proc exmaple");
MODULE_LICENSE("GPL v2");

