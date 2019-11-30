#include <iostream>

#include "tester/tester.h"
#include "btree.h"

using namespace std;

int main(int argc, char *argv[]) {

	cout << "===========================================================" << endl;
    cout << "BTree Practice" << endl;
    cout << "===========================================================" << endl << endl;

    Tester::execute();

	/*for(unsigned int i=5; i<=40; i+=5){
		b.insert(i);
		//b.print();
	}
	
	b.remove(20);
	b.print();
	cout << b.search(1) << endl;
	b.remove(1);
	b.insert(1);
	b.remove(8);
	b.remove(7);
	b.print();
	cout << b.search(1) << endl;

	for(unsigned int i=40; i>0; i-=5){
        b.remove(i);
        b.print();
    }*/
    
    //system("pause");
    return EXIT_SUCCESS;
}