#include "guiplugin.h"

#include <QtPlugin>

#include <QDebug>

#include "fugio/core/uuid.h"

#include "floatnode.h"
#include "buttonnode.h"
#include "integernode.h"
#include "lcdnumbernode.h"
#include "numbermonitornode.h"
#include "keyboardnode.h"
#include "lednode.h"
#include "slidernode.h"
#include "choicenode.h"
#include "mainwindownode.h"
#include "stringnode.h"
#include "dialnode.h"
#include "screennode.h"

#include "keyboardpin.h"

QList<QUuid>				NodeControlBase::PID_UUID;

ClassEntry		GuiPlugin::mNodeClasses[] =
{
	ClassEntry( "Button", "GUI", NID_BUTTON, &ButtonNode::staticMetaObject ),
	ClassEntry( "Choice", "GUI", NID_CHOICE, &ChoiceNode::staticMetaObject ),
	ClassEntry( "Dial", "GUI", NID_GUI_DIAL, &DialNode::staticMetaObject ),
	ClassEntry( "Keyboard", "GUI", NID_KEYBOARD, &KeyboardNode::staticMetaObject ),
	ClassEntry( "LCD Number", "GUI", NID_LCD_NUMBER, &LcdNumberNode::staticMetaObject ),
	ClassEntry( "LED", "GUI", NID_LED, &LedNode::staticMetaObject ),
	ClassEntry( "Main Window", "GUI", NID_MAIN_WINDOW, &MainWindowNode::staticMetaObject ),
	ClassEntry( "Number (Float)", "GUI", NID_FLOAT, &FloatNode::staticMetaObject ),
	ClassEntry( "Number (Integer)", "GUI", NID_INTEGER, &IntegerNode::staticMetaObject ),
	ClassEntry( "Number Monitor", "GUI", NID_NUMBER_MONITOR, &NumberMonitorNode::staticMetaObject ),
	ClassEntry( "Screen", "GUI", NID_SCREEN, &ScreenNode::staticMetaObject ),
	ClassEntry( "Slider", "GUI", NID_SLIDER, &SliderNode::staticMetaObject ),
	ClassEntry( "String", "GUI", NID_STRING, &StringNode::staticMetaObject ),
	ClassEntry()
};

ClassEntry		GuiPlugin::mPinClasses[] =
{
	ClassEntry( "Keyboard", PID_KEYBOARD, &KeyboardPin::staticMetaObject ),
	ClassEntry()
};

GuiPlugin::GuiPlugin( void )
	: mApp( 0 )
{
}

GuiPlugin::~GuiPlugin( void )
{
}

PluginInterface::InitResult GuiPlugin::initialise( fugio::GlobalInterface *pApp, bool pLastChance )
{
	Q_UNUSED( pLastChance )

	mApp = pApp;

	mApp->registerNodeClasses( mNodeClasses );

	mApp->registerPinClasses( mPinClasses );

	return( INIT_OK );
}

void GuiPlugin::deinitialise()
{
	mApp->unregisterPinClasses( mPinClasses );

	mApp->unregisterNodeClasses( mNodeClasses );

	mApp = 0;
}
