

Finding middle from LinkedList 

```cpp

//initilize fslow /fast pointr
 
Node* fast= head;
Node* slow = head; 

while(fast != NULL && fast->next != NULL){
 fast = fast->next->next;
 slow = slow->next;
}


```


Reversing LinkedList 

```cpp

//initialize prev/next to NULL

Node* prev  = NULL;
Node* nxt   = NULL ;

Node* curr = head;

while(curr != NULL){
    nxt = curr->next;
    curr->next = prev;
    prev = curr; 
    curr = nxt;
}


while(prev != NULL){
    cout << prev->val;
    prev = prev->val;
}

```


Detecting Cycle 
```cpp
   ListNode *detectCycle(ListNode *head) {

        ListNode* fast = head;
        ListNode* slow = head;

        while(fast != NULL && fast->next != NULL){
            slow = slow->next;
            fast = fast->next->next;
            if(slow  == fast){
               slow = head;
               while(fast != slow){
                   fast = fast->next;
                   slow = slow->next;
               }
                return slow;
            }
        }

        return NULL;

    }
```
