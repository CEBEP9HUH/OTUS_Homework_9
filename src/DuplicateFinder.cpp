#include "DuplicateFinder.hpp"


#include <iostream>
#include <queue>
#include "experimental/filesystem"
#include "boost/program_options.hpp"
#include <fstream>
#include <iterator>


using namespace DF;
namespace fs = std::experimental::filesystem;


DuplicateFinder::DuplicateFinder(int argc, char* argv[]){
    parseOptions(argc, argv);
}


void DuplicateFinder::parseOptions(int argc, char* argv[]) {
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    std::vector<std::string> tempExc;
    std::string tempHash;
    desc.add_options()
    /*               option name           |                         option values                       |       description       */
                    ("help,h",                                                                             "provide help messages")
                    ("include,i",        po::value<inc_dirs_t>(&_options.includeDirs)->multitoken(),     "included directories") 
                    ("exclude,e",        po::value<std::vector<std::string> >(&tempExc)->multitoken(),   "excluded directories") 
                    ("depth,d",            po::value<size_t>(&_options.depth)->default_value(0),           "depth of nested folders scanning\n0 - is for current dirrectory only") 
                    ("document_size,D",   po::value<size_t>(&_options.documentSize)->default_value(1),    "minimum document size in bytes for scanning.\nDefault minimum size is equals to 1 byte") 
                    ("masks,m",            po::value<masks_t>(&_options.masks)->multitoken(),              "masks for names of files to scan") 
                    ("block_size,B",      po::value<std::streamsize>(&_options.blockSize)
                                                                                    ->default_value(256),  "size of block which can be read by a single read operation") 
                    ("hash_algorithm,H",  po::value<std::string>(&tempHash)->default_value("md5"),        "hash algoritm: crc32, md5") 
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);  
    _options.excludeDirs.insert(tempExc.begin(), tempExc.end());
    _options.hashType = HashSum::stringToHashType(tempHash);
    if(!vm["help"].empty()){
        std::cout << desc;
    }
}

void DuplicateFinder::findFiles(const std::string& path){
    std::queue<std::pair<fs::path, size_t> > dirsQueue;
    dirsQueue.emplace(fs::path(path), 0);
    while(!dirsQueue.empty()){
        auto dir = dirsQueue.front();
        dirsQueue.pop();
        for(auto& file : fs::directory_iterator(dir.first)){
            switch(file.status().type()){
            case fs::file_type::directory:
                if(_options.excludeDirs.find(file.path()) == _options.excludeDirs.end() && dir.second < _options.depth){
                    dirsQueue.emplace(file.path(), dir.second + 1);
                }
                break;
            case fs::file_type::regular:
                if(fs::file_size(file)>=_options.documentSize){
                    auto emplRes = _duplicateList.emplace( 
                                        std::piecewise_construct, 
                                        std::make_tuple(file.path(), _options.hashType, _options.blockSize), 
                                        std::make_tuple(1, file.path()));
                    if(!emplRes.second){
                        emplRes.first->second.emplace_back(file.path());
                    }
                }
                break;
            default: break;
            }
        }
    }
}

void DuplicateFinder::run(){
    for(auto& path : _options.includeDirs){
        if(_options.excludeDirs.find(path) == _options.excludeDirs.end()){
            if(fs::exists(path)){
                findFiles(path);
            } else {
                std::cerr << "Directory doesn't exists: " << path << std::endl;
            }
        }
    }
    printResults();
}

void DuplicateFinder::printResults(){
    for(auto& i : _duplicateList){
        if(i.second.size() > 1){
            std::cout << i.second.begin()->filename() << " has duplicates:\n";
            std::copy(std::next(i.second.begin()), i.second.end(), std::ostream_iterator<fs::path>(std::cout, "\n"));
            std::cout << "\n";
        }
    }
}