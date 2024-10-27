#ifndef UART_LITE_H
#define UART_LITE_H

int uart0_cfg(int baudrate);
int uart0_send_string(const char *s);
int uart0_get_tx_busy();
int uart0_recv_string(char *txt);
int uart0_get_rx_busy();

#endif // UART_LITE_H
