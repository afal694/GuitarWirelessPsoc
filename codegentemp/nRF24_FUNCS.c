/**
* @file     nRF24_FUNCS.c
* @version  3
* @brief    This file define all the functions available to the user.
*/

#include "nRF24_CONFIG.h"
#include "nRF24_HAL.h"
#include "nRF24_COMMANDS.h"
#include "nRF24_FUNCS.h"
#include "nRF24_DEFS.h"

/* "Helper" global variables */
/* _nrf_addr_width expresed in bytes, declared with the default value */
static nrf_pipe_addr_width _nrf_addr_width = NRF_PIPE_ADDR_WIDTH_5BYTES;

static void
_addr_width_conf_to_bytes(nrf_addr_width addr_width)
{
    switch(addr_width)
    {
    case NRF_SETUP_AW_3BYTES:
        _nrf_addr_width = NRF_PIPE_ADDR_WIDTH_3BYTES;
        break;
    case NRF_SETUP_AW_4BYTES:
        _nrf_addr_width = NRF_PIPE_ADDR_WIDTH_4BYTES;
        break;
    case NRF_SETUP_AW_5BYTES:
        _nrf_addr_width = NRF_PIPE_ADDR_WIDTH_5BYTES;
        break;
    }
}

/**
 * @brief Configure the radio and clear IRQs, TX and RX FIFOs.
 */
void nRF24_start(void)
{
    // Recommended delay before start using the nRF24
    CyDelay(NRF_POWER_UP_DELAY_MS);

    // Now the radio is in Power Down mode

#if defined (_PSOC6)
    /* We are using the low level driver, so pass NULL to the context */
    (void) Cy_SCB_SPI_Init(SPI_HW, &SPI_config, NULL);
    Cy_SCB_SPI_Enable(SPI_HW);
#else // _PSoC4_SCB and _PSOC_UDB    
    // _initVar is set to 1 when _Start is called 
    if (0 == SPI_initVar) {
        SPI_Start();
    }
#endif
    nRF24_ss_write(GPIO_SET);
    nRF24_ce_write(GPIO_CLEAR);

    // Flush both nRF24 FIFOs
    nRF24_cmd_flush_rx();
    nRF24_cmd_flush_tx();
    // Clear IRQ flags
    nRF24_clear_all_irqs();

    // Configure the nRF24 with the data from the customizer
    nRF24_init();

    // After PWR_UP = 1 the radio is in Standby-I mode, 130us of delay for settling
    CyDelayUs(150);
}

/**
 * Configure the nRF radio with the data from the customizer.
 */
void nRF24_init(void)
{
    nRF24_write_reg(NRF_REG_EN_AA, (uint8_t []){CUSTOMIZER_EN_AA}, 1);
    nRF24_write_reg(NRF_REG_EN_RXADDR, (uint8_t []){CUSTOMIZER_EN_RXADDR}, 1);
    nRF24_write_reg(NRF_REG_SETUP_AW, (uint8_t []){CUSTOMIZER_SETUP_AW}, 1);
    nRF24_write_reg(NRF_REG_SETUP_RETR, (uint8_t []){CUSTOMIZER_SETUP_RETR}, 1);
    nRF24_write_reg(NRF_REG_RF_CH, (uint8_t []){CUSTOMIZER_RF_CH}, 1);
    nRF24_write_reg(NRF_REG_RF_SETUP, (uint8_t []){CUSTOMIZER_RF_SETUP}, 1);
    nRF24_write_reg(NRF_REG_DYNPD, (uint8_t []){CUSTOMIZER_DYNPD}, 1);
    nRF24_write_reg(NRF_REG_FEATURE, (uint8_t []){CUSTOMIZER_FEATURE}, 1);

// Configuring data pipes
#if (ENABLE_PIPE0 == 1)
    nRF24_write_reg(NRF_REG_RX_PW_P0, (uint8_t []){CUSTOMIZER_RX_PW_P0}, 1);
#endif
#if (ENABLE_PIPE1 == 1)
    nRF24_write_reg(NRF_REG_RX_PW_P1, (uint8_t []){CUSTOMIZER_RX_PW_P1}, 1);
#endif
#if (ENABLE_PIPE2 == 1)
    nRF24_write_reg(NRF_REG_RX_ADDR_P2, (uint8_t []){CUSTOMIZER_RX_ADDR_P2}, 1);
    nRF24_write_reg(NRF_REG_RX_PW_P2, (uint8_t []){CUSTOMIZER_RX_PW_P2}, 1);
#endif
#if (ENABLE_PIPE3 == 1)
    nRF24_write_reg(NRF_REG_RX_ADDR_P3, (uint8_t []){CUSTOMIZER_RX_ADDR_P3}, 1);
    nRF24_write_reg(NRF_REG_RX_PW_P3, (uint8_t []){CUSTOMIZER_RX_PW_P3}, 1);
#endif
#if (ENABLE_PIPE4 == 1)
    nRF24_write_reg(NRF_REG_RX_ADDR_P4, (uint8_t []){CUSTOMIZER_RX_ADDR_P4}, 1);
    nRF24_write_reg(NRF_REG_RX_PW_P4, (uint8_t []){CUSTOMIZER_RX_PW_P4}, 1);
#endif
#if (ENABLE_PIPE5 == 1)
    nRF24_write_reg(NRF_REG_RX_ADDR_P5, (uint8_t []){CUSTOMIZER_RX_ADDR_P5}, 1);
    nRF24_write_reg(NRF_REG_RX_PW_P5, (uint8_t []){CUSTOMIZER_RX_PW_P5}, 1);
#endif

    nRF24_write_reg(NRF_REG_CONFIG, (uint8_t []){CUSTOMIZER_CONFIG}, 1);

    // Set the _nrf_addr_width variable
    _addr_width_conf_to_bytes(CUSTOMIZER_SETUP_AW);
}

/**
 * Enable the nRF24 radio.
 *
 * @todo Implement this function
 */
void nRF24_enable(void)
{
}

/**
 * Stop the nRF24 radio.
 *
 * @todo Implement this function
 */
void nRF24_stop(void)
{
}

/**
 * Put the nRF24 radio on Standby-I mode.
 * 
 * @note Is standby-I the lowest power mode?
 */
void nRF24_sleep(void)
{
    nRF24_set_standby_i_mode();
}

/**
 * Wakeup the nRF24 radio, and restore the configuration.
 */
void nRF24_wakeup(void)
{
    nRF24_set_bit(NRF_REG_CONFIG, NRF_CONFIG_BIT_PWR_UP);
    // after leaving standby-I mode the radio need a time to return to TX or
    // RX Mode
    CyDelay(5);
}

/**
 * Save the nRF24 radio configuration.
 *
 * @todo Implement this function.
 */
void nRF24_save_config(void)
{
}

/**
 * Restore the nRF24 radio configuration.
 *
 * @todo Implement this function.
 */
void nRF24_restore_config(void)
{
}

/**
 * Configure the radio as Receiver or Transmitter.
 *
 * @param const nrf_mode mode: The radio can be configured as Receiver (PRX)
 * or Transmitter (PTX).
 */
void nRF24_set_mode(const nrf_mode mode)
{
    mode == NRF_MODE_RX ? nRF24_set_rx_mode() : nRF24_set_tx_mode();
}

/**
 * @brief Set radio in Power Down Mode.
 *
 * In power down mode nRF24 is disabled using minimal current consumption.
 * All register values available are maintained and the SPI is kept active,
 * enabling change of configuration and the uploading/downloading of data
 * registers.
 * Power down mode is entered by setting the PWR_UP bit (CONFIG register) to 0.
 */
void nRF24_set_power_down_mode(void)
{
    nRF24_clear_bit(NRF_REG_CONFIG, NRF_CONFIG_BIT_PWR_UP);
}

/**
 * @brief Set the radio in Standby I Mode.
 *
 * By setting the PWR_UP bit in the CONFIG register to 1, the device enters
 * standby-I mode. Standby-I mode is used to minimize average current
 * consumption while maintaining short start up times.
 * In this mode only part of the crystal oscillator is active. Change to
 * active modes only happens if CE is set high and when CE is set low,
 * the nRF24 returns to standby-I mode from both the TX and RX modes.
 */
void nRF24_set_standby_i_mode(void)
{
    nRF24_stop_listening();
    nRF24_set_bit(NRF_REG_CONFIG, NRF_CONFIG_PWR_UP);
}

/**
 * @brief Set the radio in Standby II Mode.
 *
 * In standby-II mode extra clock buffers are active and more current is used
 * compared to standby-I mode. nRF24 enters standby-II mode if CE is held high
 * on a TX device with an empty TX FIFO.
 * If a new packet is uploaded to the TX FIFO, the PLL immediately starts and
 * the packet is transmitted after the normal PLL settling  delay (130us).
 */
void nRF24_set_standby_ii_mode(void)
{
    nRF24_start_listening();
    nRF24_set_bit(NRF_REG_CONFIG, NRF_CONFIG_BIT_PWR_UP);
}

/**
 * @brief Set the radio in Receiver (Rx) Mode.
 *
 * RX mode is an active mode where the nRF24 radio is used as receiver. To
 * enter this mode, the nRF24 must have the PWR_UP bit, PRIM_RX bit and the CE
 * pin set high.
 */
void nRF24_set_rx_mode(void)
{
    nRF24_set_bit(NRF_REG_CONFIG, NRF_CONFIG_BIT_PRIM_RX);
}

/**
 * @brief Set the radio in Transmitted (Tx) Mode.
 *
 * TX mode is an active mode for transmitting packets. To enter this mode,
 * the nRF24 must have the PWR_UP bit set high, PRIM_RX bit set low, a payload
 * in the TX FIFO and a high pulse on the CE for more than 10us.
 */
void nRF24_set_tx_mode(void)
{
    nRF24_clear_bit(NRF_REG_CONFIG, NRF_CONFIG_BIT_PRIM_RX);
}

/**
 * @brief Enable AutoACK in the given pipe.
 *
 * @param pipe: Enable AutoACK in the given pipe.
 */
void nRF24_enable_auto_ack(const nrf_pipe pipe)
{
    nRF24_set_bit(NRF_REG_EN_AA, pipe);
}

/**
 * @brief Disable AutoACK in the given pipe.
 *
 * @param pipe: Disable AutoACK in the given pipe.
 */
void nRF24_disable_auto_ack(const nrf_pipe pipe)
{
    nRF24_clear_bit(NRF_REG_EN_AA, pipe);
}

/**
 * @brief Set the channel where the radio will work.
 *
 * @param uint8_t channel: Channel where the radio will work.
 */
void nRF24_set_channel(uint8_t channel)
{
    if (NRF_MAX_RF_CHANNEL < channel) {
        channel = NRF_MAX_RF_CHANNEL;
    }

    nRF24_write_reg(NRF_REG_RF_CH, &channel, 1);

    nRF24_cmd_flush_rx();
    nRF24_cmd_flush_tx();
}

/**
 * @brief Set the data pipes address width.
 *
 * @param const nrf_setup_address_width addr_width:
 */
uint8_t nRF24_get_channel(void)
{
    uint8_t channel;
    nRF24_read_reg(NRF_REG_RF_CH, &channel, 1);
    return channel;
}

/**
 * @brief Set the data pipes address width.
 *
 * @param const nrf_addr_width addr_width:
 */
void nRF24_set_address_width(const nrf_addr_width addr_width)
{
    nRF24_write_reg(NRF_REG_SETUP_AW, &addr_width, 1);

    // update the _nrf_addr_width variable
    _addr_width_conf_to_bytes(addr_width);
}

/**
 * @brief Get the address width of the data pipes.
 *
 * @return uint8_t: Address width in bytes.
 */
uint8_t nRF24_get_address_width(void)
{
    return _nrf_addr_width;
}

/**
 * @brief Set the address of the RX Pipe 0 in the radio.
 *
 * This function configure the address of the Rx Pipe 0 of the radio.
 *
 * @param const uint8_t* addr:
 * @param size_t size:
 */
void nRF24_set_rx_pipe_address(const nrf_addr_rx_pipe pipe,
                                            const uint8_t *addr, size_t size)
{
    if (NULL == addr) {
        return;
    }
    
    switch(pipe) {
    // For pipes 0 and 1 we can change up to 5 bytes of it's addresses
    case NRF_ADDR_PIPE0:
    case NRF_ADDR_PIPE1:
        // The smaller address is 3 bytes
        if (NRF_PIPE_ADDR_WIDTH_3BYTES > size) {
            // TODO
        }
        
        if (_nrf_addr_width < size) {
            size = _nrf_addr_width;
        }
        nRF24_write_reg(pipe, addr, size);
        break;
    // For pipes 2, 3, 4 and 5 we can change only the LSB of it's address
    case NRF_ADDR_PIPE2:
    case NRF_ADDR_PIPE3:
    case NRF_ADDR_PIPE4:
    case NRF_ADDR_PIPE5:
        (void)size; // so we don't get a warning on unused variable
        nRF24_write_reg(pipe, addr, 1);
        break;
    }
}

/**
 * @brief Get the address of the RX Pipe 0 in the radio.
 *
 * @param uint8_t* addr:
 * @param size_t size:
 */
void nRF24_get_rx_pipe_address(const nrf_addr_rx_pipe pipe,
                                            uint8_t *addr, size_t size)
{
    if (NULL == addr) {
        return;
    }
    
    // The smaller address is 3 bytes
    if (NRF_PIPE_ADDR_WIDTH_3BYTES > size) {
        // TODO
    }
    
    if (_nrf_addr_width < size) {
        size = _nrf_addr_width;
    }
    
    switch(pipe) {
    // For pipes 0 and 1 we can read up to 5 bytes of it's addresses
    case NRF_ADDR_PIPE0:
    case NRF_ADDR_PIPE1:
        nRF24_read_reg(pipe, addr, size);
        break;
    // For pipes 2, 3, 4 and 5 the address is the same to pipe1 except the LSB
    case NRF_ADDR_PIPE2:
    case NRF_ADDR_PIPE3:
    case NRF_ADDR_PIPE4:
    case NRF_ADDR_PIPE5:
        nRF24_read_reg(NRF_REG_RX_ADDR_P1, addr, size - 1);
        nRF24_read_reg(pipe, &addr[size - 1], 1);
        break;
    }
}

/**
 * @brief Set the TX Address of the radio.
 *
 * @param const uint8_t* addr:
 * @param size_t size:
 */
void nRF24_set_tx_address(const uint8_t *const addr, size_t size)
{
    if (NULL == addr) {
        return;
    }

    if (_nrf_addr_width < size) {
        size = _nrf_addr_width;
    }

    nRF24_write_reg(NRF_REG_TX_ADDR, addr, size);
}

/**
 * @brief Get the TX Address of the radio.
 *
 * @param const uint8_t* addr:
 * @param const size_t size:
 */
void nRF24_get_tx_address(uint8_t* addr, size_t size)
{
    if (NULL == addr) {
        return;
    }

    if (_nrf_addr_width < size) {
        size = _nrf_addr_width;
    }

    nRF24_read_reg(NRF_REG_TX_ADDR, addr, size);
}

/**
 * @brief Set the payload size of the given pipe.
 *
 * Configure the payload size of the given pipe.
 *
 * @param const nrf_pipe pipe:
 * @param uint8_t size:
 */
void nRF24_set_payload_size(const nrf_pld_size pipe, uint8_t size)
{
    if (NRF_PAYLOAD_SIZE_MAX < size) {
        size = NRF_PAYLOAD_SIZE_MAX;
    }

    nRF24_write_reg(pipe, &size, 1);
}

/**
 * Get the payload size of the given pipe.
 *
 * @param const nrf_pipe pipe: Pipe to be read.
 *
 * @return uint8_t: Configured payload size of the given pipe.
 */
uint8_t nRF24_get_payload_size(const nrf_pld_size pipe)
{
    uint8_t payload_size;
    nRF24_read_reg(pipe, &payload_size, 1);
    return payload_size;
}

/**
 * @brief Reuse last transmitted payload.
 *
 * This function issue the command ReuseTxPayload and then toggle the CE pin
 * to transmit the last transmitted payload.
 */
void nRF24_reuse_last_transmitted_payload(void)
{
    nRF24_cmd_reuse_tx_payload();
    nRF24_transmit_pulse();
}

/**
 * @brief Enable dynamic payload on the given pipe.
 *
 * @param const nrf_pipe pipe:
 */
void nRF24_enable_dynamic_payload(void)
{
    // TODO: Read the FEATURE register, check the needed values and set
    // the bits if not set already
    nRF24_set_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_ACK_PAY);
    nRF24_set_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_DPL);
}

/**
 * @brief Enable dynamic payload on the given pipe.
 *
 * @param const nrf_pipe pipe:
 */
void nRF24_enable_dynamic_payload_on_pipe(const nrf_pipe pipe)
{
    nRF24_set_bit(NRF_REG_EN_AA, pipe);
    nRF24_set_bit(NRF_REG_DYNPD, pipe);
}

/**
 * @brief Disable dynamic payload on the given pipe.
 *
 * @param const nrf_pipe pipe:
 */
void nRF24_disable_dynamic_payload()
{
    // TODO: Read the FEATURE register, check the needed values and clear
    // the bits if not clear already
    nRF24_clear_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_ACK_PAY);
    nRF24_clear_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_DPL);
}

/**
 * @brief Disable dynamic payload on the given pipe.
 *
 * @param const nrf_pipe pipe:
 */
void nRF24_disable_dynamic_payload_on_pipe(const nrf_pipe pipe)
{
    nRF24_clear_bit(NRF_REG_EN_AA, pipe);
    nRF24_clear_bit(NRF_REG_DYNPD, pipe);
}

/**
 * @brief Enable dinamic payload length.
 */
void nRF24_enable_dynamic_payload_length(void)
{
    nRF24_set_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_DPL);
}

/**
 * @brief Disable dynamic payload length.
 */
void nRF24_disable_dynamic_payload_length(void)
{
    nRF24_clear_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_DPL);
}

/**
 * @brief Enable payload with ACK.
 */
void nRF24_enable_payload_with_ack(void)
{
    nRF24_set_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_ACK_PAY);
}

/**
 * @brief Disable Payload with ACK.
 */
void nRF24_disable_payload_with_ack(void)
{
    nRF24_clear_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_ACK_PAY);
}

/**
 * @brief Enable dynamic payload length.
 */
void nRF24_enable_payload_with_no_ack(void)
{
    nRF24_set_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_DYN_ACK);
}

/**
 * @brief Disable Payload with no ACK.
 */
void nRF24_disable_payload_with_no_ack(void)
{
    nRF24_clear_bit(NRF_REG_FEATURE, NRF_FEATURE_BIT_EN_DYN_ACK);
}

/**
 * @brief The nRF24 radio will start listening.
 *
 * This function set the pin CE to logic high, this enable the radio for
 * listening.
 */
void nRF24_start_listening(void)
{
    nRF24_ce_write(GPIO_SET);
}

/**
 * @brief The nRF24 radio will stop listening.
 *
 * This function the pin CE of the nRF24 radio will be set to logic low,
 * this disable the radio for listening.
 */
void nRF24_stop_listening(void)
{
    nRF24_ce_write(GPIO_CLEAR);
}

/**
 * @brief Transmit pulse on the CE pin.
 *
 * With this function the CE pin of the nRF24 radio will have a pulse of 15us,
 * this pulse trigger a transmission of the content of the TX FIFO.
 */
void nRF24_transmit_pulse(void)
{
    nRF24_ce_write(GPIO_SET);
    CyDelayUs(NRF_CE_PULSE_WIDTH_US);
    nRF24_ce_write(GPIO_CLEAR);
}

/**
 * @brief Get the STATUS register of the nRF24.
 *
 * @return uint8_t: STATUS register of the nRF24.
 */
uint8_t nRF24_get_status(void)
{
    return nRF24_cmd_nop();
}

/**
 * @brief Get the STATUS register of the nRF24.
 *
 * @return uint8_t: STATUS register of the nRF24.
 */
uint8_t nRF24_get_fifo_status(void)
{
    uint8_t fifo_status;
    nRF24_read_reg(NRF_REG_FIFO_STATUS, &fifo_status, 1);
    return fifo_status;
}

/**
 * @brief Get the number of retransmissions.
 *
 * @return uint8_t: Retransmissions count.
 */
uint8_t nRF24_get_retransmissions_count(void)
{
    uint8_t count;
    nRF24_read_reg(NRF_REG_OBSERVE_TX, &count, 1);
    return count & NRF_OBSERVE_TX_ARC_CNT_MASK;
}

/**
 * @brief Get the number of lost packets.
 *
 * @return uint8_t: Lost packets.
 */
uint8_t nRF24_get_lost_packets_count(void)
{
    uint8_t lostPackets;
    nRF24_read_reg(NRF_REG_OBSERVE_TX, &lostPackets, 1);
    lostPackets = lostPackets & NRF_OBSERVE_TX_PLOS_CNT_MASK;
    return lostPackets >> NRF_OBSERVE_TX_BIT_PLOS_CNT;
}

/**
 * Put data into the TX FIFO wihout sending it.
 *
 * @param const uint8_t* data:
 * @param const size_t size:
 *
 * @return true if payload was placed on the fifo, false otherwise.
 */
bool nRF24_put_in_tx_fifo(const uint8_t* payload, size_t payload_size)
{
    bool placed_on_fifo = false;
    
    if (NULL == payload) {
        return false;
    }

    if (NRF_PAYLOAD_SIZE_MAX < payload_size) {
        payload_size = NRF_PAYLOAD_SIZE_MAX;
    }
    
    if (false == nRF24_is_tx_fifo_full()) {
        placed_on_fifo = true;
        nRF24_cmd_write_tx_payload(payload, payload_size);
    }

    return placed_on_fifo;
}

/**
 * Put data in TX FIFO and transmit it.
 *
 * @param const uint8_t* data:
 * @param const size_t size:
 *
 * TODO
 * This function assumes the tx fifo is empty, so it doesn't check if the fifo
 * already have payloads in.
 */
void nRF24_transmit(const uint8_t* payload, size_t payload_size)
{
    if (NULL == payload) {
        return;
    }

    if (NRF_PAYLOAD_SIZE_MAX < payload_size) {
        payload_size = NRF_PAYLOAD_SIZE_MAX;
    }

    nRF24_put_in_tx_fifo(payload, payload_size);
    nRF24_transmit_pulse();
}

/**
 * @brief
 *
 * @return bool: True if there's data ready.
 */
bool nRF24_is_data_ready(void)
{
    return NRF_STATUS_RX_DR_MASK & nRF24_get_status();
}

/**
 * @brief
 *
 * @param uint8_t* data:
 * @param const size_t size:
 */
void nRF24_get_rx_payload(uint8_t *payload, const size_t payload_size)
{
    if (NULL == payload) {
        return;
    }

    nRF24_ce_write(GPIO_CLEAR);
    nRF24_cmd_read_rx_payload(payload, payload_size);
    nRF24_ce_write(GPIO_SET);
}

/**
 * @brief
 *
 * @param const uint8_t* data:
 * @param size_t size:
 */
void nRF24_tx_transmit_no_ack(const uint8_t *payload, size_t payload_size)
{
    if (NULL == payload) {
        return;
    }

    if (NRF_PAYLOAD_SIZE_MAX < payload_size) {
        return;
    }

    nRF24_cmd_no_ack_payload(payload, payload_size);
    nRF24_transmit_pulse();
}

/**
 * @brief
 *
 * @param const nrf_pipe pipe:
 * @param const uint8_t* data:
 * @param size_t size:
 */
void nRF24_rx_write_payload(const nrf_pipe pipe,
                                        const uint8_t *payload, size_t payload_size)
{
    if (NULL == payload) {
        return;
    }

    if (NRF_PAYLOAD_SIZE_MAX < payload_size) {
        payload_size = NRF_PAYLOAD_SIZE_MAX;
    }

    nRF24_cmd_write_ack_payload(pipe, payload, payload_size);
}

/**
 * Return the pipe number with data.
 *
 * @return uint8_t:
 */
uint8_t nRF24_get_data_pipe_with_payload(void)
{
    uint8_t pipe;
    nRF24_read_reg(NRF_REG_STATUS, &pipe, 1);
    return (pipe & NRF_STATUS_PIPES_MASK) >> NRF_STATUS_PIPES_SHIFT;
}

/**
 * @brief
 *
 * @return uint8_t:
 */
uint8_t nRF24_received_power_detector(void)
{
    return nRF24_read_bit(NRF_REG_RPD, NRF_RPD_BIT_RPD);
}

/**
 * @return bool: true if the TX FIFO is full, false if it have available
 * locations.
 */
bool nRF24_is_tx_fifo_full(void)
{
    return nRF24_read_bit(NRF_REG_FIFO_STATUS, NRF_FIFO_STATUS_BIT_TX_FULL);
}

/**
 * @return bool: true if the RX FIFO es empty, false if any pipe have data.
 */
bool nRF24_is_rx_fifo_empty(void)
{
    return nRF24_read_bit(NRF_REG_FIFO_STATUS, NRF_FIFO_STATUS_BIT_RX_EMPTY);
}

/**
 * Received Power Detector triggers at received power levels above -64dBm that
 * are present in the RF channel you receive on. If the received power is less
 * than -64dBm, RDP = 0.
 * The RPD can be read out at any time while nRF24L01+ is in receive mode.
 */
bool nRF24_test_carrier(void)
{
    return nRF24_read_bit(NRF_REG_RPD, NRF_RPD_BIT_RPD);
}

/**
 * This function will write 1 to all the three IRQ "flag" bits on the
 * STATUS register.
 */
void nRF24_clear_all_irqs(void)
{
    uint8_t mask = NRF_ALL_IRQ_MASK;
    nRF24_write_reg(NRF_REG_STATUS, &mask, 1);
}

/**
 * @brief Clears the specific IRQ flag.
 *
 * Clear the flag by writing 1 to the interrupt flag bit.
 *
 * @param nrf_irq irq_flag: Interrupt flag to clear.
 */
void nRF24_clear_irq_flag(const nrf_irq irq_flag)
{
    const uint8_t mask = 1 << irq_flag;
    uint8_t status = nRF24_get_status();
    status |= mask;
    
    nRF24_write_reg(NRF_REG_STATUS, &status, 1);
}

/**
 * This method is used to get if any of the IRQ "flag" bits on the STATUS
 * register is set to 1. This function returns 0 is none of the flag bits
 * is not set to 1.
 *
 * @return nrf_irq: Asserted bit of the interrupt flags.
 */
nrf_irq nRF24_get_irq_flag(void)
{
    return NRF_ALL_IRQ_MASK & nRF24_cmd_nop();
}

void nRF24_poll_interrupt(void)
{
    // TODO
}

uint8_t nRF24_get_status_clear_irq(void)
{
    uint8_t mask = NRF_ALL_IRQ_MASK;
    return nRF24_write_reg(NRF_REG_STATUS, &mask, 1);
}

// command wrappers
void nRF24_flush_rx(void)
{
    nRF24_cmd_flush_rx();
}

void nRF24_flush_tx(void)
{
    nRF24_cmd_flush_tx();
}

/* [] END OF FILE */
