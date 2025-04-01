class Solution:
    def partition(self, s: str) -> List[List[str]]:
        def is_palindrome(sub: str) -> bool:
            return sub == sub[::-1]

        def backtrack(start: int):
            if start == len(s):
                result.append(path[:])
                return
            
            for end in range(start + 1, len(s) + 1):
                substring = s[start:end]
                if is_palindrome(substring):
                    path.append(substring)
                    backtrack(end)
                    path.pop()

        result = []
        path = []
        backtrack(0)
        return result
