#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <float.h>
#include <unordered_set>

using namespace std;

template <typename T>
T GetCorrectNumber(T min, T max) {
	T x;
	while (!(cin >> x) || cin.fail() || (std::cin.peek() != '\n') || (x < min) || (x > max)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "������� ���������� ������ (����� � ��������: " << min << "-" << max << "):";
	}
	return x;
}

class Truba {
public:
	static int MAX_TRUBA_ID;
	string name;
	bool workingStatus;

	Truba() {
		id = MAX_TRUBA_ID++;
		diameter = 0;
		length = 0;
		workingStatus = false;
	}

	friend istream& operator>> (istream& in, Truba& truba) {
		in.clear();
		in.ignore(INT_MAX, '\n');
		cout << "���: ";
		getline(in, truba.name);
		cout << "\n�������: ";
		truba.diameter = GetCorrectNumber(0.0, DBL_MAX);
		cout << "\n�����: ";
		truba.length = GetCorrectNumber(0.0, DBL_MAX);
		cout << "\n������ �����: ";
		truba.workingStatus = GetCorrectNumber(0, 1);
		return in;
	}

	friend ostream& operator<< (ostream& out, Truba& truba) {
		out << "�����: " << endl;
		out << "ID: " << truba.id << endl;
		out << "���: " << truba.name << endl;
		out << "�������: " << truba.diameter << endl;
		out << "�����: " << truba.length << endl;
		if (truba.workingStatus) {
			out << "��������" << endl;
		}
		else {
			out << "� �������" << endl;
		}
		return out;
	}

	void redactTruba();
	void sohranTruba(ofstream& fout);
	void zagruzTruba(ifstream& fin);
	int GetID() { return id; }
	void SetWorkingStatus(bool status) { workingStatus = status; }

private:
	int id;
	double diameter;
	double length;
};

class Kaes {
public:
	static int MAX_KAES_ID;
	string name;

	Kaes() {
		id = MAX_KAES_ID++;
		count = 0;
		countInWork = 0;
		efficiency = 0;
	}

	friend istream& operator>> (istream& in, Kaes& kaes) {
		in.clear();
		in.ignore(INT_MAX, '\n');
		cout << "���: ";
		getline(in, kaes.name);
		cout << "\n���������� �����: ";
		kaes.count = GetCorrectNumber(1, INT_MAX);
		cout << "\n���������� ����� � ������:";
		kaes.countInWork = GetCorrectNumber(1, kaes.count);
		cout << "\n�������������: ";
		kaes.efficiency = GetCorrectNumber(0.0, 1.0);
		return in;
	}

	friend ostream& operator<< (ostream& out, Kaes& kaes) {
		out << "������������� �������: " << endl;
		out << "ID: " << kaes.id << endl;
		out << "���: " << kaes.name << endl;
		out << "���������� �����: " << kaes.count << endl;
		out << "���������� ����� � ������: " << kaes.countInWork << endl;
		out << "������������: " << kaes.efficiency << endl;
		return out;
	}

	void redactKaes();
	void sohranKaes(ofstream& fout);
	void zagruzKaes(ifstream& fin);
	int GetID() { return id; }
	double GetUnusedShopsPercentage() { return ((double)(count - countInWork) / count); }

private:
	int id;
	int count;
	int countInWork;
	double efficiency;
};

template <typename T>
using filterPipes = bool (*) (Truba& truba, T param);

template <typename T>
using filterStations = bool (*) (Kaes& kaes, T param);

template <typename T>
vector<int> poiskTrubaParam(unordered_map<int, Truba>& Trubaline, filterPipes<T> f, T param) {
	vector<int> result;
	for (auto& truba : Trubaline)
		if (f(truba.second, param))
			result.push_back(truba.second.GetID());
	return result;
}

template <typename T>
vector<int> poiskKaesParam(unordered_map<int, Kaes>& kaes_grupa, filterStations<T> f, T param) {
	vector<int> result;
	for (auto& kaes : kaes_grupa)
		if (f(kaes.second, param))
			result.push_back(kaes.second.GetID());
	return result;
}

bool chekTrubaname(Truba& truba, string name) { return (truba.name.find(name) != string::npos); }

bool chekTrubastatus(Truba& truba, bool status) { return (truba.workingStatus == status); }

bool chekKaesname(Kaes& kaes, string name) { return (kaes.name.find(name) != string::npos); }

bool chekKaesnerabceh(Kaes& kaes, double param) { return (kaes.GetUnusedShopsPercentage() >= param); }

void poiskTruba(unordered_map<int, Truba>& trubaline, vector<int>& resultVector) {
	int number;
	cout << "�������� ���������� (0 - �� �����, 1 - �� ������� �����): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0) {
		string name;
		cout << "������� ��� ��� ������ ����: ";
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		getline(cin, name);
		resultVector = poiskTrubaParam(trubaline, chekTrubaname, name);
	}
	else if (number == 1) {
		bool status;
		cout << "\n������� ��������� ����� ��� ������ (0 - ���� ��� � �������, 1 - ���� ��� ��������): ";
		status = GetCorrectNumber(0, 1);
		resultVector = poiskTrubaParam(trubaline, chekTrubastatus, status);
	}
}

void poiskKaes(unordered_map<int, Kaes>& kaes_grupa, vector<int>& resultVector) {
	int number;
	cout << "�������� ���������� (0 - �� �����, 1 - �� �������� �� ��������������� �����): ";
	number = GetCorrectNumber(0, 1);
	if (number == 0) {
		string name;
		cout << "\n������� ��� ��� ������ ������������� �������: ";
		cin >> name;
		resultVector = poiskKaesParam(kaes_grupa, chekKaesname, name);
	}
	else if (number == 1) {
		double param;
		cout << "\n������� ������� �� ��������������� ����� ��� ������ (� ����� �� �������): ";
		param = GetCorrectNumber(0.0, 1.0);
		resultVector = poiskKaesParam(kaes_grupa, chekKaesnerabceh, param);
	}
}
}

void Truba::zagruzTruba(ifstream& fin) {
	fin >> this->id;
	getline(fin, name, '\n');
	getline(fin, name, '\n');
	fin >> this->diameter;
	fin >> this->length;
	fin >> this->workingStatus;
}

void Kaes::zagruzKaes(ifstream& fin) {
	fin >> id;
	getline(fin, name, '\n');
	getline(fin, name, '\n');
	fin >> count;
	fin >> countInWork;
	fin >> efficiency;
}

void zagruz(unordered_map<int, Truba>& trubaline, unordered_map<int, Kaes>& kaes_grupa, string filepath) {
	ifstream fin;

	fin.open(filepath, ios::in);
	if (!fin) {
		cout << "�� ���������� ������� ����!";
	}
	else {
		Truba::MAX_TRUBA_ID = 0;
		Kaes::MAX_KAES_ID = 0;
		Truba newTruba;
		Kaes newKaes;
		trubaline.clear();
		kaes_grupa.clear();
		int trubaNomer, kaesNomer;
		fin >> trubaNomer >> kaesNomer;
		for (int i(0); i < trubaNomer; i++) {
			newTruba.zagruzTruba(fin);
			trubaline.insert({ newTruba.GetID(), newTruba });
			Truba::MAX_TRUBA_ID = (Truba::MAX_TRUBA_ID < newTruba.GetID() ? newTruba.GetID() : Truba::MAX_TRUBA_ID);
		}
		for (int i(0); i < kaesNomer; i++) {
			newKaes.zagruzKaes(fin);
			kaes_grupa.insert({ newKaes.GetID(), newKaes });
			Kaes::MAX_KAES_ID = (Kaes::MAX_KAES_ID < newKaes.GetID() ? newKaes.GetID() : Kaes::MAX_KAES_ID);
		}

		Truba::MAX_TRUBA_ID++;
		Kaes::MAX_KAES_ID++;

		fin.close();
	}
}