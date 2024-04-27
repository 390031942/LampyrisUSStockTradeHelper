#include "Utilities.h"

// Split函数，将字符串str根据给定的分隔符delimiter分割，返回一个包含分割后的字符串的vector
std::vector<std::string> Utilities::Split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens; // 用于存储分割后的字符串
    size_t start = 0; // 分割起始位置
    size_t end = 0; // 分割结束位置

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        // 当找到分隔符时，提取从start到end（不包括end）之间的子字符串
        tokens.push_back(str.substr(start, end - start));
        // 更新start为下一个子字符串的起始位置
        start = end + delimiter.length();
    }
    // 添加最后一个子字符串到tokens中
    // 这是因为最后一个分隔符后面还可能有字符串，这部分也需要被提取出来
    tokens.push_back(str.substr(start));

    return tokens;
}
