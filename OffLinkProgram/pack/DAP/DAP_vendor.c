/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 * Copyright 2019, Cypress Semiconductor Corporation 
 * or a subsidiary of Cypress Semiconductor Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ----------------------------------------------------------------------
 *
 * $Date:        20. May 2015
 * $Revision:    V1.10
 *
 * Project:      CMSIS-DAP Source
 * Title:        DAP_vendor.c CMSIS-DAP Vendor Commands
 *
 *---------------------------------------------------------------------------*/

#include "DAP_config.h"
#include "DAP.h"
#include <string.h>


#ifdef DRAG_N_DROP_SUPPORT
#include "file_stream.h"
#endif

//**************************************************************************************************
/**
\defgroup DAP_Vendor_Adapt_gr Adapt Vendor Commands
\ingroup DAP_Vendor_gr
@{

The file DAP_vendor.c provides template source code for extension of a Debug Unit with
Vendor Commands. Copy this file to the project folder of the Debug Unit and add the
file to the MDK-ARM project under the file group Configuration.
*/

/** Process DAP Vendor Command and prepare Response Data
\param request   pointer to request data
\param response  pointer to response data
\return          number of bytes in response (lower 16 bits)
                 number of bytes in request (upper 16 bits)
*/
uint32_t DAP_ProcessVendorCommand(const uint8_t *request, uint8_t *response) {
  uint32_t num = (1U << 16) | 1U;

  *response++ = *request;        // copy Command ID

  switch (*request++) {          // first byte in request is Command ID
    case ID_DAP_Vendor0: {

        break;
    }
    case ID_DAP_Vendor2: {

        break;
    }
    case ID_DAP_Vendor3:  {

        break;
    }
    case ID_DAP_Vendor4:  {

        break;
    }
    case ID_DAP_Vendor5:  break;
    case ID_DAP_Vendor6:  break;
    case ID_DAP_Vendor7:  break;
    case ID_DAP_Vendor8: {
        break;
    }
    case ID_DAP_Vendor9: {
        break;
    }
#ifdef DRAG_N_DROP_SUPPORT
    case ID_DAP_Vendor10: {
        // open mass storage device stream
        *response = stream_open((stream_type_t)(*request));
        num += (1 << 16) | 1;
        break;
    }
    case ID_DAP_Vendor11: {
        // close mass storage device stream
        *response = stream_close();
        num += 1;
        break;
    }
    case ID_DAP_Vendor12: {
        // write to mass storage device
        uint32_t write_len = *request;
        request++;
        main_blink_msc_led(MAIN_LED_FLASH);
        *response = stream_write((uint8_t *)request, write_len);
        num += ((write_len + 1) << 16) | 1;
        break;
    }
#endif
    case ID_DAP_Vendor13: {
        break;
    }
    case ID_DAP_Vendor14: break;
    case ID_DAP_Vendor15: break;
    case ID_DAP_Vendor16: break;
    case ID_DAP_Vendor17: break;
    case ID_DAP_Vendor18: break;
    case ID_DAP_Vendor19: break;
    case ID_DAP_Vendor20: break;
    case ID_DAP_Vendor21: break;
    case ID_DAP_Vendor22: break;
    case ID_DAP_Vendor23: break;
    case ID_DAP_Vendor24: break;
    case ID_DAP_Vendor25: break;
    case ID_DAP_Vendor26: break;
    case ID_DAP_Vendor27: break;
    case ID_DAP_Vendor28: break;
    case ID_DAP_Vendor29: break;
    case ID_DAP_Vendor30: break;
    case ID_DAP_Vendor31: break;
    default: break;
  }

  return (num);
}

///@}
