class Solution:
    def isValid(self, s: str) -> bool:
        if len(s) % 2 != 0:
            return False
        pair_map = {")":"(","}":"{","]":"["}
        stack = list()
        for ch in s:
            if ch not in pair_map: #这是什么
                stack.append(ch)
                continue
            if not stack or stack[-1] != pair_map[ch]:
                return False
            stack.pop()
        return not stack


        
