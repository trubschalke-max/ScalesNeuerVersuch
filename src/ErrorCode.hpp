#ifndef ERRORCODE_HPP
#define ERRORCODE_HPP
#include<string>
#include <initializer_list>

enum class ErrorCode{NOTHING_HAPPENED = -1, OK = 0, SUCCESS = 1, MERELY_VISUAL_IMPLICATIONS = 2, FAILURE = 0xF001, IGNORE_THIS = 0xF002, INVALID_SCALE_SELECTION = 0xF003, INVALID_COMMIT, ALREADY_COMMITTED, INVALID_REDOING, DRAWING_OVERSHOT_WINDOW = 0xF004, DRAWING_CONFLICT, INVALID_NODE_NUMBER = 0xF006, INVALID_BUTTON_TYPE, NODE_NOT_INITTED, GAMEMAIN_NOT_INITTED, EVENT_CREATION_FAIL, ALREADY_REGISTERED, 
    INVALID_REORDERING_BUTTON, REORDERING_IN_THE_SAME_ORDER, TRIGGER_NAME_EMPTY, TRIGGER_NAME_WRONG, SDL_RENDERER_NULL, NOT_YET_IMPLEMENTED,
    THERE_IS_NO_STANDARD_NAME, NULLPOINTER_EXCEPTION, DUPLICATE_LIST_INSERTION,
    FONT_OPENING_FAIL, WRONG_SIZE_OF_BUTTONS_VECTOR, INVALID_RECT, NULL_DRAWING_IN_EVENT_HANDLER, NOT_ACTUALLY_CLICKED, DRAWING_MUST_NOT_HANDLE_EVENTS, SCALE_BUTTON_REFRESHING_FAIL, TOO_BIG_RECT, PUTTING_INTO_EFFECT_NOT_POSSIBLE
};

typedef ErrorCode EC;
typedef std::initializer_list<ErrorCode> ErrorList;

void checkForError(ErrorCode newEC, ErrorCode& oldEC);
void checkForError(int newEC, ErrorCode& oldEC);
void checkIfSomethingHappened(ErrorCode newEC, ErrorCode& oldEC);
namespace ErrorHandling{
    ErrorList standardErrorHierarchy();
    void checkForErrorHierarchy(ErrorCode newEC, ErrorCode& oldEC, ErrorList hierarchy);
    void checkForErrorStandardHierarchy(ErrorCode newEC, ErrorCode& oldEC);
}

std::string toString(ErrorCode ec);
std::ostream& operator<<(std::ostream& os, const ErrorCode& ec);

#endif // ERRORCODE_HPP