/*
    TN SaveState Plugin
    Copyright (C) 2014, Total_Noob

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <time.h>

#include "../common.h"

#include "start.h"
#include "ya2d.h"
#include "intraFont.h"
#include "menucommon.h"
#include "menumgr.h"

int main(int argc, char** argv)
{

    intraFontInit();
    ya2d_init();

    common::loadData(argc, argv);
    
    MenuManager* menu = new MenuManager();
    menu->run();
    delete menu;
    
    common::deleteData();
    
    intraFontShutdown();
    ya2d_shutdown();

    return 0;
}
