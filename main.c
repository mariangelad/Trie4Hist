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
#include <dirent.h>
#include <string.h>
#include <math.h>
#include "prog.h"
#include "main_functions.h"

/* The function print the executable file name followed by the arguments of the main() function */
void usage(char *argv[]){
  printf("Usage: %s  [n_bins] [directory] [ext] \n\n" , argv[0]);
}


int main(int argc, char *argv[]) {

  if (argc != 4){  /* if the number of arguments of main is not 4 the function usage is called */
    usage(argv);
    exit(1);
  }



/* An array, called char_map, of lenght 256 is used, so that each element is put in correspondence to a character of the extended ASCII. */
/* The "alphabet.txt" file contains the characters we want to use. They are read from the files one by one, while a counter is incremented, */
/* when a character "c" is read, it is coverted as an integer so that one gives to the corresponding element of the array char_map the value taken by the counter. */
/* So the elements of the array char_map, corresponding to the characters included in "alphabet.txt", will assume positive values (between 0 and the number of used characters), */
/*   while the others elements will assume a negative value. */

  int i;
  int* char_map;
  char_map=(int*)malloc(sizeof(int)*256);

  char_map[0]=0;  /* The first element char_map[0] corrisponds to the Null character "\0", which has ASCII value 0.
                     It is not a printable character, so it can be added in the file "alphabet.txt",
		     but it is used as a control character to indicate the end of the string */
		     
  for (i=1;i<256;i++){  /* the members of the array "char_map" are initialized to a negative value */
    char_map[i]=-1;
  }
  char character;
  FILE* f;
  f=fopen("alphabet.txt", "r");
  i=1;
  if(f==NULL){
    fprintf(stderr,"Error in opening alphabet.txt\n");
    exit(EXIT_FAILURE);
  }
  while((fscanf(f, " %c", &character))==1){ /* the array members, in the position corresponding to the integer value of the characters read from "alphabet.txt",
                                               to the same value taken by the counter */
    char_map[(int)character]=i;
    i++;
  }
  fclose(f);


/* We select the files from which read the data by taking the files in the directory given by argv[2] with the extension given by argv[3]
   and we stores the file names in the array "fileslist"  */

  DIR *dp;   /* pointer to a directory */

  char path[200];
  long unsigned tot_bytes=0;
  long unsigned partial_bytes=0;


  ext = malloc(10*sizeof(char));   /* extension of the input files */
  memcpy(ext,argv[3],strlen(argv[3])+1);

  if((dp = opendir(argv[2]))==NULL){   /* opendir opens a directory stream corresponding to the directory named by argv[2] and returns the pointer (dp) to this directory */
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  struct dirent **fileslist;   /* dirent is a structure type used to return information about directory entries */

  long unsigned* file_bytes;  /* in this array we will save the total dimension of each file */

  int nfiles=scandir(argv[2], &fileslist, ext_filter, 0);    /* number of files into the directory argv[2] having extension "ext" */

  if(nfiles<0){
    perror("scandir");
    exit(EXIT_FAILURE);
  }
  else if (nfiles > 0) {
    file_bytes=(long unsigned*)malloc(sizeof(long unsigned)*nfiles);

    for(i=0; i<nfiles; i++){

      if((strcmp(argv[2],"."))==0){   /* if we are in the current directory */
        if((f=fopen(fileslist[i]->d_name, "r"))==NULL) perror("fopen");  /* fileslist[i]->d_name is one of the fields of the structure dirent,
	                                                                    the one which contains the names of the files */
      }
      else{   /* if we want to follow the path to the destination directory */
        snprintf(path, strlen(argv[2])+strlen(fileslist[i]->d_name)+2,"%s%s%s",argv[2],"/",fileslist[i]->d_name);
        if((f=fopen(path, "r"))==NULL) perror("fopen");
      }

      file_bytes[i]=f_tot_size(f);   /* files total size array */
      tot_bytes=tot_bytes+file_bytes[i];
      fclose(f);  
    }
  }
  else{
    fprintf(stderr,"No files of the specified extension\n");
    exit(EXIT_FAILURE);
  }


//  printf("Total bytes: %lu\n", tot_bytes);


/* Reading the input files */

  FILE *fp;

  char key[FIELDNAME];   /* fields names, max 100 characters */
  double dato;

  ptree key_tree;   /* pointer to the tree structure */
  key_tree=tree_create();

  pleaf foglia;   /* pointer to a leaf node */

  pleaf* leaves_book;   /* array of pointers to the leaf nodes */
  leaves_book=(pleaf*)malloc(sizeof(pleaf)*FIELD_NUM);   /* max 50 fields */

  p_array_list list;   /* pointer to the list */

  pdouble array;   /* pointer to the data array, in this case we want an array of double */
  unsigned int array_size;


  for(i=0; i<nfiles; i++){   /* nfiles = number of input files */

    if((strcmp(argv[2],"."))==0){  /* current directory */
      if((fp=fopen(fileslist[i]->d_name, "r"))==NULL) perror("fopen");
    }
    else{  /* path to the destination directory */
      snprintf(path, strlen(argv[2])+strlen(fileslist[i]->d_name)+2,"%s%s%s",argv[2],"/",fileslist[i]->d_name);
      if((fp=fopen(path, "r"))==NULL) perror("fopen");
    }

    while(fscanf(fp, "%s", key)==1){


      if(fscanf(fp, "%lf", &dato)!=1) perror("scanf");
   
      foglia=search_value(key_tree, leaves_book, char_map, key, dato);  /* the function search_value returns the pointer to the leaf corresponding to that key,
                                                                           if the leaf does not exists (i.e. the key is a new one) the address of the leaf is stored in leaves_book,
									   if the leaf already exists the function updates its records */

      list=foglia->list_address;   /* each time the pointer "list" goes to the list corresponding to that key */


/* When the number of stored data is equal to the array dimension a new element has to be added to the list, */
/* it will contain an array whose dimension will be determid according to the percentage of data to be still read  */
      if((foglia->data_count)==(list->pstart)->size){

 
        partial_bytes=data_partial_size(fp, file_bytes, i);  /* bytes already read */
	
/* The dimension of the array in the new element is determined in proportion to the ratio between the bytes already read and the remaining bytes  */
        array_size=(unsigned int)(((list->pstart)->size)*(((double)tot_bytes)/((double)partial_bytes)-1)*1.15);  /* 1.15 parameter taking into account the error in the estimated new dimension */
        array_size=array_size+1;      /* +1 to make the array_size be always greater or equal to 1 */

        if(add_element(list, array_size)){
          foglia->data_count=0;   /* set the data counter to zero, since a new array is going to be filled */
          partial_bytes=0;
        }
      }

      array=data_array_access(0, list);   /* we insert the data starting from the head of the list O(1) */

      array[foglia->data_count]=dato;   /* copying the data into the arrays */


      if(dato<list->min) list->min=dato;      /* we compare each datum with the minimum (and the maximum) value until now, */
      else if(dato>list->max) list->max=dato; /* if the new one is lower than the min (or greater than the max) it will be set as the new min (max) */

    }

    fclose(fp);
  }

  closedir(dp);
  free(char_map);
  for(i=0; i<nfiles; i++) free(fileslist[i]);
  free(fileslist);
  free(file_bytes);


/* Making the histograms */

  int n_bins = atoi(argv[1]);
  int k,ii;
  unsigned int* histo;   /* in this array we store the counts in each bin */
  double* center;   /* this array contains the value of the centre of each bin */
  histo=(unsigned int*)malloc(sizeof(unsigned int)*n_bins);
  center=(double*)malloc(sizeof(double)*n_bins);


  for(i=0;i<(key_tree->leaf_count+1);i++){  /* iterate through the leaves, that is through the different entries for which we want to determine the histograms */


    for(ii=0;ii<n_bins;ii++){  /* histo[i] and center[i] are set to 0 at the beginning and then each time we pass from one leaf to the other one */
      histo[ii]=0;      
      center[ii]=0;
    }

    double min=leaves_book[i]->list_address->min;    /* here we find the minimum and maximum value of data for each leaf */
    double max=leaves_book[i]->list_address->max;

    double dx=(max-min)/n_bins;    /* bins width */
    double inv_dx=n_bins/(max+dx-min);

    for(ii=0;ii<n_bins;ii++) center[ii]=min+dx*(ii+0.5);  /* calculation of the value of the centre of each bin */

    int j=0;
    array=data_array_access(j,leaves_book[i]->list_address);  /* here we access to the array in the element on top of the list (position 0) */

    for(k=0;k<leaves_book[i]->data_count;k++){      /* to read the data we iterate from 0 to the value of data stored in this array, which is given by data_count */
      histo[(unsigned int)floorl((array[k]-min)*inv_dx)]++;   /* the count of the corresponding bin is increased */
    }

    for(j=1;j<leaves_book[i]->list_address->pos+1;j++){   /* now we iterate over the number of elements in the list */

      unsigned int size=element_size(j, leaves_book[i]->list_address);
      array=data_array_access(j ,leaves_book[i]->list_address);

      for(k=0;k<size;k++){    /* since these arrays are full, we iterate from 0 to size */
        histo[(unsigned int)floorl((array[k]-min)*inv_dx)]++;
      }
    }


/* Saving the histograms in the output files */

      FILE* out;
      char name[FIELDNAME+5];
      snprintf(name, strlen(leaves_book[i]->field_name)+6, "%s%s", leaves_book[i]->field_name, "_hist");
      out=fopen(name, "w+");
      for(ii=0;ii<n_bins;ii++){
        fprintf(out,"%f \t %d\n",center[ii], histo[ii]);
      }
      fclose(out);
                                                                        
    }

  free(key_tree);
  free(foglia);
  free(list);
  free(leaves_book);
  free(histo);
  free(center);



  return 1;
}
  


