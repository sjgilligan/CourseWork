#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system_misc.h> /* cli(), *_flags */
#include <linux/uaccess.h>
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/timer.h>
#include <linux/time.h>

#define BUF_LEN 1024
#define from_timer(var, callback_timer, timer_fieldname) \
	container_of(callback_timer, typeof(*var), timer_fieldname)

MODULE_LICENSE("Dual BSD/GPL");

struct timer_struct { //stuct used to hold timer info
    struct timer_list t;
    int index;
    int active;
    int delay;
    time_t time;
};


static struct timer_struct timer_array[1000]; //array of timers
static char messages[1000][256]; //array of messages
static int max_timers = 1; //-m set
static int reg_timers = 0; //timer index used
static int active_timers = 0; //active
static int major_number = 61; 
static char kbuf[BUF_LEN]; //buffer used for input from user program

// Declaration of functions
static int timer_open(struct inode *inode, struct file *file);
static int timer_release(struct inode *inode, struct file *file);
static ssize_t timer_write(struct file *file,const char *buf, size_t count, loff_t *f_pos);
static int __init timer_init(void);
static void __exit timer_exit(void);


struct file_operations timer_fops = //file ops struct
{
write:
    timer_write,
open:
    timer_open,
release:
    timer_release
};

module_init(timer_init);
module_exit(timer_exit);

static void timer_msg(struct timer_list *timer) {
    int ind;
    struct timer_struct *ctimer = from_timer(ctimer, timer, t);  //get index
    ind = ctimer->index;
    printk(KERN_ALERT "%s\n",messages[ind]); //print the message
    timer_array[ind].active = 0; //not active anymore
    active_timers--; //total active is one less now
    del_timer(timer); //delete timer
}

static int __init timer_init(void) {
    int result = register_chrdev(major_number, "mytimer", &timer_fops); //register character device
    if (result < 0)
    {
        printk(KERN_ALERT "memory: cannot obtain major number %d\n", major_number);
        return result;
    }

    return 0;
}

static void __exit timer_exit(void) {
	//exit
    unregister_chrdev(major_number,"mytimer");
}

static int timer_open(struct inode *inode, struct file *file) {
    //open
    return 0;
}

static int timer_release(struct inode *inode, struct file *filp)
{
    //release
    return 0;
}

static ssize_t timer_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
    char flag; //used to clean input
    
    if(copy_from_user(kbuf, buf, count) != 0) { //get info from user
        printk(KERN_ALERT "timer_write unsuccseful\n");
    }

    kbuf[count] = '\0'; //end the string

    if(kbuf[0] == 's') {
        int delay; 
        int i = 0;
        int repeat = 0; //if 1, this string is already used so update
        char message_buf[256]; 
	
        sscanf(kbuf,"%c %d %99[^\n]",&flag, &delay, message_buf);//format input
	
        for(i = 0; i < reg_timers; i++) { //loop through all active timers to see if message matches
            if(timer_array[i].active == 1 && strncmp(message_buf,messages[i],sizeof(messages[i])) == 0) {
                repeat = 1; //if so set this to 1
                break;
            }
        }
        if(repeat == 1) {
            timer_array[i].delay = delay; //update delay
            mod_timer(&timer_array[i].t, jiffies + (msecs_to_jiffies(delay*1000))); //update timer
            printk(KERN_CONT "The timer %s was updated!\n",messages[i]); //let user know
        } else {
            if(active_timers >= max_timers) {
                printk(KERN_CONT "%d timer(s) already exist(s)!\n", active_timers); //too many active timers
            } else {
		struct timespec ts;
		getnstimeofday(&ts);	
		timer_array[reg_timers].time = ts.tv_sec;
		//printk("time is: %ld\n",ts.tv_sec);
                strcpy(messages[reg_timers],message_buf); //put new message in array
                timer_array[reg_timers].index = reg_timers; //update index
                timer_array[reg_timers].active = 1; //set active bit
                timer_array[reg_timers].delay = delay; //update delay
                timer_setup(&timer_array[reg_timers].t, timer_msg, 0); //set up timer with print function
                mod_timer(&timer_array[reg_timers].t, jiffies + (msecs_to_jiffies(delay*1000))); //set delay, convert seconds to jiffies
                reg_timers++; //new timer so increase
                active_timers++; //same here
            }
        }
    } else if(kbuf[0] == 'm') {
        sscanf(kbuf,"%s %d",&flag,&max_timers); //update max
    } else if(kbuf[0] == 'l') {
        int i = 0;
        for(i = 0; i < reg_timers; i++) {
            if(timer_array[i].active == 1) {
		struct timespec ts2;
		int time_elapsed;
		int print_time;
		getnstimeofday(&ts2);
		time_elapsed = ts2.tv_sec - timer_array[i].time;
		print_time = timer_array[i].delay - time_elapsed;
                printk(KERN_CONT "%s %d\n",messages[i],print_time); //print all active timers
            }
        }
    }


    return count;
}
