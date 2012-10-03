/*
  ==============================================================================

    PrefsPanel.cpp
    Created: 19 Apr 2012 2:25:09pm
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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "PrefsPanel.h"
#include "../Utilities/StoredSettings.h"


//==============================================================================
class MiscPage  : public Component
{
public:
    MiscPage()
    : fcDataDir ("Synth-A-Modeler location:",
    		StoredSettings::getInstance()->getDataDir(),
    		true, true, false,
    		"*.plx", String::empty,
    		"(select the Synth-A-Modeler.plx file)"),
    	fcFaustDir ("FAUST directory:",
			StoredSettings::getInstance()->getFaustDir(),
			true, true, false,
			"*.*", String::empty,
			"(select the directory which faust is in)"),
		tbExportConfirm("Confirm export"),
        tbOpenFaustExport("Open Faust file after export"),
#ifdef DEBUG
        tbLoggingOn("Logging (After change, restart required)"),
#endif
        labelDataDir (String::empty, "Data Dir"),
		labelFaustDir (String::empty, "FAUST Dir")
    {
        addAndMakeVisible (&fcDataDir);
        labelDataDir.attachToComponent (&fcDataDir, false);
        addAndMakeVisible (&fcFaustDir);
        labelFaustDir.attachToComponent (&fcFaustDir, false);
        addAndMakeVisible(&tbExportConfirm);
        tbExportConfirm.setToggleState(StoredSettings::getInstance()->getIsExportConfirm(), false);
        addAndMakeVisible(&tbOpenFaustExport);
        tbOpenFaustExport.setToggleState(StoredSettings::getInstance()->getOpenFaustExport(), false);
#ifdef DEBUG
        addAndMakeVisible(&tbLoggingOn);
        tbLoggingOn.setToggleState(StoredSettings::getInstance()->getIsLoggingOn(), false);
#endif
    }

    ~MiscPage()
    {
        StoredSettings::getInstance()->setDataDir(fcDataDir.getCurrentFile().getFullPathName());
        StoredSettings::getInstance()->setFaustDir(fcFaustDir.getCurrentFile().getFullPathName());
        StoredSettings::getInstance()->setIsExportConfirm(tbExportConfirm.getToggleState());
        StoredSettings::getInstance()->setOpenFaustExport(tbOpenFaustExport.getToggleState());
#ifdef DEBUG
        StoredSettings::getInstance()->setIsLoggingOn(tbLoggingOn.getToggleState());
#endif
    }

    void resized()
    {
        fcDataDir.setBounds (10, 30, getWidth() - 20, 22);
        fcFaustDir.setBounds (10, 100, getWidth() - 20, 22);
        tbExportConfirm.setBounds(10, 220, getWidth() - 20, 22);
        tbOpenFaustExport.setBounds(10, 270, getWidth() - 20, 22);
#ifdef DEBUG
        tbLoggingOn.setBounds(10, 320, getWidth() - 20, 22);
#endif
    }

private:
    FilenameComponent fcDataDir;
    FilenameComponent fcFaustDir;
    ToggleButton tbExportConfirm;
    ToggleButton tbOpenFaustExport;
#ifdef DEBUG
    ToggleButton tbLoggingOn;
#endif
    Label labelDataDir;
    Label labelFaustDir;
};

//==============================================================================
class ExporterInputPanel : public DialogWindow
{
public:
	ExporterInputPanel(int exporterIndex)
	: DialogWindow("Exporter", Colours::lightgrey, true)
	{
		ExporterInputComponent* const eic = new ExporterInputComponent(*this, exporterIndex);
		eic->setSize(350, 100);

		setContentOwned (eic, true);

		centreAroundComponent (0, getWidth(), getHeight());

		setResizable (false, false);
	}

	~ExporterInputPanel()
	{

	}
    void closeButtonPressed()
    {
    	setVisible(false);
    }

    static void show(int exporterIndex)
    {
    	ExporterInputPanel eip(exporterIndex);
        eip.runModalLoop();
    }

private:
	int returnVal;
    class ExporterInputComponent : public Component,
                                   public Button::Listener,
                                   public TextEditor::Listener
    {
    public:
    	ExporterInputComponent(ExporterInputPanel& parent_, int exporterIndex_)
    	: parent(parent_),
    	  labelVarName("Variable"),
    	  labelVarValue("Value"),
    	  inputVarName("Input variable"),
    	  inputVarValue("Input value"),
    	  btOk("Ok"),
    	  btCancel("Cancel"),
          exporterIndex(exporterIndex_)
    	{
    		labelVarName.setText("Variable", false);
    		addAndMakeVisible(&labelVarName);
    		labelVarValue.setText("Value", false);
    		addAndMakeVisible(&labelVarValue);
            inputVarName.addListener(this);
    		addAndMakeVisible(&inputVarName);
            inputVarValue.addListener(this);
    		addAndMakeVisible(&inputVarValue);
    		btOk.addListener(this);
    		addAndMakeVisible(&btOk);
    		btCancel.addListener(this);
    		addAndMakeVisible(&btCancel);

            if(exporterIndex >= 0)
            {
                StringPairArray spa = StoredSettings::getInstance()->getExporters().getAllProperties();
                inputVarName.setText(spa.getAllKeys()[exporterIndex], false);
                inputVarValue.setText(spa.getAllValues()[exporterIndex], false);
            }

    	}

    	~ExporterInputComponent()
    	{
    	}

    	void resized()
    	{
    		labelVarName.setBounds(0, 5, 60, 22);
    		inputVarName.setBounds(60, 5, getWidth() - 65, 22);
    		labelVarValue.setBounds(0, 40, 60, 22);
    		inputVarValue.setBounds(60, 40, getWidth() - 65, 22);
    		btOk.setBounds(getWidth()/2 - 65, getHeight() - 30, 60, 22);
    		btCancel.setBounds(getWidth()/2 + 5, getHeight() - 30, 60, 22);
    	}

    	void buttonClicked(Button* button)
    	{
    		if(button == &btOk)
    		{
                applyEdits();
    		}
    		else if(button == &btCancel)
    		{
                cancelEdits();
    		}
    	}

        void applyEdits()
        {
            StringPairArray& spa = StoredSettings::getInstance()->getExporters().getAllProperties();
            if(exporterIndex >= 0)
            {
                // we have to copy the array first to keep order when editing a value.
                StringPairArray other;
                for (int i = 0; i < spa.size(); ++i)
                {
                    if(i == exporterIndex)
                    {
                        other.set(inputVarName.getText(), inputVarValue.getText());
                    }
                    else
                    {
                        other.set(spa.getAllKeys()[i], spa.getAllValues()[i]);
                    }
                }

                spa.clear();
                spa.addArray(other);
            }
            else
            {
                spa.set(inputVarName.getText(), inputVarValue.getText());
            }
            StoredSettings::getInstance()->getExporters().save();
            parent.closeButtonPressed();
        }

        void cancelEdits()
        {
            parent.closeButtonPressed();
        }

        void textEditorTextChanged(TextEditor& editor)
        {
        }
        void textEditorReturnKeyPressed(TextEditor& editor)
        {
            applyEdits();
        }
        void textEditorEscapeKeyPressed(TextEditor& editor)
        {
            cancelEdits();
        }

    private:
    	ExporterInputPanel& parent;
    	Label labelVarName;
    	Label labelVarValue;
    	TextEditor inputVarName;
    	TextEditor inputVarValue;
    	TextButton btOk;
    	TextButton btCancel;
        int exporterIndex;
    };

};


class ExporterTable : public Component,
                      public TableListBoxModel
{
public:
    ExporterTable()
    : table("Exporter table", this)
    {
        data = &StoredSettings::getInstance()->getExporters().getAllProperties();

        table.setColour (ListBox::outlineColourId, Colours::grey);
	    table.setOutlineThickness (1);

	    table.getHeader().addColumn("exporter",1,100);
	    table.getHeader().addColumn("value",2,300);

	    table.setMultipleSelectionEnabled(false);
		addAndMakeVisible(&table);
    }

    void resized()
	{
		table.setBounds(0, 0, getWidth(), getHeight());
	}
	int getNumRows()
	{
		return data->size();
	}
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height,
			bool rowIsSelected)
	{
		if(rowIsSelected)
			g.fillAll(Colours::lightseagreen);
	}

	void paintCell(Graphics& g, int rowNumber, int columnId, int width,
			int height, bool rowIsSelected)
	{
		g.setColour(Colours::black);
        StringArray keys = data->getAllKeys();
		String varName = keys[rowNumber];
		String varValue = data->getValue(varName, "");
		if(columnId == 1)
		{
			g.drawText(varName, 2, 0, width - 4, height, Justification::left, true);
		}
		else if(columnId == 2)
		{
			g.drawText(varValue, 2, 0, width -4, height, Justification::left, true);
		}
	}

	void addRow()
	{
        ExporterInputPanel::show(-1);
        table.updateContent();
	}

	void editRow()
	{
		int rowIndex = table.getSelectedRow();
		if(rowIndex >= 0)
		{
            ExporterInputPanel::show(rowIndex);
			table.updateContent();
			table.repaintRow(rowIndex);
		}
	}

	void removeSelectedRow()
	{
		int rowIndex = table.getLastRowSelected();
		if(rowIndex >= 0)
		{
            StringPairArray& spa = StoredSettings::getInstance()->getExporters().getAllProperties();
            StoredSettings::getInstance()->getExporters().removeValue(spa.getAllKeys()[rowIndex]);
			table.updateContent();
		}
	}
    void cellDoubleClicked (int rowNumber, int columnId, const MouseEvent& e)
    {
        ExporterInputPanel::show(rowNumber);
        table.updateContent();
        table.repaintRow(rowNumber);
    }
private:
    TableListBox table;
    StringPairArray* data;
};
class ExporterPage : public Component,
                     public Button::Listener
{
public:
    ExporterPage()
    : btAdd("+"),
	  btRemove("-")
    {
        btAdd.addListener(this);
        addAndMakeVisible(&btAdd);
        btRemove.addListener(this);
        addAndMakeVisible(&btRemove);
        addAndMakeVisible(&exporterTable);

    }

    void resized()
    {
        exporterTable.setBounds(5, 10, getWidth() - 20, getHeight() - 50);
        btAdd.setBounds(5, getHeight() - 30, 22, 22);
		btRemove.setBounds(30, getHeight() - 30, 22, 22);
    }

    void buttonClicked(Button* button)
	{
        if(button == &btAdd)
		{
			exporterTable.addRow();
		}
		else if( button == &btRemove)
		{
			exporterTable.removeSelectedRow();
		}
    }
private:
  	TextButton btAdd;
	TextButton btRemove;
    ExporterTable exporterTable;
};

//==============================================================================
class AboutPage   : public Component
{
public:
    AboutPage()
        : link ("https://github.com/ptrv/Synth-A-Modeler",
                URL ("https://github.com/ptrv/Synth-A-Modeler")),
          logo (ImageCache::getFromMemory (BinaryData::synthamodeler_icon_png,
        		  BinaryData::synthamodeler_icon_pngSize))
    {
        text1.setJustification (Justification::centredTop);
        String infoTxt(BinaryData::about_txt);
        text1.append (infoTxt, Font (13.0f));

        text2.setJustification (Justification::centred);
        String buildDate = " (Build: " + String(__DATE__) + " " + String(__TIME__) + ")";
        text2.append ("Synth-A-Modeler v" + JUCEApplication::getInstance()->getApplicationVersion()
                        + buildDate + ", " + SystemStats::getJUCEVersion(), Font (12.0f, Font::bold));

        addAndMakeVisible (&link);
        link.setFont (Font (10.0f, Font::bold | Font::underlined), true);
    }

    void paint (Graphics& g)
    {
        g.fillAll (Colour (0xffebebeb));
        g.drawImageWithin (logo, 0, 4, getWidth(), getHeight()*0.3,
                           RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
                           false);

        text1.draw (g, Rectangle<int> (12, getHeight()*0.4, getWidth() - 24, 100).toFloat());
        text2.draw (g, Rectangle<int> (12, getHeight() - 50, getWidth() - 24, 20).toFloat());
    }

    void resized()
    {
        link.setSize (100, 22);
        link.changeWidthToFitText();
        link.setTopLeftPosition ((getWidth() - link.getWidth()) / 2, getHeight() - link.getHeight() - 10);
    }

private:
    HyperlinkButton link;
    Image logo;
    AttributedString text1, text2;
};


//==============================================================================
static const char* miscPage = "Misc";
static const char* exporterPage = "Exporter";
static const char* aboutPage = "About";

class PrefsTabComp  : public PreferencesPanel
{
public:
    PrefsTabComp()
    {
        addSettingsPage (miscPage,
                         BinaryData::prefs_misc_png,
                         BinaryData::prefs_misc_pngSize);
        addSettingsPage (exporterPage,
                         BinaryData::prefs_export_png,
                         BinaryData::prefs_export_pngSize);
        addSettingsPage (aboutPage,
                         BinaryData::prefs_about_png,
                         BinaryData::prefs_about_pngSize);
    }

    ~PrefsTabComp()
    {
        StoredSettings::getInstance()->flush();
    }

    Component* createComponentForPage (const String& pageName)
    {
    	if (pageName == miscPage)
        {
            return new MiscPage();
        }
        else if (pageName == aboutPage)
        {
            return new AboutPage();
        }
        else if (pageName == exporterPage)
        {
            return new ExporterPage();
        }

        return new Component();
    }
};


//==============================================================================
static String prefsWindowPos;


PrefsPanel::PrefsPanel()
    : DialogWindow ("Synth-A-Modeler Preferences", Colour::greyLevel (0.92f), true)
{
    PrefsTabComp* const p = new PrefsTabComp();
    p->setSize (456, 510);

    setContentOwned (p, true);

    if (! restoreWindowStateFromString (prefsWindowPos))
        centreWithSize(getWidth(), getHeight());

    setResizable (true, true);
    setResizeLimits (400, 400, 1000, 800);
}

PrefsPanel::~PrefsPanel()
{
    prefsWindowPos = getWindowStateAsString();
}

void PrefsPanel::closeButtonPressed()
{
    setVisible (false);
}

void PrefsPanel::show()
{
    PrefsPanel pp;
    pp.runModalLoop();
}