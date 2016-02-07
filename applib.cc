/**
 * @file applib.cc
 * @brief Definitions for AppLib class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "applib.h"
#include "applib-private.h"
#include "assert.h"

#include <QTranslator>
#include <QLocale>
#include <QWidget>
#include <QCoreApplication>
#include <QDateTime>

#include <translate/translang.h>
#include <translate/translate.h>

#ifndef TMP_A
#   define TMP_A(__s__) __s__.toLatin1 ().constData ()
#endif

/* ------------------------------------------------------------------------- */
class TimeInterval {

    QDateTime t1_;
    QDateTime t2_;
    int secs_in_min_;
    int hours_run_;
    int min_in_hour_;

public:

    TimeInterval (
            const QDateTime & t1,
            const QDateTime & t2 = QDateTime::currentDateTime ()):
        t1_(t1), t2_(t2)
    {
        qint64 secs_run = t1_.secsTo (t2_);
        qint64 min_run = secs_run / 60;
        secs_in_min_ = static_cast<int> (secs_run - min_run * 60);
        hours_run_ = static_cast<int> (min_run / 60);
        min_in_hour_ = static_cast<int> (min_run - hours_run_ * 60);
    }

    QString
    toString (QString pattern = QString ()) const {
        if (pattern.isEmpty()) {
            pattern = QCoreApplication::translate (
                        "%1 hours, %2 minutes and %3 seconds",
                        "AppLib");
        }
        return QString ()
                .arg (hours_run_)
                .arg (min_in_hour_, 10, 2, QChar('0'))
                .arg (secs_in_min_, 10, 2, QChar('0'));
    }

    static QString
    simplePattern () {
        return QLatin1String ("%1:%2:%3");
    }

    const QDateTime &
    start () const {
        return t1_; }

    const QDateTime &
    end () const {
        return t2_; }

    int
    miliseconds () const {
        return static_cast<int>(t1_.msecsTo (t2_));
    }

    int
    seconds () const {
        return static_cast<int>(t1_.secsTo (t2_));
    }

};
/* ========================================================================= */

/**
 * @class AppLib
 *
 * Detailed description.
 */

AppLib * AppLib::singleton_ = NULL;

/* ------------------------------------------------------------------------- */
/**
 * Detailed description for constructor.
 */
AppLib::AppLib() : QObject (),
    app_start_moment_ (QDateTime::currentDateTime ()),
    gui_mode_ (false),
    mw_ (NULL),
    state_ (InitialState)
{
    APPLIB_TRACE_ENTRY;
    Q_ASSERT (singleton_ == NULL);
    singleton_ = this;
    APPLIB_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * Detailed description for destructor.
 */
AppLib::~AppLib()
{
    APPLIB_TRACE_ENTRY;
    assert(state_ == TerminatedState);
    APPLIB_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void AppLib::end ()
{
    if (singleton_ != NULL) {
        singleton_->changeState (TerminatingState);
        singleton_->_end ();
        singleton_->changeState (TerminatedState);
        singleton_->deleteLater();
        singleton_ = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool AppLib::startGui ()
{
    Q_ASSERT(singleton_ != NULL);
    singleton_->changeState (RunningGuiState);
    singleton_->mw_ = singleton_->_startGui ();
    singleton_->connect (singleton_->mw_, SIGNAL(guiEnding ()),
             singleton_, SIGNAL(guiEnding ()));
    singleton_->connect (singleton_->mw_, SIGNAL(guiEnded ()),
             singleton_, SIGNAL(guiEnded ()));
    emit singleton_->guiStarted();
    return singleton_->mw_ != NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
AppLib * AppLib::uniqAppLib ()
{
    assert (singleton_ != NULL);
    return singleton_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
AppLib::BuildType AppLib::buildType()
{
    assert (singleton_ != NULL);
    return singleton_->_buildType ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void AppLib::setGUI (bool b_gui)
{
    assert (singleton_ != NULL);
    singleton_->gui_mode_ = b_gui;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool AppLib::changeState (AppLib::State value)
{
    bool b_ret = false;
    switch (state_) {
    case InitialState: {
        if (value != InitializingState) {
            APPLIB_DEBUGM("Only valid state from Initial is Initializing\n");
            break;
        }
        APPLIB_DEBUGM("APPLIB: ==========================================\n");
        APPLIB_DEBUGM("APPLIB: lib %s is being initialized\n", appUserName ());
        APPLIB_DEBUGM("APPLIB: %s\n", app_start_moment_.toString ().toLatin1 ().constData ());
        APPLIB_DEBUGM("APPLIB: ==========================================\n");

        switch (buildType ()) {
        case ReleaseWithDebugBuild: {
            qsrand (QDateTime::currentDateTime().toTime_t());
            APPLIB_DEBUG ("LIBMAKEINST: Release version with debug information\n");
            break; }
        case DebugBuild: {
            qsrand (QDateTime::currentDateTime().toTime_t());
            APPLIB_DEBUG ("LIBMAKEINST: Debug version\n");
            break; }
        case ReleaseBuild: {
            APPLIB_DEBUG ("LIBMAKEINST: Release version\n");
            break; }
        }

        state_ = value;
        emit libStarting ();
        break;
    }
    case InitializingState: {
        if (
                (value != RunningState) ||
                (value != RunningGuiState) ||
                (value != TerminatingState)) {
            APPLIB_DEBUGM("Application can go from Initializing "
                          "only to Running or Terminating\n");
            break;
        }
        TimeInterval ti (app_start_moment_);
        APPLIB_DEBUGM("APPLIB: ==========================================\n");
        APPLIB_DEBUGM("APPLIB: %s\n", TMP_A(ti.start ().toString ()));
        APPLIB_DEBUGM("APPLIB: lib %s was started in %d miliseconds\n",
                      TMP_A(appUserName ()), ti.miliseconds ());
        APPLIB_DEBUGM("APPLIB: ==========================================\n");

        emit libStarted();
        b_ret = true;
        break;
    }

    case RunningState: {
        if (
                (value != RunningGuiState) ||
                (value != TerminatingState)) {
            APPLIB_DEBUGM("Application can go from Running "
                          "only to Running with GUI or Terminating\n");
            break;
        }

        if (value == TerminatingState) {
            APPLIB_DEBUGM("APPLIB: ==========================================\n");
            APPLIB_DEBUGM("APPLIB: lib %s is being terminated\n",
                          TMP_A(appUserName ()));
            APPLIB_DEBUGM("APPLIB: ==========================================\n");
            emit libEnding ();
        } else {
            APPLIB_DEBUGM("APPLIB: %s GUI is starting\n", TMP_A(appUserName ()));
            emit guiStarting ();
        }

        b_ret = true;
        break;
    }

    case RunningGuiState: {
        if (
                (value != RunningState) ||
                (value != TerminatingState)) {
            APPLIB_DEBUGM("Application can go from Running "
                          "only to Running without GUI or Terminating\n");
            break;
        }

        if (value == TerminatingState) {
            APPLIB_DEBUGM("APPLIB: ==========================================\n");
            APPLIB_DEBUGM("APPLIB: lib %s is being terminated\n",
                          TMP_A(appUserName ()));
            APPLIB_DEBUGM("APPLIB: ==========================================\n");
            emit libEnding ();
        } else {
            APPLIB_DEBUGM("APPLIB: %s lost its GUI\n", TMP_A(appUserName ()));
            emit guiEnded ();
        }

        b_ret = true;
        break;
    }

    case TerminatingState: {
        if (value != TerminatedState) {
            APPLIB_DEBUGM("Application can go from Terminating "
                          "only to Terminated\n");
            break;
        }

        TimeInterval ti (app_start_moment_);
        APPLIB_DEBUGM("APPLIB: ==========================================\n");
        APPLIB_DEBUGM("APPLIB: %s\n", TMP_A(ti.start ().toString ()));
        APPLIB_DEBUGM("APPLIB: lib %s has run %s\n",
                      TMP_A(appUserName ()), ti.toString ());
        APPLIB_DEBUGM("APPLIB: ==========================================\n");
        emit libEnded();

        b_ret = true;
        break;
    }

    default:
        APPLIB_DEBUGM("Invalid current state\n");
        Q_ASSERT(false);
    }

    if (b_ret) {
        state_ = value;
    }

    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * The method must be explicitly called by the implementation, maybe inside
 * _init() implementation.
 *
 * Example:
 * @code
 * QString locale = APP_STGS->valueS (
 *     STG_LANGUAGE, QLocale::system().name());
 * if (startTranslation (locale)) {
 *     APP_STGS->setValue (STG_LANGUAGE, locale);
 * }
 * @endcode
 *
 * @param locale (in/out) the name of the locale to use; save the resulted value to settings
 * @return true if we were able to load the language
 */
bool AppLib::startTranslation (QString & locale)
{
    // each language has its own directory that contains
    // a compiled translation file (.qm), a metadata file
    // loadable by QSettings (metadata.ini) and an icon.png file.
    QString s_error;
    if (!Translate::init (&s_error)) {
        APPLIB_DEBUGM(TMP_A(s_error));
        return false;
    }
    if (Translate::count () <= 0) {
        APPLIB_DEBUGM("No available translations");
        return false;
    }

    // Name of the language or default
    if (locale.isEmpty ()) {
        APPLIB_DEBUGM("No default language");
        return false;
    }

    // see if we have this language
    int lang = Translate::itemIndex (locale);
    if (lang == -1) {
        lang = Translate::itemIndexFromLocale (QLocale::system().name());
        if (lang == -1) {
            APPLIB_DEBUGM("Locale %s not available", TMP_A(locale));
            return false;
        }
        locale = Translate::item (lang).langName();
    }

    QTranslator * translator;
    // order is important when installing translators
    translator = Translate::qtTranslator (lang);
    if (translator == NULL) {
        APPLIB_DEBUGM("No qt translation for locale %s", TMP_A(locale));
    } else {
        qApp->installTranslator (translator);
    }

    translator = Translate::translator (lang);
    if (translator == NULL) {
        APPLIB_DEBUGM("Failed to load existing locale %s", TMP_A(locale));
        return false;
    }
    qApp->installTranslator (translator);

    // save this in case it was default value
    Translate::setCurrent (lang);

    return true;
}
/* ========================================================================= */

