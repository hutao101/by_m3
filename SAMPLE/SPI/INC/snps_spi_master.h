/*
 * SNPS SPI master driver file
 */

#ifndef __SNPS_SPI_MASTER_H__
#define __SNPS_SPI_MASTER_H__

#include "snps-spi-common.h"

void snps_spi_master_cs_set(struct snps_spi_init *init, u8 cs);

u32 snps_spi_master_write_cmd(u32 base, u8 *cmd_mesg, u8 cmd_len);

void snps_spi_master_init(struct snps_spi_init *init);

void snps_spi_master_transmit(struct snps_spi_init *init);

void snps_spi_master_receive(struct snps_spi_init *init);

void snps_spi_master_duplex(struct snps_spi_init *init);

void snps_spi_master_eeprom_read(struct snps_spi_init *init);

u8 snps_spi_master_default_value_test(struct snps_spi_init *init);

extern struct snps_spi_init snps_spi_master_init_table[2];

#endif //__SNPS_SPI_MASTER_H__
