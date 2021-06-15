#ifndef LITTLEENGINE_PIPELINE_HPP
#define LITTLEENGINE_PIPELINE_HPP


#include <string>
#include <vector>


class Pipeline {
public:
    Pipeline(const std::string& vert, const std::string& frag);

    ~Pipeline();

private:
    static std::vector<char> readFile(const std::string& name);
};


#endif //LITTLEENGINE_PIPELINE_HPP
