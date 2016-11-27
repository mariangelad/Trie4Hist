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
#include <stdlib.h>
#include "prog.h"


/* The function creates a linked list of arrays */
p_array_list malloc_array_list(double dato){

  p_array_list pal;
  if((pal=(p_array_list)malloc(sizeof(array_list)))==NULL){
    fprintf(stderr,"No sufficient memory to allocate the list.\n");
    exit(EXIT_FAILURE);
  }
  pelement pe;
  if((pe=malloc(sizeof(element)))==NULL){    /* The list contains a dummy element used to indicates its end, this is an empty element so you don't need to allocate memory for the array */
    fprintf(stderr,"No sufficient memory to allocate an element.\n"); 
    free(pal);
    exit(EXIT_FAILURE);
  }
  pal->pstart=(pelement)pe;  /* pointer to the beginning of the list */
  pal->pos=-1;               /* In an empty list pos=-1, so the first element will have pos=0 */
  pal->min=dato;             /* Minimum value of the data stored in the list, at first it is set equal to the first datum value */
  pal->max=dato;             /* The values of minimum and maximum are initialized to the value of the first datum */

  ((pelement)(pal->pstart))->size=0;            /* There is not allocated memory for the array, so the size is set to 0 */
  ((pelement)(pal->pstart))->next=pal->pstart;  /* the "next" pointer of the dummy element points to itself to terminate the linked list */

  return pal;
}

/* The function adds a new element to the linked list. The insertion is always made at beginning so it's O(1) */
int add_element(p_array_list pal, unsigned int array_dim){  /* you can choose the size of the array, passing it as an argument of the function */

  pelement pe;
  ppelement ppe;
  ppe=(ppelement)&(pal->pstart);
  if((pe=malloc(sizeof(element)+(sizeof(double)*array_dim)-sizeof(pvoid)))==NULL)return 0;

  pe->next=(*ppe);
  (*ppe)=pe;
  pal->pos++;
  pe->size=array_dim;   /* here the size of the array is set to the passed value */

  return 1;
}


/* The function allows you to access the array of the element placed in the position "num" in the linked list */
pvoid data_array_access(int num, p_array_list pal){

  int i;
  pelement pe;
  pe=(pelement)pal->pstart;

  for(i=0;i<num;i++){  /* the pointer is placed in the position "num" */
    pe=pe->next;
  }
  pvoid array;
  array=(pdouble)&pe->data;

  return array;
}



/* The function returns a pointer to the element placed in the position "num" in the linked list */    /* Not used */
pelement element_access(int num, p_array_list pal){

  int i;
  pelement pe;
  pe=pal->pstart;

  for(i=0;i<num;i++){
    pe=pe->next;
  }

  return pe;
}

/* The function give the size of the array in the element placed in the position "num" in the linked list */
unsigned int element_size(int num, p_array_list pal){

  int i;
  pelement pe;
  pe=pal->pstart;

  for(i=0;i<num;i++){
    pe=pe->next;
  }

  return pe->size;
}
