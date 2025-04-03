class Solution:
    def searchInsert(self, nums: List[int], target: int) -> int:
        
        left, right = 0, len(nums)-1
        ans = 0
        while (left <= right):
            mid_len = (left + right) // 2
            #print(left,right,mid_len)
            if nums[mid_len] == target:
                return mid_len
            if nums[mid_len] < target:
                left = mid_len + 1
                ans = left
            else:
                right = mid_len - 1
                ans = right + 1
            #print(left,right,mid_len)
            
        return ans if ans >= 0 else 0
        
