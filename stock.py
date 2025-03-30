class Solution:
    def maxProfit(self, prices: List[int]) -> int:
        if not prices:
            return 0
        maxProfit = 0
        min_price = prices[0]
        for i in prices:
            min_price = min(min_price, i)
            maxProfit = max(i - min_price, maxProfit)
            #print(min_price, maxProfit)
        return maxProfit
