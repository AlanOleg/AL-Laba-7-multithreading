#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
using namespace std;

mutex mut;
condition_variable con_var;
bool First_half_sorted = false;
bool Second_half_sorted = false;

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


void direct_selection_sort(vector<double> &array, int start, int end){
    for (int i = start + 1; i < end; i++) {
        int placeholder = array[i];
        int j = i - 1;
        while (j >= start and array[j] > placeholder) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = placeholder;
    }
};

void sort_first_half(vector<double>& array) {
    //mut.lock();
    direct_selection_sort(array, 0, (array.size() / 2));
    mut.lock();
    //lock_guard <mutex> lock(mut);
    print_vector("thread1", array);
    mut.unlock();
    First_half_sorted = true;
    con_var.notify_one();
};

void sort_second_half(vector<double>& array) {
    //mut.lock();
    direct_selection_sort(array, (array.size() / 2), array.size());
    mut.lock();
    //lock_guard <mutex> lock(mut);
    print_vector("thread2", array);
    mut.unlock();
    Second_half_sorted = true;
    con_var.notify_one();
};

void sort_full(vector<double>& array) {
    unique_lock<mutex> lock(mut);
    con_var.wait(lock, [] {return First_half_sorted and Second_half_sorted;});
    lock.unlock();
    direct_selection_sort(array, 0, array.size());
    print_vector("thread3", array);
    First_half_sorted = false;
    Second_half_sorted = false;
};

int main() {
    vector<double> array;
    srand(time(0));
    for (int i = 0; i < 10; i++) {
        array.push_back(rand() % 10);
    }
    print_vector("main", array);

    thread thread1(sort_first_half, ref(array));
    thread thread2(sort_second_half, ref(array));
    thread thread3(sort_full, ref(array));

    thread1.join();
    thread2.join();
    thread3.join();

    cout << "MY WORK IS DONE\n";
}
