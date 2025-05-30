/*
Copyright (c) 2021, Kazuhisa Yokota, JN1DFF
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the names of its contributors 
  may be used to endorse or promote products derived from this software 
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "tnc.h"
#include "unproto.h"
#include "tty.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pico/stdlib.h"

//#include "timer.h"
#include "tnc.h"
#include "bell202.h"
#include "ax25.h"
#include "usb_output.h"
#include "tty.h"
#include "digipeat.h"
#include "kiss.h"

static uint32_t beacon_time = 0;

void beacon_reset(void)
{
    beacon_time = tnc_time();
}

void beacon(void)
{
    if (!param.beacon) return;

    if (tnc_time() - beacon_time < param.beacon * 60 * 100) return; // convert minutes to 10 ms

    send_unproto(&tnc[BEACON_PORT], param.btext, strlen(param.btext));
  //--------------------
    char combined[256];
    int offset = snprintf(combined, sizeof(combined), "%s>", param.mycall);

    // Add comma-separated unproto paths
    for (int i = 0; i < 4 && param.unproto[i] != '\0'; i++) {
        if (i > 0) {
            offset += snprintf(combined + offset, sizeof(combined) - offset, ",");
        }
        offset += snprintf(combined + offset, sizeof(combined) - offset, "%s", param.unproto[i]);
    }

    // Add colon and btext
    snprintf(combined + offset, sizeof(combined) - offset, ":%s", param.btext);
//--------------------
    tty_write_str(ttyp, combined);
    beacon_time = tnc_time();
}
