/*!
 * \file netx90_dspim.h
 * \brief Interface for the netX90 SPI Master module.
 *
 * Provides configuration structures, enumerations, and function declarations for the
 * SPI master implementation on the netX90 platform. This module supports various SPI modes,
 * clock frequencies, and loopback testing.
 */

#ifndef NETX90_DSPIM_H
#define NETX90_DSPIM_H

#include "netx90_app.h"
#include "dspiMaster.h"

/*!
 * \def NETX90_DSPIM_RX_BUFFER_SIZE
 * \brief The size of the receive buffer for SPI transactions.
 */
#define NETX90_DSPIM_RX_BUFFER_SIZE 256

/*!
 * \def NETX90_DSPIM_TX_BUFFER_SIZE
 * \brief The size of the transmit buffer for SPI transactions.
 */
#define NETX90_DSPIM_TX_BUFFER_SIZE 256

/*!
 * \enum nx90_dspi_mode_e
 * \brief Defines the available SPI modes for the netX90 SPI Master.
 */
typedef enum
{
	NX90_SPI_MODE_0 = 0x0u,/*!< CPOL 0 CPHA 0 */
	NX90_SPI_MODE_1 = 0x1u, /*!< CPOL 0 CPHA 1 */
	NX90_SPI_MODE_2 = 0x2u, /*!< CPOL 1 CPHA 0 */
	NX90_SPI_MODE_3 = 0x3u, /*!< CPOL 1 CPHA 1 */
} nx90_dspi_mode_e;

/*!
 * \enum nx90_dspi_frequency_e
 * \brief Defines the supported SPI clock frequencies.
 *
 * Frequencies range from 24.4 KHz to 99 MHz. Usage of loopback requires usage if low
 * frequency to work as expected.
 */
typedef enum
{
	NX90_SPI_FREQUENCY_STOPPED = 0x000u,/*!< The SPI is stopped. */
	NX90_SPI_FREQUENCY_24_4KHz = 0x001u,/*!< 24KHz */
	NX90_SPI_FREQUENCY_48_8KHz = 0x002u,/*!< 48KHz */
	NX90_SPI_FREQUENCY_97_6KHz = 0x004u,/*!< 97KHz */
	NX90_SPI_FREQUENCY_195KHz = 0x008u,/*!< 195KHz */
	NX90_SPI_FREQUENCY_390KHz = 0x010u,/*!< 390KHz */
	NX90_SPI_FREQUENCY_781KHz = 0x020u,/*!< 0.8MHz */
	NX90_SPI_FREQUENCY_1_56MHz = 0x040u,/*!< 1.5MHz */
	NX90_SPI_FREQUENCY_3_125MHz = 0x080u,/*!< 3.125MHz */
	NX90_SPI_FREQUENCY_6_25MHz = 0x100u,/*!< 6.25MHz */
	NX90_SPI_FREQUENCY_12_5MHz = 0x200u,/*!< 12.5MHz */
	NX90_SPI_FREQUENCY_25MHz = 0x400u,/*!< 25MHz */
	NX90_SPI_FREQUENCY_50MHz = 0x800u,/*!< 50MHz */
	NX90_SPI_FREQUENCY_99MHz = 0xFFFu,/*!< 99MHz */
	NX90_SPI_FREQUENCY_MAX = NX90_SPI_FREQUENCY_99MHz, /*!< SPI maximum frequency */
	NX90_SPI_FREQUENCY_MIN = NX90_SPI_FREQUENCY_STOPPED /*!< SPI minimum frequency */
} nx90_dspi_frequency_e;

/*!
 * \enum nx90_dspi
 * \brief Enum for selecting the netX90 SPI peripheral interface.
 */
typedef enum
{
	Netx90SPI0, /**< SPI App 0 */
	Netx90SPI1, /**< SPI App 1 */
	Netx90SPI2, /**< SPI App 2 */
} nx90_dspi;

/*!
 * \enum nx90_dspim_loopback_e
 * \brief Enum to enable or disable SPI loopback mode.
 *
 * When enabled, transmitted data will be placed placed as received in the spi data register.
 */
typedef enum
{
  NX90_SPIM_LOOPBACK_DISABLED = 0x0u,
  NX90_SPIM_LOOPBACK_ENABLED = 0x1u,
} nx90_dspim_loopback_e;

/*!
 * \enum nx90_dspi_hardwareCE
 * \brief Enum for selecting hardware-controlled chip enable pins.
 *
 * Review hardware configuration to verify the available harware routed CE pins.
 */
typedef enum
{
  NX90_SPI_CHIP_ENABLE_EXTERNAL = 0,    /**< Do not use peripheral controlled*/
  NX90_SPI_CHIP_ENABLE_0 = 0x1u,        /**< Peripheral controlled 0*/
  NX90_SPI_CHIP_ENABLE_1 = 0x2u,        /**< Peripheral controlled 1*/
  NX90_SPI_CHIP_ENABLE_2 = 0x4u,        /**< Peripheral controlled 2*/
}nx90_dspi_hardwareCE;

/**
 * @brief Structure for the dSpiM configuration.
 */
typedef struct
{
	nx90_dspi netx90Spi;                     /**< The SPI to use */
	nx90_dspi_frequency_e speed;             /**< The speed rate to use */
	nx90_dspi_mode_e spiMode;                /**< The SPI Mode */
	nx90_dspim_loopback_e loopbackMode;      /**< Is loopback enabled */
	nx90_dspi_hardwareCE hardwareChipEnable; /**< Hardware Chip Enable Mode*/
	uint32_t interruptPriority;              /**< Interrupt priority */
} nx90_spim_conf;

/**
 * @brief Structure for the dSpiM.
 */
typedef struct
{
	struct dSpiM super;                             /**< The SPI */
	nx90_dspi_frequency_e speed;                    /**< The baud rate */
	nx90_dspi_mode_e spiMode;                       /**< The SPI Mode */
	nx90_dspi spi;                                  /**< The SPI to ue */
	spi0_app_Type* spiRegister;                     /**< The SPI CMSIS register */
	nx90_dspi_hardwareCE pinChipEnable;             /**< The SPI Enable Pin used */
	uint8_t RxBuffer[NETX90_DSPIM_RX_BUFFER_SIZE];  /**< The receive buffer */
	uint8_t TxBuffer[NETX90_DSPIM_RX_BUFFER_SIZE];  /**< The transmit buffer */
} nx90_dspim_t;

/*!
 * \brief Initializes the netX90 SPI master module.
 *
 * \param self Pointer to the SPI master instance.
 * \param conf Pointer to the configuration structure.
 */
void Nx90Dspim_Init(nx90_dspim_t* self, nx90_spim_conf* conf);

/*!
 * \brief SPI interrupt handler for SPI0.
 *
 * To be placed within the init file for handling the peripheral interrupt
 */
void dSPI0_IRQHandler();

/*!
 * \brief SPI interrupt handler for SPI1.
 *
 * To be placed within the init file for handling the peripheral interrupt
 */
void dSPI1_IRQHandler();

/*!
 * \brief SPI interrupt handler for SPI2.
 *
 * To be placed within the init file for handling the peripheral interrupt
 */
void dSPI2_IRQHandler();

#endif

