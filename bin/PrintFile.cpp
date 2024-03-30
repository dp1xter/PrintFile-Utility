#include "PrintFile.h"

bool IsNumeric(const char* number) {
    while (*number != '\0') {
        if (*number < '0' || *number > '9') {
            return false;
        }
        ++number;
    }

    return true;
}

void RaiseError(const char* message) {
    std::cerr << message;
    exit(EXIT_FAILURE);
}

char DelimiterParameterValidationCheck(ConfigFile& config, const char* delimiter) {
    //Delimiter input ' ' or " " or without quotation marks
    if (!strcmp(delimiter, "\'\\n\'") || !strcmp(delimiter, "\\n") || !strcmp(delimiter, "\"\\n\"")) {
        return '\n';
    } else if (!strcmp(delimiter, "\'\\t\'") || !strcmp(delimiter, "\\t") || !strcmp(delimiter, "\"\\t\"")) {
        return '\t';
    } else if (!strcmp(delimiter, "\'\\a\'") || !strcmp(delimiter, "\\a") || !strcmp(delimiter, "\"\\a\"")) {
        return '\a';
    } else if (!strcmp(delimiter, "\'\\b\'") || !strcmp(delimiter, "\\b") || !strcmp(delimiter, "\"\\b\"")) {
        return '\b';
    } else if (!strcmp(delimiter, "\'\\f\'") || !strcmp(delimiter, "\\f") || !strcmp(delimiter, "\"\\f\"")) {
        return '\f';
    } else if (!strcmp(delimiter, "\'\\r\'") || !strcmp(delimiter, "\\r") || !strcmp(delimiter, "\"\\r\"")) {
        return '\r';
    } else if (!strcmp(delimiter, "\'\\0\'") || !strcmp(delimiter, "\\0") || !strcmp(delimiter, "\"\\0\"")) {
        return '\0';
    } else if (delimiter[0] == '\'' || delimiter[0] == '\"') {
        if (strlen(delimiter) != 3 || delimiter[strlen(delimiter) - 1] != delimiter[0]) {
            RaiseError("Invalid delimiter parameter.");
        }
        config.input_delimiter_check = true;

        return delimiter[1];

    } else if (strlen(delimiter) > 1 || strlen(delimiter) == 0) {
        RaiseError("Invalid delimiter parameter.");
    }
    config.input_delimiter_check = true;

    return delimiter[0];
}

long long LinesParameterValidationCheck(const char* number_of_rows) {
    if (!IsNumeric(number_of_rows)) {
        RaiseError("Invalid lines parameter.");
    }

    return std::stoll(number_of_rows);
}

void ParsingCommands(ConfigFile& config, int argc, char* argv[]) {
    bool waiting_parameter_rows = false;
    bool waiting_parameter_delimiter = false;
    size_t parameter_length;

    for (int i = 1; i < argc; ++i) {
        parameter_length = strlen(argv[i]);
        if (parameter_length == 0) {
            RaiseError("Invalid commands.");
        } else if (waiting_parameter_rows) {
            config.number_of_rows = LinesParameterValidationCheck(argv[i]);
            waiting_parameter_rows = false;
        } else if (waiting_parameter_delimiter) {
            config.delimiter = DelimiterParameterValidationCheck(config, argv[i]);
            waiting_parameter_delimiter = false;
        } else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--tail")) {
            config.output_direction = false;
        } else if (!strcmp(argv[i], "-l") && config.number_of_rows == -1) {
            waiting_parameter_rows = true;
        } else if (!strcmp(argv[i], "-d") && !config.input_delimiter_check) {
            waiting_parameter_delimiter = true;
        } else if (!strncmp(argv[i], "--lines=", kMinimalLengthOfLinesArgument) && config.number_of_rows == -1 && parameter_length > kMinimalLengthOfLinesArgument) {
            config.number_of_rows = LinesParameterValidationCheck(&argv[i][kMinimalLengthOfLinesArgument]);
        } else if (!strncmp(argv[i], "--delimiter=", kMinimalLengthOfDelimiterArgument) && !config.input_delimiter_check && parameter_length > kMinimalLengthOfDelimiterArgument) {
            config.delimiter = DelimiterParameterValidationCheck(config, &argv[i][kMinimalLengthOfDelimiterArgument]);
        } else if (config.file_path.empty()) {
            config.file_path = argv[i];
        } else {
            RaiseError("Invalid commands.");
        }
    }
    if (waiting_parameter_delimiter || waiting_parameter_rows) {
        RaiseError("Invalid commands.");
    }
}

void PrintFileFromBeg(const ConfigFile& config, std::ifstream& file_in, char* buffer, size_t file_size,
                      size_t size_of_buffer) {
    long long number_of_rows = config.number_of_rows;
    file_in.seekg(0, std::ios::beg);

    //Output with delimiter counting
    while (file_in.tellg() != file_size && number_of_rows) {
        size_of_buffer = (file_size - file_in.tellg() < kMaxBufferSize) ? file_size - file_in.tellg() : kMaxBufferSize;
        file_in.read(buffer, size_of_buffer);

        for (int i = 0; i < size_of_buffer && number_of_rows; ++i) {
            if (buffer[i] == config.delimiter) {
                --number_of_rows;
            }
            std::cout << buffer[i];
        }
    }
}

void PrintFileFromEnd(const ConfigFile& config, std::ifstream& file_in, char* buffer, size_t file_size,
                      bool the_end_flag, size_t size_of_buffer) {
    size_t pointer_to_left_side_of_buffer_block = file_in.tellg();
    size_t pointer_to_start_of_output = 0;
    long long number_of_rows = config.number_of_rows;

    //Finding the beginning of output in a file
    while (pointer_to_left_side_of_buffer_block > 0 && !the_end_flag) {
        size_of_buffer = (pointer_to_left_side_of_buffer_block < kMaxBufferSize) ? pointer_to_left_side_of_buffer_block : kMaxBufferSize;
        pointer_to_left_side_of_buffer_block -= size_of_buffer;
        file_in.seekg(pointer_to_left_side_of_buffer_block);
        file_in.read(buffer, size_of_buffer);

        for (int i = size_of_buffer - 1; i >= 0; --i) {
            if (buffer[i] == config.delimiter) {
                --number_of_rows;
            }
            if (!number_of_rows) {
                the_end_flag = true;
                pointer_to_start_of_output = pointer_to_left_side_of_buffer_block + i + 1;
                break;
            }
        }
    }
    file_in.seekg(pointer_to_start_of_output);

    //Output from current pointer
    while (file_in.tellg() != file_size) {
        size_of_buffer = (file_size - file_in.tellg() < kMaxBufferSize) ? file_size - file_in.tellg() : kMaxBufferSize;
        file_in.read(buffer, size_of_buffer);

        for (int i = 0; i < size_of_buffer; ++i) {
            std::cout << buffer[i];
        }
    }
}

void CheckAndStartWorkingWithFile(const ConfigFile& config) {
    std::ifstream file_in(config.file_path, std::ios::binary);

    //Checking for file availability
    if (!file_in.is_open()) {
        RaiseError("No such file or directory.");
    }

    //Transfer to output
    char buffer[kMaxBufferSize];
    file_in.seekg(0, std::ios::end);
    size_t file_size = file_in.tellg();
    bool the_end_flag = false;
    size_t size_of_buffer = kMaxBufferSize;
    if (config.output_direction || config.number_of_rows == -1) {
        PrintFileFromBeg(config, file_in, buffer, file_size, size_of_buffer);
    } else {
        PrintFileFromEnd(config, file_in, buffer, file_size, the_end_flag, size_of_buffer);
    }
}

int main(int argc, char* argv[]) {
    ConfigFile config;
    
    //Changing CodePage in console
    SetConsoleCP(kConsoleCodePage);
    SetConsoleOutputCP(kConsoleCodePage);

    ParsingCommands(config, argc, argv);
    CheckAndStartWorkingWithFile(config);
}
