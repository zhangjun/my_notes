/* Copyright (C) 1991-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <unistd.h>

//#include <alloca.h>

// glibc/sysdeps/posix/readv.c
#include "readv.h"

//  add
#define __set_errno(val) (errno = (val))

static void ifree(char **ptrp) { free(*ptrp); }

/* Read data from file descriptor FD, and put the result in the
   buffers described by VECTOR, which is a vector of COUNT 'struct iovec's.
   The buffers are filled in the order specified.
   Operates just like 'read' (see <unistd.h>) except that data are
   put in VECTOR instead of a contiguous buffer.  */
ssize_t my_readv(int fd, const struct iovec *vector, int count) {
  /* Find the total number of bytes to be read.  */
  size_t bytes = 0;
  for (int i = 0; i < count; ++i) {
    /* Check for ssize_t overflow.  */
    if (SSIZE_MAX - bytes < vector[i].iov_len) {
      __set_errno(EINVAL);
      return -1;
    }
    bytes += vector[i].iov_len;
  }

  /* Allocate a temporary buffer to hold the data.  We should normally
     use alloca since it's faster and does not require synchronization
     with other threads.  But we cannot if the amount of memory
     required is too large.  */
  char *buffer;
  char *malloced_buffer __attribute__((__cleanup__(ifree))) = NULL;
  if (bytes < 128)
    buffer = (char *)alloca(bytes);
  else {
    malloced_buffer = buffer = (char *)malloc(bytes);
    if (buffer == NULL)
      return -1;
  }

  /* Read the data.  */
  // ssize_t bytes_read = read (fd, buffer, bytes);

  // recv loop
  int bytes_read = 0, ret = 0;
  while (bytes_read < bytes) {
    ret = read(fd, buffer + bytes_read, bytes - bytes_read);
    if (ret > 0) {
      bytes_read += ret;
      continue;
    }

    if (ret == 0) {
      break;
    } else {
      if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
        continue;
      }
      break;
    }
  }

  if (bytes_read < 0)
    return -1;

  /* Copy the data from BUFFER into the memory specified by VECTOR.  */
  bytes = bytes_read;
  for (int i = 0; i < count; ++i) {
    size_t copy = MIN(vector[i].iov_len, bytes);

    (void)memcpy((void *)vector[i].iov_base, (void *)buffer, copy);

    buffer += copy;
    bytes -= copy;
    if (bytes == 0)
      break;
  }

  return bytes_read;
}
