#ifndef MY_SCULL
#define MY_SCULL
#include "linux/types.h"
#include <linux/cdev.h>
#include <linux/seq_file.h>

//load time parameters
extern int qset_data_size; 
extern int number_of_devs; 
extern char *proc_entry_name;
extern char *dev_entry_name;

struct myscull_dev {
	struct cdev this_cdev;
	struct qset *head;
	size_t filled_size;
	size_t capacity; // number of qsets allocated
};
struct qset{
	struct qset *next;
	char *data;
};
extern struct file_operations fops;
int myscull_open(struct inode *, struct file *);
ssize_t myscull_read(struct file *, char __user *, size_t, loff_t *);
ssize_t myscull_write(struct file *, const char __user *, size_t, loff_t *);

/*
 * frees the qsets starting with the given node 
 */
void free_qsets(struct qset *);
/*
 * @param size_t size: size of data to be allocated
 * @error : NULL
*/
struct qset *alloc_and_init_qset(size_t);
/*
 * append a qset that's initilized with alloc_and_init_qset(qset_data_size)
 * @param head: head of qset, can be any node of the linked list though
 * @return: pointer to the appended qset
 * @error: NULL
 */
struct qset *qset_append_one_quantum(struct qset **);
/*
 * @param head: head of qset, can be any node of the linked list though
 * @param n: nth element to be retreived
 * @return: the nth element in qset linked list
 * @error: NULL
 */
struct qset *qset_nth_element(struct qset *, int);

int __init myscull_init(void);
void __exit myscull_exit(void);

extern struct seq_operations sops;
extern struct file_operations myscull_proc_fops;
void *myscull_iter_start(struct seq_file *, loff_t *);
void *myscull_iter_next(struct seq_file *, void *, loff_t *);
void myscull_iter_stop(struct seq_file *, void *);
int myscull_iter_show(struct seq_file *, void *);

int myscull_proc_open(struct inode *, struct file *);
#endif
