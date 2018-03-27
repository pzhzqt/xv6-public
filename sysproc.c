#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//print the number of pages allocated
//return 0 on success
int
sys_mem(void)
{
	pte_t *pgtab;
	struct proc *proc=myproc();
	int num_page=0,num_user=0;

	for (int i=0;i<NPDENTRIES;i++){
		if (proc->pgdir[i] & PTE_P){
			pgtab = (pte_t *)P2V(PTE_ADDR(proc->pgdir[i]));
			for (int j=0;j<NPTENTRIES;j++){
				if (pgtab[j] & PTE_P){
					num_page++;
					if((pgtab[j] & PTE_U) && (pgtab[j] & PTE_W)){
						num_user++;
					}
				}
			}
		}
	}

	cprintf("number of total pages: %d\n",num_page);
	cprintf("number of user accessible and writable pages: %d\n",num_user);

	return 0;
}
