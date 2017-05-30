#include <iostream>


#include <iostream>//вырезать метод erase
#include <vector>
#include <limits>
#include <fstream>


using std::cout;
using std::cin;
using std::endl;


template <class T>
class Heap {
public:
    Heap() = delete; //запрещаем конструктор без параметров
    Heap(T *, size_t); //конструктор с параметрами
    size_t size_heap();
    void insert(T &);
    T pop_max();
    const T &operator[](size_t number);
    template <class K>
    friend std::ostream & operator << (std::ostream &os, const Heap<K> &obj);
private:
    std::vector<T> heap_value;
};



template <class T>
Heap<T>::Heap(T *elem_array, size_t size) {
    for (size_t i = 0; i < size; i++)
        heap_value.push_back(elem_array[i]);

    //цикл для верхней половины элементов т.к. size t беззнаковый. Т.е. ждем переполнения инта. костыль
    //строим кучу снизу вверх
    for (size_t i  = (size - 2) / 2; i < std::numeric_limits<int>::max(); i--)
        for (size_t j = i; j <= (size - 2)/2; ) { //
            size_t k = j * 2 + 1;
            if ((k + 1) < (size) && heap_value[k] < heap_value[k + 1]) {
                k++;
            }
            if (heap_value[j] < heap_value[k]) {
                std::swap(heap_value[j], heap_value[k]);
                j = k;
            }
            else
                break;
        }
}


template <class T>
void Heap<T>::insert(T &new_elem) {
    heap_value.push_back(new_elem);
    for (size_t i = heap_value.size() - 1; i > 0;) {
        if (heap_value[i] > heap_value[(i - 1) / 2])
            std::swap(heap_value[i], heap_value[(i - 1) / 2]);
        else
            break;
        i = (i - 1) / 2;
    }
}


template <class T>
T Heap<T>::pop_max() {
    size_t number_elem = 0;
    std::swap(heap_value[number_elem], heap_value[heap_value.size() - 1]);
    for (size_t i = number_elem; (2 * i + 1) < (heap_value.size() - 1);) {
        i = (i * 2) + 1;
        if ((i + 1) < (heap_value.size() - 1) && heap_value[i] < heap_value[i + 1])
            i++;
        if (heap_value[(i - 1) / 2] < heap_value[i])
            std::swap(heap_value[(i - 1) / 2], heap_value[i]);
        else
            break;
    }
    T temp = heap_value[heap_value.size() - 1];
    heap_value.pop_back();
    return temp;
}


template <class T>
const T &Heap<T>::operator[](size_t number) {
    if (number >= heap_value.size())
        throw;
    return this->heap_value[number];
}


template <class K>
std::ostream &operator<<(std::ostream &os, const Heap<K> &obj) {
    for (size_t i = 0; i < obj.heap_value.size(); i++)
        os << obj.heap_value[i] << std::endl;
    return os;
}


template <class T>
size_t Heap<T>::size_heap() {
    return heap_value.size();
}


bool open_file(std::string file_name, std::ifstream &is) {
    is.exceptions(std::ios::failbit);
    while (true) {
        cout << "Введите имя файла (требуемый тип INT) или нажмите q для выхода" << endl;
        cin >> file_name;
        if (file_name == "q")
            return 0;
        try {
            is.open(file_name);
        }
        catch (std::ios_base::failure &e) { //есл не верное имя файла
            cout << "Caught an ios_base::failure." << endl
                 << "Explanatory string: " << e.what() << endl
                 << "Error code: " << e.code() << endl
                 << "Короче говоря, не получилось открыть файл" << endl;
            is.clear();
            continue;
        }
        break;//если файл открылся, то выходим из цикла
    }
    return 1;
}


void read_file(std::ifstream &is, std::vector<int> &data) {
    int temp;
    try {
        while (is) {
            is >> temp;
            if (!is) {//костыль, т.к. is доходит до end of file а потом читает еще раз. надо решать флагами
                cout << " , " << endl;
                break;
            }
            data.push_back(temp);
        }
    }
    catch (const std::runtime_error &s) {
        cin.clear(std::ios::failbit | std::ios::eofbit);
    }
    return;
}


void add_number(Heap<int> &obj) {
    std::string choice;
    int value = 0;
    cin.clear();
    while (true) {
        cout << "Добавить еще число? (Y, N)" << endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        choice = (char)cin.get();
        if (choice == "Y" || choice == "y") {
            cout << "Введите число: " << endl;
            cin >> value;
            cin.clear();
            obj.insert(value);
            continue;
        }
        else if (choice == "N" || choice == "n") {
            break;
        }
    }
}


void heap_sort(Heap<int> &obj, std::vector<int> &result) {
    size_t total_heap_elements = obj.size_heap();
    for (size_t i = 0; i < total_heap_elements; i++) {
        result.push_back(obj.pop_max());
    }
    return;
}


void save_data(std::vector<int> sort_array) {
    //сохраняем в исходный файл
    std::string file_to_save = "sorted_test.txt";
    std::ofstream os(file_to_save);
    for (size_t i = 0; i < sort_array.size(); i++) {
        os << sort_array[i];
        os << " ";
    }
    os.close();
    return;
}



int main() {
    try {
        std::string file_name;
        std::ifstream is;

        //блок чтения из файла
        if (!open_file(file_name, is))
            return 0;

        std::vector<int> data;
        data.reserve(1024);

        read_file(is, data);
        is.close();
        //конец блока чтения

        cout << "Исходный массив: " << endl;

        for (int i = 0; i < data.size(); ++i) {
            cout << data[i] << " ";
        }
        cout << endl;

        Heap<int> obj(&data[0], data.size());//создаем нашу кучу из элементов вектора
        add_number(obj);

        cout << "Отсортированный массив: " << endl;

        std::vector<int> sort_array;
        sort_array.reserve(obj.size_heap());
        heap_sort(obj, sort_array);

        for (int j = 0; j < sort_array.size(); ++j) {
            cout << sort_array[j] << " ";
        }
        cout << endl;

        save_data(sort_array);
    }

    catch(const std::runtime_error &s){
        std::cout << s.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(const std::ios_base::failure &err){
        std::cout << err.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...){
        std:: cout << "ERROR FIND" << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}