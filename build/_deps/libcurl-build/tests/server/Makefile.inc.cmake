#***************************************************************************
#                                  _   _ ____  _
#  Project                     ___| | | |  _ \| |
#                             / __| | | | |_) | |
#                            | (__| |_| |  _ <| |___
#                             \___|\___/|_| \_\_____|
#
# Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
#
# This software is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at https://curl.se/docs/copyright.html.
#
# You may opt to use, copy, modify, merge, publish, distribute and/or sell
# copies of the Software, and permit persons to whom the Software is
# furnished to do so, under the terms of the COPYING file.
#
# This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
# KIND, either express or implied.
#
# SPDX-License-Identifier: curl
#
###########################################################################

SET(noinst_PROGRAMS getpart resolve rtspd sockfilt sws tftpd fake_ntlm    
 socksd disabled mqttd)

SET(CURLX_SRCS 
 ../../lib/mprintf.c 
 ../../lib/nonblock.c 
 ../../lib/strtoofft.c 
 ../../lib/warnless.c 
 ../../lib/timediff.c 
 ../../lib/dynbuf.c 
 ../../lib/strdup.c 
 ../../lib/strcase.c 
 ../../lib/curl_multibyte.c)

SET(CURLX_HDRS 
 ../../lib/curlx.h 
 ../../lib/nonblock.h 
 ../../lib/strtoofft.h 
 ../../lib/warnless.h 
 ../../lib/timediff.h 
 ../../lib/curl_ctype.h 
 ../../lib/dynbuf.h 
 ../../lib/strdup.h 
 ../../lib/curl_multibyte.h)

SET(USEFUL 
 getpart.c 
 getpart.h 
 server_setup.h 
 ../../lib/base64.c 
 ../../lib/curl_base64.h 
 ../../lib/memdebug.c 
 ../../lib/memdebug.h)

SET(UTIL 
 util.c 
 util.h)

SET(getpart_SOURCES ${CURLX_SRCS} ${CURLX_HDRS} ${USEFUL} 
 testpart.c)
SET(getpart_LDADD ${CURL_NETWORK_AND_TIME_LIBS})
SET(getpart_CFLAGS ${AM_CFLAGS})

SET(resolve_SOURCES ${CURLX_SRCS} ${CURLX_HDRS} ${USEFUL} ${UTIL} 
 resolve.c)
SET(resolve_LDADD ${CURL_NETWORK_AND_TIME_LIBS})
SET(resolve_CFLAGS ${AM_CFLAGS})

SET(rtspd_SOURCES ${CURLX_SRCS} ${CURLX_HDRS} ${USEFUL} ${UTIL} 
 server_sockaddr.h 
 rtspd.c)
SET(rtspd_LDADD ${CURL_NETWORK_AND_TIME_LIBS})
SET(rtspd_CFLAGS ${AM_CFLAGS})

SET(sockfilt_SOURCES ${CURLX_SRCS} ${CURLX_HDRS} ${USEFUL} ${UTIL} 
 server_sockaddr.h 
 sockfilt.c 
 ../../lib/inet_pton.c)
SET(sockfilt_LDADD ${CURL_NETWORK_AND_TIME_LIBS})
SET(sockfilt_CFLAGS ${AM_CFLAGS})

SET(socksd_SOURCES ${CURLX_SRCS} ${CURLX_HDRS} ${USEFUL} ${UTIL} 
 server_sockaddr.h socksd.c 
 ../../lib/inet_pton.c)
SET(socksd_LDADD ${CURL_NETWORK_AND_TIME_LIBS})
SET(socksd_CFLAGS ${AM_CFLAGS})

SET(mqttd_SOURCES ${CURLX_SRCS} ${CURLX_HDRS} ${USEFUL} ${UTIL} 
 server_sockaddr.h mqttd.c 
 ../../lib/inet_pton.c)
SET(mqttd_LDADD ${CURL_NETWORK_AND_TIME_LIBS})
SET(mqttd_CFLAGS ${AM_CFLAGS})

SET(sws_SOURCES ${CURLX_SRCS} ${CURLX_HDRS} ${USEFUL} ${UTIL} 
 server_sockaddr.h 
 sws.c 
 ../../lib/inet_pton.c)
SET(sws_LDADD ${CURL_NETWORK_AND_TIME_LIBS})
SET(sws_CFLAGS ${AM_CFLAGS})

SET(tftpd_SOURCES ${CURLX_SRCS} ${CURLX_HDRS} ${USEFUL} ${UTIL} 
 server_sockaddr.h 
 tftpd.c 
 tftp.h)
SET(tftpd_LDADD ${CURL_NETWORK_AND_TIME_LIBS})
SET(tftpd_CFLAGS ${AM_CFLAGS})

SET(fake_ntlm_SOURCES ${CURLX_SRCS} ${CURLX_HDRS} ${USEFUL} ${UTIL} 
 fake_ntlm.c)
SET(fake_ntlm_LDADD ${CURL_NETWORK_AND_TIME_LIBS})
SET(fake_ntlm_CFLAGS ${AM_CFLAGS})

SET(disabled_SOURCES disabled.c)
