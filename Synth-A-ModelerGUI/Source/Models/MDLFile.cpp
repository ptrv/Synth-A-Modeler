/*
  ==============================================================================

    MDLFile.cpp
    Created: 11 Apr 2012 3:18:35pm
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

#include "../Application/CommonHeaders.h"
#include "../Utilities/MDLParser.h"
#include "../Utilities/MDLWriter.h"

#include "MDLFile.h"

const char* MDLFile::mdlFileExtension = ".mdl";

MDLFile::MDLFile()
: FileBasedDocument(".mdl", "*.mdl", "Open mdl file", "Save mdl file"),
  mdlRoot(Objects::MDLROOT)
{
	initMDL();
	mdlRoot.addListener(this);

}
MDLFile::MDLFile(const File& file)
: FileBasedDocument(".mdl", "*.mdl", "Open mdl file", "Save mdl file"),
  mdlRoot(Objects::MDLROOT)
{
	initMDL();
	mdlRoot.addListener(this);

	loadDocument(file);
}

MDLFile::~MDLFile()
{
	mdlRoot.removeListener(this);
	removeAllChangeListeners();
	destroyMDL();
}

bool MDLFile::perform (UndoableAction* const action, const String& actionName)
{
	return undoMgr.perform(action, actionName);
}

void MDLFile::initMDL()
{
	mdlRoot = ValueTree(Objects::MDLROOT);
	mdlRoot.setProperty(Ids::mdlName, "Untitled", nullptr);
	mdlRoot.setProperty(Ids::mdlPath, String::empty, nullptr);
	setChangedFlag(false);
}

bool MDLFile::isEmpty()
{
	if(getDocumentTitle().compare("Untitled") == 0 && mdlRoot.getNumChildren() == 0)
	{
		return true;
	}
	return false;
}
void MDLFile::destroyMDL()
{
	mdlRoot = ValueTree();
}

void MDLFile::newMDL()
{
	destroyMDL();
	initMDL();
}


void MDLFile::close()
{
	newMDL();
}

String MDLFile::getDocumentTitle()
{
    return mdlRoot.getProperty(Ids::mdlName).toString();
}
Result MDLFile::loadDocument (const File& file)
{
	destroyMDL();
	initMDL();
	MDLParser pa(*this);
	if(pa.parseMDL())
	{
		// success
		SAM_LOG("Opened MDL file: "+getFilePath());
		setFile(file);
		setChangedFlag(false);
		return Result::ok();
	}
	else
	{
		// fail
		String errorMsg = "ERROR: could not load mdl file.";
		SAM_LOG(errorMsg);
		return Result::fail(errorMsg);
	}
}
Result MDLFile::saveDocument (const File& file)
{
	this->setChangedFlag(false);
	MDLWriter wr(*this);
	if(wr.writeMDL(file))
	{
		SAM_LOG("Saved MDL file: "+file.getFullPathName());
        setFile(file);
        setChangedFlag(false);
		return Result::ok();
	}
	else
	{
		String errorMsg = "ERROR: could not save mdl file.";
		SAM_LOG(errorMsg);
		return Result::fail(errorMsg);
	}
}

File MDLFile::lastDocumentOpened;
File MDLFile::getLastDocumentOpened()
{
	return lastDocumentOpened;
}
void MDLFile::setLastDocumentOpened (const File& file)
{
	lastDocumentOpened = file;
	StoredSettings::getInstance()->setLastDocument(file.getFullPathName());
}
void MDLFile::mdlChanged()
{
	changed();
}
//==============================================================================
void MDLFile::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    mdlChanged();
}

void MDLFile::valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
{
	mdlChanged();
}

void MDLFile::valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved)
{
	mdlChanged();
}

void MDLFile::valueTreeChildOrderChanged (ValueTree& parentTree)
{
	mdlChanged();
}

void MDLFile::valueTreeParentChanged (ValueTree& tree)
{
}

//==============================================================================

ValueTree MDLFile::getObjectWithName(const String& objName)
{
	for (int i = 0; i < mdlRoot.getNumChildren(); ++i) {
		for (int j = 0; j < mdlRoot.getChild(i).getNumChildren(); ++j) {
			ValueTree ch = mdlRoot.getChild(i).getChild(j);
			if(ch[Ids::identifier].toString().compare(objName) == 0)
			{
				return ch;
			}
		}
	}
	return ValueTree::invalid;
}


//==============================================================================
#if UNIT_TESTS

#include "../../Testsuite/MDLFile_test.h"

#endif

