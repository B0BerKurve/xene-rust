#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <ShlObj.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <variant>
#include <iomanip>
#include "../vcpkg_installed/x64-windows-static/include/zlib-ng.h"
#include <stdexcept>
#include <random>

namespace global {
	inline std::unordered_map<std::string, float> float_map;

	inline float get_float(const std::string& name) {
		if (float_map.find(name) != float_map.end()) {
			return float_map[name];
		}
		return 0.0f;
	}

	inline void set_float(const std::string& name, float value) {
		float_map[name] = value;
	}

	inline bool is_base64(char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	inline std::string base64_encode(const std::string& input) {
		std::stringstream encoded;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];
		for (char c : input) {
			char_array_3[i++] = c;
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3F;
				for (i = 0; i < 4; i++) {
					encoded << base64_chars[char_array_4[i]];
				}
				i = 0;
			}
		}
		if (i > 0) {
			for (j = i; j < 3; j++) {
				char_array_3[j] = '\0';
			}
			char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3F;
			for (j = 0; j < i + 1; j++) {
				encoded << base64_chars[char_array_4[j]];
			}
			while (i++ < 3) {
				encoded << '=';
			}
		}
		return encoded.str();
	}

	inline std::string base64_decode(const std::string& encoded) {
		std::stringstream decoded;
		int in_len = encoded.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		while (in_len-- && (encoded[in_] != '=') && (is_base64(encoded[in_]))) {
			char_array_4[i++] = encoded[in_];
			in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++) {
					char_array_4[i] = base64_chars.find(char_array_4[i]);
				}
				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0x0F) << 4) + ((char_array_4[2] & 0x3C) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];
				for (i = 0; i < 3; i++) {
					decoded << char_array_3[i];
				}
				i = 0;
			}
		}
		if (i > 0) {
			for (j = i; j < 4; j++) {
				char_array_4[j] = 0;
			}
			for (j = 0; j < 4; j++) {
				char_array_4[j] = base64_chars.find(char_array_4[j]);
			}
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0x0F) << 4) + ((char_array_4[2] & 0x3C) >> 2);
			for (j = 0; j < i - 1; j++) {
				decoded << char_array_3[j];
			}
		}
		if (in_len > 0 || (i > 0 && in_len < 0)) {
			return "";
		}
		return decoded.str();
	}

	inline std::string gzipCompress(const std::string& data)
	{
		zng_stream deflateS; // zlib state
		deflateS.zalloc = Z_NULL;
		deflateS.zfree = Z_NULL;
		deflateS.opaque = Z_NULL;
		deflateS.avail_in = 0; // No data yet
		deflateS.next_in = Z_NULL; // No data yet

		// Initialize deflate using gzip method
		int ret = zng_deflateInit2(&deflateS, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
		if (ret != Z_OK) {
			throw std::runtime_error("Failed to initialize zlib deflate");
		}

		// Prepare output string to receive compressed data
		std::string output;
		std::vector<char> outbuffer(16384); // 16 KB chunks

		// Input data
		deflateS.avail_in = data.size();
		deflateS.next_in = reinterpret_cast<const Bytef*>(data.data());

		// Loop to deflate (compress)
		do {
			deflateS.avail_out = outbuffer.size();
			deflateS.next_out = reinterpret_cast<Bytef*>(outbuffer.data());
			ret = zng_deflate(&deflateS, Z_FINISH);
			if (output.size() < deflateS.total_out) {
				output.append(outbuffer.data(), deflateS.total_out - output.size());
			}
		} while (ret == Z_OK);

		// Clean up
		zng_deflateEnd(&deflateS);

		if (ret != Z_STREAM_END) { // an error occurred that was not EOF
			throw std::runtime_error("Exception during zlib compression: (" + std::to_string(ret) + ") " + deflateS.msg);
		}
		return base64_encode(output);
	}

	inline std::string r_string(const std::string& input) {
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		// Hash the input string to use as a seed for the random number generator
		std::hash<std::string> hasher;
		size_t seed = hasher(input);
		std::mt19937 gen(seed);
		std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

		std::string result(20, ' ');

		for (int i = 0; i < 20; ++i) {
			result[i] = alphanum[dis(gen)];
		}

		return result;
	};

	inline std::string gzipDecompress(const std::string& input_base64)
	{
		// Decode from Base64 to compressed binary
		std::string input_compressed = base64_decode(input_base64);

		zng_stream inflateS;
		inflateS.zalloc = Z_NULL;
		inflateS.zfree = Z_NULL;
		inflateS.opaque = Z_NULL;
		inflateS.avail_in = 0;
		inflateS.next_in = Z_NULL;

		// Initialize inflate using gzip method
		int ret = zng_inflateInit2(&inflateS, 31);
		if (ret != Z_OK) {
			throw std::runtime_error("Failed to initialize zlib inflate");
		}

		// Prepare input
		inflateS.avail_in = input_compressed.size();
		inflateS.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input_compressed.data()));

		// Prepare output
		std::string output;
		std::vector<char> outbuffer(16384);  // 16 KB chunks

		// Loop to inflate (decompress)
		do {
			inflateS.avail_out = outbuffer.size();
			inflateS.next_out = reinterpret_cast<Bytef*>(outbuffer.data());
			ret = zng_inflate(&inflateS, Z_NO_FLUSH);

			if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
				zng_inflateEnd(&inflateS);
				throw std::runtime_error("Exception during zlib decompression: (" + std::to_string(ret) + ")");
			}

			if (output.size() < inflateS.total_out) {
				output.append(outbuffer.data(), inflateS.total_out - output.size());
			}
		} while (inflateS.avail_out == 0);

		// Clean up
		zng_inflateEnd(&inflateS);

		return output;
	}

	inline std::string GetTextFromClipboard() {
		if (!OpenClipboard(nullptr)) {
			return ""; // Failed to open clipboard
		}

		HANDLE hData = GetClipboardData(CF_TEXT);
		if (!hData) {
			CloseClipboard();
			return ""; // Failed to get clipboard data
		}

		char* clipboardData = static_cast<char*>(GlobalLock(hData));
		if (!clipboardData) {
			CloseClipboard();
			return ""; // Failed to lock global memory
		}

		std::string clipboardText(clipboardData);

		GlobalUnlock(hData);
		CloseClipboard();

		return clipboardText;
	}

	inline bool CopyTextToClipboard(const std::string& text) {
		if (!OpenClipboard(nullptr)) {
			return false;
		}
		EmptyClipboard();
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
		if (!hGlobal) {
			CloseClipboard();
			return false;
		}
		char* clipboardData = static_cast<char*>(GlobalLock(hGlobal));
		strcpy_s(clipboardData, text.size() + 1, text.c_str());
		GlobalUnlock(hGlobal);
		SetClipboardData(CF_TEXT, hGlobal);
		CloseClipboard();
		return true;
	}

	inline std::string to_lowercase(const std::string& input_str) {
		std::string output_str = input_str;
		std::transform(output_str.begin(), output_str.end(), output_str.begin(), ::tolower);
		return output_str;
	}

	inline std::string steam_id_64(long id) {
		if (id != 0) {
			long long y;
			long long z;

			if (id % 2 == 0) {
				y = 0;
				z = id / 2;
			}
			else {
				y = 1;
				z = (id - 1) / 2;
			}

			return "7656119" + std::to_string((z * 2) + (7960265728 + y));
		}
		else {
			return "0";
		}
	}

	inline std::string removeNonAlphabetical(const std::string& input) {
		std::string result;

		for (char c : input) {
			if (std::isalnum(c)) {
				result += c;
			}
		}

		return result;
	}

	inline std::vector<std::unordered_map<std::string, std::string>> parseJSON(const std::string& jsonString) {
		std::vector<std::unordered_map<std::string, std::string>> result;

		size_t startPos = jsonString.find('{');
		while (startPos != std::string::npos) {
			std::unordered_map<std::string, std::string> keyValuePairs;

			// Find the end of the current object
			size_t endPos = jsonString.find('}', startPos);
			if (endPos == std::string::npos) {
				break; // Error: Missing closing brace
			}

			// Extract the object as a substring
			std::string objectString = jsonString.substr(startPos, endPos - startPos + 1);

			// Find and extract key-value pairs within the object
			size_t keyStartPos = 0;
			while (keyStartPos < objectString.size()) {
				size_t keyEndPos = objectString.find(':', keyStartPos);
				if (keyEndPos == std::string::npos) {
					break; // Error: Missing colon
				}

				// Extract key (within double quotes) and value
				size_t keyBegin = objectString.find('"', keyStartPos);
				size_t keyEnd = objectString.find('"', keyBegin + 1);
				size_t valueBegin = objectString.find('"', keyEnd + 1);
				size_t valueEnd = objectString.find('"', valueBegin + 1);

				if (keyBegin == std::string::npos || keyEnd == std::string::npos ||
					valueBegin == std::string::npos || valueEnd == std::string::npos) {
					break; // Error: Malformed JSON-like object
				}

				std::string key = objectString.substr(keyBegin + 1, keyEnd - keyBegin - 1);
				std::string value = objectString.substr(valueBegin + 1, valueEnd - valueBegin - 1);

				keyValuePairs[key] = value;
				keyStartPos = valueEnd + 1;
			}

			result.push_back(keyValuePairs);
			startPos = jsonString.find('{', endPos + 1);
		}

		return result;
	}
	inline std::string GenerateRandomString(int length) {
		const std::string charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		const int charsetSize = charset.size();

		std::string result;
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		for (int i = 0; i < length; ++i) {
			int randomIndex = std::rand() % charsetSize;
			result += charset[randomIndex];
		}

		return result;
	}

	inline std::string floatToString(float value) {
		std::ostringstream stream;
		stream << value;
		std::string result = stream.str();

		// Find the position of the decimal point
		size_t decimalPos = result.find('.');

		if (decimalPos != std::string::npos) {
			// Remove everything after (and including) the decimal point
			result.erase(decimalPos);
		}

		return result;
	};

	//POTENTIALLY DONT NEED THIS ANYMORE
	inline std::string encrypt(const std::string& input, const std::string& key) {
		std::ostringstream os;
		for (size_t i = 0; i < input.length(); i++) {
			char encrypted_char = input[i] ^ key[i % key.length()];
			os << std::setw(2) << std::setfill('0') << std::hex << (int)encrypted_char;
		}
		return os.str();
	}

	inline std::clock_t getCurrentTimeMillis() {
		return std::clock() * 1000 / CLOCKS_PER_SEC;
	}

	inline std::string GetAppDataFolderPath() {
		TCHAR szPath[MAX_PATH];
		const std::string subFolder = "Xene";
		std::string folderPath;

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath))) {
			folderPath.assign(szPath);
			folderPath += "\\" + subFolder;
		}
		else {
			std::cerr << "Failed to get the AppData folder path." << std::endl;
		}

		return folderPath;
	}


	inline bool WriteToAppDataFolder(const std::string& file, const std::string& content) {
		TCHAR szPath[MAX_PATH];
		const std::string subFolder = "Xene";

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath))) {
			std::string folderPath(szPath);
			folderPath += "\\" + subFolder;
			std::string filePath = folderPath + "\\" + file;

			// Optionally, create the directory if it doesn't exist
			CreateDirectory(folderPath.c_str(), NULL);

			std::ofstream outputFile(filePath);
			if (!outputFile.is_open()) {
				std::cerr << "Failed to open the file for writing." << std::endl;
				return false;
			}

			outputFile << content;
			outputFile.close();

			std::cout << "Content successfully written to: " << filePath << std::endl;
			return true;
		}
		else {
			std::cerr << "Failed to get the AppData folder path." << std::endl;
			return false;
		}
	}

	inline bool ReadFromAppDataFolder(const std::string& fileName, std::string& content) {
		TCHAR szPath[MAX_PATH];
		const std::string subFolder = "Xene";

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath))) {
			std::string folderPath(szPath);
			folderPath += "\\" + subFolder;
			std::string filePath = folderPath + "\\" + fileName;

			std::ifstream inputFile(filePath);
			if (!inputFile.is_open()) {
				std::cerr << "Failed to open the file for reading." << std::endl;
				return false;
			}

			// Read the content of the file into the provided string
			content = std::string((std::istreambuf_iterator<char>(inputFile)),
				std::istreambuf_iterator<char>());

			inputFile.close();

			return true;
		}
		else {
			std::cerr << "Failed to get the AppData folder path." << std::endl;
			return false;
		}
	}
}