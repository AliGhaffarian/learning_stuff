#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/cdev.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include "myscull.h"
MODULE_LICENSE("GPL");

#ifdef DEBUG_MYSCULL
#define printkdbg printk

#else
#define printkdbg(...)

#endif //DEBUG_MYSCULL


int qset_data_size = 2048; // in bytes
module_param(qset_data_size, int, S_IRUGO);
int number_of_devs = 5;
module_param(number_of_devs, int, S_IRUGO);
char *proc_entry_name = "myscull";
module_param(proc_entry_name, charp, S_IRUGO);
char *dev_entry_name = "myscull";
module_param(dev_entry_name, charp, S_IRUGO);

dev_t initial_dev_number;
struct myscull_dev *myscull_devs;
struct proc_dir_entry *myscull_proc_entry;

#define HEX_DUMP_ROWSIZE 16
#define HEX_DUMP_GRPSIZE 2
#define HEX_DUMP_PREFIX_TYPE DUMP_PREFIX_OFFSET
#define HEX_DUMP_ASCII 1
#define HEX_DUMP_PREFIX_STR ""
#define ITEM_DELIM "-------------\n"
void free_qsets(struct qset *head){
	struct qset *next;
	while(head){
		kfree(head->data);
		next = head->next;
		kfree(head);
		head = next;
	}
}

struct qset *alloc_and_init_qset(size_t size){
	struct qset *result = kmalloc(sizeof(struct qset), GFP_KERNEL);
	if(!result)
		return NULL;

	result->data = kmalloc(size, GFP_KERNEL);
	if(!result->data){
		kfree(result);
		return NULL;
	}

	result->next = NULL;

	return result;
}

struct qset *qset_append_one_quantum(struct qset **head){
	struct qset **curr = head;
	while(*curr){
		curr = &((*curr)->next);
	}
	*curr = alloc_and_init_qset(qset_data_size);
	return *curr;
}

struct qset *qset_nth_element(struct qset *head, int n){
	int i = 0;
	while(i < n && head){
		head = head->next;
		i++;
	}
	return head;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = myscull_open,
	.read = myscull_read,
	.write = myscull_write,
};

int __init myscull_init(void){
alloc_chrdev_region(&initial_dev_number, 0, number_of_devs, dev_entry_name);

	myscull_devs = kmalloc(number_of_devs * sizeof(struct myscull_dev), GFP_KERNEL);
	if (!myscull_devs)
		goto fail;

	int i = 0;
	for(; i < number_of_devs; i++){
		dev_t current_dev_num = MKDEV(MAJOR(initial_dev_number), i);
		myscull_devs[i].filled_size = 0;
		myscull_devs[i].capacity = 0;
		myscull_devs[i].head = NULL;
		cdev_init(&myscull_devs[i].this_cdev, &fops);
		cdev_add(&myscull_devs[i].this_cdev, current_dev_num, 1);
	}

	myscull_proc_entry = proc_create_seq(proc_entry_name, O_RDONLY, 0, &sops);
	return 0;
fail:
	unregister_chrdev_region(initial_dev_number, number_of_devs);
	return 1;
}

void __exit myscull_exit(void){
	proc_remove(myscull_proc_entry);
	int i = 0;
	for(; i < number_of_devs; i++){
		free_qsets(myscull_devs[i].head);
		cdev_del(&myscull_devs[i].this_cdev);
	}
	unregister_chrdev_region(initial_dev_number, number_of_devs);
	kfree(myscull_devs);
}

ssize_t myscull_read(struct file *filp, char *buf, size_t count, loff_t *offset){
	struct myscull_dev *current_dev = (struct myscull_dev *)filp->private_data;
	struct qset *quantum_to_read = NULL;

	int nth_item = *offset / qset_data_size, rest = *offset % qset_data_size ;

	if (*offset >= current_dev->filled_size){
		printkdbg(KERN_INFO "returning EOF because *offset > current_dev->filled_size (%d > %d)\n", *offset, current_dev->filled_size);
		return 0;
	} else
		quantum_to_read = qset_nth_element(current_dev->head, nth_item);
	/*if quantum_to_read is NULL here, we did something wrong*/

	int copy_count = count + rest < qset_data_size ? count : qset_data_size - rest; 
	//if nth_item == last item
	if(nth_item == (current_dev->filled_size / qset_data_size)){
		int last_item_end = current_dev->filled_size % qset_data_size;
		copy_count = count < last_item_end - rest ? count : last_item_end - rest;
	}
	int retval = copy_count;

	if (copy_to_user(buf, (quantum_to_read->data + rest), copy_count))
		return -EINVAL;
	
	*offset += retval;
	return retval;	
}

ssize_t myscull_write(struct file *filp, const char *buf, size_t count, loff_t *offset){
	struct myscull_dev *current_dev = (struct myscull_dev *)filp->private_data;
	struct qset *quantum_to_read = NULL;

	int nth_item = *offset / qset_data_size, rest = *offset % qset_data_size;
	int copy_count = count + rest <= qset_data_size ? count : qset_data_size - rest; 
	int retval = copy_count;

	//if the offset can't be reached with allocation of one quantum, give up
	if ((*offset / qset_data_size) > (current_dev->capacity / qset_data_size) + 1){
		printkdbg(KERN_INFO "%s: returning -EINVAL because offset couldn't be reached with allocation of one quantum\n", __func__);
		return -EINVAL;
	}

	if (*offset >= current_dev->capacity){
		quantum_to_read = qset_append_one_quantum(&current_dev->head);
		if(!quantum_to_read){
			return -ENOMEM;
		}
		current_dev->capacity += qset_data_size;
	}
	else
		quantum_to_read = qset_nth_element(current_dev->head, nth_item);


	int bytes_remaining;
	if ((bytes_remaining = copy_from_user((quantum_to_read->data + rest), buf, copy_count))){
		printkdbg(KERN_INFO "%s: unexpected error, bytes_remaining != 0 (%d != 0)\n", bytes_remaining);
		return -EINVAL;
	}
	current_dev->filled_size += retval;	

	*offset += retval;
	return retval;	
}

int myscull_open(struct inode *inode, struct file *filp){
	struct myscull_dev *dev_ptr = container_of(inode->i_cdev, struct myscull_dev, this_cdev);
	filp->private_data = dev_ptr;
	
	if ((filp->f_flags & O_ACCMODE) == O_WRONLY){
		free_qsets(dev_ptr->head);
		dev_ptr->head = NULL;
		dev_ptr->capacity = 0;
		dev_ptr->filled_size = 0;
	}
	
	return 0;
}

void *myscull_iter_start(struct seq_file *sfile, loff_t *pos){
	if(*pos >= number_of_devs)
		return NULL;
	return myscull_devs + *pos;
}

void *myscull_iter_next(struct seq_file *sfile, void *v, loff_t *pos){
	(*pos)++;
	if(*pos >= number_of_devs)
		return NULL;
	return myscull_devs + *pos;
}

int myscull_iter_show(struct seq_file *sfile, void *v){
	struct myscull_dev *dev = v;
	seq_printf(sfile, "device number: %d:%d\n", MAJOR(dev->this_cdev.dev), MINOR(dev->this_cdev.dev));
	seq_printf(sfile, "filled_size: %ld\n", dev->filled_size);
	seq_printf(sfile, "capacity: %ld\n", dev->capacity);
	int i = 0;
	struct qset *current_node = dev->head;
	while( i < dev->filled_size / qset_data_size ){
		seq_hex_dump(
				sfile, 
				HEX_DUMP_PREFIX_STR, 
				HEX_DUMP_PREFIX_TYPE, 
				HEX_DUMP_ROWSIZE, 
				HEX_DUMP_GRPSIZE, 
				current_node->data, 
				qset_data_size, 
				HEX_DUMP_ASCII
				);
		i++;
		current_node = current_node->next;
	}

	if(!current_node)
		goto done;

	seq_hex_dump(
			sfile, 
			HEX_DUMP_PREFIX_STR, 
			HEX_DUMP_PREFIX_TYPE, 
			HEX_DUMP_ROWSIZE, 
			HEX_DUMP_GRPSIZE, 
			current_node->data, 
			dev->filled_size % qset_data_size, 
			HEX_DUMP_ASCII
			);
done:
	seq_puts(sfile, ITEM_DELIM);
	return 0;
}

void myscull_iter_stop(struct seq_file *sfile, void *v){
	return;
}
struct seq_operations sops = {
	.start = myscull_iter_start,
	.next = myscull_iter_next,
	.show = myscull_iter_show,
	.stop = myscull_iter_stop
};

int myscull_proc_open(struct inode *inode, struct file *filp){
	return seq_open(filp, &sops);
}

struct file_operations myscull_proc_fops = {
	.owner = THIS_MODULE,
	.open = myscull_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release
};

module_init(myscull_init);
module_exit(myscull_exit);
