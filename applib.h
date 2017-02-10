/**
 * @file applib.h
 * @brief Declarations for AppLib class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_APPLIB_H_INCLUDE
#define GUARD_APPLIB_H_INCLUDE

#include <applib/applib-config.h>
#include <QObject>
#include <QDateTime>

//! Base class for application's main library.
class APPLIB_EXPORT AppLib : public QObject {
    Q_OBJECT
public:

    //! The state of the application.
    enum State {
        InvalidState = -1,
        InitialState = 0,
        InitializingState,
        RunningState,
        RunningGuiState,
        TerminatingState,
        TerminatedState
    };

    //! The type of build.
    enum BuildType {
        DebugBuild,
        ReleaseBuild,
        ReleaseWithDebugBuild
    };

    //! Allows filtering the text stream comming from Qt.
    enum FilterQtMsg {
        NoFilter    = 0x00000001,

        ExcludeError = 0x00000002,
        ExcludeWarning = 0x00000004,
        ExcludeDebug = 0x00000008,
        ExcludeFatal = 0x00000010,
        ExcludeAll = ExcludeError | ExcludeWarning | ExcludeDebug | ExcludeFatal,

    };

protected:

    //! Default constructor.
    AppLib ();

    //! Destructor.
    virtual ~AppLib();

    //! copy constructor
    AppLib (const AppLib &) {}

    //! assignment operator
    AppLib& operator=( const AppLib& ) {
        return *this;
    }


public:

    //! end-up the mbtiles
    static void
    end ();

    //! GUI or console mode
    ///
    static  bool
    isGUI ();

    //! GUI or console mode
    static void
    setGUI (
            bool b_gui);

    //! start the gui
    ///
    static bool
    startGui ();

    //! The main gui.
    static QWidget *
    mainGui () {
        return singleton_->mw_;
    }

    //! The unique instance of the class.
    static AppLib *
    uniqAppLib ();

    //! Has the unique instance been created yet?
    static bool
    hasUniqAppLib () {
        return singleton_ != NULL;
    }

    //! The type of build (debug, release).
    static BuildType
    buildType ();

    //! Handler function for Qt messages.
    static void
    echoQtMessages (
            QtMsgType type,
            const QMessageLogContext & context,
            const QString &msg);

    //! Tell if a flag or combination of flags are set.
    bool
    isQtMsgFilterSet (
            int flag) {
        return ((fqmsg_ & flag) == flag);
    }

    //! Set a flag.
    void
    setQtMsgFilter (
            int flag) {
        fqmsg_ = (FilterQtMsg)(fqmsg_ | flag);
    }

protected:

    //! Subclass implements this to start-up the instance.
    virtual bool
    _init () {
        return true;
    }

    //! Subclass implements this to end-up the instance.
    virtual void
    _end () {}

    //! Subclass implements this to start the GUI.
    virtual QWidget *
    _startGui () {
        return NULL; }

    //! Subclass implements this to inform us about the type of build.
    virtual BuildType
    _buildType () {
        return ReleaseBuild; }

    //! The user name for this library.
    virtual QString
    appUserName () = 0;

    //! A no space, all lower name for this library.
    virtual QString
    appUnixName () = 0;


    //! Checks the transition, changes the state and generates proper signals.
    bool
    changeState (
            State value);

    //! Initializes the translation system.
    bool
    startTranslation (
            QString & locale);

signals:

    //! library is starting
    void
    libStarting ();

    //! library is started
    void
    libStarted ();

    //! library is ending
    void
    libEnding ();

    //! library is ended
    void
    libEnded ();

    //! gui is ending
    void
    guiStarting ();

    //! gui is ended
    void
    guiStarted ();

    //! gui is ending
    void
    guiEnding ();

    //! gui is ended
    void
    guiEnded ();

private:
    QDateTime app_start_moment_; /**< when was the application started ?*/
    bool gui_mode_; /**< is this a GUI application or not */
    QWidget * mw_; /**< main GUI object */
    State state_; /**< the state of the application */
    FilterQtMsg fqmsg_; /**< how to filter the messages from Qt */

    static AppLib * singleton_;
};

#endif // GUARD_APPLIB_H_INCLUDE
