class Solution:
    def longestPalindrome(self, s: str) -> str:
        if len(s) == 0:
            return ""
        
        start, end = 0, 0
        
        for i in range(len(s)):
            # 对于奇数长度的回文（单个字符为中心）
            len1 = self.expandAroundCenter(s, i, i)
            # 对于偶数长度的回文（两个字符为中心）
            len2 = self.expandAroundCenter(s, i, i + 1)
            
            # 取两者中的最大值
            max_len = max(len1, len2)
            
            # 如果找到的回文比当前记录的要长，则更新start和end
            if max_len > end - start:
                start = i - (max_len - 1) // 2
                end = i + max_len // 2
        
        # 返回最长回文子串
        return s[start:end + 1]
    
    def expandAroundCenter(self, s: str, left: int, right: int) -> int:
        L, R = left, right
        # 向两边扩展直到不再是回文
        while L >= 0 and R < len(s) and s[L] == s[R]:
            L -= 1
            R += 1
        # 返回回文子串的长度
        return R - L - 1

# 使用示例
solution = Solution()
print(solution.longestPalindrome("babad"))  # 输出 "bab" 或 "aba"
print(solution.longestPalindrome("cbbd"))   # 输出 "bb"
