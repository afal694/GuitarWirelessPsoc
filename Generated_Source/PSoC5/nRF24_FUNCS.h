/**
* @file     nRF24_FUNCS.h
* @version  3
* @brief   This file define all the functions available to the user.
*/

#ifndef nRF24_FUNCS_H
#define nRF24_FUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "nRF24_DEFS.h"

// PSoC Component Functions
void nRF24_start(void);
void nRF24_init(void);
void nRF24_enable(void);
void nRF24_stop(void);
void nRF24_sleep(void);
void nRF24_wakeup(void);
void nRF24_save_config(void);
void nRF24_restore_config(void);

// nRF24 Functions

// Configuration functions
void nRF24_set_mode(const nrf_mode mode);
void nRF24_set_power_down_mode(void);
void nRF24_set_standby_i_mode(void);
void nRF24_set_standby_ii_mode(void);
void nRF24_set_rx_mode(void);
void nRF24_set_tx_mode(void);

// Functions related to registers
void nRF24_enable_auto_ack(const nrf_pipe pipe);
void nRF24_disable_auto_ack(const nrf_pipe pipe);
void nRF24_set_channel(uint8_t channel);
uint8_t nRF24_get_channel(void);
void nRF24_set_address_width(const nrf_addr_width addr_width);
uint8_t nRF24_get_address_width(void);
void nRF24_set_tx_address(const uint8_t *const addr, size_t size);
void nRF24_get_tx_address(uint8_t *addr, size_t size);
void nRF24_set_payload_size(const nrf_pld_size pipe, uint8_t size);
uint8_t nRF24_get_payload_size(const nrf_pld_size pipe);
void nRF24_enable_dynamic_payload(void);
void nRF24_disable_dynamic_payload(void);
void nRF24_enable_dynamic_payload_on_pipe(const nrf_pipe pipe);
void nRF24_disable_dynamic_payload_on_pipe(const nrf_pipe pipe);
void nRF24_enable_dynamic_payload_length(void);
void nRF24_disable_dynamic_payload_length(void);
void nRF24_enable_payload_with_ack(void);
void nRF24_disable_payload_with_ack(void);
void nRF24_enable_payload_with_no_ack(void);
void nRF24_disable_payload_with_no_ack(void);

/* General purpose functions */
void nRF24_start_listening(void);
void nRF24_stop_listening(void);
void nRF24_transmit_pulse(void);
uint8_t nRF24_get_status(void);
uint8_t nRF24_get_retransmissions_count(void);
uint8_t nRF24_get_lost_packets_count(void);
void nRF24_transmit(const uint8_t *payload, size_t payload_size);
bool nRF24_is_data_ready(void);
void nRF24_get_rx_payload(uint8_t *payload, const size_t payload_size);
void nRF24_tx_transmit_no_ack(const uint8_t *payload, size_t payload_size);
void nRF24_rx_write_payload(const nrf_pipe pipe, const uint8_t *payload,
                                         size_t payload_size);
uint8_t nRF24_get_data_pipe_with_payload(void);
uint8_t nRF24_received_power_detector(void);

bool nRF24_test_carrier(void);

// tx and rx fifo related functions
bool nRF24_is_tx_fifo_full(void);
bool nRF24_is_rx_fifo_empty(void);
uint8_t nRF24_get_fifo_status(void);
bool nRF24_put_in_tx_fifo(const uint8_t *payload, size_t payload_size);

// v2.0
void nRF24_set_rx_pipe_address(const nrf_addr_rx_pipe pipe,
                                            const uint8_t *addr, size_t size);
void nRF24_get_rx_pipe_address(const nrf_addr_rx_pipe pipe,
                                            uint8_t *addr, size_t size);

// IRQ Handle functions
void nRF24_clear_all_irqs(void);
void nRF24_clear_irq_flag(const nrf_irq irq_flag);
nrf_irq nRF24_get_irq_flag(void);
void nRF24_poll_interrupt(void);
uint8_t nRF24_get_status_clear_irq(void);

// command wrappers
void nRF24_flush_rx(void);
void nRF24_flush_tx(void);
void nRF24_reuse_last_transmitted_payload(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* nRF24_FUNCS_H */

/* [] END OF FILE */
