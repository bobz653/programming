class Solution:
    def firstMissingPositive(self, nums: List[int]) -> int:
        # 使用set来存储数组中的元素
        hash_set = set()
        for num in nums:
            hash_set.add(num)
        
        # 遍历从1到len(nums)的整数，寻找第一个不在hash_set中的整数
        for i in range(1, len(nums) + 1):
            if i not in hash_set:
                return i
        
        # 如果所有1到len(nums)的整数都在数组中，则返回len(nums) + 1
        return len(nums) + 1
