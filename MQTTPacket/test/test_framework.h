/*******************************************************************************
 * Copyright (c) 2023 Microsoft Corporation. All rights reserved.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 *******************************************************************************/

#ifndef TEST_FRAMEWORK_H_
#define TEST_FRAMEWORK_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if !defined(_WINDOWS)
	#include <sys/time.h>
  	#include <sys/socket.h>
	#include <unistd.h>
  	#include <errno.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#define MAXHOSTNAMELEN 256
#define EAGAIN WSAEWOULDBLOCK
#define EINTR WSAEINTR
#define EINPROGRESS WSAEINPROGRESS
#define EWOULDBLOCK WSAEWOULDBLOCK
#define ENOTCONN WSAENOTCONN
#define ECONNRESET WSAECONNRESET
#endif

#if defined(__cplusplus) /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

struct Options
{
	char* host;         /**< connection to system under test. */
	int port;
	int verbose;
	int test_no;
};

static struct Options options =
{
	"localhost",
	1883,
	0,
	0,
};

void usage();
void getopts(int argc, char** argv);

#define LOGA_DEBUG 0
#define LOGA_INFO 1
#include <stdarg.h>
#include <time.h>
#include <sys/timeb.h>
void MyLog(int LOGA_level, char* format, ...);

#if defined(WIN32) || defined(_WINDOWS)
#define mqsleep(A) Sleep(1000*A)
#define START_TIME_TYPE DWORD
static DWORD start_time = 0;
START_TIME_TYPE start_clock(void);
#elif defined(AIX)
#define mqsleep sleep
#define START_TIME_TYPE struct timespec
START_TIME_TYPE start_clock(void);
#else
#define mqsleep sleep
#define START_TIME_TYPE struct timeval
START_TIME_TYPE start_clock(void);
#endif

#if defined(WIN32)
long elapsed(START_TIME_TYPE start_time);
#elif defined(AIX)
#define assert(a)
long elapsed(struct timespec start);
#else
long elapsed(START_TIME_TYPE start_time);
#endif

#define assert(a, b, c, d) myassert(__FILE__, __LINE__, a, b, c, d)
#define assert1(a, b, c, d, e) myassert(__FILE__, __LINE__, a, b, c, d, e)

static int tests = 0;
static int failures = 0;
static FILE* xml;
static START_TIME_TYPE global_start_time;
static char output[3000];
static char* cur_output = output;

void write_test_result();

void myassert(char* filename, int lineno, char* description, int value, char* format, ...);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif // TEST_FRAMEWORK_H_
