class Solution:
    def letterCombinations(self, digits: str) -> List[str]:
        result = []
        n = len(digits)
        if n == 0:
            return result
        
        digit_map = {
            '2': ["a", "b", "c"],
            '3': ["d", "e", "f"],
            '4': ["g", "h", "i"],
            '5': ["j", "k", "l"],
            '6': ["m", "n", "o"],
            '7': ["p", "q", "r", "s"],
            '8': ["t", "u", "v"],
            '9': ["w", "x", "y", "z"]
        }
        
        if digits[0] in digit_map:
            result = digit_map[digits[0]]
        if n == 1:
            return result
        
        i = 1
        while i < n:
            temp = []
            for s1 in result:
                for s2 in digit_map[digits[i]]:
                    temp.append(s1 + s2)
            result = temp
            i += 1
            
        return result
        
