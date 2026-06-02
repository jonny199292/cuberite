#include <print>
#include <expected>
#include <vector>

int main() {
    std::expected<int, const char*> e = 42;
    if (e) {
        std::print("Hello C++23! Value: {}\n", *e);
    }
    return 0;
}
