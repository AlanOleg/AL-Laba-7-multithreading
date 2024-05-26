#include <iostream>
#include <vector>
#include <future>
using namespace std;

void print_vector(const string& thread_name, const vector<double>& array) {
    for (int i = 0; i < array.size(); i++) {
        cout << thread_name << ": " << array[i] << endl;
    }
    cout << thread_name << "_total: ";
    for (int i = 0; i < array.size(); i++) {
        cout << array[i] << " ";
    }
    cout << endl;
    cout << endl;
};


void direct_selection_sort(vector<double>& array, int start, int end, const string& thread_name) {
    for (int i = start + 1; i < end; i++) {
        int placeholder = array[i];
        int j = i - 1;
        while (j >= start and array[j] > placeholder) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = placeholder;
    }
    print_vector(thread_name, array);
};

int main() {
    vector<double> array;
    srand(time(0));
    for (int i = 0; i < 10; i++) {
        array.push_back(rand() % 10);
    }
    print_vector("main", array);

    auto future1 = async(launch::async, direct_selection_sort, ref(array), 0, (array.size() / 2), "thread 1");
    auto future2 = async(launch::async, direct_selection_sort, ref(array), (array.size() / 2), array.size(), "thread 2");
    future1.get();
    future2.get();

    auto future3 = async(launch::async, direct_selection_sort, ref(array), 0, array.size(), "thread 3");
    future3.get();

    cout << "MY WORK IS DONE\n";
}
