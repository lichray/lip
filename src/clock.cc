/*-
 * Copyright (c) 2018 Zhihao Yuan.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <lip/lip.h>

#ifndef _WIN32
#include <time.h>
#else
#include <Windows.h>
#include <WinBase.h>
#pragma comment(lib, "kernel32")
#endif

namespace lip
{

template <>
archive_clock::time_point archive_clock::from(timespec const& ts) noexcept
{
	return archive_clock::time_point(archive_clock::duration{
	    archive_clock::rep{ ts.tv_sec } * 10000000 +
	    INT64_C(116444736000000000) + ts.tv_nsec / 100 });
}

#ifdef _WIN32
template <>
archive_clock::time_point archive_clock::from(FILETIME const& ts) noexcept
{
	return archive_clock::time_point(archive_clock::duration{
	    (archive_clock::rep{ ts.dwHighDateTime } << 32) ^
	    ts.dwLowDateTime });
}
#endif

archive_clock::time_point archive_clock::now() noexcept
{
#ifndef _WIN32
	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
#else
	FILETIME ts;
	GetSystemTimePreciseAsFileTime(&ts);
#endif
	return archive_clock::from(ts);
}

}
