#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "serialPort.h"
#include "hdlc.h"
#include "SC14CVMDECT.h"

serialPort_init_t SC14CVMDECT;
serialPort_rxCallback_t rxcallback;

hdlc_parsedPacketFct_t hdlc_parsedPacketFct;

#define PRINT(a, n) do {        \
    int i;                      \
    for (i = 0; i<n; i++) {     \
        printf("%02X ", a[i]);  \
    }                           \
} while(0)

#define CLEARBUF(a, n) do {     \
    int i;                      \
    for (i = 0; i<n; i++) {     \
        a[i] = 0;               \
    }                           \
} while(0)



int ReadOnce(uint8_t *buffer, uint16_t buffersize){
    int ret = serialPort_readBlocking(buffer, buffersize);
    if(ret <= 0){
        return -1;
    }
    else{
        return 0;
    }
}

hdlc_sendSerialFct_t SendSerial(uint8_t *data, uint16_t length){
    hdlc_sendSerialFct_t sendSerialFct;
    if(serialPort_writeBlocking(data, length) < 0){
        printf("write failed~");
    }
    else{
        printf("%.02X ", data);
        printf("%i\n", length);
    }
    return sendSerialFct;
}

hdlc_allocateMemoryFct_t memalloc(uint8_t **data, uint16_t length){
    hdlc_allocateMemoryFct_t allocateMemoryFct;
    *data = (uint8_t *)malloc(length * sizeof(uint8_t));
    return allocateMemoryFct;
}

hdlc_freeMemoryFct_t freeMemory(uint8_t *data){
    hdlc_freeMemoryFct_t freeMemoryFct;
    free(data);
    return freeMemoryFct;
}

hdlc_freeMemoryFct_t parsedPacket(uint8_t *data, uint16_t length) {
    hdlc_freeMemoryFct_t parsedPacketFct;
    // Handle the parsed HDLC packet
    printf("parsed packet function called");
    return parsedPacketFct;
}

int main() {
    SC14CVMDECT.portname = "/dev/ttyUSB0";
    SC14CVMDECT.baudrate = E_SERIAL_PORT_BAUDRATE_115200;
    SC14CVMDECT.rxBytesBlock = 8;

    uint8_t SyncPacket[5] = {'\x10','\x00','\x01','\xC8','\xC8'};

    uint8_t payload[] = {'\x59', '\x06', '\x01', '\xFF','\x48','\x1E', '\x00', '\x02', '\x18', '\x6'};
    uint16_t payloadlength = sizeof(payload);
    uint8_t framebuf[80];
    uint16_t framebufsize = sizeof(framebuf);

    uint8_t buf[8];
    uint16_t buffersize = sizeof(buf);

    printf("Opening serial port..\n");
    if(serialPort_open(&SC14CVMDECT, rxcallback) < 0){
        return -1;
    }
    printf("Serial connected.\n");
    printf("Sending sync packet.\n");
    int ret = serialPort_writeBlocking(SyncPacket, sizeof(SyncPacket));
    printf("%i\n", ret);


    printf("Initializing HDLC module..\n");
    sleep(1);
    hdlc_init((hdlc_parsedPacketFct_t)parsedPacket, (hdlc_allocateMemoryFct_t)memalloc, (hdlc_freeMemoryFct_t)freeMemory, (hdlc_sendSerialFct_t)SendSerial);
    sleep(1);
    hdlc_infoFrameCreate(0, payload, payloadlength);

    //PRINT(framebuf,framebufsize);

    //ReadOnce(buf, buffersize);
    //PRINT(buf, buffersize);

    printf("\nClosing serial connection.\n");
    serialPort_close();

    return 0;
}