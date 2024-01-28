#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"

#define Q_SEATS 5

int semaphores[Q_SEATS]; //Available sems. 
struct spinlock lock[1]; //Spinlock.
int lock_flag[1] = {0};  //In order to know if a spinlock is initialized.

//Initialize this module.
void sem_init(void) 
{
  initlock(lock, "semaphore_lock");
  *lock_flag = 1;
  for (unsigned int i = 0; i < Q_SEATS; i++) 
  {
    semaphores[i] = 0;
  }
}

int sem_open(int sem, int value) 
{
    if (sem < 0 || sem >= Q_SEATS) //Check for error.
    {
        exit(0);
    }
    if (value < 0) //Check for error.
    {
        exit(0);
    }
    acquire(lock); //blocks the corresponding spinlock.
    semaphores[sem] = value;
    release(lock); //free the corresponding spinlock.
  
    return sem;
}

int sem_close(int sem)
{
    //Check for error.
    if (sem < 0 || sem >= Q_SEATS)
    {
        exit(0);
    }
    return sem;
}

int sem_up(int sem) 
{
    //Check for error.
    if (sem < 0 || sem >= Q_SEATS)
    {
        exit(0);
    }
    acquire(lock);
    if (semaphores[sem] == 0) 
    {
     wakeup(&semaphores[sem]);
    }
    ++semaphores[sem];
    release(lock);
 
    return sem;
}

int sem_down(int sem) 
{
    
    if (sem < 0 || sem >= Q_SEATS) //check sem index.
    {
        exit(0);
    }
    acquire(lock); //blocks the corresponding spinlock.
    while (semaphores[sem] == 0)
    {
        sleep(&semaphores[sem], lock);
    }
    --semaphores[sem];
    release(lock); //release the spinlock.
    
    return sem;
}