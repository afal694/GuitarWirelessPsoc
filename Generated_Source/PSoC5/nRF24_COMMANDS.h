/**
* @file     nRF24_COMMANDS.h
* @version  3
* @brief    The nRF24 radio is controlled via commands, this file implement all
* the available commands.
*/

#ifndef nRF24_COMMANDS_H
#define nRF24_COMMANDS_H
    
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "nRF24_DEFS.h"

void nRF24_cmd_reuse_tx_payload(void);
void nRF24_cmd_read_rx_payload(uint8_t* payload, const size_t payload_size);
void nRF24_cmd_write_tx_payload(const uint8_t* payload, const size_t payload_size);
void nRF24_cmd_flush_rx(void);
void nRF24_cmd_flush_tx(void);
uint8_t nRF24_cmd_read_payload_width(void);
void nRF24_cmd_write_ack_payload(const nrf_pipe pipe, const uint8_t* payload, const size_t payload_size);
void nRF24_cmd_no_ack_payload(const uint8_t* payload, const size_t payload_size);
uint8_t nRF24_cmd_nop(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* nRF24_NRF_COMMANDS_H */

/* [] END OF FILE */
