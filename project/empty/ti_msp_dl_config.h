/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM_LED */
#define PWM_LED_INST                                                       TIMA1
#define PWM_LED_INST_IRQHandler                                 TIMA1_IRQHandler
#define PWM_LED_INST_INT_IRQN                                   (TIMA1_INT_IRQn)
#define PWM_LED_INST_CLK_FREQ                                            1000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_LED_C0_PORT                                               GPIOA
#define GPIO_PWM_LED_C0_PIN                                       DL_GPIO_PIN_15
#define GPIO_PWM_LED_C0_IOMUX                                    (IOMUX_PINCM37)
#define GPIO_PWM_LED_C0_IOMUX_FUNC                   IOMUX_PINCM37_PF_TIMA1_CCP0
#define GPIO_PWM_LED_C0_IDX                                  DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 0 */
#define GPIO_PWM_LED_C0_CMPL_PORT                                          GPIOA
#define GPIO_PWM_LED_C0_CMPL_PIN                                   DL_GPIO_PIN_8
#define GPIO_PWM_LED_C0_CMPL_IOMUX                               (IOMUX_PINCM19)
#define GPIO_PWM_LED_C0_CMPL_IOMUX_FUNC         IOMUX_PINCM19_PF_TIMA1_CCP0_CMPL




/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA0)
#define TIMER_0_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                           (999U)



/* Defines for UART_DEBUG */
#define UART_DEBUG_INST                                                    UART0
#define UART_DEBUG_INST_IRQHandler                              UART0_IRQHandler
#define UART_DEBUG_INST_INT_IRQN                                  UART0_INT_IRQn
#define GPIO_UART_DEBUG_RX_PORT                                            GPIOA
#define GPIO_UART_DEBUG_TX_PORT                                            GPIOA
#define GPIO_UART_DEBUG_RX_PIN                                    DL_GPIO_PIN_11
#define GPIO_UART_DEBUG_TX_PIN                                    DL_GPIO_PIN_10
#define GPIO_UART_DEBUG_IOMUX_RX                                 (IOMUX_PINCM22)
#define GPIO_UART_DEBUG_IOMUX_TX                                 (IOMUX_PINCM21)
#define GPIO_UART_DEBUG_IOMUX_RX_FUNC                  IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_DEBUG_IOMUX_TX_FUNC                  IOMUX_PINCM21_PF_UART0_TX
#define UART_DEBUG_BAUD_RATE                                            (115200)
#define UART_DEBUG_IBRD_40_MHZ_115200_BAUD                                  (21)
#define UART_DEBUG_FBRD_40_MHZ_115200_BAUD                                  (45)





/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOB)

/* Defines for B21: GPIOB.21 with pinCMx 49 on package pin 20 */
// pins affected by this interrupt request:["B21"]
#define KEY_INT_IRQN                                            (GPIOB_INT_IRQn)
#define KEY_INT_IIDX                            (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define KEY_B21_IIDX                                        (DL_GPIO_IIDX_DIO21)
#define KEY_B21_PIN                                             (DL_GPIO_PIN_21)
#define KEY_B21_IOMUX                                            (IOMUX_PINCM49)
/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOB)

/* Defines for B22: GPIOB.22 with pinCMx 50 on package pin 21 */
#define LED_B22_PIN                                             (DL_GPIO_PIN_22)
#define LED_B22_IOMUX                                            (IOMUX_PINCM50)
/* Port definition for Pin Group SPI_LCD */
#define SPI_LCD_PORT                                                     (GPIOB)

/* Defines for LCD_SDA: GPIOB.8 with pinCMx 25 on package pin 60 */
#define SPI_LCD_LCD_SDA_PIN                                      (DL_GPIO_PIN_8)
#define SPI_LCD_LCD_SDA_IOMUX                                    (IOMUX_PINCM25)
/* Defines for LCD_SCL: GPIOB.9 with pinCMx 26 on package pin 61 */
#define SPI_LCD_LCD_SCL_PIN                                      (DL_GPIO_PIN_9)
#define SPI_LCD_LCD_SCL_IOMUX                                    (IOMUX_PINCM26)
/* Defines for LCD_RES: GPIOB.10 with pinCMx 27 on package pin 62 */
#define SPI_LCD_LCD_RES_PIN                                     (DL_GPIO_PIN_10)
#define SPI_LCD_LCD_RES_IOMUX                                    (IOMUX_PINCM27)
/* Defines for LCD_DC: GPIOB.11 with pinCMx 28 on package pin 63 */
#define SPI_LCD_LCD_DC_PIN                                      (DL_GPIO_PIN_11)
#define SPI_LCD_LCD_DC_IOMUX                                     (IOMUX_PINCM28)
/* Defines for LCD_CS: GPIOB.14 with pinCMx 31 on package pin 2 */
#define SPI_LCD_LCD_CS_PIN                                      (DL_GPIO_PIN_14)
#define SPI_LCD_LCD_CS_IOMUX                                     (IOMUX_PINCM31)
/* Defines for LCD_BLK: GPIOB.26 with pinCMx 57 on package pin 28 */
#define SPI_LCD_LCD_BLK_PIN                                     (DL_GPIO_PIN_26)
#define SPI_LCD_LCD_BLK_IOMUX                                    (IOMUX_PINCM57)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_LED_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_UART_DEBUG_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
