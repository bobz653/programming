# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next
class Solution:
    def mergeTwoLists(self, list1: Optional[ListNode], list2: Optional[ListNode]) -> Optional[ListNode]:
        p1, p2 = list1, list2
        if list1 is None:
            return list2
        if list2 is None:
            return list1
        
        preHead= ListNode()
        cur = preHead
        while(p1 and p2):
            if p1.val <= p2.val:
                cur.next = p1
                p1 = p1.next
            else:
                cur.next= p2
                p2 =p2.next
            cur = cur.next
        if p1:
            cur.next = p1
        if p2:
            cur.next = p2
        return preHead.next
                
