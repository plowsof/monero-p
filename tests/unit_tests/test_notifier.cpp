// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    fprintf(stderr, "usage: %s <filename> <hash>\n", argv[0]);
    return 1;
  }
  const char *filename = argv[1];
  const char *hash = argv[2];

  FILE *f = fopen(filename, "a+");
  if (!f)
  {
    fprintf(stderr, "error opening file %s: %s\n", filename, strerror(errno));
    return 1;
  }
  fprintf(f, "%s", hash);
  fclose(f);

  return 0;
}
