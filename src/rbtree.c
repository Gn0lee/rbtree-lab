  
#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1,sizeof(node_t));
  nil->color = RBTREE_BLACK;

  p->nil = nil;
  p->root = nil;

  return p;
}
//x노드 기준으로 왼쪽 회전
int leftRotate(rbtree *t,node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;

  if(y->left != t->nil)
    y->left->parent = x;
  
  y->parent = x->parent;

  if(x->parent == t->nil)
    t->root = y;
  else if(x==x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;

  return 0;
}
//x 노드 기준으로 오른쪽 회전
int rightRotate(rbtree *t,node_t *x)
{
  node_t *y = x->left;
  x->left = y->right;

  if(y->right != t->nil)
    y->right->parent = x;
  
  y->parent = x->parent;

  if(x->parent == t->nil)
    t->root = y;
  else if(x==x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;

  y->right = x;
  x->parent = y;

  return 0;
}

//좌,우,루트 순으로 메모리 할당 해제
void delete_post(rbtree *t,node_t *p){
  if(p==t->nil)
    return;
  delete_post(t,p->left);
  delete_post(t,p->right);
  free(p);
}
//후위순회로 할당 해제 후 nil 및 트리 해제
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_post(t,t->root);
  free(t->nil);
  free(t);
}

int rbInsertFixup(rbtree *t,node_t *z)
{
  while(z->parent->color == RBTREE_RED)
  {
    if(z->parent->parent->left == z->parent)
    {
      node_t *y = z->parent->parent->right;
      if(y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if(z == z->parent->right)
        {
          z = z->parent;
          leftRotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rightRotate(t,z->parent->parent);
      }
    }
    else
    {
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if(z == z->parent->left)
        {
          z = z->parent;
          rightRotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        leftRotate(t,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;

  return 0;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *parent = t->nil;
  node_t *p = t->root;

  while(p != t->nil)
  {
    parent = p;
    if(p->key > key)
      p = p->left;
    else
      p = p->right;
  }
  
  node_t *addnode = (node_t*)calloc(1,sizeof(node_t));
  addnode -> key = key;
  addnode ->color = RBTREE_RED;
  addnode ->parent = parent;
  addnode ->left = t->nil;
  addnode ->right = t->nil;

  
  if(parent == t->nil)
    t->root = addnode;
  else
    if(parent->key > key)
      parent->left = addnode;
    else
      parent->right = addnode;
  
  rbInsertFixup(t,addnode);
  return addnode;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *p = t->root;

  while(p != t->nil)
  {
    if(p->key == key)
      return p;
    else if(p->key > key)
      p = p->left;
    else
      p = p-> right;
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *p = t->root;
  node_t *min_v = t->nil;
  while(p != t->nil){
    min_v = p;
    p = p->left;
  }

  return min_v;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *p = t->root;
  node_t *max_v = t->nil;
  while(p != t->nil){
    max_v = p;
    p = p->right;
  }
  return max_v;
}

//실제로 삭제할 계승노드 찾기
node_t *tree_successor(rbtree *t,node_t *x){
  if (x->right != t->nil){
    node_t *y = x ->right;
    node_t *z = x;
    while(y->left != t->nil){
      z = y;
      y = y->left;
    }
    return z;
  }
  else{
    node_t *y = x->parent;
    while((y != t->nil)&&(x == y->right)){
      x = y;
      y = y->parent;
      }
    return y;
  }
}

int rbDeleteFixup(rbtree *t,node_t *x){
  while((x != t->root)&&(x->color == RBTREE_BLACK)){
    if(x->parent->left == x){
      node_t *w = x->parent->right;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        leftRotate(t,x->parent);
        w = x->parent->right;
      }
      if((w->left->color==RBTREE_BLACK)&&(w->right->color==RBTREE_BLACK)){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rightRotate(t,w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        leftRotate(t,x->parent);
        x = t->root;
      }
    }
    else{
      node_t *w = x->parent->left;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rightRotate(t,x->parent);
        w = x->parent->left;
      }
      if((w->right->color==RBTREE_BLACK)&&(w->left->color==RBTREE_BLACK)){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          leftRotate(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rightRotate(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
  return 0;
}

void rbtree_transplant(rbtree *tree, node_t *node, node_t *adj_node)
{
  if (node -> parent == tree -> nil){
    tree -> root = adj_node;
  } 
  else if(node == node -> parent ->left){
    node -> parent -> left = adj_node;
  } else{
    node -> parent -> right = adj_node;
  }
  adj_node -> parent = node -> parent;
}

int rbtree_erase(rbtree *tree, node_t *node){
  if (node == NULL) {
    return 0;
  }
  node_t *to_be_deleted = node;
  node_t *temp;
  color_t to_be_deleted_original_color;
  to_be_deleted_original_color = to_be_deleted -> color;
  if (node -> left == tree -> nil){
    temp = node -> right;
    rbtree_transplant(tree, node, node -> right);
  } 
  else if (node -> right == tree -> nil){
    temp = node -> left;
    rbtree_transplant(tree, node, node -> left);
  } 
  else {
    to_be_deleted = node -> right;
    while(to_be_deleted -> left != tree -> nil){
      to_be_deleted = to_be_deleted -> left;
    }
    to_be_deleted_original_color = to_be_deleted -> color;
    temp = to_be_deleted -> right;
    if(to_be_deleted -> parent == node){
      temp -> parent = to_be_deleted;
    } 
    else {
      rbtree_transplant(tree, to_be_deleted, to_be_deleted -> right);
      to_be_deleted -> right = node -> right;
      to_be_deleted -> right -> parent = to_be_deleted;
    }
    rbtree_transplant(tree, node, to_be_deleted);
    to_be_deleted -> left = node -> left;
    to_be_deleted -> left -> parent = to_be_deleted;
    to_be_deleted -> color = node -> color;
  }
  if(to_be_deleted_original_color == RBTREE_BLACK){
    rbDeleteFixup(tree, temp);
  }
  free(node);
  return 0;
}
//중위순회 하며 arr에 데이터 기입
int inorder(node_t *x, const rbtree *t, key_t *arr, int i){
  if(x == t->nil)
    return i;
  i = inorder(x->left ,t,arr,i);
  arr[i++] = x->key;
  // i++;
  i = inorder(x->right,t,arr,i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  const rbtree *x = t;
  inorder(t->root, x, arr,0);
  return 0;
}