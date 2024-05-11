#include <windows.h>
#include "settings.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <variant>
#include "game/classes.hpp"

namespace config
{
    inline std::string GetTempDirectory() {
        TCHAR tempPath[MAX_PATH];
        if (GetTempPath(MAX_PATH, tempPath) == 0) {
            // Handle error, e.g., unable to get temp directory
            return "";
        }
        return std::string(tempPath);
    }

    inline bool FileExists(const std::string& filePath) {
        //bool chamsValue = ui_element.food.value;

        DWORD fileAttributes = GetFileAttributes(filePath.c_str());
        return (fileAttributes != INVALID_FILE_ATTRIBUTES &&
            !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
    }

    inline bool WriteToTempFile(const std::string& file, const std::string& content) {
        char* tmpDir;
        if (_dupenv_s(&tmpDir, nullptr, "TMP") != 0 || tmpDir == nullptr) {
            std::cerr << "Failed to get the TMP directory path." << std::endl;
            return false;
        }

        std::string tempDir = GetTempDirectory();
        if (tempDir.empty()) {
            // Handle error, e.g., unable to get temp directory
            return false;
        }

        // Generate a unique temporary file name
        std::string tempFilePath = tempDir + "\\" + file;

        // Open the temporary file for writing
        std::ofstream outputFile(tempFilePath);
        if (!outputFile.is_open()) {
            std::cerr << "Failed to open the temporary file for writing." << std::endl;
            free(tmpDir); // Don't forget to free the allocated memory
            return false;
        }

        // Write the content to the temporary file
        outputFile << content;
        outputFile.close();

        std::cout << "Content successfully written to: " << tempFilePath << std::endl;

        // Don't forget to free the allocated memory
        free(tmpDir);

        return true;
    }

    inline bool GetFileContentFromTemp(const std::string& fileName, std::string& content) {
        std::string tempDir = GetTempDirectory();
        if (tempDir.empty()) {
            // Handle error, e.g., unable to get temp directory
            return false;
        }

        std::string filePath = tempDir + "\\" + fileName;
        if (!FileExists(filePath)) {
            // Handle error, file doesn't exist
            return false;
        }

        std::ifstream file(filePath);
        if (!file.is_open()) {
            // Handle error, unable to open file
            return false;
        }

        // Read the content of the file
        content = std::string((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        return true;
    }
	void config_load() {
		std::string content;
		if (GetFileContentFromTemp("xsr.config", content)) {
			std::string line;
			std::istringstream iss(content);
			while (std::getline(iss, line)) {
				// Check if the line's length is greater than 1
				if (line.length() > 1) {
					// Process the line here
					size_t spacePos = line.find(" ");

					if (spacePos != std::string::npos) {
						// Extract the first part (before the space)
						std::string varName = line.substr(0, spacePos);
						std::string varValue = line.substr(spacePos + 1);

						if (ui::element.find(varName) != ui::element.end()) {
							uiElement& element = ui::element[varName];

							if (std::holds_alternative<bool>(element.value)) {
								if (varValue == "true") {
									std::get<bool>(ui::element[varName].value) = true;
								}
								else if (varValue == "false") {
									std::get<bool>(ui::element[varName].value) = false;
								}
							}
							else if (std::holds_alternative<float>(element.value)) {
								std::get<float>(ui::element[varName].value) = std::stof(varValue);
							}
							else if (std::holds_alternative<int>(element.value)) {
								//std::get<int>(ui::element[name].value) = std::stoi(varValue);
							}
							else if (std::holds_alternative<KeyCode>(element.value)) {
								for (const auto& entry : keyCodeToString) {
									if (entry.second == varValue) {
										std::get<KeyCode>(ui::element[varName].value) = entry.first;
										break;  // Stop searching once found
									}
								}
								//config_output << global::keyCodeToString[std::get<KeyCode>(element.value)];
							}
						}
						//std::cout << "Line: " << line << std::endl;
					}
				}
			}
		}
	};
	void config_save() {
		std::ostringstream config_output;
		for (const auto& [name, element] : ui::element) {
			if (std::holds_alternative<bool>(element.value)) {
				//config_output << std::get<bool>(element.value);
				if (std::get<bool>(ui::element[name].value)) {
					config_output << name << " true";
				}
				else {
					config_output << name << " false";
				}
			}
			else if (std::holds_alternative<float>(element.value)) {
				config_output << name << " " << std::get<float>(ui::element[name].value);
			}
			else if (std::holds_alternative<int>(element.value)) {
				config_output << name << " " << std::get<int>(ui::element[name].value);
			}
			else if (std::holds_alternative<KeyCode>(element.value)) {
				config_output << name << " " << keyCodeToString[std::get<KeyCode>(element.value)];
			}
			config_output << std::endl;
		}

		std::string jsonOutputStr = config_output.str();
		WriteToTempFile("xsr.config", jsonOutputStr);
	};
}