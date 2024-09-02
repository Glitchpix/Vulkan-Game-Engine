#include <core/asserts.hpp>
#include <core/logger.hpp>

// TODO: Test
#include <core/application.hpp>

int main(void) {

    Application app{100, 100, 1280, 720, "Test Engine"};

    app.run();
    
    return 0;
}