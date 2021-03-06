/*
  ==============================================================================

    Application.h
    Created: 11 Apr 2012 1:28:10pm
    Author:  Peter Vasil

  ==============================================================================

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#ifndef __APPLICATION_H_BBAD6F5C__
#define __APPLICATION_H_BBAD6F5C__

#include "JuceHeader.h"

namespace synthamodeler
{
class MainAppWindow;
class PostWindow;
class SAMLookAndFeel;
class ObjectsHolder;
//==============================================================================
/**
 * The main class of the Synth-A-Modeler Designer application.
 *
 * Here happens all initialization on startup and all cleaning up on shutdown.
 *
 */
class SAMApplication  : public JUCEApplication
{
public:
    //==============================================================================
    SAMApplication();
    ~SAMApplication();

    //==============================================================================
    void initialise (const String& commandLine);
    void shutdown();
    //==============================================================================
    void systemRequestedQuit();
    void closeWindow(MainAppWindow* w);
    //==============================================================================
    const String getApplicationName();
    const String getApplicationVersion();
    bool moreThanOneInstanceAllowed();
    void anotherInstanceStarted (const String& commandLine);

    static SAMApplication* getApp();

    String getUniqueMDLPath(const String& mdlPath);

    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);
    bool isCommandActive (const CommandID commandID);

    ObjectsHolder* getActiveHolderComponent();
    //==============================================================================
    /**
     * MainMenuModel is actually the menu bar.
     *
     */
    class MainMenuModel  : public MenuBarModel
    {
    public:
        MainMenuModel();
        StringArray getMenuBarNames();

        PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/);
        void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/);
    };

    enum
    {
        recentProjectsBaseID = 100,
        exportersBaseID = 200,
        snappingSizesBaseID = 300,
        activeMDLsBaseID = 400,
    };


    ScopedPointer<MainMenuModel> menuModel;

    bool openFile(const File& file);

    virtual MainAppWindow* createNewMainWindow();

    void creatNewMDLDocument();
    void askUserToOpenFile();
    bool closeAllDocuments (bool askUserToSave);
    void updateRecentProjectList();

    void togglePostWindowFocus();

    void mainWindowActivityChanged (MainAppWindow* mainWin);

    void removeFromFileList(const File& f);

    void updateOpenFilesList();
    void addToFileList(const File& newFile);

private:
    OwnedArray<MainAppWindow> mainWindows;

    MainAppWindow* getFrontmostWindow();
    MainAppWindow* getOrCreateFrontmostWindow();
    MainAppWindow* getOrCreateEmptyWindow();
    void avoidSuperimposedWindows (MainAppWindow* const mw);

    class AsyncQuitRetrier  : public Timer
    {
    public:
    	AsyncQuitRetrier();
    	void timerCallback();
    	JUCE_DECLARE_NON_COPYABLE (AsyncQuitRetrier);
    };

    ScopedPointer<SAMLookAndFeel> samLookAndFeel;
#ifdef DEBUG
    ScopedPointer<Logger> samLogger;
#endif

    bool isRunningCommandLine;

    HashMap<String, String> openFilesList;

    void togglePropertiesWindow();
    void togglePropertiesWindowAlwaysOnTop();

    void switchToNextWindow();
    void switchToPreviousWindow();

    Array<MainAppWindow*> activeWindowsList;
    //==============================================================================

};
}


#endif  // __APPLICATION_H_BBAD6F5C__
