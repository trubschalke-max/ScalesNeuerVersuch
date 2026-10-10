#include "ErrorCode.hpp"

std::string toString(ErrorCode code){
    switch(code){
        case EC::OK: return "OK";
        case EC::FAILURE: return "FAILURE";
        case EC::ALREADY_REGISTERED: return "ALREADY_REGISTERED";
        case EC::DUPLICATE_LIST_INSERTION: return "DUPLICATE_LIST_INSERTION";
        case EC::FONT_OPENING_FAIL: return "FONT_OPENING_FAIL";
        case EC::INVALID_RECT: return "INVALID_RECT";
        case EC::NULL_DRAWING_IN_EVENT_HANDLER: return "NO_DRAWING_IN_EVENT_HANDLER";
        case EC::NOTHING_HAPPENED: return "NOTHING_HAPPENED";
        case EC::NOT_ACTUALLY_CLICKED: return "NOT_ACTUALLY_CLICKED";
        case EC::SUCCESS: return "SUCCESS";
        case EC::DRAWING_MUST_NOT_HANDLE_EVENTS: return "DRAWING_MUST_NOT_HANDLE_EVENTS";
        case EC::SCALE_BUTTON_REFRESHING_FAIL: return "SCALE_BUTTON_REFRESHING_FAIL";
        case EC::TOO_BIG_RECT: return "TOO_BIG_RECT";
        case EC::PUTTING_INTO_EFFECT_NOT_POSSIBLE: return "PUTTING_INTO_EFFECT_NOT_POSSIBLE";
        case EC::MULTIPLE_CORRECT_VALUES: return "MULTIPLE_CORRECT_VALUES";
        case EC::MULTIPLE_SUGGESTIONS: return "MULTIPLE_SUGGESTIONS";
        case EC::NO_CORRECT_VALUE: return "NO_CORRECT_VALUE";
        case EC::INVALID_WORD_SELECTION: return "INVALID_WORD_SELECTION";
        case EC::CORRECT_WORD_ALREADY_SELECTED: return "CORRECT_WORD_ALREADY_SELECTED";
        default: std::string tmp = "ERROR_CODE_NUMBER: ";        
        tmp.append(std::to_string((int) code));
        return tmp.c_str();
    }
}

std::ostream& operator<<(std::ostream& os, const ErrorCode& ec){return os<<toString(ec);}
void checkForError(ErrorCode newEC, ErrorCode& oldEC){if(oldEC == ErrorCode::OK) oldEC = newEC;}
void checkForError(int newEC, ErrorCode& oldEC){checkForError((ErrorCode)newEC, oldEC);}


void ErrorHandling::checkForErrorHierarchy(ErrorCode newEC, ErrorCode& oldEC, ErrorList hierarchy){
    for(ErrorCode ec: hierarchy){
        if(newEC == ec){
            return;
        }
        if(oldEC == ec){
            oldEC = newEC;
        }
    }
}
void ErrorHandling::checkForErrorStandardHierarchy(ErrorCode newEC, ErrorCode& oldEC){
    checkForErrorHierarchy(newEC, oldEC, standardErrorHierarchy());
}

ErrorList ErrorHandling::standardErrorHierarchy(){return {ErrorCode::NOTHING_HAPPENED, ErrorCode::OK, ErrorCode::IGNORE_THIS, ErrorCode::MERELY_VISUAL_IMPLICATIONS};}

void checkIfSomethingHappened(ErrorCode newEC, ErrorCode& oldEC){if(oldEC == ErrorCode::NOTHING_HAPPENED) oldEC = newEC;}
