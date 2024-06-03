/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: Utilities.h
** Desc: 实用工具类
****************************************************************************/
#pragma once
#include <vector>
#include <string>
#include <QByteArray>
#include <zlib.h>
#include <QDir>
#include <QApplication>
#include <QDateTime>
#pragma execution_character_set("utf-8")

namespace Utilities {
    static std::string Trim(const std::string& str) {
        size_t first_non_space = str.find_first_not_of(" \t\n\r\f\v");
        size_t last_non_space = str.find_last_not_of(" \t\n\r\f\v");

        if (first_non_space == std::string::npos || last_non_space == std::string::npos) {
            return ""; // 如果字符串只包含空白字符，则返回空字符串
        }

        return str.substr(first_non_space, (last_non_space - first_non_space + 1));
    }

    static std::string ToUpper(const std::string& str) {
        std::string upper_str;
        for (char c : str) {
            // 使用 std::toupper 转换每个字符
            upper_str += std::toupper(static_cast<unsigned char>(c));
        }
        return upper_str;
    }

    static std::vector<std::string> Split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0; // 分割起始位置
        size_t end = 0; // 分割结束位置

        std::string handleStr = Trim(str);
        while ((end = handleStr.find(delimiter, start)) != std::string::npos) {
            std::string sub = handleStr.substr(start, end - start);
            tokens.push_back(sub);
            start = end + delimiter.length();
        }
        tokens.push_back(handleStr.substr(start));

        return tokens;
    }

    static void SplitNoAlloc(const std::string& str, const std::string& delimiter, std::vector<std::string>&tokens) {
        tokens.clear();
        size_t start = 0; // 分割起始位置
        size_t end = 0; // 分割结束位置

        while ((end = str.find(delimiter, start)) != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
        }
        tokens.push_back(str.substr(start));
    }

    static QByteArray GzipCompress(const QByteArray& postBody) {
        QByteArray outBuf;
        z_stream c_stream;
        int err = 0;
        int windowBits = 15;
        int GZIP_ENCODING = 16;
        if (!postBody.isEmpty()) {
            c_stream.zalloc = (alloc_func)0;
            c_stream.zfree = (free_func)0;
            c_stream.opaque = (voidpf)0;
            c_stream.next_in = (Bytef*)postBody.data();
            c_stream.avail_in = postBody.size();
            if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                MAX_WBITS + GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK) return QByteArray();
            for (;;) {
                char destBuf[4096] = { 0 };
                c_stream.next_out = (Bytef*)destBuf;
                c_stream.avail_out = 4096;
                int err = deflate(&c_stream, Z_FINISH);
                outBuf.append(destBuf, 4096 - c_stream.avail_out);
                if (err == Z_STREAM_END || err != Z_OK) {
                    break;
                }
            }
            auto total = c_stream.total_out;
            deflateEnd(&c_stream);
            total = c_stream.total_out;
        }
        return outBuf;
    }

    static QByteArray GZipUnCompress(const QByteArray& src) {
        QByteArray outBuffer;
        z_stream strm;
        strm.zalloc = NULL;
        strm.zfree = NULL;
        strm.opaque = NULL;

        strm.avail_in = src.size();
        strm.next_in = (Bytef*)src.data();

        int err = -1, ret = -1;
        err = inflateInit2(&strm, MAX_WBITS + 16);
        if (err == Z_OK) {
            while (true) {
                char buffer[4096] = { 0 };
                strm.avail_out = 4096;
                strm.next_out = (Bytef*)buffer;
                int code = inflate(&strm, Z_FINISH);
                outBuffer.append(buffer, 4096 - strm.avail_out);
                if (Z_STREAM_END == code) {
                    break;
                }
            }
        }
        inflateEnd(&strm);
        return outBuffer;
    }

    static std::string MakeTempPath(const char* dstPath) {
        return QDir::temp().filePath(dstPath).toStdString();
    }

    static std::string MakeAppPath(const char* dstPath) {
        return QDir(QApplication::applicationDirPath()).filePath(dstPath).toStdString();
    }

    static QString GetMoneyString(float money, int round1 = 2, int round2 = 0, int round3 = 2) {
        if (money < 10000.0f) {
            // 使用qRound进行四舍五入，并转换为QString
            return QString::number(qRound(money * pow(10, round1)) / pow(10, round1));
        }
        else if (money >= 10000.0f && money < 100000000.0f) {
            // 处理万单位的情况
            return QString::number(qRound((money / 10000.0f) * pow(10, round2)) / pow(10, round2)) + "万";
        }
        else {
            // 处理亿单位的情况
            return QString::number(qRound((money / 100000000.0f) * pow(10, round3)) / pow(10, round3)) + "亿";
        }
    }

    static bool IsAfterTodayUSMarketOpenTime(QDateTime givenDateTime) {
        // 获取当天的日期
        QDateTime todayMidnight = QDateTime::currentDateTime().addDays(0);
        todayMidnight.setTime(QTime(0, 0));

        // 设置时间为晚上9:30
        QTime eveningTime(21, 30);
        QDateTime dateWithTime(todayMidnight.date(), eveningTime);

        return givenDateTime > dateWithTime;
    }
}