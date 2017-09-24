#include <vector>
#include <cmath>
#include <cassert>
#include <dlib/all/source.cpp>
#include <memory>
#include <array>
#include <chrono>
#include <dlib/iosockstream.h>

std::array<int, 40001> solutions, solutions0, minrange, minrange0, maxrange, maxrange0;

int proc(std::vector<int>& input, int T)
{
    const std::size_t N = input.size();
    for (std::size_t i = 0; i < N+1; ++i)
    {
        solutions[i] = 1;
        minrange[i] = input[i];
        maxrange[i] = input[i];
    }
    for (std::size_t k = 2; k < N+1; ++k)
    {
        minrange0 = minrange;
        maxrange0 = maxrange;
        solutions0 = solutions;
        for (std::size_t i = 0; i < N+1-k; ++i)
        {
            const std::size_t j = i + k;
            auto minrange_ = std::min(minrange0[i+1], minrange0[i]);
            auto maxrange_ = std::max(maxrange0[i+1], maxrange0[i]);
            minrange[i] = minrange_;
            maxrange[i] = maxrange_;
            if (maxrange_ - minrange_ <= T)
            {
                solutions[i] = k;
            }
            else
            {
                solutions[i] = std::max(solutions0[i+1], solutions0[i]);
            }
        }
    }
    return solutions[0];
}

bool recv(dlib::iosockstream& sock, std::vector<int>& input, std::size_t& T, bool print)
{
    auto begin = std::chrono::system_clock::now();
    input.clear();
    std::string tmp;
    std::getline(sock, tmp);
    if (tmp == " limit exceeded!")
    {
        return false;
    }
    std::size_t size;
    sock >> T;
    sock >> size;
    input.resize(size);
    for (std::size_t i = 0; i < size; ++i)
    {
        sock >> input[i];
    }
    auto now  = std::chrono::system_clock::now();
    std::cout << "parsing took " << std::chrono::duration<double>(now - begin).count() << " seconds to parse" << "\n";
    return true;
}

void send(dlib::iosockstream& sock, int result)
{
    std::cout << "senging " << result << "\n";
    sock << result << "\n";
    std::string temp;
    sock >> temp;
    std::cout << "answrr " << temp << std::endl;
}

int main(int argc, char* argv[])
{

    std::array<char, 1000000> buffer;
    std::vector<int> solutions(1000, 0);

    std::ifstream in("in.txt", std::ifstream::in);
    if (in.good())
    {
        int i = 0;
        while (in >> solutions[i++]);

        std::cout << "cached solutions = " << std::endl;
        for (int j = 0; j < solutions.size(); ++j)
        {
            std::cout << solutions[j] << std::endl;
        }
    }

    while(true)
    {
        std::cout << "trying again" << std::endl;
        dlib::iosockstream sock("lab.shellterlabs.com:" + std::string(argv[1]));
        std::string tmp;
        int i=0;
        while (tmp != "start")
        {
            sock >> tmp;
        }
        sock << "\n";

        std::vector<int> input;
        input.reserve(80000);
        std::size_t T;
        int current = 0;
        std::ofstream txt("out.txt", std::ofstream::app);
        txt << "BEGIN\n";

        while (recv(sock, input, T, current > 84))
        {
            std::cout << "CURRENT #" << current << std::endl;
            int result = solutions[current];
            if(result == 0)
            {
                result = proc(input, T);
                solutions[current] = result;
            }
            ++current;
            txt << result << "\n";
            send(sock, result);
        }
        txt << "END\n";
    }
}
