/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef NRF24L01_H
#define	NRF24L01_H

#include <xc.h>
#include "../05_MCC_16F1575/mcc.h"
#include "../01_Appli/Application.h"
#include "../02_Soft_SPI/Soft_SPI.h"

/*------------------------------------------------*/
/*              NRF24L01 PHYSICAL INTERFACE       */
/*------------------------------------------------*/
#define NRF_PIN_CE                  CE_LAT
#define NRF_PIN_CSN                 CS__LAT

/*------------------------------------------------*/
/*              NRF24L01 SPI commands             */
/*------------------------------------------------*/
#define CMD_NRF_R_REGISTER          0x00u
#define CMD_NRF_W_REGISTER          0x20u
#define CMD_NRF_R_RX_PAYLOA         0x61u
#define CMD_NRF_W_TX_REGISTR        0xA0u
#define CMD_NRF_FLUSH_TX            0xE1u
#define CMD_NRF_FLUSH_RX            0xE2u
#define CMD_NRF_REUSE_TX_PL         0xE3u
#define CMD_NRF_ACTIVATE            0x50u
#define CMD_NRF_R_RX_PL_WID         0x60u
#define CMD_NRF_W_ACK_PAYLOAD       0xA8u
#define CMD_NRF_W_TX_PAYLOAD_NO_ACK 0xB0u
#define CMD_NRF_NOP                 0x00u

/*------------------------------------------------*/
/*              NRF24L01 Register Addresses       */
/*------------------------------------------------*/
#define REG_NRF_CONFIG              0x00u
#define REG_NRF_EN_AA               0x01u
#define REG_NRF_EN_RXADDR           0x02u
#define REG_NRF_SETUP_AW            0x03u
#define REG_NRF_SETUP_RETR          0x04u
#define REG_NRF_RF_CH               0x05u
#define REG_NRF_RF_SETUP            0x06u
#define REG_NRF_STATUS              0x07u
#define REG_NRF_OBSERVE_TX          0x08u
#define REG_NRF_CD                  0x09u
#define REG_NRF_RX_ADDR_P0          0x0Au
#define REG_NRF_RX_ADDR_P1          0x0Bu
#define REG_NRF_RX_ADDR_P2          0x0Cu
#define REG_NRF_RX_ADDR_P3          0x0Du
#define REG_NRF_RX_ADDR_P4          0x0Eu
#define REG_NRF_RX_ADDR_P5          0x0Fu
#define REG_NRF_TX_ADDR             0x10u
#define REG_NRF_RX_PW_P0            0x11u
#define REG_NRF_RX_PW_P1            0x12u
#define REG_NRF_RX_PW_P2            0x13u
#define REG_NRF_RX_PW_P3            0x14u
#define REG_NRF_RX_PW_P4            0x15u
#define REG_NRF_RX_PW_P5            0x16u
#define REG_NRF_FIFO_STATUS         0x17u
#define REG_NRF_DYNPD               0x1Cu
#define REG_NRF_FEATURE             0x1Du

/*------------------------------------------------*/
/*              NRF24L01 Register bits            */
/*------------------------------------------------*/
/*        CONFIG        */
#define NRF_PRX                     0x01u
#define NRF_PWR_UP                  0x02u
#define NRF_CRC_2BYTES              0x04u
#define NRF_IRQ_MAX_RT              0x60u
#define NRF_IRQ_TX_DS               0x50u
#define NRF_IRQ_RX_DR               0x30u
#define NRF_IRQ_NONE                0x70u

/*    ADDRESS_WIDTH     */
#define NRF_ADDR_3BYTES             0x01u
#define NRF_ADDR_4BYTES             0x02u
#define NRF_ADDR_5BYTES             0x03u

/*       RF SETUP       */
#define NRF_POWER_0                 0x00u
#define NRF_POWER_1                 0x02u
#define NRF_POWER_2                 0x04u
#define NRF_POWER_3                 0x06u
#define NRF_HIGH_DR                 0x08u

/*     STATUS MASKS     */
#define NRF_IS_TX_FULL              0xFEu
#define NRF_RX_ON_PIPE              0xF1u
#define NRF_IS_MAX_RT               0xEFu
#define NRF_IS_TX_SENT              0xDFu
#define NRF_IS_RX_READY             0xBFu

/*      FIFO MASKS      */
#define NRF_RX_FIFO_EMPTY           0xFEu
#define NRF_RX_FIFO_FULL            0xFDu
#define NRF_TX_FIFO_EMPTY           0xEFu
#define NRF_TX_FIFO_FULL            0xDFu
#define NRF_TX_REUSE                0xBFu

#define NRF_UNPACK_8(b, p, m) ((b >> p) & m)

typedef struct
{
    union
    {
        uint8_t byte;
        struct
        {
            uint8_t TX_FULL :       1;
            uint8_t RX_P_NO :       3;
            uint8_t MAX_RT :        1;
            uint8_t TX_DS :         1;
            uint8_t RX_DR :         1;
        }s;
    }STATUS;
    union
    {
        uint8_t byte;
        struct
        {
            uint8_t PRIM_RX:        1;
            uint8_t PWR_UP:         1;
            uint8_t CRCO:           1;
            uint8_t EN_CRC:         1;
            uint8_t MASK_MAX_RT:    1;
            uint8_t MASK_TX_DS:     1;
            uint8_t MASK_RX_DR:     1;
        }s;
    }CONFIG;
    uint8_t TX_ADDR[5];
    uint8_t RF_CHANNEL;
    union
    {
        uint8_t byte;
        struct
        {
            uint8_t :               1;
            uint8_t PWR:            2;
            uint8_t DR_H:           1;
            uint8_t PLL_LOCK:       1;
            uint8_t DR_L:           1;
            uint8_t CONT_WAVE:      1;
        }s;
    }RF_SETUP;
    uint8_t ADDR_WIDTH;
    union
    {
        uint8_t byte;
        struct
        {
            uint8_t ARD :           4;
            uint8_t ARC :           4;
        }s;
    }SETUP_RETR;
    uint8_t ReadFlag;
} t_NRF_Setup;

typedef struct
{
    uint8_t PAY_LEN;
    uint8_t PIPE_ADDR[5];
} t_NRF_RX_PIPE;


void NRF24L01_Init(void);
void NRF_OpenReadingPipe(uint8_t PipeNo, uint8_t * PipeAddr, uint8_t PayloadLength, uint8_t AutoAck, uint8_t Enable);
void NRF_PipeEnable(uint8_t PipeNo);
void NRF_PipeEnableAA(uint8_t PipeNo);
void NRF_SetPrimaryAs(uint8_t asPrimary);
void NRF_SetRFChannel(uint8_t RF_Channel);
void NRF_SetRFPower(uint8_t RF_Pow);
void NRF_SetRFDataRate(uint8_t Datarate);
void NRF_SetAddrWidth(uint8_t AddressWidth);
uint8_t NRF_Write_Register(uint8_t Register, uint8_t *Bytes, uint8_t Length);
uint8_t NRF_Read_Register(uint8_t Register, uint8_t *Bytes, uint8_t Length);
void NRF_IRQ_ISR_Handler(void);
void NRF_Set_SPI_Handler(uint8_t (*SPI_Handler)(uint8_t));
#endif /* NRF24L01_H */