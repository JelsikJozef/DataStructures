#include <libds/mm/compact_memory_manager.h>

#include <string>
#include <print>
#include <vector>


using namespace std;
using namespace ds::mm;

struct Person {
	int id_;


	Person() {
		id_ = 100;
		
	}
};

struct Student : Person {
	int student_number_;
	Student() {
		student_number_ = 200;
	}
};

int main() {
	ds::mm::CompactMemoryManager<Student> mm;

	using namespace std;
	using namespace ds::mm;
	mm.allocateMemory();
	mm.print(std::cout);

	Student* studPtr = &mm.getBlockAt(0);
	
	Person* persPtr = studPtr;

	std::cout << studPtr->student_number_ << std::endl;
	std::cout << persPtr->id_<< std::endl;

	std::cout << studPtr << std::endl;
	std::cout << persPtr << std::endl;
}