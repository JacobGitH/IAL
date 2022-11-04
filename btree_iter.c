/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while (tree)
  {
    if(tree->key == key){
      *value = tree->value;
      return true;
    }else if(key < tree->key){
        tree = tree->left;
    }else if(key > tree->key){
        tree = tree->right;
    }else{
      return false;
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *tree_node = *tree;


  if(tree_node == NULL){
    bst_node_t *tree_malloc = malloc(sizeof(bst_node_t));
    tree_malloc->key = key;
    tree_malloc->value = value;
    tree_malloc->left = NULL;
    tree_malloc->right = NULL;

    (*tree) = tree_malloc;
  }

  while(tree_node != NULL){
    if (tree_node->key == key){
      tree_node->value = value;
      break;
    }else if(tree_node->key > key){
      if(tree_node->left != NULL){
        tree_node = tree_node->left;
      }else{
        bst_node_t *tree_malloc = malloc(sizeof(bst_node_t));
        tree_malloc->key = key;
        tree_malloc->value = value;
        tree_malloc->left = NULL;
        tree_malloc->right = NULL;

        tree_node->left = tree_malloc;
        break;
      }

    }else{
      if(tree_node->right != NULL){
        tree_node = tree_node->right;
      }else{
        bst_node_t *tree_malloc = malloc(sizeof(bst_node_t));
        tree_malloc->key = key;
        tree_malloc->value = value;
        tree_malloc->left = NULL;
        tree_malloc->right = NULL;

        tree_node->right = tree_malloc;
        break;
      }
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *tree_node = *tree;
  bst_node_t *rightmost_node = target;

  while(tree_node->right && tree_node != NULL){
    rightmost_node = tree_node;
    tree_node = tree_node->right;
  }
  if(tree_node != NULL){
    if(tree_node->left != NULL){
      target->value = tree_node->value;
      target->key = tree_node->key;
      rightmost_node->right = tree_node->left;
      free(tree_node);
    }else{
      target->value = tree_node->value;
      target->key = tree_node->key;
      if(target == rightmost_node){
        rightmost_node->left = NULL;
      }else{
        rightmost_node->right = NULL;
      }
      free(tree_node);
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
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {//todo fck me
    bst_node_t *tree_node = *tree;
    bst_node_t *father_node= *tree;

    while(tree_node){
      if(tree_node->key > key){
        if(tree_node->left != NULL){
          father_node = tree_node;
          tree_node = tree_node->left;
        }else
        break;
      }else if(tree_node->key < key){
        if(tree_node->right != NULL){
          father_node = tree_node;
          tree_node = tree_node->right;
        }else
        break;

      }else{
        if(tree_node->left != NULL && tree_node->right != NULL){
          bst_replace_by_rightmost(tree_node, &tree_node->left);
        }else if(tree_node->right == NULL && tree_node->left == NULL){ 
          if(tree_node == *tree){
            free(tree_node);
            break;
          }
          if(father_node->right == tree_node){
            father_node->right = NULL;
          }else{
            father_node->left = NULL;
          }
          free(tree_node);
          break; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







        }else{ 
          if(father_node != NULL){
            if(father_node->left == tree_node){
              if(tree_node->left != NULL){
                father_node->left = tree_node->left;
              }else if(tree_node->right != NULL){
                father_node->left = tree_node->right;
              }
            }
          }

            if(father_node != NULL){
            if(father_node->right == tree_node){
              if(tree_node->right != NULL){
                father_node->right = tree_node->right;
              }else if(tree_node->right != NULL){
                father_node->right = tree_node->left;
              }
            }
          }else{
            if(tree_node->left != NULL){
            (*tree) = tree_node->left;
          }else if(tree_node->right != NULL){
            (*tree) = tree_node->right;
          }
          }




          free(tree_node);
          break;
        
      
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
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  bst_node_t *tree_node = *tree;
  stack_bst_t stack;
  bst_node_t *top_node = NULL;
  stack_bst_init(&stack);

  if(tree_node != NULL){
    stack_bst_push(&stack, (*tree));

  while(stack_bst_empty(&stack) != false){
    top_node = stack_bst_pop(&stack);

    if(top_node != NULL){
      if(top_node->left != NULL){
        stack_bst_push(&stack, top_node->left);
      }
      if(top_node->right != NULL){
        stack_bst_push(&stack, top_node->right);
      }
      free(top_node);
    }
  }
  
  *tree = NULL;
  }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *tree_node = tree;

  while(tree_node){
    
    stack_bst_push(to_visit, tree_node);
    bst_print_node(tree_node);
    tree_node = tree_node->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  bst_node_t *tree_node = tree;
  stack_bst_t stack;
  stack_bst_init(&stack);

  if(tree_node != NULL){
    bst_leftmost_preorder(tree_node, &stack);

    while(stack_bst_empty(&stack) == false){
      tree_node = stack_bst_top(&stack);
      stack_bst_pop(&stack);
      //bst_print_node(tree_node); // nevim jestli tu ma bejt print kdyz je print i v preorderu, god end this misery
      bst_leftmost_preorder(tree_node->right, &stack);
    }
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *tree_node = tree;
  while(tree_node != NULL){
    stack_bst_push(to_visit, tree_node);
    tree_node = tree_node->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) { //todo pls work 
  bst_node_t *tree_node = tree;
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_leftmost_inorder(tree_node, &stack);

  while(stack_bst_empty(&stack) == false){

    tree_node = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_print_node(tree_node);
    bst_leftmost_inorder(tree_node->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  bst_node_t *tree_node = tree;

  while(tree_node != NULL){
    stack_bst_push(to_visit, tree_node);
    stack_bool_push(first_visit, true);
    tree_node = tree_node->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  bst_node_t *tree_node = tree;
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bool_t stack2;
  stack_bool_init(&stack2);

  bst_leftmost_postorder(tree_node, &stack, &stack2);
  bool b_node;

  while(stack_bst_empty(&stack) == false){
    tree_node = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    b_node = stack_bool_pop(&stack2);

    if(b_node){
      stack_bst_push(&stack, tree_node);
      stack_bool_push(&stack2, false);
      bst_leftmost_postorder(tree_node->right, &stack, &stack2);
    }else{
      bst_print_node(tree_node);
    }

  }
}
