#include <process/process.hh>

int main() {
    process::process(std::cin, std::cout, "<stdin>");
    return 0;
}
