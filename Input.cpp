#include "Input.h"
using namespace std;

bool InputClear() {
	bool buffer_clearing_flag = true;

	char temp;
	while ((temp = getchar()) != '\n' && temp != EOF) {
		buffer_clearing_flag = false;
	}

	return buffer_clearing_flag;
}

char* StrInput(int& variable_size) {
	int symb_index = 0;
	char* new_variable = new char[variable_size + 1];

	char symb = getchar();
	while (symb != '\n') {
		if (symb_index == variable_size) {
			new_variable = IncreaseBuffer(new_variable, variable_size);
		}
		new_variable[symb_index++] = symb;
		symb = getchar();
	}

	if (symb_index == variable_size) {
		new_variable = IncreaseBuffer(new_variable, variable_size);
	}
	new_variable[symb_index] = '\0';

	return new_variable;
}

char* IncreaseBuffer(char* buffer, int& buffer_size) {
	char* new_buffer = new char[buffer_size * 2];

	for (int i = 0; i < buffer_size; ++i)
	{
		new_buffer[i] = buffer[i];
	}

	delete[] buffer;
	buffer_size *= 2;
	return new_buffer;
}

bool CorrectIntInput(char* input_str) {
	int input_str_len = strlen(input_str);
	bool correct_flag = true;

	for (int i = 0; i < input_str_len; i++) {
		if (not (isdigit(input_str[i]) || input_str[0] == '-')) {
			correct_flag = false;
			break;
		}
	}

	if (correct_flag) {
		if (input_str[0] == '-') {
			if (strlen(input_str) == strlen(max_variables_sizes::CharMinIntValue) && input_str < max_variables_sizes::CharMinIntValue) return true;
			else if (strlen(input_str) < strlen(max_variables_sizes::CharMinIntValue)) return true;
		}
		else {
			if (strlen(input_str) == strlen(max_variables_sizes::CharIntValue) && input_str < max_variables_sizes::CharIntValue) return true;
			else if (strlen(input_str) < strlen(max_variables_sizes::CharIntValue)) return true;
		}
	}
	return false;
}

long long ProgramStartInputs(long long min_limit, long long max_limit, string& error_message, int buffer_size) {
	bool input_flag = true;
	int entered_num;
	while (input_flag) {
		char* entered_num_str = StrInput(buffer_size);

		if (CorrectIntInput(entered_num_str)) {
			entered_num = atoi(entered_num_str);
			delete[] entered_num_str;

			if (entered_num < min_limit || entered_num > max_limit) {
				cout << error_message;
			}
			else {
				input_flag = false;
			}
		}
		else printf("Некорректный ввод! Попробуйте еще раз: ");
	}

	return entered_num;
}
