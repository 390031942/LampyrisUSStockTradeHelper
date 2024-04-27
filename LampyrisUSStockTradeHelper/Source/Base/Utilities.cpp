#include "Utilities.h"

// Split���������ַ���str���ݸ����ķָ���delimiter�ָ����һ�������ָ����ַ�����vector
std::vector<std::string> Utilities::Split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens; // ���ڴ洢�ָ����ַ���
    size_t start = 0; // �ָ���ʼλ��
    size_t end = 0; // �ָ����λ��

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        // ���ҵ��ָ���ʱ����ȡ��start��end��������end��֮������ַ���
        tokens.push_back(str.substr(start, end - start));
        // ����startΪ��һ�����ַ�������ʼλ��
        start = end + delimiter.length();
    }
    // ������һ�����ַ�����tokens��
    // ������Ϊ���һ���ָ������滹�������ַ������ⲿ��Ҳ��Ҫ����ȡ����
    tokens.push_back(str.substr(start));

    return tokens;
}
