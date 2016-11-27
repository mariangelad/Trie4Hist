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
#include <string.h>
#include "prog.h"


/* The function creates the tree structure and allocates the first node, called root */
ptree tree_create(){

  ptree pt;
  if((pt=(ptree)malloc(sizeof(tree)))==NULL){
  fprintf(stderr, "No sufficient memory to allocate the tree.\n");
  exit(EXIT_FAILURE);
  }
  pt->root=malloc_node();
  pt->leaf_count=-1;
  return pt;
}


/* The function creates an internal node */
pnode malloc_node(void){

  pnode pn;
  if((pn=(pnode)malloc(sizeof(internal_node)))==NULL){
    fprintf(stderr,"No sufficient memory to allocate the node.\n");
    exit(EXIT_FAILURE);
  }

  int i;
  for(i=0;i<ALPHABET_SIZE;i++){
    (pn->children[i]) = NULL;
  }
  return pn;
}


/* This function creates an external node, called leaf */
pleaf malloc_leaf(void){

  pleaf pl;
  if((pl=(pleaf)malloc(sizeof(leaf)))==NULL){
    fprintf(stderr,"No sufficient memory to create the leaf.\n");
    exit(EXIT_FAILURE);
  }

  pl->data_count=-1;
  pl->list_address=NULL;
  return pl; 
}

/* The function searches the key along the tree: if the key is found the function updates the records associated with that key;
   if the key is not found the function creates the path and the leaf corresponding to the string and,at the end, makes the records update */
pleaf search_value(ptree pt, pleaf* ppl, int* alphabet, char key[], double dato){
/* The arguments are: a pointer to the tree structure, an array of pointers to leaf, an array of 256 integers one for each ASCII character, a string "key" and a double */

  int level=0;   /* char position along the string */
  int index;     /* index of the pointers array */
  pnode pn;      /* pointer to a node, it acts as a cursor to go through the tree */
  pn=pt->root;   /* at first the pointer adress is assigned to the first node (root) */

  do{

    if((index=alphabet[(int)key[level]])==-1){  /* the index is given by an integer value univocally associated to each char of the strings, as stored in the array "alphabet"  */
      fprintf(stderr,"The character %c is not included in alphabet.txt\n", key[level]); /* if the char is not included in the input file "alphabet.txt", an error message is shown */
      exit(EXIT_FAILURE);
    }
      if( !pn->children[index] ){      /* if the children[index] pointer has not yet been addressed,
	                                  a new internal node is created and the pointer is addressed to the node */
         pn->children[index]=malloc_node();
      }
      pn = pn->children[index];    /* the pointer "pn" flows down the tree from one node to the next one, */
      level++;                     /* as the string is scrolled down, until the string ends in the null-termination character "\0" */

  } while(key[level]!='\0');


    if((index=alphabet[(int)key[level]])==0){   /* when the strings ends, index=alphabet[0]=0, the pointer children[0] points to a leaf, to the one corresponding to that string */


      if( !pn->children[index] ){   /* if the pointer to that leaf has not been addressed yet (the leaf has not been created) */

        pn->children[index]=malloc_leaf();  /* a new leaf is created, and the pointer children[0] is directed to the leaf */

        pt->leaf_count++;  /* the leaf counter is updated */

        ppl[pt->leaf_count]=((pleaf)(pn->children[index]));  /* the address of the new leaf is stored in the array of pointers to leaf ppl[] */

        memcpy((((pleaf)(pn->children[index]))->field_name), key, strlen(key)+1);   /* the string "key" is copied in the variable "field_name" */

        ((pleaf)(pn->children[index]))->list_address=malloc_array_list(dato);   /* a new linked list is created that can be accessed via the pointer "list_address" */
	
        add_element(((pleaf)(pn->children[index]))->list_address, 100);   /* a first element is added to the list, the first array has an arbitrary size of 100 */
      }


      ((pleaf)(pn->children[index]))->data_count++;  /* the counter of data relating to this leaf is updated
                                                        when the array is filled with the data, a new element is added (and a new array),
                                                        then the counter restart again from 0 */
    }

      return (pleaf)(pn->children[index]);   /* the function returns a pointer to the leaf */
}



