#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <unistd.h> 

static std::string getexepath()
{
    static bool flag = false;
    static std::string path;
    if (!flag)
    {
        #define PATH_MAX 10000
        char result[ PATH_MAX ];
        readlink( "/proc/self/exe", result, PATH_MAX );
        auto found = std::string(result).find_last_of("/");
        path = std::string(result).substr(0,found) + "/";
        flag = true;
        #undef PATH_MAX
    }
    return path;
}

static std::string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
        {
            result += buffer.data();
        }
    }
    return result;
}


static std::string genom(const std::string& args)
{
    return exec((getexepath() + "genom " + args).c_str());
}
 
TEST(Graph, LinearGraph)
{
    std::string result = genom(getexepath() + "../test/brack1 " + getexepath() + "../test/linear_graph");
    ASSERT_TRUE(result.find("(0, 6, P)") != std::string::npos);
}

TEST(Graph, GraphWithOnePoint)
{
    std::string result = genom(getexepath() + "../test/brack1 " + getexepath() + "../test/one_point_graph");
    ASSERT_TRUE(result.find("(0, 5, P)") != std::string::npos);
}

TEST(Graph, HardGraph)
{
    std::string result = genom(getexepath() + "../test/brack1 " + getexepath() + "../test/hard_graph");
    ASSERT_TRUE(result.find("(0, 4, P)") != std::string::npos);
}

TEST(Genom, PerformanceTest)
{
    std::string result = genom(getexepath() + "../test/grammar");
}
 
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
