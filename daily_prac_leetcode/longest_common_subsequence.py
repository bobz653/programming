def longest_common_subsequence(str1: str, str2: str) -> int:
    """
    计算两个字符串之间的最长公共子序列的长度。
    
    :param str1: 第一个字符串
    :param str2: 第二个字符串
    :return: 最长公共子序列的长度
    """
    m, n = len(str1), len(str2)
    # 创建一个二维数组来存储最长公共子序列的长度。
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    # 自底向上地构建dp数组
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if str1[i - 1] == str2[j - 1]:
                # 如果当前字符相同，则将这一对字符加入到LCS中
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                # 否则，考虑不包括str1[i-1]或str2[j-1]的情况
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

    # dp[m][n] 包含了str1[0..m-1]和str2[0..n-1]的最长公共子序列的长度
    return dp[m][n]

# 示例使用
s1 = "abcde"
s2 = "ace"
print("最长公共子序列的长度:", longest_common_subsequence(s1, s2))
