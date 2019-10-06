#include <stdint.h>

typedef enum {
  IO_PORT_A = 0,
  IO_PORT_B = 1,
  IO_PORT_C = 2
} IOPort;

typedef enum {
  IO_PIN_00 = 0,
  IO_PIN_01 = 1,
  IO_PIN_02 = 2,
  IO_PIN_03 = 3,
  IO_PIN_04 = 4,
  IO_PIN_05 = 5,
  IO_PIN_06 = 6,
  IO_PIN_07 = 7,
  IO_PIN_08 = 8,
  IO_PIN_09 = 9,
  IO_PIN_10 = 10,
  IO_PIN_11 = 11,
  IO_PIN_12 = 12,
  IO_PIN_13 = 13,
  IO_PIN_14 = 14,
  IO_PIN_15 = 15
} IOPin;

void configure_clock(IOPort port)
{
  /*
   * 0x4002 1000 - 0x4002 13FF: Reset and clock control RCC (Section 7.3.11, pg. 121)
   *   0x18: RCC_APB2ENR
   *     2: IOPAEN (I/O port A clock enable) (Section 8.3.7, pg. 147)
   *     3: IOPAEN (I/O port B clock enable) (Section 8.3.7, pg. 147)
   *     4: IOPCEN (I/O port C clock enable) (Section 8.3.7, pg. 147)
   *       0: disabled, 1: enabled
   */

  volatile uint8_t * const rcc_apb2enr = (volatile uint8_t *) (0x40021000 + 0x18);
  const uint8_t iopxen_mask = 0b1 << (port + 2);
  *rcc_apb2enr |= iopxen_mask;
}

void configure_pin(IOPort port, IOPin pin)
{
  /* const uint8_t MODE = 0b01; */ /* output mode, max speed 10MHz (see below) */
  const uint8_t MODE = 0b10; /* output mode, max speed 2MHz (see below) */
  /* const uint8_t MODE = 0b11; */ /* output mode, max speed 50MHz (see below) */

  const uint8_t CNF = 0b00; /* general purpose output push-pull (see below) */

  /*
   * 0x4001 0800 - 0x4000 0BFF: GPIO Port A (Section 9.5, pg. 194)
   * 0x4001 0C00 - 0x4001 0FFF: GPIO Port B (Section 9.5, pg. 194)
   * 0x4001 1000 - 0x4001 13FF: GPIO Port C (Section 9.5, pg. 194)
   *   0x00: GPIOx_CRL (Port configuration register low; x=A..G) (Section 9.2.1, pg.171)
   *   0x04: GPIOx_CRH (Port configuration register high; x=A..G) (Section 9.2.2, pg.172)
   *     - 4 bits per pin (2 bits for mode, 2 bits for cnf) -> 2 pins per byte (8 pins per 32-bit word)
   *       - bits 0,1: mode
   *           - 00: input mode (reset state)
   *           - 01: output mode, max speed 10 MHz
   *           - 10: output mode, max speed 2 MHz
   *           - 11: output mode, max speed 50 MHz
   *       - bits 0,1: configuration
   *         - input
   *           - 00: analog mode
   *           - 01: floating input (reset state)
   *           - 10: input with pull-up/pull-down
   *           - 11: reserved
   *         - output
   *           - 00: general purpose output push-pull
   *           - 01: general purpose output open-drain
   *           - 10: alternate function output push-pull
   *           - 11: alternate function output open-drain
   */

  volatile uint8_t * const gpiox_cr = (volatile uint8_t *) ((0x40010800 + 0x400*port) + 0x00 + pin/2);
  const int pin_shift = (pin%2) * 4;
  const int mode_shift = pin_shift + 0; /* mode: 2 low bits */
  const int cnf_shift = pin_shift + 2; /* cnf: 2 high bits */

  const uint8_t zeroed_mode = *gpiox_cr & ~(((uint8_t) 0b11) << mode_shift);
  *gpiox_cr = zeroed_mode | (MODE << mode_shift);

  const uint8_t zeroed_cnf = *gpiox_cr & ~(((uint8_t) 0b11) << cnf_shift);
  *gpiox_cr = zeroed_cnf | (CNF << cnf_shift);
}

void set_pin(IOPort port, IOPin pin)
{
  /*
   * 0x4001 0800 - 0x4000 0BFF: GPIO Port A (Section 9.5, pg. 194)
   * 0x4001 0C00 - 0x4001 0FFF: GPIO Port B (Section 9.5, pg. 194)
   * 0x4001 1000 - 0x4001 13FF: GPIO Port C (Section 9.5, pg. 194)
   *   0x10: GPIOx_BSRR (Port bit set/reset register; x=A..G) (Section 9.2.5, pg.173)
   *     - write only, can be accessed in word mode only
   *     - low 16 bits: set
   *     - high 16 bits: reset
   */

  volatile uint32_t * const gpiox_bsrr = (volatile uint32_t *) ((0x40010800 + 0x400*port) + 0x10);
  *gpiox_bsrr = ((uint32_t) 0x01) << (pin + 0);
}

void reset_pin(IOPort port, IOPin pin)
{
  /*
   * 0x4001 0800 - 0x4000 0BFF: GPIO Port A (Section 9.5, pg. 194)
   * 0x4001 0C00 - 0x4001 0FFF: GPIO Port B (Section 9.5, pg. 194)
   * 0x4001 1000 - 0x4001 13FF: GPIO Port C (Section 9.5, pg. 194)
   *   0x10: GPIOx_BSRR (Port bit set/reset register; x=A..G) (Section 9.2.5, pg.173)
   *     - write only, can be accessed in word mode only
   *     - low 16 bits: set
   *     - high 16 bits: reset
   */

  volatile uint32_t * const gpiox_bsrr = (volatile uint32_t *) ((0x40010800 + 0x400*port) + 0x10);
  *gpiox_bsrr = ((uint32_t) 0x01) << (pin + 16);
}

void configure(IOPort port, IOPin pin)
{
  configure_clock(port);
  configure_pin(port, pin);
}

int main(void)
{
  uint32_t i;

  configure(IO_PORT_C, IO_PIN_13);

  while(1) {
    reset_pin(IO_PORT_C, IO_PIN_13);
    for(i = 0; i < 0x10000; ++i) {
      asm("nop"); /* busy wait */
    }
    set_pin(IO_PORT_C, IO_PIN_13);
    for(i = 0; i < 0x100000; ++i) {
      asm("nop"); /* busy wait */
    }
  }
}


/* vim: set ts=2 sw=2 et: */


