class Solution:
    def lengthOfLIS(self, nums: List[int]) -> int:
        if not nums:
            return 0
        dp = [1] * len(nums)
        max_dp = 1
        for i in range(1, len(nums)):
            for j in range(i):
                if nums[j]< nums[i]:
                    dp[i]= max(dp[j]+1,dp[i])

            max_dp = max(max_dp, dp[i])
        return max_dp
