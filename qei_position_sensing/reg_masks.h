#ifndef REG_MASKS_H
#define REG_MASKS_H

// Interrupt Register (IR):

#define MR0_INT 0x01
#define MR1_INT 0x02
#define MR2_INT 0x04
#define MR3_INT 0x08

#define CR0_INT 0x10
#define CR1_INT 0x20

#define CAP0RE  0x01
#define CAP0FE  0x02
#define CAP0I   0x04

#define CAP1RE  0x08
#define CAP1FE  0x10
#define CAP1I   0x20

// Timer Control Register (TCR):
#define CNT_EN    0x01
#define CNT_RST   0x02
#define PWM_EN    0x08


// Match Control Register (MCR):

#define MR0I    0x0001
#define MR0R    0x0002
#define MR0S    0x0004

#define MR1I    0x0008
#define MR1R    0x0010
#define MR1S    0x0020

#define MR2I    0x0040
#define MR2R    0x0080
#define MR2S    0x0100

#define MR3I    0x0200
#define MR3R    0x0400
#define MR3S    0x0800

//--------------------------

// PWM Control Register (PCR)

#define PWMSEL2 0x0004 // 2
#define PWMSEL3 0x0008 // 3
#define PWMSEL4 0x0010 // 4
#define PWMSEL5 0x0020 // 5
#define PWMSEL6 0x0040 // 6

#define PWMENA1 0x0200 // 9
#define PWMENA2 0x0400 // 10
#define PWMENA3 0x0800 // 11
#define PWMENA4 0x1000 // 12
#define PWMENA5 0x2000 // 13
#define PWMENA6 0x4000 // 14

// PWM Load Enable Register (LER)

#define M0LEN 0x0001
#define M1LEN 0x0002
#define M2LEN 0x0004
#define M3LEN 0x0008
#define M4LEN 0x0010
#define M5LEN 0x0020
#define M6LEN 0x0040
#define M7LEN 0x0080

#define PWM_EN  0x08


// Power Control for Peripherals register (PCONP):

#define PCTIM0     0x00000002 //  1
#define PCTIM1     0x00000004 //  2
#define PCUART0    0x00000008 //  3
#define PCUART1    0x00000010 //  4

#define PCPWM1     0x00000040 //  6
#define PCI2C0     0x00000080 //  7
#define PCSPI      0x00000100 //  8
#define PCRTC      0x00000200 //  9
#define PCSSP1     0x00000400 // 10

#define PCADC      0x00001000 // 12   // <= !!!!
#define PCCAN1     0x00002000 // 13
#define PCCAN2     0x00004000 // 14
#define PCGPIO     0x00008000 // 15
#define PCRIT      0x00010000 // 16
#define PCMCPWM    0x00020000 // 17
#define PCQEI      0x00040000 // 18
#define PCI2C1     0x00080000 // 19

#define PCSSP0     0x00200000 // 21
#define PCTIM2     0x00400000 // 22
#define PCTIM3     0x00800000 // 23
#define PCUART2    0x01000000 // 24
#define PCUART3    0x02000000 // 25
#define PCI2C2     0x04000000 // 26
#define PCI2S      0x08000000 // 27

#define PCGPDMA    0x20000000 // 29
#define PCENET     0x40000000 // 30
#define PCUSB      0x80000000 // 31


// External Interrupt Mode register (EXTMODE):
#define EXTMODE0  0x0001  // 0
#define EXTMODE1  0x0002  // 1
#define EXTMODE2  0x0004  // 2
#define EXTMODE3  0x0008  // 3

// External Interrupt Polarity register (EXTPOLAR):
#define EXTPOLAR0 0x0001  // 0
#define EXTPOLAR1 0x0002  // 1
#define EXTPOLAR2 0x0004  // 2
#define EXTPOLAR4 0x0008  // 3

// A/D Control Register (AD0CR):
#define ADC_CLK_DIV(x)  (((x) & 0xFF) << 8)
#define ADC_BURST_MODE  (1U << 16)
#define ADC_NOT_PDN     (1U << 21)  // power_down

#define ADC_NO_START    (0x00 << 24)
#define ADC_SW_START    (0x01 << 24)
#define ADC_EDGE        (1U << 27)

#define ADC_START_MASK  (0x7U << 24)

// A/D Global Data Register (ADGDR):
#define ADC_GET_CHANNEL(x)  (((x) >> 24) & 0x7)
#define ADC_GET_RESULT(x)   (((x) >> 4) & 0xFFF)
#define ADC_OVR         (1U << 30)
#define ADC_DONE        (1U << 31)

// A/D Interrupt Enable register (ADINTEN):
#define ADINTEN0  0x0001U
#define ADINTEN1  0x0002U
#define ADINTEN2  0x0004U
#define ADINTEN3  0x0008U
#define ADINTEN4  0x0010U
#define ADINTEN5  0x0020U
#define ADINTEN6  0x0040U
#define ADINTEN7  0x0080U

#define ADGINTEN  0x0100U

// A/D Status register (ADSTAT):
#define ADC_DONE0   0x00001
#define ADC_DONE1   0x00002
#define ADC_DONE2   0x00004
#define ADC_DONE3   0x00008
#define ADC_DONE4   0x00010
#define ADC_DONE5   0x00020
#define ADC_DONE6   0x00040
#define ADC_DONE7   0x00080

#define ADC_OVR0    0x00100
#define ADC_OVR1    0x00200
#define ADC_OVR2    0x00400
#define ADC_OVR3    0x00800
#define ADC_OVR4    0x01000
#define ADC_OVR5    0x02000
#define ADC_OVR6    0x04000
#define ADC_OVR7    0x08000

#define ADC_ADINT   0x10000


//--------------------------

// RTC Interrupt Location Register (ILR)
#define RTC_RTCCIF  0x0001  // 0
#define RTC_RTCALF  0x0002  // 1

// RTC Clock Control Register (CCR)
#define RTC_CLKEN   0x0001  // 0
#define RTC_CTCRST  0x0002  // 1
#define RTC_CCALEN  0x0010  // 4


// RTC Counter Increment Interrupt Register (CIIR)
#define RTC_IMSEC   0x0001     // 0
#define RTC_IMMIN   0x0002     // 1
#define RTC_IMHOUR  0x0004     // 2
#define RTC_IMDOM   0x0008     // 3
#define RTC_IMDOW   0x0010     // 4
#define RTC_IMDOY   0x0020     // 5
#define RTC_IMMON   0x0040     // 6
#define RTC_IMYEAR  0x0080     // 7

// RTC Auxiliary control register (RTC_AUX)
#define RTC_OSCF    0x0010     // 4


// RTC Auxiliary Enable register (RTC_AUXEN)
#define RTC_OSCFEN  0x0010     // 4



//--------------------------

// Watchdog Mode register (WDMOD)
#define WDT_WDEN    0x0001     // 0
#define WDT_WDRESET 0x0002     // 1
#define WDT_WDTOF   0x0004     // 2
#define WDT_WDINT   0x0008     // 3

// Watchdog Timer Clock Source Selection register (WDCLKSEL)

#define WDT_WDSEL_IRC   0x0000
#define WDT_WDSEL_PCLK  0x0001
#define WDT_WDSEL_RTC   0x0002

#define WDT_WDLOCK      0x80000000


//--------------------------

#define PCLK_QEI_MASK       0x00000003   // Peripheral clock selection for the Quadrature Encoder Interface
#define PCLK_QEI_DIV_4      0x00000000
#define PCLK_QEI_DIV_1      0x00000001
#define PCLK_QEI_DIV_2      0x00000002
#define PCLK_QEI_DIV_8      0x00000003

#define PCLK_GPIOINT_MASK   0x0000000C   // Peripheral clock selection for GPIO interrupts
#define PCLK_PCB_MASK       0x00000030   // Peripheral clock selection for the Pin Connect block
#define PCLK_I2C1_MASK      0x000000C0   // Peripheral clock selection for I2C1

#define PCLK_SSP0_MASK      0x00000C00   // Peripheral clock selection for SSP0
#define PCLK_TIMER2_MASK    0x00003000   // Peripheral clock selection for TIMER2
#define PCLK_TIMER3_MASK    0x0000C000   // Peripheral clock selection for TIMER3

#define PCLK_UART2_MASK     0x00030000   // Peripheral clock selection for UART2
#define PCLK_UART3_MASK     0x000C0000   // Peripheral clock selection for UART3
#define PCLK_I2C2_MASK      0x00300000   // Peripheral clock selection for I2C2
#define PCLK_I2S_MASK       0x00C00000   // Peripheral clock selection for I2S

#define PCLK_RIT_MASK       0x0C000000   // Peripheral clock selection for Repetitive Interrupt Timer
#define PCLK_SYSCON_MASK    0x30000000   // Peripheral clock selection for the System Control block
#define PCLK_MC_MASK        0xC0000000   // Peripheral clock selection for the Motor Control PWM

// QEI:

#define QEICON_RESP       0x01
#define QEICON_RESPI      0x02
#define QEICON_RESV       0x04
#define QEICON_RESI       0x08

#define QEICONF_DIRINV    0x01
#define QEICONF_SIGMODE   0x02
#define QEICONF_CAPMODE   0x04
#define QEICONF_INVINX    0x08

#define QEISTAT_DIR       0x01

#define QEI_INX_INT       0x0001
#define QEI_TIM_INT       0x0002
#define QEI_VELC_INT      0x0004
#define QEI_DIR_INT       0x0008
#define QEI_ERR_INT       0x0010
#define QEI_ENCLK_INT     0x0020
#define QEI_POS0_INT      0x0040
#define QEI_POS1_INT      0x0080
#define QEI_POS2_INT      0x0100
#define QEI_REV_INT       0x0200
#define QEI_POS0REV_INT   0x0400
#define QEI_POS1REV_INT   0x0800
#define QEI_POS2REV_INT   0x1000




#endif // REG_MASKS_H
