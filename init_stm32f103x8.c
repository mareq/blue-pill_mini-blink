int main(void);

/* weak: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#Common-Function-Attributes */
/* naked: https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html#ARM-Function-Attributes */
void __attribute__((weak, naked)) reset_handler(void)
{
  main();
}

void blocking_handler(void)
{
  while(1)
    ;
}

void null_handler(void) {
}

extern unsigned _stack;

/* vector table (Section 10.1.2, pg. 204) */
__attribute__ ((section(".vectors")))
struct {
  unsigned int *initial_sp_value;
  void (*reset)(void);
  void (*nmi)(void);
  void (*hard_fault)(void);
  void (*memory_manage_fault)(void);
  void (*bus_fault)(void);
  void (*usage_fault)(void);
  void (*reserved_x001c[4])(void);
  void (*sv_call)(void);
  void (*debug_monitor)(void);
  void (*reserved_x0034)(void);
  void (*pend_sv)(void);
  void (*systick)(void);
  void (*irq[59])(void);
} vector_table = {
  .initial_sp_value = &_stack,
  .reset = reset_handler,
  .nmi = null_handler,
  .hard_fault = blocking_handler,

  .sv_call = null_handler,
  .pend_sv = null_handler,
  .systick = null_handler,
  .irq = {
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
    null_handler,
  }
};


/* vim: set ts=2 sw=2 et: */


