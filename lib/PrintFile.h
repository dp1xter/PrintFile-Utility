#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <cstring>

static const size_t kMaxBufferSize = 8192;
static const size_t kConsoleCodePage = 1251;
static const size_t kMinimalLengthOfLinesArgument = 8;
static const size_t kMinimalLengthOfDelimiterArgument = 12;

//Output settings
struct ConfigFile {
    bool output_direction = true;
    bool input_delimiter_check = false;
    long long number_of_rows = -1;
    char delimiter = '\n';
    std::filesystem::path file_path;
};

char DelimiterParameterValidationCheck(ConfigFile& config, const char* delimiter);

long long LinesParameterValidationCheck(const char* number_of_rows);

void ParsingCommands(ConfigFile& config, int argc, char* argv[]);

void PrintFileFromBeg(const ConfigFile& config, std::ifstream& file_in, char* buffer, size_t file_size,
                      size_t size_of_buffer);

void PrintFileFromEnd(const ConfigFile& config, std::ifstream& file_in, char* buffer, size_t file_size,
                      bool the_end_flag, size_t size_of_buffer);

void CheckAndStartWorkingWithFile(const ConfigFile& config);
