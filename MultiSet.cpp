#pragma once
#include <iostream>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <unordered_map>

using namespace std::chrono;

#include "MultiSet.h"

using namespace std;

vector<int>& BaseSet::GetGrayFunctionElement(vector <int>& temp, int i) const {
	if (i != 0) {
		int p = GrayPodFunction(i);
		temp[p - 1] = 1 - temp[p - 1];
	}

	return temp;
}

int BaseSet::GrayPodFunction(int i) const {
	int q = 1, j = i;
	while (j % 2 == 0) {
		j /= 2;
		q += 1;
	}
	return q;
}

BaseSet::BaseSet(short elements_size, string set_name) {
	this->name = set_name;
	this->elements_size = elements_size;
	this->power = 0;
}

BaseSet::~BaseSet() {
	
}

int BaseSet::operator[] (int index) const{
	if (index == pow(2, this->elements_size)) {
		return 0;
	}
	return this->main_vector[index];
}

BaseSet& BaseSet::operator=(BaseSet&& other) {
	if (this == &other) {
		return *this;
	}

	this->elements_size = other.elements_size;
	this->name = other.name;
	this->power = other.power;
	this->main_vector = move(other.main_vector);
}

short BaseSet::GetElementsSize() const {
	return this->elements_size;
}

long long BaseSet::GetPower() const {
	return this->power;
}

vector<int> BaseSet::GetMainVectorCopy() const {
	return this->main_vector;
}

ostream& operator << (ostream & fout, BaseSet & set) {
	EmptySet* emptyset1 = dynamic_cast<EmptySet*>(&set);
	if (emptyset1) {
		fout << "Множество " << set.name << ": Пустое множество!" << endl;
		return fout;
	}

	fout << "Множество " << set.name << ": {";

	vector<int> element(set.elements_size);

	int elements_count = pow(2, set.elements_size);
	for (int i = 0; i < elements_count; i++) {
		element = set.GetGrayFunctionElement(element, i);
		PrintGrayFunctionElement(element);
		fout << ": " << set.main_vector[i];
		if (i != elements_count - 1) {
			fout << ", ";
		}
	}

	fout << "}" << endl;

	fout << "Мощность множества " << set.name << ": " << set.power << endl;

	return fout;
}

void PrintGrayFunctionElement(vector <int>& element) {
	for (int i = 0; i < element.size(); i++) {
		cout << element[i];
	}
}

Universum::Universum(short elements_size, string set_name) : BaseSet(elements_size, set_name) {
	this->name = set_name;
	this->elements_size = elements_size;
	this->power = 0;

	for (int i = 0; i < pow(2, this->elements_size); i++) {
		short element_multiplicity = rand();
		this->main_vector.push_back(element_multiplicity);
		this->power += element_multiplicity;
	}
}

int get_random_int(long long end) {
	int iter_count = end / 0x7fff;
	int s = 0;

	if (iter_count == 0) {
		s = rand() % (end + 1);
	}
	else {
		int iter_count_rand = rand() % (iter_count + 1);
		s = rand() * iter_count_rand + (rand() % (end - iter_count_rand * 0x7fff + 1));
	}

	if (s > end) {
		return end;
	}

	return s;
}

MultiSet::MultiSet() : BaseSet(0, "") {
	this->universum = nullptr;
	this->power = 0;
}

MultiSet::MultiSet(Universum& universum, string set_name, long long power, int not_null_elements) : BaseSet(universum.GetElementsSize(), set_name) {
	auto start = high_resolution_clock::now(); 
	
	this->universum = &universum;

	int element_multiplicity = 0;
	int elements_count = pow(2, this->elements_size);
	vector<long long> temp_vector = {0, power};
	unordered_map<long long, int> temp_map = { {0, 1}, {power, 1} };
	int zero_count = 0;
	int element;

	for (int i = 0; i < elements_count - 1; i++) {
		element = get_random_int(power);
		int element_in_temp_count = temp_map[element];
		
		if (element_in_temp_count) {
			if (elements_count - not_null_elements != zero_count) zero_count += 1;
			else {
				while (temp_map[element]) {
					element = get_random_int(power);
				}
			}
		}

		temp_vector.push_back(element);
		temp_map[element] += 1;
	}

	sort(temp_vector.begin(), temp_vector.end());
	
	int credit = 0;
	for (int i = 0; i < elements_count; i++) {
		element_multiplicity = temp_vector[i + 1] - temp_vector[i];

		if (element_multiplicity > universum[i]) {
			credit += element_multiplicity - universum[i];
			element_multiplicity = universum[i];
		}

		this->main_vector.push_back(element_multiplicity);
		this->power += element_multiplicity;
	}
;
	int element_ind = rand() % (elements_count);
	while (credit != 0) {
		if (universum[element_ind] - this->main_vector[element_ind] >= credit) {
			this->main_vector[element_ind] += credit;
			this->power += credit;
			credit = 0;
		}
		else {
			int temp = rand() % (universum[element_ind] - this->main_vector[element_ind] + 1);
			this->main_vector[element_ind] += temp;
			this->power += temp;
			credit -= temp;
		}
		element_ind = (element_ind + 1) % elements_count;
	}
}

MultiSet::MultiSet(Universum& universum, string set_name, vector<int>& multiplicities) : BaseSet(universum.GetElementsSize(), set_name) {
	this->universum = &universum;

	for (int i = 0; i < pow(2, this->elements_size); i++) {
		this->main_vector.push_back(multiplicities[i]);
		this->power += multiplicities[i];
	}
}

MultiSet::MultiSet(BaseSet& other, string set_name) : BaseSet(other.GetElementsSize(), set_name) {
	Universum* universum = dynamic_cast<Universum*>(&other);
	this->power = other.GetPower();
	this->universum = universum;
	vector<int> other_vector = other.GetMainVectorCopy();
	for (int i = 0; i < pow(2, this->elements_size); i++) {
		this->main_vector.push_back(other_vector[i]);
	}
}

MultiSet::MultiSet(const MultiSet& other, string set_name) : BaseSet(other.elements_size, set_name) {
	this->universum = other.universum;
	this->power = other.power;
	for (int i = 0; i < pow(2, this->elements_size); i++) {
		this->main_vector.push_back(other.main_vector[i]);
	}
}

Universum& MultiSet::GetUniversum() {
	return *this->universum;
}

unique_ptr<BaseSet> operator! (BaseSet& set) {
	string set_name = "!" + set.name;

	EmptySet* emptyset1 = dynamic_cast<EmptySet*>(&set);
	if (emptyset1) return make_unique<MultiSet>(emptyset1->GetUniversum(), set_name);

	Universum* universum1 = dynamic_cast<Universum*>(&set);
	if (universum1) return make_unique<EmptySet>(*universum1, set_name);

	MultiSet* multiset1 = dynamic_cast<MultiSet*>(&set);
	vector<int> multiplicities;
	vector<int> universum_vector = multiset1->GetUniversum().GetMainVectorCopy();
	for (int i = 0; i < pow(2, set.elements_size); i++) {
		multiplicities.push_back(universum_vector[i] - set.main_vector[i]);
	}

	return make_unique<MultiSet>(multiset1->GetUniversum(), set_name, multiplicities);
}

unique_ptr<BaseSet> BaseSet::operator+(BaseSet& set) {
	string set_name = this->name + " + " + set.name;

	EmptySet* emptyset1 = dynamic_cast<EmptySet*>(this);
	EmptySet* emptyset2 = dynamic_cast<EmptySet*>(&set);
	if (emptyset1 && emptyset2) return make_unique<EmptySet>(emptyset1->GetUniversum(), set_name);
	else if (emptyset1) {
		MultiSet* multiset1 = dynamic_cast<MultiSet*>(&set);
		return make_unique<MultiSet>(*multiset1, set_name);
	}
	else if (emptyset2) {
		MultiSet* multiset2 = dynamic_cast<MultiSet*>(this);
		return make_unique<MultiSet>(*multiset2, set_name);
	}

	MultiSet* multiset1 = dynamic_cast<MultiSet*>(this);
	MultiSet* multiset2 = dynamic_cast<MultiSet*>(&set);
	vector<int> multiplicities;
	for (int i = 0; i < pow(2, this->elements_size); i++) {
		multiplicities.push_back(min(multiset1->main_vector[i] + set.main_vector[i], multiset1->GetUniversum().GetMainVectorCopy()[i]));
	}

	if (multiset1) {
		return make_unique<MultiSet>(multiset1->GetUniversum(), set_name, multiplicities);
	}
	if (multiset2) {
		return make_unique<MultiSet>(multiset2->GetUniversum(), set_name, multiplicities);
	}
}

unique_ptr<BaseSet> BaseSet::operator-(BaseSet& set) {
	string set_name = this->name + " - " + set.name;

	EmptySet* emptyset1 = dynamic_cast<EmptySet*>(this);
	EmptySet* emptyset2 = dynamic_cast<EmptySet*>(&set);
	if (emptyset1) {
		return make_unique<EmptySet>(emptyset1->GetUniversum(), set_name);
	}
	else if (emptyset2) {
		MultiSet* multiset2 = dynamic_cast<MultiSet*>(this);
		return make_unique<MultiSet>(*multiset2, set_name);
	}

	MultiSet* multiset1 = dynamic_cast<MultiSet*>(this);
	MultiSet* multiset2 = dynamic_cast<MultiSet*>(&set);
	bool zero_flag = true;
	vector<int> multiplicities;
	for (int i = 0; i < pow(2, this->elements_size); i++) {
		multiplicities.push_back(max(multiset1->main_vector[i] - set.main_vector[i], 0));
		if (zero_flag && max(multiset1->main_vector[i] - set.main_vector[i], 0) != 0) {
			zero_flag = false;
		}
	}

	if (multiset1) {
		if (zero_flag) return make_unique<EmptySet>(multiset1->GetUniversum(), set_name);
		return make_unique<MultiSet>(multiset1->GetUniversum(), set_name, multiplicities);
	}
	if (multiset2) {
		if (zero_flag) return make_unique<EmptySet>(multiset2->GetUniversum(), set_name);
		return make_unique<MultiSet>(multiset2->GetUniversum(), set_name, multiplicities);
	}
}

unique_ptr<BaseSet> BaseSet::operator*(BaseSet& set) {
	string set_name = this->name + " * " + set.name;

	EmptySet* emptyset1 = dynamic_cast<EmptySet*>(this);
	EmptySet* emptyset2 = dynamic_cast<EmptySet*>(&set);
	if (emptyset1 || emptyset2) {
		if (emptyset1) {
			return make_unique<EmptySet>(emptyset1->GetUniversum(), set_name);
		}
		return make_unique<EmptySet>(emptyset2->GetUniversum(), set_name);
	}

	MultiSet* multiset1 = dynamic_cast<MultiSet*>(this);
	MultiSet* multiset2 = dynamic_cast<MultiSet*>(&set);
	vector<int> multiplicities;
	for (int i = 0; i < pow(2, this->elements_size); i++) {
		multiplicities.push_back(min(multiset1->main_vector[i] * set.main_vector[i], multiset1->GetUniversum().GetMainVectorCopy()[i]));
	}

	if (multiset1) {
		return make_unique<MultiSet>(multiset1->GetUniversum(), set_name, multiplicities);
	}
	if (multiset2) {
		return make_unique<MultiSet>(multiset2->GetUniversum(), set_name, multiplicities);
	}
}

unique_ptr<BaseSet> BaseSet::operator/(BaseSet& set) {
	string set_name = this->name + " / " + set.name;

	EmptySet* emptyset1 = dynamic_cast<EmptySet*>(this);
	EmptySet* emptyset2 = dynamic_cast<EmptySet*>(&set);
	if (emptyset1 || emptyset2) {
		if (emptyset1) {
			return make_unique<EmptySet>(emptyset1->GetUniversum(), set_name);
		}
		return make_unique<EmptySet>(emptyset2->GetUniversum(), set_name);
	}

	MultiSet* multiset1 = dynamic_cast<MultiSet*>(this);
	MultiSet* multiset2 = dynamic_cast<MultiSet*>(&set);
	bool zero_flag = true;
	vector<int> multiplicities;
	for (int i = 0; i < pow(2, this->elements_size); i++) {
		if (set.main_vector[i] == 0) multiplicities.push_back(0);
		else multiplicities.push_back(max(multiset1->main_vector[i] / set.main_vector[i], 0));
		if (set.main_vector[i] != 0) {
			if (zero_flag && max(multiset1->main_vector[i] / set.main_vector[i], 0) != 0) {
				zero_flag = false;
			}
		}
	}

	if (multiset1) {
		if (zero_flag) return make_unique<EmptySet>(multiset1->GetUniversum(), set_name);
		return make_unique<MultiSet>(multiset1->GetUniversum(), set_name, multiplicities);
	}
	if (multiset2) {
		if (zero_flag) return make_unique<EmptySet>(multiset2->GetUniversum(), set_name);
		return make_unique<MultiSet>(multiset2->GetUniversum(), set_name, multiplicities);
	}
}

unique_ptr<BaseSet> SetUnion(BaseSet& set1, BaseSet& set2, string set_name) {
	EmptySet* emptyset1 = dynamic_cast<EmptySet*>(&set1);
	EmptySet* emptyset2 = dynamic_cast<EmptySet*>(&set2);
	if (emptyset1 && emptyset2) {
		return make_unique<EmptySet>(emptyset1->GetUniversum(), set_name);
	}
	else if (emptyset1) {
		MultiSet* multiset2 = dynamic_cast<MultiSet*>(&set2);
		if (multiset2) return make_unique<MultiSet>(*multiset2, set_name);
	}
	else if (emptyset2) {
		MultiSet* multiset1 = dynamic_cast<MultiSet*>(&set1);
		if (multiset1) return make_unique<MultiSet>(*multiset1, set_name);
	}

	vector<int> multiplicities;
	bool zero_flag = true;
	for (int i = 0; i < pow(2, set1.elements_size); i++) {
		multiplicities.push_back(max(set1[i], set2[i]));
		if (zero_flag && max(set1[i], set2[i]) != 0) {
			zero_flag = false;
		}
	}

	MultiSet* multiset1 = dynamic_cast<MultiSet*>(&set1);
	MultiSet* multiset2 = dynamic_cast<MultiSet*>(&set2);
	if (multiset1) {
		if (zero_flag) return make_unique<EmptySet>(multiset1->GetUniversum(), set_name);
		return make_unique<MultiSet>(multiset1->GetUniversum(), set_name, multiplicities);
	}
	if (multiset2) {
		if (zero_flag) return make_unique<EmptySet>(multiset2->GetUniversum(), set_name);
		return make_unique<MultiSet>(multiset2->GetUniversum(), set_name, multiplicities);
	}
}

unique_ptr<BaseSet> SetUnion(BaseSet& set1, BaseSet& set2) {
	string new_set_name = set1.name + " \\/ " + set2.name;
	return SetUnion(set1, set2, new_set_name);
}

unique_ptr<BaseSet> SetIntersection(BaseSet& set1, BaseSet& set2, string set_name) {
	EmptySet* emptyset1 = dynamic_cast<EmptySet*>(&set1);
	EmptySet* emptyset2 = dynamic_cast<EmptySet*>(&set2);
	if (emptyset1 || emptyset2) {
		if (emptyset1) return make_unique<EmptySet>(emptyset1->GetUniversum(), set_name);
		return make_unique<EmptySet>(emptyset2->GetUniversum(), set_name);
	}

	vector<int> multiplicities;
	bool zero_flag = true;
	for (int i = 0; i < pow(2, set1.elements_size); i++) {
		multiplicities.push_back(min(set1[i], set2[i]));
		if (zero_flag && min(set1[i], set2[i]) != 0) {
			zero_flag = false;
		}
	}

	MultiSet* multiset1 = dynamic_cast<MultiSet*>(&set1);
	MultiSet* multiset2 = dynamic_cast<MultiSet*>(&set2);
	if (multiset1) {
		if (zero_flag) return make_unique<EmptySet>(multiset1->GetUniversum(), set_name);
		return make_unique<MultiSet>(multiset1->GetUniversum(), set_name, multiplicities);
	}
	if (multiset2) {
		if (zero_flag) return make_unique<EmptySet>(multiset2->GetUniversum(), set_name);
		return make_unique<MultiSet>(multiset2->GetUniversum(), set_name, multiplicities);
	}
}

unique_ptr<BaseSet> SetIntersection(BaseSet& set1, BaseSet& set2)
{
	string new_set_name = set1.name + " /\\ " + set2.name;
	return SetIntersection(set1, set2, new_set_name);
}

unique_ptr<BaseSet> SetDifference(BaseSet& set1, BaseSet& set2, string set_name) {
	return SetIntersection(set1, *!set2, set_name);
}

unique_ptr<BaseSet> SetDifference(BaseSet& set1, BaseSet& set2) {
	string new_set_name = set1.name + " \\ " + set2.name;
	return SetDifference(set1, set2, new_set_name);
}

unique_ptr<BaseSet> SetXor(BaseSet& set1, BaseSet& set2, string set_name) {
	return SetDifference(*SetUnion(set1, set2), *SetIntersection(set1, set2), set_name);
}

unique_ptr<BaseSet> SetXor(BaseSet& set1, BaseSet& set2) {
	string new_set_name = set1.name + " |+| " + set2.name;
	return SetXor(set1, set2, new_set_name);
}


EmptySet::EmptySet(Universum& universum) : BaseSet(0, "None") {
	this->universum = &universum;
}

EmptySet::EmptySet(Universum& universum, string set_name) : BaseSet(0, set_name) {
	this->universum = &universum;
}

Universum& EmptySet::GetUniversum() {
	return *this->universum;
}