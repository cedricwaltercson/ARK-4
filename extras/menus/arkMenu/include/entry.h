#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <malloc.h>
#include "systemctrl.h"
#include "systemctrl_se.h"
#include "common.h"
#include "controller.h"
#include "gfx.h"

#define PSN_DRIVER 0
#define ISO_DRIVER 3

#define HOMEBREW_RUNLEVEL 0x141
#define HOMEBREW_RUNLEVEL_GO 0x152
#define ISO_RUNLEVEL 0x123
#define ISO_RUNLEVEL_GO 0x125
#define POPS_RUNLEVEL 0x144
#define POPS_RUNLEVEL_GO 0x155
#define RECOVERY_RUNLEVEL 0x141

#define ZIP_MAGIC 0x04034b50
#define RAR_MAGIC 0x21726152

int loadIconThread(SceSize _args, void *_argp);

class Entry{

    private:
        void executeHomebrew();
        void executePSN();
        void executePOPS();
        void gameBoot();

    protected:

        string name;
        string path;
        Image* icon0;
        void* icon1;
        Image* pic0;
        Image* pic1;
        void* snd0;
        
        int icon1_size;
        int at3_size;

        //virtual void extractFile(const char * name, unsigned block, unsigned size)=0;
        
        void executeISO();
        
        void executeEboot();
                
    public:
        Entry();
        Entry(string path);
        virtual ~Entry()=0;
        
        string getName();
        void setName(string name);
        string getPath();
        
        Image* getIcon();
        Image* getPic1();
        Image* getPic0();
        
        void freeIcon();
        
        virtual void loadIcon()=0;
        virtual void getTempData1()=0;
        virtual void getTempData2()=0;
        
        void freeTempData();
        
        bool run();
        
        void drawBG();
        
        virtual void execute()=0;
        
        virtual char* getType()=0;
        virtual char* getSubtype()=0;
        
        static bool isZip(const char* path);
        static bool isRar(const char* path);
        
};

#endif
