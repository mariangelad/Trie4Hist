## Trie4Hist
A fast _key/value_ search and store code for strings, implemented by using a _multiway trie_.
For each key the frequency distribution histogram is calculated and represented.

The code reads a random series of data made by the name of an observable and its value, the data are sorted according to the relative observables
and the frequency distribution histogram is generated for each observable.

To run the code:

    ./launch.script


### Input

Trie4Hist reads data from all the files in a given directory with a given extension.
The data have to be organized as follow:

* field name
* value

You can change the directory and the extension of the input files, and the number of bins for which to calculate the histogram acting on the arguments of main():

    ./Trie4Hist  nbins  directory  extension


### Setting

The files _alphabet.txt_ contains the list of the characters you want to include in the trie.
The number of permitted characters sets the value of ALPHABET_SIZE:

    #define ALPHABET_SIZE  54

Then you can decide the maximum length of the field name string (according to the maximum expected size of the name of the observables) and the maximum number of different observables.

    #define FIELDNAME  100      /* Maximum size of the field name string */
    #define FIELD_NUM  50       /* Maximum number of fields */                      


### Trie

At first you have to create the tree data structure:

    ptree key_tree;   /* pointer to the tree structure */
    key_tree=tree_create();
    
The trie consists of _internal nodes_ and _external nodes_ or _leaves_.
In particular in Trie4Hist the trie is implemented as a _multiway tree_ where the _internal nodes_ are simply made by an array of pointers to node.
We have one pointer for each character of the alphabet listed in _alphabet.txt_ plus one pointer corresponding to the NULL character. 
    
Each leaf corresponds to a field name and will store all the informations about that observable.
An array of pointers to the leaves is used to access later to the leaves without having to travel the tree. 

    pleaf foglia;   /* pointer to a leaf node */

    pleaf* leaves_book;   /* array of pointers to the leaf nodes */
    leaves_book=(pleaf*)malloc(sizeof(pleaf)*FIELD_NUM);

The function _search_value_ searches and adds the key (the field name) into the trie. If the key is not found it is added to the tree and a new leaf is created, 
if the key has been already inserted into the tree the function finds the corresponding leaf and updates its records.

    foglia=search_value(key_tree, leaves_book, char_map, key, dato);
    
The arguments are:
* a pointer to the tree structure,
* the array of pointers which save the address of the leaves,
* the array _char_map_ that associates each character to one index of the array of pointers in an internal node,
* the field name,
* its value.

### Linked List

To store the data we have used a _linked list_, one for each observable, where each element contains a static array.

The size of the first array ( _dim_ ) is arbitrarily fixed,
while the size of the second one ( _newdim_ ) is chosen so as to be sufficient to store all the remaining data.
In particular _newdim_ is calculated according to the ratio between the total bytes of the input files and the bytes already read when the first array is full.

In this way we work with lists made by no more than two elements to optimize time and memory performance.


