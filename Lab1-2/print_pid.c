#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/list.h>

static __init int print_pid(void)
{

	struct task_struct *task, *p;   // task it used to point to the init_task
                                    // p is used to point to every entry of running task
	struct list_head *pos;          // pos is used to traverse every node of the task list
    
    // definition of counters	
    int total = 0;
	int running = 0,
	    interruptible = 0,
        uninterruptible=0,
        stopped=0,
        traced=0,
        unknown=0;

	task = &init_task;  // initialize the beginning pointer 
    
    // print the title and the head of the list
	printk("\n\n====== print_pid module starts ======\n");
	printk("PID\tNAME\t\tSTATE\tPARENT\n");

	list_for_each(pos,&task->tasks) 
    // task->tasks is the (dummy) head node of the task linked-list
    // thus &task->tasks is the head pointer.
    // and this statement is equivalent to 
    //     for(pos = &(task->tasks)->; pos != &task->tasks; pos = pos->next){//...}
    // which can traverse every running task
	{
		p=list_entry(pos,struct task_struct,tasks); // use p to point to a single entry
		total++;    // add 1 to total task counter
		printk("%d\t%s\t%ld\t%d\n",p->pid,p->comm,p->state,p->parent->pid); // print the status of this task 
        
        // classify the current task and add 1 to the correspond counter
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
    
    // print all the counters
	printk("total tasks:			%4d\n", total);
	printk("TASK_RUNNING:			%4d\n", running);
	printk("TASK_INTERRUPTIBLE:		%4d\n", interruptible);
	printk("TASK_UNINTERRUPTIBLE:		%4d\n", uninterruptible);
	printk("TASK_STOPPED:			%4d\n", stopped);
	printk("TASK_TRACED:			%4d\n", traced);
    
    // print the end of the list
	printk("\n====== print_pid module ends ======\n\n");

	return 0;
}

static __exit void print_exit(void)  
{     
       	printk("<0>end!\n");  
} 

module_init(print_pid);
module_exit(print_exit);
