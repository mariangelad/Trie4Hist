/*
Copyright (C) 2015  Mariangela Di Prima (mariangela.diprima@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "prog.h"

extern char *ext;

/* The function compares the strings "name" (file name) and "ext" (extension) and returns TRUE if the file name ends with "ext", otherwise it returns FALSE */
int ext_filter(const struct dirent *d){
  size_t nl = strlen(d->d_name), el = strlen(ext);
  return nl >= el && !strcmp((d->d_name) + nl - el, ext);
}


/* The function returns size in bytes unit of an input function */
unsigned long f_tot_size(FILE* f){

    fseek(f, 0, SEEK_END);   /* int fseek(FILE * stream, long int offset, int origin) sets the position indicator associated with the stream to a new position
                                SEEK_END = end of file */
    unsigned long len = (unsigned long)ftell(f);   /* long int ftell(FILE * stream) returns the current value of the position indicator of the stream */

    return len;
}

/* The function returns the size of the file in the position indicator of the stream */
unsigned long f_partial_size(FILE* f){

    unsigned long len = (unsigned long)ftell(f);
    return len;
}

/* The function returns the partial data size as the sum of the current value of the position indicator of the stream plus the size of the preceding files */
 unsigned long data_partial_size(FILE* f, unsigned long* f_bytes, int num_files){
   
  int i;
  unsigned long partial_bytes=0;
  unsigned long* file_bytes=f_bytes;
  unsigned long len = f_partial_size(f);
  for(i=0;i<num_files;i++){    /* sum over the files already read */
    partial_bytes=partial_bytes+file_bytes[i];
  }
  return len+partial_bytes;
}
