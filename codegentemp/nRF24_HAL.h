/**
* @file     nRF24_HAL.h
* @version  3
*/

#ifndef nRF24_HAL_H
#define nRF24_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "nRF24_DEFS.h"

/* SPI Control */

// Function pointer to send and receive data from the nrf24 radio
typedef void (*nrf_spi_xfer)(const uint8_t *in, uint8_t *out, const size_t xfer_size);

extern nrf_spi_xfer nRF24_spi_xfer;

// Default function, spi sends byte by byte
void nRF24_default_spi_xfer(const uint8_t *in, uint8_t *out, const size_t xfer_size);
// Register your own spi xfer function
void nRF24_register_spi_xfer(nrf_spi_xfer new_spi_xfer);

uint8_t nRF24_read_reg(const nrf_register reg, uint8_t *data, const size_t data_size);
uint8_t nRF24_write_reg(const nrf_register reg, const uint8_t *data, const size_t data_size);
bool nRF24_read_bit(const nrf_register reg, const uint8_t bit_pos);
void nRF24_clear_bit(const nrf_register reg, const uint8_t bit_pos);
void nRF24_set_bit(const nrf_register reg, const uint8_t bit_pos);

/* IO Control */
typedef enum {
    GPIO_CLEAR,
    GPIO_SET
} gpio_state;

void nRF24_ss_write(gpio_state state);
void nRF24_ce_write(gpio_state state);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* nRF24_HAL_H */

/* [] END OF FILE */
