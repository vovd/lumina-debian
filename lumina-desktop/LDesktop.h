//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012-2015, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_LDESKTOP_H
#define _LUMINA_DESKTOP_LDESKTOP_H

#include <QCoreApplication>


#include <QSettings>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QLabel>
#include <QWidgetAction>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QRegion>


#include <LuminaXDG.h>

#include "LPanel.h"
//#include "Globals.h"
#include "AppMenu.h"
#include "desktop-plugins/LDPlugin.h"
#include "desktop-plugins/LDPluginContainer.h"
#include "desktop-plugins/NewDP.h"

class LDesktop : public QObject{
	Q_OBJECT
public:
	LDesktop(int deskNum=0, bool setdefault = false);
	~LDesktop();
	
	int Screen(); //return the screen number this object is managing
	void show();
	void hide();
	void prepareToClose();

	WId backgroundID();
	QRect availableScreenGeom();

	void UpdateGeometry();
	
public slots:
	void SystemLogout();
	void SystemTerminal();
	void SystemFileManager();
	void SystemApplication(QAction*);	

	void checkResolution();

private:
	QSettings *settings;
	QTimer *bgtimer;
	QDesktopWidget *desktop;
	QString DPREFIX;
	int desktopnumber;
	QRegion availDPArea;
	bool defaultdesktop, desktoplocked, issyncing, usewinmenu, bgupdating;
	QStringList oldBGL;
	QList<LPanel*> PANELS;
	QMdiArea *bgDesktop; //desktop widget area
	QWidget *bgWindow; //full screen background
	QMenu *deskMenu, *winMenu;
	//AppMenu *appmenu;
	QLabel *workspacelabel;
	QWidgetAction *wkspaceact;
	QList<LDPlugin*> PLUGINS;
	QString CBG; //current background
	QRect globalWorkRect;
	LDPluginContainer* CreateDesktopPluginContainer(LDPlugin*);

	QPoint findNewPluginLocation(QRegion avail, QSize winsize);
	
private slots:
	void InitDesktop();
	void SettingsChanged();
	void UnlockSettings(){ issyncing=false; }
	void LocaleChanged();
	
	//Menu functions
	void UpdateMenu(bool fast = false);
	void ShowMenu(const QPoint &pt = QPoint()){
	  UpdateMenu(true); //run the fast version
	  //qDebug() << "Show Context Menu:" << QCursor::pos() << pt;
	  if(pt.isNull()){ deskMenu->popup(QCursor::pos()); }
	  else{ deskMenu->popup( bgWindow->mapToGlobal(pt)); }
	}
	void UpdateWinMenu();
	void winClicked(QAction*);
	
	//Desktop plugin system functions
	void UpdateDesktop();
	void ToggleDesktopLock();
	void AlignDesktopPlugins();
	void DesktopPluginRemoved(QString ID, bool internal = false);
	
	void UpdatePanels();
	
	void UpdateDesktopPluginArea(); //make sure the area is not underneath any panels

	void UpdateBackground();
};
#endif
