#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>


#include "settings.h"
#include "MultiSet.h"

class Screen {
private:
	BaseSet* universum;
	BaseSet* setA;
	BaseSet* setB;

public:
	Screen(Universum& universum, BaseSet& set1, BaseSet& set2);
	Screen(EmptySet& universum, BaseSet& set1, BaseSet& set2);
	void Clear();
	void PrintMainMenu();
	void PrintActionMenu(void (Screen::* user_choice)());
	void PrintSetUnionMenu();
	void PrintSetIntersectionMenu();
	void PrintDifferenceMenu();
	void PrintXorMenu();
	void PrintComplementMenu();
	void PrintArithmeticMenu();
	void ProgramStop();
	void ProgramRestart();
	void EditSetA();
	void EditSetB();
};

struct {
	void (Screen::*func)();
	char symb;
} main_menu_choices[] = {
	{&Screen::PrintSetUnionMenu, '1'},
	{&Screen::PrintSetIntersectionMenu, '2'},
	{&Screen::PrintDifferenceMenu, '3'},
	{&Screen::PrintXorMenu, '4'},
	{&Screen::PrintComplementMenu, '5'},
	{&Screen::PrintArithmeticMenu, '6'},
	{&Screen::ProgramRestart, '7'},
	{&Screen::EditSetA, '8'},
	{&Screen::EditSetB, '9'},
	{&Screen::ProgramStop, 'x'},
};

void ProgramStart();
void GenerateMultisetMenu(BaseSet& multiset, Universum& universum, string Multiset_name);

void GenerateRandomMultiSet(MultiSet& multiset, Universum& universum, string set_name);
void GenerateMultiSet(MultiSet& multiset, Universum& universum, string set_name);

struct {
	void (*func)(MultiSet&, Universum&, string);
	char symb;
} multiset_generate_choises[]{
	{GenerateRandomMultiSet, '1'},
	{GenerateMultiSet, '2'}
};
