#include <windows.h>
#include <iostream>

#include "MainMenu.h"
#include "Input.h"

using namespace std;

void Screen::Clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

Screen::Screen(Universum& universum, BaseSet& set1, BaseSet& set2) {
	this->universum = &universum;
	this->setA = &set1;
	this->setB = &set2;
}

Screen::Screen(EmptySet& universum, BaseSet& set1, BaseSet& set2) {
	this->universum = &universum;
	this->setA = &set1;
	this->setB = &set2;
}

void Screen::PrintMainMenu() {
	if (settings::programShouldEnd) return;
	Clear();
	cout << "������� ����!\n";
	cout << "------------------------------------------------------------------------------------\n";
	cout << "������� ���������:\n";
	cout << endl << *this->universum << endl << *this->setA << endl << *this->setB << endl;
	cout << "------------------------------------------------------------------------------------\n";
	cout << "[1] - ����������� ��������.\n";
	cout << "[2] - ����������� ��������.\n";
	cout << "[3] - �������� ��������.\n";
	cout << "[4] - �������������� �������� ��������.\n";
	cout << "[5] - ���������� ��������.\n";
	cout << "[6] - �������������� �������� ��� �����������.\n";
	cout << "------------------------------------------------------------------------------------\n";
	cout << "[7] - ������������� ���������.\n";
	cout << "[8] - �������� ��������� A.\n";
	cout << "[9] - �������� ��������� B.\n";
	cout << "------------------------------------------------------------------------------------\n";
	cout << "[x] - ����� �� ���������.\n";
	cout << "------------------------------------------------------------------------------------\n\n����: ";

	bool main_menu_choice_flag = true;
	while (main_menu_choice_flag) {
		char user_main_menu_input = getchar();
		bool one_symb_input_flag = InputClear();

		if (one_symb_input_flag) {
			void (Screen:: * user_choice)() = nullptr;

			for (int i = 0; i < sizeof(main_menu_choices) / sizeof(main_menu_choices[0]); i++) {
				if (user_main_menu_input == main_menu_choices[i].symb) {
					user_choice = main_menu_choices[i].func;
					break;
				}
			}

			if (user_choice) {
				if (user_main_menu_input == 'x' || user_main_menu_input == '7' ||
					user_main_menu_input == '8' || user_main_menu_input == '9') (*this.*user_choice)();
				else PrintActionMenu(user_choice);

				main_menu_choice_flag = false;
			}
			else cout << "�� ����������� � ��������������� ��������, ���������� ��� ���!\n\n����: ";
		}
		else cout << "������������ ����! ���������� ��� ���: ";
	}
}

void Screen::PrintActionMenu(void (Screen:: *user_choice)()) {
	if (settings::programShouldEnd) return;

	Clear();
	cout << "------------------------------------------------------------------------------------------------------------------------------\n";

	cout << *this->universum << endl << *this->setA << endl << *this->setB << endl;
	(*this.*user_choice)();

	cout << "\n������� ����� ������, ����� ��������� � ���� : ";
	char user_answer = getchar();

	PrintMainMenu();
}

void Screen::PrintSetUnionMenu() {
	cout << *SetUnion(*setA, *setB);
}

void Screen::PrintSetIntersectionMenu() {
	cout << *SetIntersection(*setA, *setB);
}

void Screen::PrintDifferenceMenu() {
	cout << *SetDifference(*setA, *setB) << endl;
	cout << *SetDifference(*setB, *setA) << endl;
}

void Screen::PrintXorMenu() {
	cout << *SetXor(*setA, *setB) << endl;
}

void Screen::PrintComplementMenu() {
	EmptySet* emptyset = dynamic_cast<EmptySet*>(this->universum);
	if (emptyset) {
		Universum u(1, "");
		EmptySet aa(u, "!A");
		EmptySet bb(u, "!B");
		cout << "U: " << aa << endl << "U: " << bb << endl;
		return;
	}

	cout << *!*setA << endl << *!*setB << endl;
}

void Screen::PrintArithmeticMenu() {
	cout << *(*setA + *setB) << endl;
	cout << *(*setA - *setB) << endl;
	cout << *(*setA * *setB) << endl;
	cout << *(*setA / *setB) << endl;
	cout << *(*setB + *setA) << endl;
	cout << *(*setB - *setA) << endl;
	cout << *(*setB * *setA) << endl;
	cout << *(*setB / *setA) << endl;
}

void Screen::ProgramStop() {
	settings::programShouldEnd = true;
	settings::programShouldWork = false;
}

void Screen::ProgramRestart() {
	settings::programShouldEnd = true;
	this->Clear();
}

void Screen::EditSetA() {
	MultiSet a;
	this->setA = &a;
	this->Clear();
	Universum* universum1 = dynamic_cast<Universum*>(this->universum);
	GenerateMultisetMenu(*setA, *universum1, "A");

	PrintMainMenu();
}

void Screen::EditSetB() {
	MultiSet b;
	this->setB = &b;
	this->Clear();
	Universum* universum1 = dynamic_cast<Universum*>(this->universum);
	GenerateMultisetMenu(*setB, *universum1, "B");

	PrintMainMenu();
}

void ProgramStart() {
	settings::programShouldEnd = false;

	cout << "���������� ������� ���������! ������� ��� ����������� (�� 0 �� 24): ";
	string error_message = "\n�� ����� ������������ �����������\n���������� ��� ��� : ";
	int n = ProgramStartInputs(0, 24, error_message, buffer_sizes::n_int_size);

	if (n == 0) {
		Universum TempUniversum(n, "U");
		EmptySet MainEmptySet(TempUniversum, "U");
		EmptySet a(TempUniversum, "A");
		EmptySet b(TempUniversum, "B");
		Screen screen(MainEmptySet, a, b);
		screen.PrintMainMenu();
	}
	else {
		Universum MainUniversum(n, "U");

		cout << endl << "��� ���������:" << endl;
		cout << MainUniversum << endl;

		MultiSet a; 
		GenerateMultisetMenu(a, MainUniversum, "A");
		cout << endl << "���������� ���������:" << endl << a << endl;

		MultiSet b;
		GenerateMultisetMenu(b, MainUniversum, "B");
		cout << endl << "���������� ���������:" << endl << b << endl;

		Screen screen(MainUniversum, a, b);
		screen.PrintMainMenu();
	}
}

void GenerateMultisetMenu(BaseSet& multiset, Universum& universum, string Multiset_name) {
	if (settings::programShouldEnd) return;
	cout << "���������� ������ ��������� " << Multiset_name << ". �������� ����� ���������:" << endl;
	cout << "1. ��������� ��������� ���������� �����������." << endl;
	cout << "2. ��������� ��������� �������." << endl;
	cout << "����: ";

	bool multipicity_choice_flag = true;
	while (multipicity_choice_flag) {
		char user_input = getchar();
		bool one_symb_input_flag = InputClear();
		
		if (one_symb_input_flag) {
			void (*user_choice)(MultiSet&, Universum&, string) = nullptr;

			for (int i = 0; i < sizeof(main_menu_choices) / sizeof(main_menu_choices[0]); i++) {
				if (user_input == multiset_generate_choises[i].symb) {
					user_choice = multiset_generate_choises[i].func;
					break;
				}
			}
			if (user_choice) {
				MultiSet* multiset1 = dynamic_cast<MultiSet*>(&multiset);
				user_choice(*multiset1, universum, Multiset_name);
				multipicity_choice_flag = false;
			}
			else cout << "�� ����������� � ��������������� ��������, ���������� ��� ���!\n\n����: ";
		}
		else cout << "������������ ����! ���������� ��� ���: ";
	}
}

void GenerateRandomMultiSet(MultiSet& multiset, Universum& universum, string set_name) {
	cout << endl << "��������� ��������� ���������!" << endl;
	cout << "������� �������� ������������� ��������� (�� 0 �� " << int(std::fmin(universum.GetPower(), max_variables_sizes::IntValue)) << "): ";
	string error_message = "\n�� ����� ������������ �����������\n���������� ��� ���: ";
	long long power = ProgramStartInputs(0, universum.GetPower(), error_message, buffer_sizes::power_int_size);

	cout << "������� ������������ ���������� ��������� ��������� ������������� ���������";
	cout << "(�� 0 �� " << min(pow(2, universum.GetElementsSize()), power) << "): ";
	string error_message2 = "\n�� ����� ������������ ���������� ��������� ���������\n���������� ��� ���: ";
	long long not_null = ProgramStartInputs(0, pow(2, universum.GetElementsSize()), error_message2, buffer_sizes::power_int_size);

	multiset = MultiSet(universum, set_name, power, not_null);
}

void GenerateMultiSet(MultiSet& multiset, Universum& universum, string set_name) {
	cout << endl << "������ ��������� ���������!" << endl;
	vector<int> temp_vector;
	string error_message = "\n�� ����� ������������ �����������\n���������� ��� ���: ";

	for (int i = 0; i < pow(2, universum.GetElementsSize()); i++) {
		cout << "������� " << i + 1 << " ������� (�� 0 �� " << universum[i] << "): ";
		int power = ProgramStartInputs(0, universum.GetPower(), error_message, buffer_sizes::power_int_size);
		temp_vector.push_back(power);
	}
	
	multiset = MultiSet(universum, set_name, temp_vector);
}
