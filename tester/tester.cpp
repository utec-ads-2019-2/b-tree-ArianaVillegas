#include "tester.h"

void Tester::execute() {
    Mocker mocker;
    for (int i = 0; i < NUMBER_OF_TESTS; ++i) {
        unsigned int size = mocker.generateRandomInt(10);
        int* elements = mocker.generateRandomArray<int>(size);
        testBTree<int>(mocker, vector<int>(elements, elements + size));
    }
}

template <typename T>
void Tester::testBTree(Mocker mocker, vector<T> elements) {
    BTree<T> *test = new BTree<T>(3);
    for (int j = 0; j < elements.size(); ++j) {
        test->insert(elements[j]);
        ASSERT(test->search(elements[j]), "There is a problem with the insert or find");
    }


    unsigned int toRemove = mocker.generateRandomInt(1, elements.size());
    for (int j = 0; j < static_cast<int>(toRemove); ++j) {
        unsigned int index = mocker.generateRandomInt(0, elements.size() - 1);
        T temp = elements.at(index);
        elements.erase(elements.begin() + index);
        test->remove(temp);
        int duplicates = count(elements.begin(), elements.end(), temp);
        ASSERT(test->search(temp) == (duplicates > 0), "There is a problem with the remove or find");
    }
}
