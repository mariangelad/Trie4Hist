/*
Copyright (C) 2015  Mariangela Di Prima mariangela.diprima@gmail.com

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

#include<stdio.h>
#include <dirent.h>



char *ext;

/* This function is used to select the files from which the input data are read: the function returns TRUE if the file name ends with ext, otherwise it returns FALSE */
int ext_filter(const struct dirent *d);

/* The function gives the size of an input file in bytes unit */
unsigned long f_tot_size(FILE* f);

/* The function provides the size of the read data from an input file input file */
unsigned long f_partial_size(FILE* f);

/* The function calculates the size of the read data taking into account also those coming from previous files */
unsigned long data_partial_size(FILE* f, unsigned long* f_bytes, int num_files);
