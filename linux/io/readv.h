#pragma once

static void ifree(char **ptrp);

ssize_t my_readv(int fd, const struct iovec *vector, int count);
