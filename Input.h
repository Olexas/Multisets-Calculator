#pragma once

#include <iostream>
#include "settings.h"

using namespace std;

bool InputClear();
char* StrInput(int& variable_size);
char* IncreaseBuffer(char* buffer, int& buffer_size);
bool CorrectIntInput(char* input_str);
long long ProgramStartInputs(long long min_limit, long long max_limit, string& error_message, int buffer_size);
