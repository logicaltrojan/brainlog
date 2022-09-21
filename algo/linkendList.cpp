
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    
    //1 2 2 1
    // 1 1 1 1
    // 1 2 3 4 3 2 1 
    // 1 2 1 2 1 2 1 2 
    bool isPalindrome(ListNode* head) {
        
        //find middle of the list 

        ListNode* fast = head;
        ListNode* slow = head; 
        
        while(fast->next != NULL && fast->next->next != NULL){
            fast = fast->next->next;
            slow = slow-> next;
        }
        
        //slow is the middle
        // if  fast == NULL 2n + 1  fast->next== NULL 2n
       
        ListNode* middle = slow->next;
        // reverse the right half
        ListNode* rightHead = middle;
        ListNode* next = NULL;
        ListNode* prev = NULL;
        
        while(rightHead != NULL){
            next = rightHead -> next;
            rightHead ->next = prev;
            prev = rightHead ;
            rightHead = next;
        }
        
        while(prev!= NULL){
            cout<< prev->val;
            prev = prev->next;
        }
        
        
        // compare the list 

        return true;
        
    }
};
