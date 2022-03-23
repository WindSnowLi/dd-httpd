#pragma once

#include <fstream>
#include <filesystem>
#include <algorithm>
#include <string_view>
#include <memory>

namespace StrUtils {
	std::vector<std::string_view> Split(std::string_view sv, char ch) {
		std::vector<std::string_view> points;
		size_t point = 0;
		for (size_t i = 0; i < sv.size(); i++) {
			if (sv[i] == ch) {
				points.emplace_back(std::string_view(&sv[point], i - point));
				point = i + 1;
			}
		}
		if (point >= sv.size())
		{
			points.emplace_back(std::string_view(&sv[point - 1], sv.size() - point));
		}
		else {
			points.emplace_back(std::string_view(&sv[point], sv.size() - point));
		}
		return points;
	}

	std::map<std::string, std::string> PropertyParse(const std::vector<std::string_view>& svs) {
		std::map<std::string, std::string> property;
		std::string buff;
		for (const std::string_view& i : svs) {
			std::vector<std::string_view>&& table = Split(i, '=');
			if (table.size() < 2) {
				continue;
			}
			for (size_t j = 1; j < table.size(); ++j) {
				buff += table[j];
			}
			property.insert(std::make_pair(table[0], buff));
			buff.clear();
		}
		return property;
	}

	void ReplaceChar(std::string& sv, char oldCh, char newCh) {
		for (char& i : sv) {
			if (i == oldCh) {
				i = newCh;
			}
		}
	}

	bool CheckPathDeepin(const std::string_view& path) {
		std::string temp(path);
		ReplaceChar(temp, '\\', '/');
		std::vector<std::string_view> svs = Split((temp), '/');
		int count = 0;
		std::string pre = "..";
		for (auto&& i : svs) {
			if (i.empty()) {
				continue;
			}
			if (i != pre) {
				++count;
			}
			else {
				--count;
			}
			if (count < 0) {
				return false;
			}
		}
		return true;
	}

	inline void ToLowCase(std::string& sv) {
		std::transform(sv.begin(), sv.end(), sv.begin(), ::tolower);
	}

	std::string GetRandStr(size_t l) {
		std::string rs;
		for (size_t i = 0; i < l; i++)
		{
			rs.push_back(rand() % 26 + 'a');
		}
		return rs;
	}
}

namespace FileUtils {
	size_t GetFileLength(const std::string& path);

	size_t GetFileStreamLength(std::ifstream& fp);

	size_t GetFileLength(const std::string& path) {
		std::ifstream fp(std::filesystem::u8path(path), std::ios::in);
		if (!fp) {
			return 0;
		}
		size_t length = GetFileStreamLength(fp);
		fp.close();
		return length;
	}

	size_t GetFileStreamLength(std::ifstream& fp) {
		if (!fp) {
			return 0;
		}
		//定位到文件末尾
		fp.seekg(0, std::ifstream::end);
		//获得文件总长度
		size_t length = fp.tellg();
		fp.seekg(0, std::ifstream::beg);
		return length;
	}

	inline bool ExistFile(const std::string& path) {
		return std::filesystem::exists(std::filesystem::u8path(path));
	}

	std::string GetContentType(const std::string& name) {
		static std::unordered_map<std::string, std::string> map{
				{"html",    "text/html"},
				{"htm",     "text/html"},
				{"shtml",   "text/html"},
				{"css",     "text/css"},
				{"xml",     "text/xml"},
				{"gif",     "image/gif"},
				{"jpeg",    "image/jpeg"},
				{"jpg",     "image/jpeg"},
				{"js",      "application/javascript"},
				{"atom",    "application/atom+xml"},
				{"rss",     "application/rss+xml"},
				{"mml",     "text/mathml"},
				{"txt",     "text/plain"},
				{"jad",     "text/vnd.sun.j2me.app-descriptor"},
				{"wml",     "text/vnd.wap.wml"},
				{"htc",     "text/x-component"},
				{"avif",    "image/avif"},
				{"png",     "image/png"},
				{"svg",     "image/svg+xml"},
				{"svgz",    "image/svg+xml"},
				{"tif",     "image/tiff"},
				{"tiff",    "image/tiff"},
				{"wbmp",    "image/vnd.wap.wbmp"},
				{"webp",    "image/webp"},
				{"ico",     "image/x-icon"},
				{"jng",     "image/x-jng"},
				{"bmp",     "image/x-ms-bmp"},
				{"woff",    "font/woff"},
				{"woff2",   "font/woff2"},
				{"jar",     "application/java-archive"},
				{"war",     "application/java-archive"},
				{"ear",     "application/java-archive"},
				{"json",    "application/json"},
				{"hqx",     "application/mac-binhex"},
				{"doc",     "application/msword"},
				{"pdf",     "application/pdf"},
				{"ps",      "application/postscript"},
				{"eps",     "application/postscript"},
				{"ai",      "application/postscript"},
				{"rtf",     "application/rtf"},
				{"m3u8",    "application/vnd.apple.mpegurl"},
				{"kml",     "application/vnd.google-earth.kml+xml"},
				{"kmz",     "application/vnd.google-earth.kmz"},
				{"xls",     "application/vnd.ms-excel"},
				{"eot",     "application/vnd.ms-fontobject"},
				{"ppt",     "application/vnd.ms-powerpoint"},
				{"odg",     "application/vnd.oasis.opendocument.graphics"},
				{"odp",     "application/vnd.oasis.opendocument.presentation"},
				{"ods",     "application/vnd.oasis.opendocument.spreadsheet"},
				{"odt",     "application/vnd.oasis.opendocument.text"},
				{"pptx",    "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
				{"xlsx",    "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
				{"docx",    "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
				{"wmlc",    "application/vnd.wap.wmlc"},
				{"wasm",    "application/wasm"},
				{"7z",      "application/x-7z-compressed"},
				{"cco",     "application/x-cocoa"},
				{"jardiff", "application/x-java-archive-diff"},
				{"jnlp",    "application/x-java-jnlp-file"},
				{"run",     "application/x-makeself"},
				{"pl",      "application/x-perl"},
				{"pm",      "application/x-perl"},
				{"prc",     "application/x-pilot"},
				{"pdb",     "application/x-pilot"},
				{"rar",     "application/x-rar-compressed"},
				{"rpm",     "application/x-redhat-package-manager"},
				{"sea",     "application/x-sea"},
				{"swf",     "application/x-shockwave-flash"},
				{"sit",     "application/x-stuffit"},
				{"tcl",     "application/x-tcl"},
				{"tk",      "application/x-tcl"},
				{"der",     "application/x-x9-ca-cert"},
				{"pem",     "application/x-x9-ca-cert"},
				{"crt",     "application/x-x9-ca-cert"},
				{"xpi",     "application/x-xpinstall"},
				{"xhtml",   "application/xhtml+xml"},
				{"xspf",    "application/xspf+xml"},
				{"zip",     "application/zip"},
				{"bin",     "application/octet-stream"},
				{"exe",     "application/octet-stream"},
				{"dll",     "application/octet-stream"},
				{"deb",     "application/octet-stream"},
				{"dmg",     "application/octet-stream"},
				{"iso",     "application/octet-stream"},
				{"img",     "application/octet-stream"},
				{"msi",     "application/octet-stream"},
				{"msp",     "application/octet-stream"},
				{"msm",     "application/octet-stream"},
				{"mid",     "audio/midi"},
				{"midi",    "audio/midi"},
				{"kar",     "audio/midi"},
				{"mp3",     "audio/mpeg"},
				{"ogg",     "audio/ogg"},
				{"m4a",     "audio/x-m4a"},
				{"ra",      "audio/x-realaudio"},
				{"3gpp",    "video/3gpp"},
				{"3gp",     "video/3gpp"},
				{"ts",      "video/mp2t"},
				{"mp4",     "video/mp4"},
				{"mpeg",    "video/mpeg"},
				{"mpg",     "video/mpeg"},
				{"mov",     "video/quicktime"},
				{"webm",    "video/webm"},
				{"flv",     "video/x-flv"},
				{"m4v",     "video/x-m4v"},
				{"mng",     "video/x-mng"},
				{"asx",     "video/x-ms-asf"},
				{"asf",     "video/x-ms-asf"},
				{"wmv",     "video/x-ms-wmv"},
				{"avi",     "video/x-msvideo"} };
		if (map.count(name) == 1) {
			return map[name];
		}
		else {
			return "text/plain";
		}
	}

	std::string GetFileTypeName(const std::string& name) {
		std::string_view sv(name);
		size_t index = sv.rfind('.');
		if (index != -1) {
			return std::string(std::string_view(name.c_str() + index + 1));
		}
		return "";
	}
}

namespace DateUtils {
	std::string GetCurrentDate() {
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string timeStr = std::ctime(&time);
		timeStr.resize(timeStr.size() - 1);
		return timeStr;
	}
}