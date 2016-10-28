#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/list.h>

static __init int print_pid(void)
{
	struct task_struct *task, *p;
	struct list_head *pos;
	int total = 0;
	int running=0,interruptible=0,uninterruptible=0,stopped=0,traced=0,unknown=0;
	task = &init_task;
	printk("PID\tNAME\tSTATE\tPARENT\n");
//	read_lock(&tasklist_lock);
	list_for_each(pos,&task->tasks)//?
	{
		p=list_entry(pos,struct task_struct,tasks);
		total++;
		printk("%d\t%s\t%ld\t%d\n",p->pid,p->comm,p->state,p->parent->pid);
		switch(p->state){
			case TASK_RUNNING:
				running++;
				break;
			case TASK_INTERRUPTIBLE:
				interruptible++;
				break;
			case TASK_UNINTERRUPTIBLE:
				uninterruptible++;
				break;
			case TASK_STOPPED:
				stopped++;
				break;
			case TASK_TRACED:
				traced++;
				break;
			default:
				unknown++;
				break;
		}
	}
//	read_unlock(&tasklist_lock);

	printk("total tasks:			%4d\n", total);
	printk("TASK_RUNNING:			%4d\n", running);
	printk("TASK_INTERRUPTIBLE:		%4d\n", interruptible);
	printk("TASK_UNINTERRUPTIBLE:		%4d\n", uninterruptible);
	printk("TASK_STOPPED:			%4d\n", stopped);
	printk("TASK_TRACED:			%4d\n", traced);
	return 0;
}

static __exit void print_exit(void)  
{     
       	printk("<0>end!\n");  
} 

module_init(print_pid);
module_exit(print_exit);
