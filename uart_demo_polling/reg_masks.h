#ifndef REG_MASKS_H
#define REG_MASKS_H

#define PIN_PULL_UP     0x0U
#define PIN_REPEATER    0x1U
#define PIN_NO_PULL     0x2U
#define PIN_PULL_DOWN   0x3U


// Interrupt Register (IR):

#define MR0_INT   0x01U
#define MR1_INT   0x02U
#define MR2_INT   0x04U
#define MR3_INT   0x08U

#define CR0_INT   0x10U
#define CR1_INT   0x20U

#define CAP0RE    0x01U
#define CAP0FE    0x02U
#define CAP0I     0x04U

#define CAP1RE    0x08U
#define CAP1FE    0x10U
#define CAP1I     0x20U

// Timer Control Register (TCR):
#define CNT_EN    0x01U
#define CNT_RST   0x02U
#define PWM_EN    0x08U


// Match Control Register (MCR):

#define MR0I      0x0001U
#define MR0R      0x0002U
#define MR0S      0x0004U

#define MR1I      0x0008U
#define MR1R      0x0010U
#define MR1S      0x0020U

#define MR2I      0x0040U
#define MR2R      0x0080U
#define MR2S      0x0100U

#define MR3I      0x0200U
#define MR3R      0x0400U
#define MR3S      0x0800U

// External Match Registers (EMR):

#define EM0       0x0001U
#define EM1       0x0002U
#define EM2       0x0004U
#define EM3       0x0008U

#define EM_HOLD   0x0U
#define EM_CLEAR  0x1U
#define EM_SET    0x2U
#define EM_TOGGLE 0x3U
#define EM_MASK   0x3U

//--------------------------

// PWM Control Register (PCR)

#define PWMSEL2   0x0004U   // 2
#define PWMSEL3   0x0008U   // 3
#define PWMSEL4   0x0010U   // 4
#define PWMSEL5   0x0020U   // 5
#define PWMSEL6   0x0040U   // 6

#define PWMENA1   0x0200U   // 9
#define PWMENA2   0x0400U   // 10
#define PWMENA3   0x0800U   // 11
#define PWMENA4   0x1000U   // 12
#define PWMENA5   0x2000U   // 13
#define PWMENA6   0x4000U   // 14

// PWM Load Enable Register (LER)

#define M0LEN     0x0001U
#define M1LEN     0x0002U
#define M2LEN     0x0004U
#define M3LEN     0x0008U
#define M4LEN     0x0010U
#define M5LEN     0x0020U
#define M6LEN     0x0040U
#define M7LEN     0x0080U

#define PWM_EN    0x08U


// Power Control for Peripherals register (PCONP):

#define PCTIM0    0x00000002U   //  1
#define PCTIM1    0x00000004U   //  2
#define PCUART0   0x00000008U   //  3
#define PCUART1   0x00000010U   //  4

#define PCPWM1    0x00000040U   //  6
#define PCI2C0    0x00000080U   //  7
#define PCSPI     0x00000100U   //  8
#define PCRTC     0x00000200U   //  9
#define PCSSP1    0x00000400U   // 10

#define PCADC     0x00001000U   // 12   // <= !!!!
#define PCCAN1    0x00002000U   // 13
#define PCCAN2    0x00004000U   // 14
#define PCGPIO    0x00008000U   // 15
#define PCRIT     0x00010000U   // 16
#define PCMCPWM   0x00020000U   // 17
#define PCQEI     0x00040000U   // 18
#define PCI2C1    0x00080000U   // 19

#define PCSSP0    0x00200000U   // 21
#define PCTIM2    0x00400000U   // 22
#define PCTIM3    0x00800000U   // 23
#define PCUART2   0x01000000U   // 24
#define PCUART3   0x02000000U   // 25
#define PCI2C2    0x04000000U   // 26
#define PCI2S     0x08000000U   // 27

#define PCGPDMA   0x20000000U   // 29
#define PCENET    0x40000000U   // 30
#define PCUSB     0x80000000U   // 31


// External Interrupt Mode register (EXTMODE):
#define EXTMODE0  0x0001U   // 0
#define EXTMODE1  0x0002U   // 1
#define EXTMODE2  0x0004U   // 2
#define EXTMODE3  0x0008U   // 3

// External Interrupt Polarity register (EXTPOLAR):
#define EXTPOLAR0 0x0001U   // 0
#define EXTPOLAR1 0x0002U   // 1
#define EXTPOLAR2 0x0004U   // 2
#define EXTPOLAR4 0x0008U   // 3

// A/D Control Register (AD0CR):
#define ADC_CLK_DIV(x)  (((x) & 0xFF) << 8)
#define ADC_BURST_MODE  (1U << 16)
#define ADC_NOT_PDN     (1U << 21)  // power_down

//#define ADC_NO_START    (0x00 << 24)
//#define ADC_SW_START    (0x01 << 24)

#define ADC_NO_START      0x00000000U   //
#define ADC_SW_START      0x01000000U   // SW trigger
#define ADC_EINT0_START   0x02000000U   // HW trigger: EINT0
#define ADC_CAP0_1_START  0x03000000U   // HW trigger: CAP0_1
#define ADC_MAT0_1_START  0x04000000U   // HW trigger: MAT0_1
#define ADC_MAT0_3_START  0x05000000U   // HW trigger: MAT0_3
#define ADC_MAT1_0_START  0x06000000U   // HW trigger: MAT1_0
#define ADC_MAT1_1_START  0x07000000U   // HW trigger: MAT1_1
#define ADC_START_MASK    0x07000000U

#define ADC_NEG_EDGE      0x08000000U


// A/D Global Data Register (ADGDR):
#define ADC_GET_CHANNEL(x)  (((x) >> 24) & 0x7)
#define ADC_GET_RESULT(x)   (((x) >> 4) & 0xFFF)
#define ADC_OVR         (1U << 30)
#define ADC_DONE        (1U << 31)

// A/D Interrupt Enable register (ADINTEN):
#define ADINTEN0    0x0001U
#define ADINTEN1    0x0002U
#define ADINTEN2    0x0004U
#define ADINTEN3    0x0008U
#define ADINTEN4    0x0010U
#define ADINTEN5    0x0020U
#define ADINTEN6    0x0040U
#define ADINTEN7    0x0080U

#define ADGINTEN    0x0100U

// A/D Status register (ADSTAT):
#define ADC_DONE0   0x00001U
#define ADC_DONE1   0x00002U
#define ADC_DONE2   0x00004U
#define ADC_DONE3   0x00008U
#define ADC_DONE4   0x00010U
#define ADC_DONE5   0x00020U
#define ADC_DONE6   0x00040U
#define ADC_DONE7   0x00080U

#define ADC_OVR0    0x00100U
#define ADC_OVR1    0x00200U
#define ADC_OVR2    0x00400U
#define ADC_OVR3    0x00800U
#define ADC_OVR4    0x01000U
#define ADC_OVR5    0x02000U
#define ADC_OVR6    0x04000U
#define ADC_OVR7    0x08000U

#define ADC_ADINT   0x10000U


//--------------------------

// RTC Interrupt Location Register (ILR)
#define RTC_RTCCIF  0x0001U   // 0
#define RTC_RTCALF  0x0002U   // 1

// RTC Clock Control Register (CCR)
#define RTC_CLKEN   0x0001U   // 0
#define RTC_CTCRST  0x0002U   // 1
#define RTC_CCALEN  0x0010U   // 4


// RTC Counter Increment Interrupt Register (CIIR)
#define RTC_IMSEC   0x0001U   // 0
#define RTC_IMMIN   0x0002U   // 1
#define RTC_IMHOUR  0x0004U   // 2
#define RTC_IMDOM   0x0008U   // 3
#define RTC_IMDOW   0x0010U   // 4
#define RTC_IMDOY   0x0020U   // 5
#define RTC_IMMON   0x0040U   // 6
#define RTC_IMYEAR  0x0080U   // 7

// RTC Auxiliary control register (RTC_AUX)
#define RTC_OSCF    0x0010U   // 4


// RTC Auxiliary Enable register (RTC_AUXEN)
#define RTC_OSCFEN  0x0010U   // 4



//--------------------------

// Watchdog Mode register (WDMOD)
#define WDT_WDEN    0x0001U   // 0
#define WDT_WDRESET 0x0002U   // 1
#define WDT_WDTOF   0x0004U   // 2
#define WDT_WDINT   0x0008U   // 3

// Watchdog Timer Clock Source Selection register (WDCLKSEL)

#define WDT_WDSEL_IRC   0x0000U
#define WDT_WDSEL_PCLK  0x0001U
#define WDT_WDSEL_RTC   0x0002U

#define WDT_WDLOCK      0x80000000U


//--------------------------

#define PCLK_CCLK_DIV_4   0x0
#define PCLK_CCLK_DIV_1   0x1
#define PCLK_CCLK_DIV_2   0x2
#define PCLK_CCLK_DIV_8   0x3
#define PCLK_CCLK_MASK    0x3

#define PCLK_WDT_MASK       (PCLK_CCLK_MASK  << 00)           // Peripheral clock selection for the WDT
#define PCLK_WDT_DIV_4      (PCLK_CCLK_DIV_4 << 00)
#define PCLK_WDT_DIV_1      (PCLK_CCLK_DIV_1 << 00)
#define PCLK_WDT_DIV_2      (PCLK_CCLK_DIV_2 << 00)
#define PCLK_WDT_DIV_8      (PCLK_CCLK_DIV_8 << 00)

#define PCLK_TIMER0_MASK    (PCLK_CCLK_MASK  << 02)           // Peripheral clock selection for the TIMER0
#define PCLK_TIMER0_DIV_4   (PCLK_CCLK_DIV_4 << 02)
#define PCLK_TIMER0_DIV_1   (PCLK_CCLK_DIV_1 << 02)
#define PCLK_TIMER0_DIV_2   (PCLK_CCLK_DIV_2 << 02)
#define PCLK_TIMER0_DIV_8   (PCLK_CCLK_DIV_8 << 02)

#define PCLK_TIMER1_MASK    (PCLK_CCLK_MASK  << 04)           // Peripheral clock selection for the TIMER1
#define PCLK_TIMER1_DIV_4   (PCLK_CCLK_DIV_4 << 04)
#define PCLK_TIMER1_DIV_1   (PCLK_CCLK_DIV_1 << 04)
#define PCLK_TIMER1_DIV_2   (PCLK_CCLK_DIV_2 << 04)
#define PCLK_TIMER1_DIV_8   (PCLK_CCLK_DIV_8 << 04)

#define PCLK_UART0_MASK     (PCLK_CCLK_MASK  << 06)           // Peripheral clock selection for the UART0
#define PCLK_UART0_DIV_4    (PCLK_CCLK_DIV_4 << 06)
#define PCLK_UART0_DIV_1    (PCLK_CCLK_DIV_1 << 06)
#define PCLK_UART0_DIV_2    (PCLK_CCLK_DIV_2 << 06)
#define PCLK_UART0_DIV_8    (PCLK_CCLK_DIV_8 << 06)

#define PCLK_UART1_MASK     (PCLK_CCLK_MASK  << 08)           // Peripheral clock selection for the UART1
#define PCLK_UART1_DIV_4    (PCLK_CCLK_DIV_4 << 08)
#define PCLK_UART1_DIV_1    (PCLK_CCLK_DIV_1 << 08)
#define PCLK_UART1_DIV_2    (PCLK_CCLK_DIV_2 << 08)
#define PCLK_UART1_DIV_8    (PCLK_CCLK_DIV_8 << 08)

#define PCLK_PWM1_MASK      (PCLK_CCLK_MASK  << 12)           // Peripheral clock selection for the PWM1
#define PCLK_PWM1_DIV_4     (PCLK_CCLK_DIV_4 << 12)
#define PCLK_PWM1_DIV_1     (PCLK_CCLK_DIV_1 << 12)
#define PCLK_PWM1_DIV_2     (PCLK_CCLK_DIV_2 << 12)
#define PCLK_PWM1_DIV_8     (PCLK_CCLK_DIV_8 << 12)

#define PCLK_I2C0_MASK      (PCLK_CCLK_MASK  << 14)           // Peripheral clock selection for the I2C0
#define PCLK_I2C0_DIV_4     (PCLK_CCLK_DIV_4 << 14)
#define PCLK_I2C0_DIV_1     (PCLK_CCLK_DIV_1 << 14)
#define PCLK_I2C0_DIV_2     (PCLK_CCLK_DIV_2 << 14)
#define PCLK_I2C0_DIV_8     (PCLK_CCLK_DIV_8 << 14)

#define PCLK_SPI_MASK       (PCLK_CCLK_MASK  << 16)           // Peripheral clock selection for the SPI
#define PCLK_SPI_DIV_4      (PCLK_CCLK_DIV_4 << 16)
#define PCLK_SPI_DIV_1      (PCLK_CCLK_DIV_1 << 16)
#define PCLK_SPI_DIV_2      (PCLK_CCLK_DIV_2 << 16)
#define PCLK_SPI_DIV_8      (PCLK_CCLK_DIV_8 << 16)

#define PCLK_SSP1_MASK      (PCLK_CCLK_MASK  << 20)           // Peripheral clock selection for the SSP1
#define PCLK_SSP1_DIV_4     (PCLK_CCLK_DIV_4 << 20)
#define PCLK_SSP1_DIV_1     (PCLK_CCLK_DIV_1 << 20)
#define PCLK_SSP1_DIV_2     (PCLK_CCLK_DIV_2 << 20)
#define PCLK_SSP1_DIV_8     (PCLK_CCLK_DIV_8 << 20)

#define PCLK_DAC_MASK       (PCLK_CCLK_MASK  << 22)           // Peripheral clock selection for the DAC
#define PCLK_DAC_DIV_4      (PCLK_CCLK_DIV_4 << 22)
#define PCLK_DAC_DIV_1      (PCLK_CCLK_DIV_1 << 22)
#define PCLK_DAC_DIV_2      (PCLK_CCLK_DIV_2 << 22)
#define PCLK_DAC_DIV_8      (PCLK_CCLK_DIV_8 << 22)

#define PCLK_ADC_MASK       (PCLK_CCLK_MASK  << 24)           // Peripheral clock selection for the ADC
#define PCLK_ADC_DIV_4      (PCLK_CCLK_DIV_4 << 24)
#define PCLK_ADC_DIV_1      (PCLK_CCLK_DIV_1 << 24)
#define PCLK_ADC_DIV_2      (PCLK_CCLK_DIV_2 << 24)
#define PCLK_ADC_DIV_8      (PCLK_CCLK_DIV_8 << 24)

// For PCLKSEL1:

#define PCLK_QEI_MASK       0x00000003U   // Peripheral clock selection for the Quadrature Encoder Interface
#define PCLK_QEI_DIV_4      0x00000000U
#define PCLK_QEI_DIV_1      0x00000001U
#define PCLK_QEI_DIV_2      0x00000002U
#define PCLK_QEI_DIV_8      0x00000003U

#define PCLK_GPIOINT_MASK   0x0000000CU   // Peripheral clock selection for GPIO interrupts
#define PCLK_PCB_MASK       0x00000030U   // Peripheral clock selection for the Pin Connect block
#define PCLK_I2C1_MASK      0x000000C0U   // Peripheral clock selection for I2C1

#define PCLK_SSP0_MASK      (PCLK_CCLK_MASK  << 10)           // Peripheral clock selection for the SSP0
#define PCLK_SSP0_DIV_4     (PCLK_CCLK_DIV_4 << 10)
#define PCLK_SSP0_DIV_1     (PCLK_CCLK_DIV_1 << 10)
#define PCLK_SSP0_DIV_2     (PCLK_CCLK_DIV_2 << 10)
#define PCLK_SSP0_DIV_8     (PCLK_CCLK_DIV_8 << 10)


#define PCLK_TIMER2_MASK    0x00003000U   // Peripheral clock selection for TIMER2
#define PCLK_TIMER2_DIV_4   0x00000000U
#define PCLK_TIMER2_DIV_1   0x00001000U
#define PCLK_TIMER2_DIV_2   0x00002000U
#define PCLK_TIMER2_DIV_8   0x00003000U

#define PCLK_TIMER3_MASK    0x0000C000U   // Peripheral clock selection for TIMER3
#define PCLK_TIMER3_DIV_4   0x00000000U
#define PCLK_TIMER3_DIV_1   0x00004000U
#define PCLK_TIMER3_DIV_2   0x00008000U
#define PCLK_TIMER3_DIV_8   0x0000C000U

#define PCLK_UART2_MASK     0x00030000U   // Peripheral clock selection for UART2
#define PCLK_UART3_MASK     0x000C0000U   // Peripheral clock selection for UART3
#define PCLK_I2C2_MASK      0x00300000U   // Peripheral clock selection for I2C2
#define PCLK_I2S_MASK       0x00C00000U   // Peripheral clock selection for I2S

#define PCLK_RIT_MASK       0x0C000000U   // Peripheral clock selection for Repetitive Interrupt Timer
#define PCLK_SYSCON_MASK    0x30000000U   // Peripheral clock selection for the System Control block
#define PCLK_MC_MASK        0xC0000000U   // Peripheral clock selection for the Motor Control PWM

// QEI:

#define QEICON_RESP       0x01U
#define QEICON_RESPI      0x02U
#define QEICON_RESV       0x04U
#define QEICON_RESI       0x08U

#define QEICONF_DIRINV    0x01U
#define QEICONF_SIGMODE   0x02U
#define QEICONF_CAPMODE   0x04U
#define QEICONF_INVINX    0x08U

#define QEISTAT_DIR       0x01U

#define QEI_INX_INT       0x0001U
#define QEI_TIM_INT       0x0002U
#define QEI_VELC_INT      0x0004U
#define QEI_DIR_INT       0x0008U
#define QEI_ERR_INT       0x0010U
#define QEI_ENCLK_INT     0x0020U
#define QEI_POS0_INT      0x0040U
#define QEI_POS1_INT      0x0080U
#define QEI_POS2_INT      0x0100U
#define QEI_REV_INT       0x0200U
#define QEI_POS0REV_INT   0x0400U
#define QEI_POS1REV_INT   0x0800U
#define QEI_POS2REV_INT   0x1000U


// UART:

// UART: Line Control Register

#define CHAR_5_BITS       0x00
#define CHAR_6_BITS       0x01
#define CHAR_7_BITS       0x02
#define CHAR_8_BITS       0x03
#define STOP_1_BIT        0x00
#define STOP_2_BITS       0x04
#define PARITY_ENABLE     0x08
#define PARITY_EVEN       0x10
#define PARITY_ODD        0x00
#define BREAK_ENABLE      0x40
#define DLAB_ENABLE       0x80

// UART: Line Status Register

#define UART_RX_RDY       0x01    // 0
#define UART_OVR_ERR      0x02    // 1
#define UART_PAR_ERR      0x04    // 2
#define UART_FRAME_ERR    0x08    // 3
#define UART_BREAK_ERR    0x10    // 4
#define UART_TX_EMPTY     0x20    // 5
#define UART_TX_IDLE      0x40    // 6
#define UART_RX_FIFO_ERR  0x80    // 7

#define UART_TX_RDY       UART_TX_EMPTY

// UART: Interrupt Enable Register

#define UART_RBR_IE       0x01    // 0
#define UART_THRE_IE      0x02    // 1
#define UART_ERROR_IE     0x04    // 0

#define UART_RX_IE  UART_RBR_IE
#define UART_TX_IE  UART_THRE_IE

// UART: Interrupt Identification Register

#define GET_INT_ID(x) (((x) >> 1) & 0x7)

#define RLS_INT_ID      0x3   // Receive Line Status
#define RDA_INT_ID      0x2   // Receive Data Available
#define CTI_INT_ID      0x6   // Character Time-out Indicator
#define THRE_INT_ID     0x1   // Transmitter Holding Register Empty


#define UART_RX_IRQ   RDA_INT_ID
#define UART_TX_IRQ   THRE_INT_ID


// SPI MODULE:

// SPI Control Register
#define SPI_BIT_EN      0x04
#define SPI_CPHA        0x08    // Data is sampled on the second clock edge of the SCK
#define SPI_CPOL        0x10    // SCK is active low
#define SPI_MSTR        0x20    // The SPI operates in Master mode
#define SPI_LSBF        0x40    // SPI data is transferred LSB (bit 0) first
#define SPI_SPIE        0x80    // Interrupt generated each time the SPIF or MODF bits are activated

#define SPI_FRAME_8     0x8   //  8 bits per transfer
#define SPI_FRAME_9     0x9   //  9 bits per transfer
#define SPI_FRAME_10    0xA   // 10 bits per transfer
#define SPI_FRAME_11    0xB   // 11 bits per transfer
#define SPI_FRAME_12    0xC   // 12 bits per transfer
#define SPI_FRAME_13    0xD   // 13 bits per transfer
#define SPI_FRAME_14    0xE   // 14 bits per transfer
#define SPI_FRAME_15    0xF   // 15 bits per transfer
#define SPI_FRAME_16    0x0   // 16 bits per transfer

// SPI Status Register
#define SPI_ABRT        0x08  // Slave abort
#define SPI_MODF        0x10  // Mode fault
#define SPI_ROVR        0x20  // Read overrun
#define SPI_WCOL        0x40  // Write collision
#define SPI_SPIF        0x80  // SPI transfer complete flag

// SSP MODULE:

// SSPn Control Register 0
#define SSP_DDS_4     3       // 4-bit transfer
#define SSP_DDS_5     4       // 5-bit transfer
#define SSP_DDS_6     5       // 6-bit transfer
#define SSP_DDS_7     6       // 7-bit transfer
#define SSP_DDS_8     7       // 8-bit transfer
#define SSP_DDS_9     8       // 9-bit transfer
#define SSP_DDS_10    9       // 10-bit transfer
#define SSP_DDS_11    10      // 11-bit transfer
#define SSP_DDS_12    11      // 12-bit transfer
#define SSP_DDS_13    12      // 13-bit transfer
#define SSP_DDS_14    13      // 14-bit transfer
#define SSP_DDS_15    14      // 15-bit transfer
#define SSP_DDS_16    15      // 16-bit transfer

#define SSP_FRF_SPI   0x0000
#define SSP_FRF_TI    0x0010
#define SSP_FRF_MW    0x0020  // Microwire

#define SSP_CPOL      0x0040
#define SSP_CPHA      0x0080

#define SSP_SET_SCR(x) (((x) & 0xFF) << 8)

// SSPn Control Register 1
#define SSP_LMB       0x0001  // Loop Back Mode
#define SSP_ENABLE    0x0002  // SSP Enable
#define SSP_SLAVE     0x0004  // Master/Slave Mode
#define SSP_SOD       0x0008  // Slave Output Disable

// SSPn Status Register
#define SSP_TFE       0x0001  // Transmit FIFO Empty
#define SSP_TNF       0x0002  // Transmit FIFO Not Full
#define SSP_RNE       0x0004  // Receive FIFO Not Empty
#define SSP_RFF       0x0008  // Receive FIFO Full
#define SSP_BSY       0x0010  // Busy


#endif // REG_MASKS_H

