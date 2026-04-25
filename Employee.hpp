#include <fstream>
#include <iostream>
#include <string>

#pragma once

class Employee {
private:
  std::string id;
  int salary;
  int seniority;

public:
  Employee() {}
  Employee(const std::string &id, int salary, int seniority)
      : id(id), salary(salary), seniority(seniority) {}

  inline std::string getId() const { return this->id; }
  inline int getSalary() const { return this->salary; }
  inline int getSeniority() const { return this->seniority; }

  inline void printEmployee() const {
    std::cout << "id: " << id << std::endl;
    std::cout << "salary: " << salary << "$" << std::endl;
    std::cout << "seniority: " << seniority << "years" << std::endl;
  }

  void load(std::ifstream &inputFile) {
    int idLength;

    inputFile.read(reinterpret_cast<char *>(&idLength), sizeof(idLength));
    id.resize(idLength);
    inputFile.read(&id[0], idLength);
    inputFile.read(reinterpret_cast<char *>(&salary), sizeof(salary));
    inputFile.read(reinterpret_cast<char *>(&seniority), sizeof(seniority));
  }

  void save(std::ofstream &outputFile) {
    int idLength = id.length();

    outputFile.write(reinterpret_cast<const char *>(&idLength),
                     sizeof(idLength));
    outputFile.write(id.c_str(), idLength);
    outputFile.write(reinterpret_cast<const char *>(&salary), sizeof(salary));
    outputFile.write(reinterpret_cast<const char *>(&seniority),
                     sizeof(seniority));
  }
};