#ifndef RAMFUNC_H
#define RAMFUNC_H

/* Place function in .ramfunc section */
#define RAMFUNC __attribute__((section(".ramfunc"), noinline, used))

#endif /* RAMFUNC_H */
