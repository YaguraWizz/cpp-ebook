#include <vector>
#include <iostream>
#include <iomanip>

//#define DEBUG
#ifdef DEBUG
#include<fstream>
#endif
#ifdef DEBUG
#include <chrono>
namespace helper {
    class Timer {
    public:
        Timer(std::string msg)
            : start_time_(std::chrono::high_resolution_clock::now()), msg_(msg)
        {}

        ~Timer() {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_).count();
            std::cerr << msg_ << ": " << duration << " ms\n";
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
        std::string msg_;
    };

#define TIMER(msg) helper::Timer timer_##__LINE__(msg)
}
#else
#define TIMER(msg)
#endif

namespace ebook {
    // Класс для обработки книги и состояний пользователей
    class EBookManager {
    public:
        EBookManager(std::ostream& output)
            : user_page_counts_(MAX_USER_COUNT_ + 1, -1),  // -1 значит, что не случилось ни одного READ
            page_achieved_by_count_(MAX_PAGE_COUNT_ + 1, 0), output_(output)
        {
        }

        void Read(int user_id, int page_count) {
            UpdatePageRange(user_page_counts_[user_id] + 1, page_count + 1);
            user_page_counts_[user_id] = page_count;
        }

        void Cheer(int user_id) const {
            const int pages_count = user_page_counts_[user_id];
            if (pages_count == -1) {
                PrintData(0.0); return;
            }
            const int user_count = GetUserCount();
            if (user_count == 1) {
                PrintData(1.0); return;
            }
            // По умолчанию деление целочисленное, поэтому
            // нужно привести числитель к типу double.
            // Один из простых способов сделать это — умножить его на 1.0.
            PrintData(1.0 - (page_achieved_by_count_[pages_count] - 1.0) / (user_count - 1.0));
        }

    private:
        static const int MAX_USER_COUNT_ = 100'000;
        static const int MAX_PAGE_COUNT_ = 1'000;

        // Номер страницы, до которой дочитал пользователь <ключ>
        std::vector<int> user_page_counts_;
        // Количество пользователей, дочитавших (как минимум) до страницы <индекс>
        std::vector<int> page_achieved_by_count_;
        std::ostream& output_;
       
        int GetUserCount() const {
            return page_achieved_by_count_[0];
        }

        void UpdatePageRange(int lhs, int rhs) {
            for (int i = lhs; i < rhs; ++i) {
                ++page_achieved_by_count_[i];
            }
        }

        void PrintData(double data) const {
            output_ << std::setprecision(6) << data << "\n";
        }
    };



    // Класс для чтения и хранения команд
    class CommandProcessor {
    public:
        CommandProcessor(std::istream& input, EBookManager& manager)
            : input_(input), manager_(manager) {}

        void ProcessCommands() {
            TIMER("ProcessCommands");
            size_t numCommands;
            input_ >> numCommands;
            for (size_t i = 0; i < numCommands; ++i) {
                // тип запроса 
                std::string commandType;
                input_ >> commandType;
                // id user
                int user;
                input_ >> user;

                if (commandType == "READ") {
                    int page;
                    input_ >> page;
                    manager_.Read(user, page);
                }
                else if (commandType == "CHEER") {
                    
                    manager_.Cheer(user);
                }
            }
        }

    private:
        std::istream& input_;
        EBookManager& manager_;
    };

#ifdef DEBUG
    namespace details {
        void runTest(const std::string& testPath, int testNumber) {
            std::cout << "Start Test " << testNumber << "\n";

            std::string input_file = "input.txt";
            std::string output_file = "rezult.txt";

            std::ifstream InFile(testPath + input_file);
            std::ofstream OutFile(testPath + output_file, std::ios::trunc);

            if (!InFile.is_open()) {
                std::cerr << "Error: Unable to open input file: " << testPath + input_file << "\n";
                return;
            }

            if (!OutFile.is_open()) {
                std::cerr << "Error: Unable to open output file: " << testPath + output_file << "\n";
                return;
            }

            ebook::EBookManager manager(OutFile);
            ebook::CommandProcessor processor(InFile, manager);
            processor.ProcessCommands();

            std::cout << "Complete Test " << testNumber << "\n";
        }
    }
#endif
}

int main() {
    TIMER("Totals");
#ifdef DEBUG
    ebook::details::runTest(R"(../test/test-1/)", 1);
    ebook::details::runTest(R"(../test/test-2/)", 2);
    ebook::details::runTest(R"(../test/test-3/)", 3);
#else
    ebook::EBookManager manager(std::cout);
    ebook::CommandProcessor processor(std::cin, manager);
    processor.ProcessCommands();
#endif
    return 0;
}
