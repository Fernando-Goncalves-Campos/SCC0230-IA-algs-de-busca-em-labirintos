#include <fstream>
#include <vector>

template <typename T, typename Alloc1, typename Alloc2>
void createCSV(const std::string&& filename, const std::vector<std::string>&& colNames, const std::vector<std::vector<T, Alloc1>, Alloc2>& data){
    std::fstream fout(filename + ".csv", std::ofstream::out | std::ofstream::trunc);

    for(int j = 0; j < colNames.size(); j++){
        if(j){
            fout << ',';
        }
        fout << colNames[j];
    }

    for(int i = 0; i < data.size(); i++){
        fout << '\n';
        for(int j = 0; j < data[i].size(); j++){
            if(j){
                fout << ',';
            }
            fout << data[i][j];
        }
    }
    fout.close();
}