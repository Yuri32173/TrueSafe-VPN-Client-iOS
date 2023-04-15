/*
 * Copyright (c) 2006 Paolo Abeni (Italy)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote 
 * products derived from this software without specific prior written 
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Basic USB data struct
 * By Paolo Abeni <paolo.abeni@email.it>
 *
 * @(#) $Header: /tcpdump/master/libpcap/pcap/usb.h,v 1.9 2008-12-23 20:13:29 guy Exp $
 */
 
#ifndef PCAP_USB_STRUCTS_H
#define PCAP_USB_STRUCTS_H

#include <cstdint>

constexpr uint8_t URB_TRANSFER_IN = 0x80;
constexpr uint8_t URB_ISOCHRONOUS = 0x0;
constexpr uint8_t URB_INTERRUPT = 0x1;
constexpr uint8_t URB_CONTROL = 0x2;
constexpr uint8_t URB_BULK = 0x3;

constexpr uint8_t URB_SUBMIT = 'S';
constexpr uint8_t URB_COMPLETE = 'C';
constexpr uint8_t URB_ERROR = 'E';

struct PcapUsbSetup {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
};

struct IsoRec {
    int32_t error_count;
    int32_t numdesc;
};

struct PcapUsbHeader {
    uint64_t id;
    uint8_t event_type;
    uint8_t transfer_type;
    uint8_t endpoint_number;
    uint8_t device_address;
    uint16_t bus_id;
    char setup_flag; // if !=0 the urb setup header is not present
    char data_flag; // if !=0 no urb data is present
    int64_t ts_sec;
    int32_t ts_usec;
    int32_t status;
    uint32_t urb_len;
    uint32_t data_len; // amount of urb data really present in this event
    PcapUsbSetup setup;
};

struct PcapUsbHeaderMmapped {
    uint64_t id;
    uint8_t event_type;
    uint8_t transfer_type;
    uint8_t endpoint_number;
    uint8_t device_address;
    uint16_t bus_id;
    char setup_flag; // if !=0 the urb setup header is not present
    char data_flag; // if !=0 no urb data is present
    int64_t ts_sec;
    int32_t ts_usec;
    int32_t status;
    uint32_t urb_len;
    uint32_t data_len; // amount of urb data really present in this event
    union {
        PcapUsbSetup setup;
        IsoRec iso;
    } s;
    int32_t interval; // for Interrupt and Isochronous events
    int32_t start_frame; // for Isochronous events
    uint32_t xfer_flags; // copy of URB's transfer flags
    uint32_t ndesc; // number of isochronous descriptors
};

struct UsbIsoDesc {
    int32_t status;
    uint32_t offset;
    uint32_t len;
    uint8_t pad[4];
};

#endif // PCAP_USB_STRUCTS_H

