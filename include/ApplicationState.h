/*******************************************************************//*
 * Abstract application state class. ApplicationState subclasses must
 * override the pure virtual methods to satisfy the main game loop
 *
 * @author      Brandon To
 * @version     1.0
 * @since       2014-08-05
 * @modified    2014-09-05
 *********************************************************************/
#ifndef SPACESHOOTER_APPLICATIONSTATE_H_
    #define SPACESHOOTER_APPLICATIONSTATE_H_

class ApplicationStateManager;
class WindowElements;

class ApplicationState
{
    private:

    public:
        //Destructor
        virtual ~ApplicationState();

        //Methods
        virtual void onEnter();
        virtual void onEvent() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onExit();
        virtual void stateTransition(int nextState);

    protected:
        //Fields
        ApplicationStateManager* applicationStateManager;
        WindowElements* windowElements;
};


/*******************************************************************//*
 * Enumerations for various states
 *********************************************************************/
enum ApplicationStates
{
    //Application states
    STATE_NULL,
    STATE_INTRO,
    STATE_MENU,
    STATE_GAME,
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_OPTIONS,
    STATE_INSTRUCTIONS,
    STATE_HIGHSCORES,
    STATE_QUIT,
    STATE_CREDITS,
    STATE_EXIT,

    //Transitional states
    PRE_INTRO,
    PRE_MENU,
    POST_MENU,
    PRE_GAMEOVER,
    POST_QUIT,
};

#endif
