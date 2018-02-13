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

#define CRTPORT 0x3d4

int
sys_clear(void)
{
  // clearing qemu terminal

  static ushort *crt = (ushort*)P2V(0xb8000);
  
  // register 14 and 15 hold the cursor position as 14 bits
  // to read/write, the index of the register is written to CRTPORT
  // then read/write is issued on CRTPORT + 1
  
  // clearing the memory

  memset(crt, 0, sizeof(crt[0]) * 25 * 80);	// cga memory (25 X 80) default.

  // writing the position of cursor

  int pos = 0;
  outb(CRTPORT, 14);
  outb(CRTPORT+1, pos>>8);
  outb(CRTPORT, 15);
  outb(CRTPORT+1, pos);

  // clearing the diplay on serial port
  // uartputc prints a character on the display on the serial port

  char *ctrl_seq = "\033[2J\033[H";
  int i;
  for (i = 0; ctrl_seq[i]; ++i)  uartputc(ctrl_seq[i]);
  
  return 0;
}
