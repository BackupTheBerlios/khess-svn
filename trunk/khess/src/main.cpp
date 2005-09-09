/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 

#include "khess.h"
#include <khapp.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

#include <qregexp.h>

static const char description[] = I18N_NOOP("A KDE Chess Application"); 

static const char version[] = "0.1";

static KCmdLineOptions options[] =
{
	{ "+[URL]", I18N_NOOP( "Document to open" ), 0 },
	KCmdLineLastOption
};


int main(int argc, char **argv)
{ 
// 	QString m = "\nSuperWeh(50): pul didn't swear, he insulted psychoGM's intelligence\nfdasfdsafsa";
// 	QString msg = "bash(50): yes and he ^ ( sme) lls ' of herring\n";
// 	QString msg1 = "GBate(*)(TM)(50): Shhhh, Megs, you'll stir them up ;)";
// 	QString msg2 = "johnnyjohnson(50): are dogs tikilish?";	
// 	QString msg3 = "krawek(50): hi e ^ ( sme) lls ' of(told 297 players in channel 50 \"Chat\")";
// 	QString msg4 = "malmklang(50): hm, I think you give me too much credit but hey, I thank t :)";
// 		/*GuestRQZL(U) tells you: hello*/
// 	
// 	QString nicks = "Channel 85 \"Unix_Linux\": AlexTheGreat Bijection Chaos Chima cox datoga  Doubleletter {Goober(C)} GooberJr(C) Gosseyn HelloGoodbye Houarzhon   kardolus KodeKruncher krawek Liakoni lovetoplay macska MAd marcelk MrBug   Nemisis(SR)(CA) nemsawi nilatac(C) Pierpaolo roem seberg(SR) Shazbat   TaoGizmo(C) {tomatenfisch} vladx(C) wilberforce32 players are in channel 85.";
// 	
// 	
// 	QString sought1 = "99 m 48 2015 BlunderStorm(C)     3   3 rated   suicide                0-9999 f 49 2240 parrot(C)           1   0 rated   lightning              0-9999  50 2434 blik(C)             2   0 rated   lightning              0-9999 mf 51 2341 CraftyNovus(C)     90   0 rated   standard            1800-9999 mf 52 2368 BigGizmo(C)         5   0 rated   blitz                  0-9999 f 53 2368 BigGizmo(C)        10   0 rated   blitz                  0-9999 f 57 2460 nilatac(C)          3   0 unrated suicide                0-9999  58 2285 blik(C)             5   0 rated   blitz                  0-9999 mf 61 2603 TaoGizmo(C)         5   1 rated   blitz                  0-9999 f 62 2603 TaoGizmo(C)        10   1 rated   blitz                  0-9999 f 63 1808 IFDThor(C)          5   0 rated   blitz                  0-9999 f 67 2402 BigGizmo(C)        15   0 rated   standard               0-9999 f 70 1259 claudiogomes       10  15 rated   standard               0-1400  71 2597 TaoGizmo(C)        15   1 rated   standard               0-9999 f 73 2269 gorgu(C)            3   0 unrated suicide                0-9999  77 1919 CatNail(C)          3   0 rated   suicide                0-9999 f 89 2166 seberg              3   0 rated   atomic                 0-9999 mf 94 2066 grogers             3   0 rated   crazyhouse             0-9999 m 95 1503 Garona(C)           4   0 unrated crazyhouse             0-9999 f!";
// 	QString sought2 = "96 1516 LirioDelAgua       15   0 rated   wild/fr    [black]     0-9999  97 1503 Garona(C)           4   0 rated   crazyhouse             0-9999 f 99 1516 LirioDelAgua       20   0 rated   wild/fr    [black]     0-9999 104 ++++ SupraPhonic         5   0 unrated suicide                0-9999 105 ++++ SupraPhonic         3   0 unrated suicide                0-9999 106 ++++ SupraPhonic         0   1 unrated suicide                0-9999 111 2166 seberg              3   3 rated   atomic                 0-9999 mf116 2021 NeedleNum(C)        1   0 rated   lightning              0-9999 mf117 1780 NeedleNum(C)        3   0 rated   blitz                  0-9999 mf118 1971 NeedleNum(C)       15   0 rated   standard               0-9999 mf122 1919 CatNail(C)          3   0 unrated suicide                0-9999 125 2249 vladx(C)            1   0 rated   lightning              0-9999 f127 2117 vladx(C)            5   0 rated   blitz                  0-9999 f128 2273 vladx(C)           15   0 rated   standard               0-9999 f51 ads displayed.";
// 	
// 
// 	QRegExp re("^[\\w]*\\([\\d]*\\):[\\s].*(\\(told \\d+ players in channel.*\\))*");
// 	
// 	QRegExp nic("^Channel \\d+ \\\"(\\w+)\\\": (.*)\\d+ players are in channel \\d+\\.");
// 	
// 	QRegExp sought("(\\d+)\\s+([(\\d+)(\\+\\+\\+\\+)(\\-\\-\\-\\-)]*)\\s+(\\w+\\(\\w\\))*\\s+(\\d+)\\s+(\\d+)\\s+(rated|unrated)\\s+(standard|blitz)");
// 	
// // 	kdDebug() << msg3 << ">>>" << re.search(msg3) << endl;
// 	
// 	while ( true )
// 	{
// 		if ( sought.search(sought1) == -1 )
// 		{
// 			kdDebug() << sought.errorString() << endl;
// 			break;
// 		}
// 	
// 		QStringList list = sought.capturedTexts();
// 	
// 		kdDebug() << ">>>>" << endl;
// 		for (uint i = 0; i < list.count(); i++)
// 		{
// 			kdDebug() << i << " " << list[i].remove('\n') << endl;
// 		}
// 		kdDebug() << "<<<<" << endl;
// 		
// 		sought1.remove(list[0]);
// 		
// 		kdDebug() << "CADENA: " << sought1 << endl;
// 	}
// 	return 0;
	
	KAboutData about("khess", I18N_NOOP("Khess"), version, description,
			 KAboutData::License_GPL, "(C) 2005 David Cuadrado", 0, 0, "krawek@gmail.com");
	about.addAuthor( "David Cuadrado", 0, "krawek@gmail.com" );
	
	about.addCredit( I18N_NOOP("Buga's chess club"), I18N_NOOP("My chess club") );

	KCmdLineArgs::init(argc, argv, &about);
	KCmdLineArgs::addCmdLineOptions(options);
	KHApp app;

    // register ourselves as a dcop client
	app.dcopClient()->registerAs(app.name(), false);

    // see if we are starting with session management
	if (app.isRestored())
	{
		RESTORE(Khess);
	}
	else
	{
        // no session.. just start up normally
		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
		if (args->count() == 0)
		{
			Khess *widget = new Khess;
			widget->show();
		}
		else
		{
			int i = 0;
			for (; i < args->count(); i++)
			{
				Khess *widget = new Khess;
				widget->show();
				widget->load(args->url(i));
			}
		}
		args->clear();
	}
	
// 	QObject::connect(kapp, SIGNAL(lastWindowClosed()), kapp, SLOT(quit()));

	return app.exec();
}
