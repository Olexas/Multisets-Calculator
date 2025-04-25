#pragma once
#include <vector>
#include <string>

using namespace std;
             
class BaseSet {
protected:
	string name;
	short elements_size;
	vector<int> main_vector;
	long long power;

	vector<int>& GetGrayFunctionElement(vector <int>& temp, int i) const;
	int GrayPodFunction(int i) const;

public:
	BaseSet(short elements_size, string set_name);
	virtual ~BaseSet();
	int operator[] (int index) const;
	BaseSet& operator=(BaseSet&& other);
	short GetElementsSize() const;
	long long GetPower() const;
	vector<int> GetMainVectorCopy() const;
	unique_ptr<BaseSet> operator+ (BaseSet& set);
	unique_ptr<BaseSet> operator- (BaseSet& set);
	unique_ptr<BaseSet> operator* (BaseSet& set);
	unique_ptr<BaseSet> operator/ (BaseSet& set);
	friend ostream& operator<<(ostream& fout, BaseSet& set);
	friend unique_ptr<BaseSet> operator! (BaseSet& set);
	friend unique_ptr<BaseSet> SetUnion(BaseSet& set1, BaseSet& set2, string set_name);
	friend unique_ptr<BaseSet> SetUnion(BaseSet& set1, BaseSet& set2);
	friend unique_ptr<BaseSet> SetIntersection(BaseSet& set1, BaseSet& set2, string set_name);
	friend unique_ptr<BaseSet> SetIntersection(BaseSet& set1, BaseSet& set2);
	friend unique_ptr<BaseSet> SetDifference(BaseSet& set1, BaseSet& set2, string set_name);
	friend unique_ptr<BaseSet> SetDifference(BaseSet& set1, BaseSet& set2);
	friend unique_ptr<BaseSet> SetXor(BaseSet& set1, BaseSet& set2, string set_name);
	friend unique_ptr<BaseSet> SetXor(BaseSet& set1, BaseSet& set2);
};

class Universum : public BaseSet {
private:

public:
	Universum(short elements_size, string set_name);
};	

class EmptySet : public BaseSet {
private:
	Universum* universum;

public:
	EmptySet(Universum& universum);
	EmptySet(Universum& universum, string set_name);
	Universum& GetUniversum();
};

class MultiSet : public BaseSet {
private:
	Universum* universum;

public:
	MultiSet();
	MultiSet(Universum& universum, string set_name, long long power, int not_null_elements);
	MultiSet(Universum& universum, string set_name, vector<int>& multiplicities);
	MultiSet(BaseSet& other, string set_name);
	MultiSet(const MultiSet& other, string set_name);
	Universum& GetUniversum();
};

void PrintGrayFunctionElement(vector <int>& element);
ostream& operator<<(ostream& fout, BaseSet& set);
unique_ptr<BaseSet> operator! (BaseSet& set);
unique_ptr<BaseSet> SetUnion(BaseSet& set1, BaseSet& set2, string set_name);
unique_ptr<BaseSet> SetUnion(BaseSet& set1, BaseSet& set2);
unique_ptr<BaseSet> SetIntersection(BaseSet& set1, BaseSet& set2, string set_name);
unique_ptr<BaseSet> SetIntersection(BaseSet& set1, BaseSet& set2);
unique_ptr<BaseSet> SetDifference(BaseSet& set1, BaseSet& set2, string set_name);
unique_ptr<BaseSet> SetDifference(BaseSet& set1, BaseSet& set2);
unique_ptr<BaseSet> SetXor(BaseSet& set1, BaseSet& set2, string set_name);
unique_ptr<BaseSet> SetXor(BaseSet& set1, BaseSet& set2);