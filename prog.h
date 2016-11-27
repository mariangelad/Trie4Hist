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

#define ALPHABET_SIZE  54   /* Number of used characters */
#define FIELDNAME  100      /* Maximum size of the field name string */
#define FIELD_NUM  50       /* Maximum number of fields */                      


typedef void* pvoid;
typedef double* pdouble;


///*  Generic element of the array list  *///
typedef struct el{

  struct el* next;     /* next points to the following element of the list */
  unsigned int size;   /* array dimension */
  pvoid data;          /* data array (starting point) */

}element;


typedef element* pelement;   /* pointer to the structure called element */
typedef pelement* ppelement;


///* List structure *///
typedef struct{

  pelement pstart;    /* pstart points to the first element of the list  */
  int pos;            /* pos+1 is the number of element in the list (the count starts from 0) */
  double min;         /* minimum value of the data stored in the list */
  double max;         /* maximum value of the data stored in the list */

}array_list;

typedef array_list* p_array_list;




//// ***         TRIE          *** ////


///* Trie internal node *///
typedef struct node{

  pvoid children[ALPHABET_SIZE];   /* Pointers array: one for each alphabet character.
                                      They are declared as void pointers since the first one (which corresponds to the Null character "\0") is going to point to a leaf,
                                      while the other ones points to an internal node of the tree. Then you need to cast the pointers */
}internal_node;

typedef internal_node* pnode;
typedef pnode* ppnode;


///* Trie external node (or leaf) *///
typedef struct{

  char field_name[FIELDNAME];   /* each leaf corresponds to a field name, which is stored in this variable */
  int data_count;               /* data_count counts the times that field name appears in the input files and a new datum relative to this field is added */
  p_array_list list_address;    /* pointer to the list where the data, relative to that field name, are being stored */

}leaf;

typedef leaf* pleaf;


///* Tree general structure *///
typedef struct{
   
  pnode root;         /* pointer to the first node of the tree, this acts like a root since you will always go down the tree starting from the first node */
  int leaf_count;     /* leaf_count counts the leaves starting from 0, so that the number of leaves is leaf_count+1 */

}tree;

typedef tree* ptree;




ptree tree_create();
pnode malloc_node(void);
pleaf malloc_leaf(void);
void insert_key(pnode root, char key[]);
int search(ptree pt, char key[]);
pleaf search_value(ptree pt, pleaf* ppl, int* alphabet, char key[], double dato);

p_array_list malloc_array_list(double dato);
int add_element(p_array_list pal, unsigned int array_dim);
pvoid data_array_access(int num, p_array_list pal);
pelement element_access(int num, p_array_list pal);
unsigned int element_size(int num, p_array_list pal);



