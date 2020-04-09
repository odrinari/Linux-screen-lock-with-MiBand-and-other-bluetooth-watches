//
//  Intel Edison Playground
//  Copyright (c) 2015 Damian Kołakowski. All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:

//* Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.

//* Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.

//* Neither the name of intel-edison-playground nor the names of its
//  contributors may be used to endorse or promote products derived from
//  this software without specific prior written permission.

//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "daemon.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <searchtask.h>


struct hci_request ble_hci_request(uint16_t ocf, int clen, void * status, void * cparam)
{
    struct hci_request rq;
    memset(&rq, 0, sizeof(rq));
    rq.ogf = OGF_LE_CTL;
    rq.ocf = ocf;
    rq.cparam = cparam;
    rq.clen = clen;
    rq.rparam = status;
    rq.rlen = 1;
    return rq;
}

DiscoveryThread::DiscoveryThread(const char* mac){
    strcpy(searching_addr, mac);
}

void DiscoveryThread::run(){
    while (1) {
        int ret, status;

        // Get HCI device.

        const int device = hci_open_dev(hci_get_route(NULL));
        if ( device < 0 ) {
            perror("Failed to open HCI device.");
            //return 0;
        }

        // Set BLE scan parameters.

        le_set_scan_parameters_cp scan_params_cp;
        memset(&scan_params_cp, 0, sizeof(scan_params_cp));
        scan_params_cp.type 			= 0x00;
        scan_params_cp.interval 		= htobs(0x0010);
        scan_params_cp.window 			= htobs(0x0010);
        scan_params_cp.own_bdaddr_type 	= 0x00; // Public Device Address (default).
        scan_params_cp.filter 			= 0x00; // Accept all.

        struct hci_request scan_params_rq = ble_hci_request(OCF_LE_SET_SCAN_PARAMETERS, LE_SET_SCAN_PARAMETERS_CP_SIZE, &status, &scan_params_cp);

        ret = hci_send_req(device, &scan_params_rq, 1000);
        if ( ret < 0 ) {
            hci_close_dev(device);
            perror("Failed to set scan parameters data.");
            //return 0;
        }

        // Set BLE events report mask.

        le_set_event_mask_cp event_mask_cp;
        memset(&event_mask_cp, 0, sizeof(le_set_event_mask_cp));
        int i = 0;
        for ( i = 0 ; i < 8 ; i++ ) event_mask_cp.mask[i] = 0xFF;

        struct hci_request set_mask_rq = ble_hci_request(OCF_LE_SET_EVENT_MASK, LE_SET_EVENT_MASK_CP_SIZE, &status, &event_mask_cp);
        ret = hci_send_req(device, &set_mask_rq, 1000);
        if ( ret < 0 ) {
            hci_close_dev(device);
            perror("Failed to set event mask.");
            //return 0;
        }

        // Enable scanning.

        le_set_scan_enable_cp scan_cp;
        memset(&scan_cp, 0, sizeof(scan_cp));
        scan_cp.enable 		= 0x01;	// Enable flag.
        scan_cp.filter_dup 	= 0x00; // Filtering disabled.

        struct hci_request enable_adv_rq = ble_hci_request(OCF_LE_SET_SCAN_ENABLE, LE_SET_SCAN_ENABLE_CP_SIZE, &status, &scan_cp);

        ret = hci_send_req(device, &enable_adv_rq, 1000);
        if ( ret < 0 ) {
            hci_close_dev(device);
            perror("Failed to enable scan.");
            //return 0;
        }

        // Get Results.

        struct hci_filter nf;
        hci_filter_clear(&nf);
        hci_filter_set_ptype(HCI_EVENT_PKT, &nf);
        hci_filter_set_event(EVT_LE_META_EVENT, &nf);
        if ( setsockopt(device, SOL_HCI, HCI_FILTER, &nf, sizeof(nf)) < 0 ) {
            hci_close_dev(device);
            perror("Could not set socket options\n");
            //return 0;
        }

        //printf("Scanning....\n");

        uint8_t buf[HCI_MAX_EVENT_SIZE];
        evt_le_meta_event * meta_event;
        le_advertising_info * info;
        int len;

        while ( 1 ) {
            len = read(device, buf, sizeof(buf));
            if ( len >= HCI_EVENT_HDR_SIZE ) {
                meta_event = (evt_le_meta_event*)(buf+HCI_EVENT_HDR_SIZE+1);
                if ( meta_event->subevent == EVT_LE_ADVERTISING_REPORT ) {
                    uint8_t reports_count = meta_event->data[0];
                    void * offset = meta_event->data + 1;
                    while ( reports_count-- ) {
                        info = (le_advertising_info *)offset;
                        char addr[18];
                        ba2str(&(info->bdaddr), addr);
                                            if (strcmp(addr, searching_addr)==0) {
                                                emit newRes((char)info->data[info->length]);
                                            }
                        //printf("%s - RSSI %d\n", addr, (char)info->data[info->length]);
                        offset = info->data + info->length + 2;
                    }
                }
            }
        }

        // Disable scanning.

        memset(&scan_cp, 0, sizeof(scan_cp));
        scan_cp.enable = 0x00;	// Disable flag.

        struct hci_request disable_adv_rq = ble_hci_request(OCF_LE_SET_SCAN_ENABLE, LE_SET_SCAN_ENABLE_CP_SIZE, &status, &scan_cp);
        ret = hci_send_req(device, &disable_adv_rq, 1000);
        if ( ret < 0 ) {
            hci_close_dev(device);
            perror("Failed to disable scan.");
            //return 0;
        }

        hci_close_dev(device);

    }
}
