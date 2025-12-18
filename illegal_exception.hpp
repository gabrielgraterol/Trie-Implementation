#include <iostream> 


/*
    CITATION:

    I referenced a video tutorial (https://www.youtube.com/watch?v=fq-ibN49bXI) on how to create a custom exception class.
*/

class illegal_exception : public std::exception {
    public: 

    illegal_exception(const std::string& phrase)
        : illegal_phrase("illegal argument") {}

    const char* what() const noexcept {
        return illegal_phrase.c_str();
    }

    private:
        std::string illegal_phrase;
};