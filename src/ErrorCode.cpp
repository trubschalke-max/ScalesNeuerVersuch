#include "ErrorCode.hpp"
std::string toString(ErrorCode code){
    switch(code){
        case EC::OK: return "OK";
        case EC::FAILURE: return "FAILURE";
        case EC::ALREADY_REGISTERED: return "ALREADY_REGISTERED";
        case EC::DUPLICATE_LIST_INSERTION: return "DUPLICATE_LIST_INSERTION";
        case EC::FONT_OPENING_FAIL: return "FONT_OPENING_FAIL";
        default: std::string tmp = "ERROR_CODE_NUMBER: ";
        tmp.append(std::to_string((int) code));
        return tmp.c_str();
    }
}

std::ostream& operator<<(std::ostream& os, const ErrorCode& ec){return os<<toString(ec);}
