#pragma once
#include <windows.h>
#include "settings.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <variant>
#include "game/classes.hpp"
#include "global.h"
#include "notifications.h"

namespace config
{
	inline extern std::vector<std::string> config_options = { };
	inline extern std::vector<configHotkeys> config_hotkeys = { };
	inline extern std::string loaded_config = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

	// Function to serialize dropdown states to a string
	inline std::string serializeDropdownStates() {
		std::string result;
		for (const auto& dropdown : dropdowns::dropdown_states) {
			result += dropdown.first + "|";
			for (const auto& option : dropdown.second) {
				result += option.first + ":" + std::to_string(option.second) + ",";
			}
			result += ";";
		}
		return result;
	}

	// Function to deserialize dropdown states from a string
	inline void deserializeDropdownStates(const std::string& serialized) {
		dropdowns::dropdown_states.clear();
		std::string dropdownName, optionName, optionState;
		std::unordered_map<std::string, bool> currentOptions;

		for (char c : serialized) {
			if (c == '|') {
				currentOptions.clear();
				dropdownName = optionState;
				optionState.clear();
			}
			else if (c == ';') {
				dropdowns::dropdown_states[dropdownName] = currentOptions;

				optionState.clear();
			}
			else if (c == ':') {
				optionName = optionState;
				optionState.clear();
			}
			else if (c == ',') {
				currentOptions[optionName] = std::stoi(optionState);
				optionState.clear();
			}
			else {
				optionState += c;
			}
		}

	}
	inline void config_import() {
		std::string clipboard_contents = global::GetTextFromClipboard();
		std::string base64 = global::base64_decode(clipboard_contents);
		if (base64 != "") {
			std::string line;
			std::istringstream iss(base64);

			while (std::getline(iss, line)) {
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
							else if (std::holds_alternative<Vector4>(element.value)) {
								if (std::count(varValue.begin(), varValue.end(), ',') == 3) {
									// Split the string using a stringstream
									std::vector<std::string> tokens;
									std::stringstream ss(varValue);
									std::string token;

									while (std::getline(ss, token, ',')) {
										tokens.push_back(token);
									}

									try {
										float num1 = std::stof(tokens[0]);
										float num2 = std::stof(tokens[1]);
										float num3 = std::stof(tokens[2]);
										float num4 = std::stof(tokens[3]);

										std::get<Vector4>(ui::element[varName].value).x = num1;
										std::get<Vector4>(ui::element[varName].value).y = num2;
										std::get<Vector4>(ui::element[varName].value).z = num3;
										std::get<Vector4>(ui::element[varName].value).w = num4;
									}
									catch (const std::invalid_argument& e) {}
									catch (const std::out_of_range& e) {}
								}
							}
							else if (varName == "dropdownStates") {
								deserializeDropdownStates(varValue);
							}
						}
					}
				}
			}
			notifications::add_notification("Imported from clipboard", 6);
		}
		else {
			notifications::add_notification("No config copied to clipboard", 6);
		}
	}

	inline void config_export() {
		std::string serializedDropdownStates = serializeDropdownStates();
		ui::element["dropdownStates"].value = serializedDropdownStates;
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
			else if (std::holds_alternative<Vector4>(element.value)) {
				config_output << name << " " << std::get<Vector4>(ui::element[name].value).x << "," << std::get<Vector4>(ui::element[name].value).y << "," << std::get<Vector4>(ui::element[name].value).z << "," << std::get<Vector4>(ui::element[name].value).w;
			}
			config_output << std::endl;
		}

		config_output << "dropdownStates " << serializedDropdownStates << std::endl;
		std::string jsonOutputStr = config_output.str();

		global::CopyTextToClipboard(global::base64_encode(jsonOutputStr));
		notifications::add_notification("Exported config to clipboard", 6);
	}

	inline void extract_sections(const std::string& file_content, std::vector<std::string>& sections) {
		std::string delimiter = "---------";
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		while ((pos_end = file_content.find(delimiter, pos_start)) != std::string::npos) {
			sections.push_back(file_content.substr(pos_start, pos_end - pos_start));
			pos_start = pos_end + delim_len;
		}
		sections.push_back(file_content.substr(pos_start));
	}

	inline bool find_line(const std::string& str, const std::string& target_line) {
		std::istringstream stream(str);
		std::string line;
		while (std::getline(stream, line)) {
			if (line == target_line) {
				return true;
			}
		}
		return false;
	}

	inline void populate_configs() {
		config_options.clear();
		config_hotkeys.clear();
		std::string content;
		if (global::ReadFromAppDataFolder("xsr.config", content)) {
			if (!find_line(content, "---------")) {
				global::WriteToAppDataFolder("xsr.config", "");
				return;
			}
			std::vector<std::string> sections;
			extract_sections(content, sections);
			for (const auto& section : sections) {
				std::string line;
				std::istringstream iss(section);
				bool should_hotkey = false;
				std::string conf_name = "";
				if (find_line(section, "config_hotkey_enable true")) {
					should_hotkey = true;
				}

				while (std::getline(iss, line)) {
					if (line.length() > 1) {
						// Process the line here
						size_t spacePos = line.find(" ");

						if (spacePos != std::string::npos) {
							// Extract the first part (before the space)
							std::string varName = line.substr(0, spacePos);
							std::string varValue = line.substr(spacePos + 1);

							if (varName == "config_name") { //config_hotkeys
								config_options.push_back(varValue);
								conf_name = varValue;
							}
							if (varName == "config_hotkey" && should_hotkey) { //config_hotkeys
								for (const auto& entry : keyCodeToString) {
									if (entry.second == varValue) {
										configHotkeys new_hotkey;
										new_hotkey.config_name = conf_name;
										new_hotkey.hotkey = entry.first;
										config_hotkeys.push_back(new_hotkey);
										break;  // Stop searching once found
									}
								}
							}
						}
					}
				}
			}
		}
	}

	inline void config_load(std::string config_name) {
		if (config_name.size() > 1) {
			std::string content;
			if (global::ReadFromAppDataFolder("xsr.config", content)) {
				std::vector<std::string> sections;
				bool found_config = false;
				extract_sections(content, sections);
				for (const auto& section : sections) {
					if (!found_config) {
						std::string line;
						std::istringstream iss(section);

						while (std::getline(iss, line)) {
							if (line.length() > 1) {
								// Process the line here
								size_t spacePos = line.find(" ");

								if (line != "---------") {
									if (spacePos != std::string::npos) {
										// Extract the first part (before the space)
										std::string varName = line.substr(0, spacePos);
										std::string varValue = line.substr(spacePos + 1);

										if (varName == "config_name" && varValue == config_name) {
											found_config = true;
											notifications::add_notification("Loaded " + config_name, 6);
											loaded_config = config_name;
											ui::config_option = config_name;
										}
										if (found_config) {
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
												else if (std::holds_alternative<Vector4>(element.value)) {
													if (std::count(varValue.begin(), varValue.end(), ',') == 3) {
														// Split the string using a stringstream
														std::vector<std::string> tokens;
														std::stringstream ss(varValue);
														std::string token;

														while (std::getline(ss, token, ',')) {
															tokens.push_back(token);
														}

														try {
															float num1 = std::stof(tokens[0]);
															float num2 = std::stof(tokens[1]);
															float num3 = std::stof(tokens[2]);
															float num4 = std::stof(tokens[3]);

															std::get<Vector4>(ui::element[varName].value).x = num1;
															std::get<Vector4>(ui::element[varName].value).y = num2;
															std::get<Vector4>(ui::element[varName].value).z = num3;
															std::get<Vector4>(ui::element[varName].value).w = num4;
														}
														catch (const std::invalid_argument& e) {}
														catch (const std::out_of_range& e) {}
													}
												}
												else if (varName == "dropdownStates") {
													deserializeDropdownStates(varValue);
												}
											}
										}
									}
								}
							}
						}
					}
					else {
						break;
					}
				}
			}
		}
		else {
			notifications::add_notification("Config name not long enough", 6);
		}
	};

	inline void config_delete(std::string config_name) {
		std::string content;
		if (global::ReadFromAppDataFolder("xsr.config", content)) {
			//bool found_config
			std::vector<std::string> sections;
			extract_sections(content, sections);
			content = "";
			std::string temp_content = "";
			for (auto it = sections.begin(); it != sections.end(); ++it) {
				//std::cout << "Section:\n" << section << std::endl;
				std::string section = *it;
				std::string line;
				std::istringstream iss(section);
				bool should_add = true;

				std::ostringstream config_output;
				config_output << "---------";
				config_output << std::endl;

				while (std::getline(iss, line)) {
					if (line.length() > 1) {
						// Process the line here
						size_t spacePos = line.find(" ");

						if (spacePos != std::string::npos) {
							// Extract the first part (before the space)
							std::string varName = line.substr(0, spacePos);
							std::string varValue = line.substr(spacePos + 1);
							if (varName == "config_name" && varValue == config_name) {
								should_add = false;
								notifications::add_notification("Deleted " + config_name, 6);
								ui::config_option = "";
								//break;
							}
							config_output << varName << " " << varValue;
							config_output << std::endl;
						}
					}
				}
				if (should_add) {
					if (section.size() > 4) {
						config_output << "---------";
						if (std::next(it) != sections.end()) {
							config_output << std::endl;
						}
						temp_content = temp_content + config_output.str();
					}
				}
			}
			content = temp_content;
		}
		else {
			notifications::add_notification("Couldnt get configs", 6);
			return;
		}
		global::WriteToAppDataFolder("xsr.config", content);
		populate_configs();
	}

	inline void config_save(std::string config_name) {
		if (config_name.size() > 1) {
			if (15 > config_options.size()) {
				std::string content;
				if (global::ReadFromAppDataFolder("xsr.config", content)) {
					std::vector<std::string> sections;
					extract_sections(content, sections);
					content = "";
					std::string content_temp = "";

					for (auto it = sections.begin(); it != sections.end(); ++it) {
						//std::cout << "Section:\n" << section << std::endl;
						std::string section = *it;
						std::string line;
						std::istringstream iss(section);
						bool should_add = true;

						std::ostringstream config_output;
						config_output << "---------";
						config_output << std::endl;

						while (std::getline(iss, line)) {
							if (line.length() > 1) {
								// Process the line here
								size_t spacePos = line.find(" ");

								if (spacePos != std::string::npos) {
									// Extract the first part (before the space)
									std::string varName = line.substr(0, spacePos);
									std::string varValue = line.substr(spacePos + 1);
									if (varName == "config_name" && varValue == config_name) {
										should_add = false;
										ui::config_option = config_name;
										//break;
									}
									config_output << varName << " " << varValue;
									config_output << std::endl;
								}
							}
						}
						//std::cout << "section: " << section << std::endl;
						if (should_add) {
							if (section.size() > 4) {
								config_output << "---------";
								if (std::next(it) != sections.end()) {
									config_output << std::endl;
								}
								content_temp = content_temp +  config_output.str();
							}
						}
					}
					content = content_temp;
				}
				else {
					content = "";
				}

				std::string serializedDropdownStates = serializeDropdownStates();
				ui::element["dropdownStates"].value = serializedDropdownStates;
				std::ostringstream config_output;
				config_output << "---------";
				config_output << std::endl;
				config_output << "config_name " << config_name;
				config_output << std::endl;
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
					else if (std::holds_alternative<Vector4>(element.value)) {
						config_output << name << " " << std::get<Vector4>(ui::element[name].value).x << "," << std::get<Vector4>(ui::element[name].value).y << "," << std::get<Vector4>(ui::element[name].value).z << "," << std::get<Vector4>(ui::element[name].value).w;
					}
					config_output << std::endl;
				}

				notifications::add_notification("Saved " + config_name, 6);

				config_output << "dropdownStates " << serializedDropdownStates << std::endl;
				config_output << "---------";
				std::string jsonOutputStr = config_output.str();

				global::WriteToAppDataFolder("xsr.config", (content.empty() ? "" : content) + jsonOutputStr);
				populate_configs();
			}
			else {
				notifications::add_notification("Config limit reached", 6);
			}
		}
		else {
			notifications::add_notification("Config name not long enough", 6);
		}
	};
}