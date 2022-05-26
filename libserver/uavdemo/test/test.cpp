#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include   <fstream>
using namespace std;

using json = nlohmann::json;



static void case0()
{
    json j = {
            {"age", 23},
            {"name", "tanxiaohai"},
            {"jobs", {"superhero", "neighborhood"}},
            {"gear", {"suits", "2099"}}
    };
    ifstream ifs;
    ifs.open("../JsonBTree/UavBTree.json");
    std::stringstream ss;
    if ( !ifs)
    {
        cout << "文件不能打开" <<endl;
    }
    else {
        string str;

        ifs>>ss.rdbuf();
       // cout<<ss<<endl;
    }
    ifs.close();
    json j2;
    ss>>j2;
    cout<<j2.dump(2)<<endl;
    cout<<j2["array"]<<endl;

}


int main()
{
    case0();
    // case1();
    // case2();
    // case3();
    return 0;
}