#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;
static int fg_pcb = 1;

extern void context_uload(PCB *pcb, const char *filename);
extern void context_kload(PCB *pcb, void *entry);
extern void naive_uload(PCB *pcb, const char *filename);

//static int fg_pcb = 1;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    //Log("Hello World from Nanos-lite for the %dth time!", j);
    j++;
    _yield();
  }
}

void init_proc() {
  //for pa3 start
  // extern void naive_uload(PCB *pcb, const char *filename);
	// naive_uload(NULL, "/bin/init");
  // return;

  // for pa4.1 hello kernel
  context_kload(&pcb[0], (void *)hello_fun);
  //context_uload(&pcb[0], "/bin/hello");
  //for pa4.1 PAL
  context_uload(&pcb[1], "/bin/hello");
  //context_uload(&pcb[1], (void *)hello_fun);

  //test
  //context_kload(&pcb[1], "/bin/init");    //reach panic

  switch_boot_pcb();


  return;
}

static uint32_t count = 0;
_Context* schedule(_Context *prev) {

  current->tf = prev;

  //for pa4.1 hello
  //current = &pcb[0];

  //for pa4.1 PAL
  //current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  if(count++ < 50)
	{
		current = &pcb[fg_pcb];
	}
	else
	{
        count = 0;
        current = &pcb[0];
	}
  return current->tf;
}
