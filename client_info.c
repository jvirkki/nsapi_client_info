/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
 * 
 * Copyright 2013 Jyri J. Virkki. All rights reserved.
 * 
 * THE BSD LICENSE
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * 
 * Neither the name of the  nor the names of its contributors may be
 * used to endorse or promote products derived from this software without 
 * specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER 
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <nsapi.h>


/**
 * Service SAF to write out the client IP address, nothing more.
 *
 */
NSAPI_PUBLIC int print_client_info(pblock * pb, Session * sn, Request * rq)
{
  int rv;
  char * ip = pblock_findval("ip", sn->client);

  protocol_status(sn, rq, PROTOCOL_OK, NULL);

  param_free(pblock_remove("content-type", rq->srvhdrs));
  pblock_nvinsert("content-type", "text/plain", rq->srvhdrs); 

  rv = protocol_start_response(sn, rq);
  if (rv == REQ_NOACTION) { /* HTTP HEAD instead of GET */
    return REQ_PROCEED;
  }

  if (rv == REQ_ABORTED) {
    protocol_status(sn, rq, PROTOCOL_SERVER_ERROR, NULL);
    return rv;
  }

  rv = net_write(sn->csd, ip, strlen(ip));
  if (rv == IO_ERROR) {
    return REQ_EXIT;
  }

  return REQ_PROCEED;
}
