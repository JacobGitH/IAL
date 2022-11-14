/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if(tree != NULL){
    if(tree->key == key){
      *value = tree->value;
      return true;
    }else if(tree->key < key){
      bst_search(tree->right, key, value);
    }else{
      bst_search(tree->left, key, value);
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *tree_node = *tree;

  if(*tree == NULL){
    bst_node_t *tree_malloc = malloc(sizeof(bst_node_t)); 
    tree_malloc->key = key;
    tree_malloc->value = value;
    tree_malloc->left = NULL;
    tree_malloc->right = NULL;

    *tree = tree_malloc;
  }else{
    if(tree_node->key < key){
      bst_insert(&tree_node->right, key, value);
    }else if(tree_node->key > key){
      bst_insert(&tree_node->left, key, value);
    }else{
      tree_node->value = value;
    }
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *tree_node = *tree;
    
    if(tree_node != NULL){
      if(tree_node->right->right != NULL){
          bst_replace_by_rightmost(target, &tree_node->right);
      }else if(tree_node->right == NULL){
        target->value = tree_node->value;
        target->key = tree_node->key;
        if(tree_node->left != NULL){
          target->right = tree_node->left;
        }else{
          target->right = NULL;
        }
        free(tree_node);
      }else{
        bst_node_t *father_node = target;
        if(tree_node->right->left != NULL){
          father_node = tree_node;
          tree_node = tree_node->right;
          target->value = tree_node->value;
          target->key = tree_node->key;
          father_node->right = tree_node->left;
          free(tree_node);
        }else{
          father_node = tree_node;
          tree_node = tree_node->right;
          target->value = tree_node->value;
          target->key = tree_node->key;
          free(tree_node);
          father_node->right = NULL;
        }
      }
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) { //TODO
  if((*tree) != NULL){
    if(key < (*tree)->key){
        bst_delete(&(*tree)->left, key);
    }else if(key > (*tree)->key){
        bst_delete(&(*tree)->right, key);
    }else{
      if((*tree)->left == NULL && (*tree)->right == NULL){
        free((*tree));
        (*tree) = NULL;
      }else if((*tree)->left != NULL && (*tree)->right != NULL){
        bst_replace_by_rightmost((*tree), &(*tree)->left);
      }else if((*tree)->left == NULL){
        bst_node_t *delete_node = (*tree);
        (*tree) = (*tree)->right;
        free(delete_node);  
      }else{
        bst_node_t *delete_node = (*tree);
        (*tree) = (*tree)->left;
        free(delete_node);  
      }
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  bst_node_t *tree_node = *tree;
  if((*tree) != NULL){
    bst_dispose(&tree_node->left);
    bst_dispose(&tree_node->right);
    free(tree_node);
    (*tree) = NULL;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  if(tree != NULL){
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if(tree != NULL){
    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if(tree != NULL){
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}