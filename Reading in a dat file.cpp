
// PHYS 30762 Programming in C++

// Program to compute mean, standard deviation and standard
// error of the a set of courses. Data is read from file

//Libraries
#include<iostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<vector>
#include<cstring>
#include<numeric>
#include<sstream>
#include<stdlib.h>
#include<algorithm>

// Declare variables
double marks;
int course_code;
int number_of_entries;
std::string course_name;
std::string data_file;
std::string line;
std::vector<double>marks_container;
std::vector<int>course_codes_container;
std::vector<std::string>course_name_container;

// Functions to compute mean and standard deviation
double mean_value(std::vector<double>v) {
  return std::reduce(v.begin(), v.end(), 0.0) / v.size();
}

double std_dev(std::vector<double>v) {
  double mean = mean_value(v);
  double difference;
  double square_differences;
  int i;
  for(i=0; i<v.size(); i++) {
    difference = (v[i] - mean);
    square_differences += pow(difference, 2);
  }
  return pow((square_differences / (v.size() - 1)), 0.5);

}

double std_error(std::vector<double>v) {
  return std_dev(v) / pow(v.size(), 0.5);
}

// Main function
int main()
{
  
  // Ask user to enter filename
  std::cout << "Enter data filename: " << std::endl;
  std::cin >> data_file;

  // Open file and check if successful
  std::ifstream course_stream(data_file);

    if(!course_stream) {
        std::cout << "There was an error reading your file." << std::endl;
    }
//Read in data
while(std::getline(course_stream, line)) {
    std::string marks_string;
    std::string course_code_string;
    std::istringstream iss(line);
    iss >> marks_string >> course_code_string;
    marks = atof(marks_string.c_str());
    course_code = atoi(course_code_string.c_str());
    marks_container.push_back(marks);
    course_codes_container.push_back(course_code);
    std::string whole;
    whole = line.substr(0, line.length());
    int pos = whole.find('.');
    course_name = whole.substr(pos + 11);
    course_name_container.push_back(course_name);
    
  }

  // Close file
  course_stream.close();
 

  std::string list_specifier;
  std::cout<<"Would you like to analyse the whole list (y/n)?"<<std::endl;
  std::cin>>list_specifier;
  
  
  if(list_specifier == "n"){
  int unit_level;
  std::vector<double> unit_indexes;

  std::cout<<"What unit level would you like to analyse "
  <<"(integer number 1 to 4 inclusive)?"<<std::endl;
  std::cin>>unit_level;

  int i = 0;
  int course_codes_element;

  //Getting indexes of courses of specified level
  while (i < course_codes_container.size()) {
    course_codes_element = course_codes_container[i];
    
    if (unit_level * 10000 <= course_codes_element && course_codes_element< 10000*(unit_level + 1)){
      unit_indexes.push_back(i);
    } 
    i++;
  }
  std::string sort_specifier;
  std::cout<<"Would you like to sort the list of courses by title or code (t/c)?"<<std::endl;
  std::cin>>sort_specifier;
  int index;
  std::vector<std::pair<std::string, int> > sorted_indexes;
  
  if (sort_specifier=="c"){
    
    for (index=0; index<unit_indexes.size(); ++index){
      sorted_indexes.push_back(std::make_pair(std::to_string
      (course_codes_container[unit_indexes[index]]), 
      unit_indexes[index]));
    }
    sort(sorted_indexes.begin(), sorted_indexes.end());

  } else if (sort_specifier == "t"){
    
    for (index = 0; index<unit_indexes.size(); ++index){
      sorted_indexes.push_back(std::make_pair(course_name_container[unit_indexes[index]], 
      unit_indexes[index]));
    }
    sort(sorted_indexes.begin(), sorted_indexes.end());
  } else{
    std::cout<<"Your input was not recognised."<<std::endl;
  }
  //Creating course titles using stringstream
  std::ostringstream oss;
  std::vector<std::string> course_titles;
  
  for (i=0; i < unit_indexes.size(); i++){
    oss<<"PHYS "<<course_codes_container[sorted_indexes[i].second]<<" "
      <<course_name_container[sorted_indexes[i].second];
    course_titles.push_back(oss.str());
    oss.str(""); //clearing stringstream

  }
  
  std::vector<std::string>::iterator vector_begin{course_titles.begin()};
  std::vector<std::string>::iterator vector_end{course_titles.end()};
  std::vector<std::string>::iterator vector_iterator;
  for(vector_iterator=vector_begin;vector_iterator<vector_end;++vector_iterator){
    std::cout<<*vector_iterator<<std::endl;
  }

  //Freeing memory
  std::vector<int>().swap(course_codes_container);
  std::vector<std::string>().swap(course_name_container);
  std::vector<std::string>().swap(course_titles);
  
  //Marks for unit courses
  std::vector<double> unit_marks;
  std::vector<std::string> unit_names;
  for (i = 0; i < unit_indexes.size(); i++){
    unit_marks.push_back(marks_container[unit_indexes[i]]);
  }
  //Computing number of results, mean, standard deviation and standard error
  std::cout<<"\n"<<"Your search returned "<<unit_indexes.size()<<" resutls."<<std::endl;
  std::cout<<"The mean mark for these modules is "<<mean_value(unit_marks)<<" +/- ";
  std::cout<<std_error(unit_marks)<<"%"<<std::endl;
  std::cout<<"The standard deviation for these modules is "<<std_dev(unit_marks)<<std::endl;

  } else if(list_specifier == "y"){
    std::string sort_specifier;
    std::cout<<"Would you like to sort the list of courses by title or code (t/c)?"<<std::endl;
    std::cin>>sort_specifier;
    int index;
    std::vector<std::pair<std::string, int> > sorted_indexes;
    
    if (sort_specifier ==" c"){
      
      for (index=0; index<marks_container.size(); ++index){
        sorted_indexes.push_back(std::make_pair(std::to_string(course_codes_container[index]), 
        index));
      }
      sort(sorted_indexes.begin(), sorted_indexes.end());

    } else if (sort_specifier == "t"){
      
      for (index=0; index<course_name_container.size(); ++index){
        sorted_indexes.push_back(std::make_pair(course_name_container[index], index));
      }
      sort(sorted_indexes.begin(), sorted_indexes.end());
    } else{
      std::cout<<"Your input was not recognised."<<std::endl;
    }
    //Creating course title using stringstream
    std::ostringstream oss;
    std::vector<std::string> course_titles;
    int i;
    
    for (i = 0; i < marks_container.size(); i++){
      oss<<"PHYS "<<course_codes_container[sorted_indexes[i].second]<<" "
      <<course_name_container[sorted_indexes[i].second];
      
      course_titles.push_back(oss.str());
      oss.str(""); //Clearing stringstream

  }
  
  //Printing out course titles using iterator
  std::vector<std::string>::iterator vector_begin{course_titles.begin()};
  std::vector<std::string>::iterator vector_end{course_titles.end()};
  std::vector<std::string>::iterator vector_iterator;
  for(vector_iterator=vector_begin;vector_iterator<vector_end;++vector_iterator){
    std::cout<<*vector_iterator<<std::endl;
  }

  //Freeing memory
  std::vector<int>().swap(course_codes_container);
  std::vector<std::string>().swap(course_name_container);
  std::vector<std::string>().swap(course_titles);

  //Computing number of results, mean, standard deviation and standard error
  std::cout<<"\n"<<"Your search returned "<<marks_container.size()<<" resutls."<<std::endl;
  std::cout<<"The mean mark for these modules is: "<<mean_value(marks_container)<<" +/- ";
  std::cout<<std_error(marks_container)<<"%"<<std::endl;
  std::cout<<"The standard deviation for these modules is: "<<std_dev(marks_container)<<std::endl;
  

  } else{
    std::cout<<"Your input was not recognised."<<std::endl;
  }


  return 0;
}
