#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;

class Foo {
public:
	Foo() {}
	~Foo() {
		cout << "Test destructor success" << endl;
	}
};

int main (int argc, char *argv[]) {
	vector<Foo *> test;
	test.push_back(new Foo());
	test.push_back(new Foo());
	test.push_back(new Foo());
	test.push_back(new Foo());
	test.push_back(new Foo());
	test.clear();
	return 0;
}
