class Solution:
    def wordBreak(self, s: str, wordDict: List[str]) -> bool:
        strlen = len(s)
        dp = [False]* (strlen+1)
        dp[0] = True
        for i in range(strlen):
            if(not dp[i]):
                 continue
            for word in wordDict:
                length = len(word)
                if s.startswith(word, i):  # 使用 startswith 方法来简化原代码中的字符串比较逻辑
                    dp[i + length] = True
        return dp[strlen]
