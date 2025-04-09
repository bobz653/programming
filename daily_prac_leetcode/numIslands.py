class Solution:
    

    def numIslands(self, grid: List[List[str]]) -> int:    

        def area(r, c):
            if not inArea(grid, r, c):
                return 
            if grid[r][c] != '1':
                return 
            grid[r][c] = '2'
            
            area(r - 1, c)
            area(r + 1, c)
            area(r, c - 1)
            area(r, c + 1)

        def inArea(grid, r, c):
            return 0 <= r < len(grid) and 0 <= c < len(grid[0])

        res = 0
        for r in range(len(grid)):
            for c in range(len(grid[0])):
                if grid[r][c] == '1':
                    area(r, c)
                    res += 1
        return res
    
