#include <string>
#include <iostream>
#include <vector>

#define ENABLE_PATH_UTILS 1

#ifdef ENABLE_PATH_UTILS
// 提取文件名
std::string get_file_name(const std::string& path) {
    auto pos = path.find_last_of('/\\');
    if (pos == std::string::npos) {
        return path;
    }

    return path.substr(pos + 1);
}

// 提取文件扩展名
std::string get_file_extension(const std::string& path) {
    auto pos = path.find_last_of('.');
    if (pos == std::string::npos) {
        return "";
    }

    return path.substr(pos + 1);
}

// 提取目录
std::string get_dir(const std::string& path) {
    auto pos = path.find_last_of("/\\");
    if (pos == std::string::npos) {
        return "";
    }

    return path.substr(0, pos);
}
#endif

#pragma region 
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> res;
    std::string tmp;
    for (auto c : str) {
        if (c != delimiter) {
            tmp += c;
        } else {
            // 避免空字符串
            if (!tmp.empty()) {
                res.push_back(tmp);
                tmp.clear();
            }
        }
    }
    // 处理最后一段
    if (!tmp.empty()) {
        res.push_back(tmp);
    }
    return res;
}
#pragma endregion


int main() {
    // std::string path{"\\home\\user\\test.txt"};
    // std::cout << "完整路径" << path << std::endl;
    // std::cout << "文件名" << get_file_name(path) << std::endl;
    // std::cout << "后缀：" << get_file_extension(path) << std::endl;
    // std::cout << "目录" << get_dir(path) << std::endl;

    // std::string csv = "Alice,20,Beijing";
    // auto res = split(csv, ',');
    // std::cout << "分割结果：" << std::endl;
    // for (const auto& field : res) {
    //     std::cout << field << std::endl;
    // }


    return 0;
}