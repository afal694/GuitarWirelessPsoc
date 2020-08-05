/**
* @file     nRF24_CONFIG.h
* @version  3
* @brief    Configuration file.
*/

#ifndef nRF24_CONFIG_H
#define nRF24_CONFIG_H

#include "SPI.h"

/* So there's no symbol to know if the API is being built
 * for a psoc4, 5lp or 6, so i found out based on the spi headers */
#if defined (CY_SCB_SPI_H)
# if defined (SPI_CY_SCB_SPI_PDL_H)
#  define _PSOC6
# else
#  define _PSOC4_SCB
   #include "SPI_SPI_UART.h"
# endif
#else
# define _PSOC_UDB
#endif

#include "nRF24_DEFS.h"

// Component version info
#define nRF24_VERSION_MAJOR  3
#define nRF24_VERSION_MINOR  0
#define nRF24_VERSION        3.0

// Data from customizer
#define ENABLE_PIPE0    0
#define ENABLE_PIPE1    1
#define ENABLE_PIPE2    0
#define ENABLE_PIPE3    0
#define ENABLE_PIPE4    0
#define ENABLE_PIPE5    0

#define CUSTOMIZER_EN_AA        ((0 << NRF_EN_AA_BIT_ENAA_P5) | (0 << NRF_EN_AA_BIT_ENAA_P4) | \
                                (0 << NRF_EN_AA_BIT_ENAA_P3) | (0 << NRF_EN_AA_BIT_ENAA_P2) | \
                                (0 << NRF_EN_AA_BIT_ENAA_P1) | (0 << NRF_EN_AA_BIT_ENAA_P0))
#define CUSTOMIZER_EN_RXADDR    ((0 << NRF_EN_RXADDR_BIT_ERX_P5) | (0 << NRF_EN_RXADDR_BIT_ERX_P4) | \
                                (0 << NRF_EN_RXADDR_BIT_ERX_P3) | (0 << NRF_EN_RXADDR_BIT_ERX_P2) | \
                                (1 << NRF_EN_RXADDR_BIT_ERX_P1) | (0 << NRF_EN_RXADDR_BIT_ERX_P0))
#define CUSTOMIZER_SETUP_AW     (3)
#define CUSTOMIZER_SETUP_RETR   ((0 << NRF_SETUP_RETR_BIT_ARD) | (0 << NRF_SETUP_RETR_BIT_ARC))
#define CUSTOMIZER_RF_CH        (75)
#define CUSTOMIZER_RF_SETUP     ((0 << NRF_RF_SETUP_BIT_CONT_WAVE) | (1 << NRF_RF_SETUP_BIT_RF_DR) | \
                                (3 << NRF_RF_SETUP_BIT_RF_PWR))
#define CUSTOMIZER_DYNPD        ((0 << NRF_DYNPD_BIT_DPL_P5) | (0 << NRF_DYNPD_BIT_DPL_P4) | \
                                (0 << NRF_DYNPD_BIT_DPL_P3) | (0 << NRF_DYNPD_BIT_DPL_P2) | \
                                (0 << NRF_DYNPD_BIT_DPL_P1) | (0 << NRF_DYNPD_BIT_DPL_P0))
#define CUSTOMIZER_FEATURE      ((0 << NRF_FEATURE_BIT_EN_DPL) | (0 << NRF_FEATURE_BIT_EN_ACK_PAY) | \
                                (0 << NRF_FEATURE_BIT_EN_DYN_ACK))
#define CUSTOMIZER_CONFIG       ((0 << NRF_CONFIG_BIT_MASK_RX_DR) | (1 << NRF_CONFIG_BIT_MASK_TX_DS) | \
                                (1 << NRF_CONFIG_BIT_MASK_MAX_RT) | (0 << NRF_CONFIG_BIT_EN_CRC) | \
                                (0 << NRF_CONFIG_BIT_CRCO) | (1<< NRF_CONFIG_BIT_PWR_UP) | (1 << NRF_CONFIG_BIT_PRIM_RX))
#define CUSTOMIZER_RX_PW_P0     (0)
#define CUSTOMIZER_RX_PW_P1     (2)
#define CUSTOMIZER_RX_PW_P2     (0)
#define CUSTOMIZER_RX_ADDR_P2   (195)
#define CUSTOMIZER_RX_PW_P3     (0)
#define CUSTOMIZER_RX_ADDR_P3   (196)
#define CUSTOMIZER_RX_PW_P4     (0)
#define CUSTOMIZER_RX_ADDR_P4   (197)
#define CUSTOMIZER_RX_PW_P5     (0)
#define CUSTOMIZER_RX_ADDR_P5   (198)

#endif /* nRF24_CONFIG_H */

/* [] END OF FILE */
