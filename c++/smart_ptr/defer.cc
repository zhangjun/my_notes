#include <iostream>
#include <memory>

using defer = std::shared_ptr<void>;

int main() {
    defer _(nullptr, [](...){ std::cout << "defer run." << std::endl; });
    std::cout << "hello." << std::endl;
    return 0;
}
