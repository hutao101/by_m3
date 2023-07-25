/*
 * SNPS SPI slave driver file
 */

#ifndef __SNPS_SPI_SLAVE_H__
#define __SNPS_SPI_SLAVE_H__

#include "snps_spi_common.h"

bool snps_spi_slave_output_isenable(struct snps_spi_init *init);

void snps_spi_slave_output_enable(struct snps_spi_init *init);

void snps_spi_slave_output_disable(struct snps_spi_init *init);

void snps_spi_deivce_init(struct snps_spi_init *init);

void snps_spi_slave_receive(struct snps_spi_init *init);

void snps_spi_slave_transmit(struct snps_spi_init *init);

void snps_spi_slave_duplex(struct snps_spi_init *init);

u8 snps_spi_slave_default_value_test(struct snps_spi_init *init);

extern struct snps_spi_init snps_spi_slave_init_table[2];

#endif //__SNPS_SPI_SLAVE_H__
