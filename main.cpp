#include <iostream>
#include <ctime>
#include <chrono>
#include <windows.h>

#include "MainMenu.h"
#include "MultiSet.h"
#include "settings.h"

using namespace std;
using namespace std::chrono;

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));

	while (settings::programShouldWork) {
		ProgramStart();
	}
};