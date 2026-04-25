#include "CacheMemory.hpp"
#include "Employee.hpp"
#include <fstream>
#include <iostream>


void createBinaryFile() {
  std::ofstream outFile("empsTest.bin", std::ios::binary);
  if (!outFile) {
    std::cerr << "Error creating file." << std::endl;
    return;
  }

  Employee e1("A123", 15000, 3);
  Employee e2("B456", 22000, 5);
  Employee e3("C789", 18000, 2);

  e1.save(outFile);
  e2.save(outFile);
  e3.save(outFile);

  outFile.close();
  std::cout << "Successfully saved employees to empsTest.bin" << std::endl;
}

int main() {
  std::cout << "--- Part B: File I/O ---" << std::endl;
  createBinaryFile();

  CacheMemory<std::string, Employee> memoryMap;

  std::ifstream inFile("empsTest.bin", std::ios::binary);
  if (!inFile) {
    std::cerr << "Error opening file." << std::endl;
    return 1;
  }

  // Read 3 employees based on our known test setup
  for (int i = 0; i < 3; ++i) {
    Employee emp;
    emp.load(inFile);

    try {
      memoryMap.add(emp.getId(), emp);
      std::cout << "Loaded and added to memory: " << emp.getId() << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Failed to add " << emp.getId() << std::endl;
    }
  }
  inFile.close();

  std::cout << "\n--- Part A: CacheMemory Logic ---" << std::endl;

  // 1. Fetching an item (moves it to cache)
  std::cout << "Fetching A123..." << std::endl;
  std::shared_ptr<Employee> ptr1 = memoryMap.get("A123");
  if (ptr1)
    ptr1->printEmployee();

  std::cout << "\nCurrent Cache Size: " << memoryMap.getCacheValues().size()
            << std::endl;

  // 2. Erasing an item
  try {
    std::cout << "Erasing A123 from main memory..." << std::endl;
    memoryMap.erase("A123");
  } catch (const ObjectNotExistException &e) {
    std::cerr << "Object does not exist!" << std::endl;
  }

  // 3. Testing cache expiration
  // The shared_ptr 'ptr1' is still holding A123 alive.
  // Let's reset it to simulate it going out of scope.
  ptr1.reset();

  std::cout << "Attempting to fetch A123 after deletion and pointer reset..."
            << std::endl;
  std::shared_ptr<Employee> ptr2 = memoryMap.get("A123");
  if (!ptr2) {
    std::cout << "A123 successfully purged from cache and not found."
              << std::endl;
  }

  // 4. Testing duplicate exception
  try {
    std::cout << "Attempting to add duplicate B456..." << std::endl;
    Employee duplicate("B456", 99999, 10);
    memoryMap.add(duplicate.getId(), duplicate);
  } catch (const DuplicateKeyException &e) {
    std::cout << "Successfully caught DuplicateKeyException!" << std::endl;
  }

  return 0;
}