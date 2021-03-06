/*
 * This file is part of PRO CFW.

 * PRO CFW is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO CFW is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO CFW. If not, see <http://www.gnu.org/licenses/ .
 */

#include "main.h"
#include <functions.h>
#include "flash_dump.h"
#include "main.h"

int kthread(SceSize args, void *argp)
{
    char path[ARK_PATH_SIZE];
    
    // dump files into zip
    strcpy(path, ark_config->arkpath);
    strcat(path, "FLASH0.ZIP");
    PRTSTR1("Dumping to: %s", path);

    flashVitaDump(path);
    
    // dump raw flash
    strcpy(path, ark_config->arkpath);
    strcat(path, "FLASH0.BIN");

    PRTSTR1("Dumping to: %s", path);
    void* sonyF0 = FLASH_SONY;
    uint32_t sonyF0Size = 0x8BF00000-FLASH_SONY;

    SceUID packFileID = k_tbl->KernelIOOpen(path, PSP_O_WRONLY|PSP_O_CREAT|PSP_O_TRUNC, 0777);
    k_tbl->KernelIOWrite(packFileID, sonyF0, sonyF0Size);
    k_tbl->KernelIOClose(packFileID);

    return 0;
}

void initKernelThread(void)
{
    SceUID kthreadID = k_tbl->KernelCreateThread( "", KERNELIFY(kthread), 1, 0x10000, PSP_THREAD_ATTR_VFPU, NULL );
    if (kthreadID >= 0){
        k_tbl->KernelStartThread(kthreadID, 0, NULL);
        k_tbl->waitThreadEnd(kthreadID, NULL);
    }
    else{
        PRTSTR1("Error: could not create kernel thread: %p", kthreadID);
    }
}

unsigned addWriteFile( SceUID packFileID, void *data, unsigned size, char *name, u8 found_nb )
{

    PRTSTR1("Dumping file %s", name);

    const char *root = "flash0";
    char path[256];
    
    strcpy(path, root);
    
    // If file name has no / at first, add it
    if ( name[0] != '/' )
        path[strlen(path)+1] = 0, path[strlen(path)] = '/';
        
    strcat(path, name);
    
    // If file was already dumped, add custom name suffix
    if ( found_nb > 1)    strcat(path, ".bakX"), path[strlen(path)-1] = '0'+found_nb-2;
    
    // Write Data
    struct minZipHeader zHead;
    strcpy(zHead.pk, "PK");
    zHead.nb = 0x000A0403;
    memset(zHead.space, 0, sizeof(zHead.space));
    zHead.fileSize = size;
    zHead.fileSizeClone = size;
    zHead.pathLen = strlen(path);
    
    k_tbl->KernelIOWrite(packFileID, &(zHead.pk), 2);
    k_tbl->KernelIOWrite(packFileID, &(zHead.nb), sizeof(u32));
    k_tbl->KernelIOWrite(packFileID, &(zHead.space), sizeof(zHead.space));
    k_tbl->KernelIOWrite(packFileID, &(zHead.fileSize), sizeof(u32));
    k_tbl->KernelIOWrite(packFileID, &(zHead.fileSizeClone), sizeof(u32));
    k_tbl->KernelIOWrite(packFileID, &(zHead.pathLen), sizeof(u32));
    k_tbl->KernelIOWrite(packFileID, path, zHead.pathLen);
    
    unsigned written = k_tbl->KernelIOWrite(packFileID, data, size);
    
    return written;
}

int findFlashIndex( const VitaFlashBufferFile *f0, void *origContent )
{
    int flash0_filecount = 0;
    
    while ( origContent != f0[flash0_filecount].content && f0[flash0_filecount].name != NULL )
        ++flash0_filecount;
    
    if ( f0[flash0_filecount].name == NULL )    return -1;
    else    return flash0_filecount;
}

void flashVitaDump( char *packName )
{
    uint32_t sonyF0 = FLASH_SONY;
    uint32_t sonyF0Backup = 0x8BF00000;
    int flash0_fileindex = 0;
    uint32_t totalwrite = 0;
    int isValid, i;
    int dummy_count = 0;
    int backup_count = 0;
    char *name = NULL;
    u8 found_f0[512];

    for ( i=0; i<sizeof(found_f0); ++i )    found_f0[i] = 0;
    
    const VitaFlashBufferFile * f0 = (VitaFlashBufferFile *)sonyF0;
    
    SceUID packFileID = k_tbl->KernelIOOpen(packName, PSP_O_WRONLY|PSP_O_CREAT|PSP_O_TRUNC, 0777);
    
    if (packFileID)
    {
        // Write all found flash files
        while ( f0[flash0_fileindex].name != NULL )
        {
            name = f0[flash0_fileindex].name;
            isValid = 0;
            
            if ( name[strlen(name)-3] == 'p' && name[strlen(name)-2] == 'r' && name[strlen(name)-1] == 'x' )
            {
                switch ( *((int*)f0[flash0_fileindex].content) )
                {
                    case 0x5053507E:
                        isValid = 1;
                        break;
                    case 0:
                        dummy_count++;
                        break;
                    default:
                        isValid = 1;
                        break;
                }
            }
            else    isValid = 1;
            
            if ( isValid )
            {
                found_f0[flash0_fileindex]++;
                
                totalwrite += addWriteFile( packFileID, f0[flash0_fileindex].content, f0[flash0_fileindex].size, name, found_f0[flash0_fileindex] );
            }
            else
            {
            }
            
            ++flash0_fileindex;
        }
        
        uint32_t *curf0Backup = (uint32_t*) sonyF0Backup;
        
        // Write all found flash backups
        while ( *curf0Backup != 0 )
        {
            uint32_t origContent = *curf0Backup;
            flash0_fileindex = findFlashIndex(f0, (void*)origContent);
            
            unsigned size = *(curf0Backup+1);
            curf0Backup += 2;
            
            if ( flash0_fileindex >= 0 )
            {
                found_f0[flash0_fileindex]++;
                name = f0[flash0_fileindex].name;
                
                totalwrite += addWriteFile( packFileID, curf0Backup, size, name, found_f0[flash0_fileindex] );
                
                backup_count++;
            }
            else
            {
                break;
            }
            curf0Backup += size/4 + 1;
        }
        k_tbl->KernelIOClose(packFileID);
    }
    else{
        PRTSTR("Could not open file for writing");
    }
}


// Kernel Permission Function
void loadKernelArk(){
    PRTSTR("Dumping flash0");
    initKernelThread();
    PRTSTR("Flash0 Dumped");
    return;
}
