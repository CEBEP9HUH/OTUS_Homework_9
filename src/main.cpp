#include "DuplicateFinder.hpp"
#include "File.hpp"

#include <iostream>

int main(int argc, char* argv[]){
/*     auto a = DF::HashType::MD5;
    std::experimental::filesystem::path p1("/home/nikolay/Projects/vcpkg/vcpkg/LICENSE.txt");
    std::experimental::filesystem::path p2("/home/nikolay/Projects/LICENSE.txt");
    std::experimental::filesystem::path p3("/home/nikolay/Projects/LICENSE.txt");
    DF::File f1(p1, a, 32);
    DF::File f2(p2, a, 32);
    DF::File f3(p3, a, 32);
    std::cout << (f3==f2) << '\n';
    std::cout << (f1==f3) << '\n'; */
    DF::DuplicateFinder finder(argc, argv);
    finder.run();
    return 0;
}