#include <queue>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
struct node
{
 int  num = 1;
 int data;
 struct node* right;
 struct node* left;
};

node* newnode(int dat)
{
 node* newn = (node*)malloc(sizeof(node));

 newn->data = dat;
 newn->num = 1;
 newn->left = newn->right = NULL;
 return(newn);
}

struct q
{
 node* temp = NULL;

 q* next = NULL;
};

node* insert(node* root, int data)
{
 if (root == NULL)
 {
  root = newnode(data);
  return root;
 }
 else if (root->data > data)
 {
  root->left = insert(root->left, data);
 }
 else if (root->data < data)
 {
  root->right = insert(root->right, data);
 }
 else if (root->data == data)
 {
  root->num++;
 }
 return root;
}

q* init(node* r1, q** h, q** t)
{
 q* temp = NULL;
 if (*h == NULL)
 {
  temp = (q*)malloc(sizeof(q));
  temp->next = NULL;
  temp->temp = r1->right;
  *h = temp;
  *t = temp;
  return temp;
 }
 else
 {
  temp = (q*)malloc(sizeof(q));
  temp->next = *h;
  temp->temp = r1->right;

  *h = temp;
  return temp;
 }
}

void out(node* r1, char r)
{
 if (r1 == NULL)
 {
  return;
 }
 if (r1->data)
 {
  cout << r << ":";
  cout << r1->data << "  ";
 }

 out(r1->left, 'l');

 out(r1->right, 'l');

}

int main()
{
 queue <int> que;
 q* h = NULL;
 struct node* root = NULL;
 node* tmp;
 int n;
 int mp;
 cout<<"Enter number of elements: ";
 cin >> n;
 for (int i = 0; i < n; i++)
 {
  cin >> mp;
  que.push(mp);
 }

 int *arr=(int*)malloc(n);

 for (int i=0;i<n;i++)
 {
  arr[i] = que.front();
  que.pop();
 }

 for (int i = 0; i < n - 1; i++)
 {
  for (int j = i + 1; j < n; j++)
  {
   if (arr[i] < arr[j]) { swap(arr[i], arr[j]); }
  }
 }
 root = insert(root,arr[n / 2]);
 for (int i = 0; i < n / 2; i++)
 {
  root = insert(root, arr[i]);
 }
 for (int i = n / 2 + 1; i < n; i++)
 {
  root = insert(root, arr[i]);
 }

 char r='h';
 out(root->left, 'l');
 cout << "h:" << root->data<<" ";
 out(root->right, 'r');
 system("pause");
}