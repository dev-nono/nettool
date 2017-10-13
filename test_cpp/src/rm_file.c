/*
 * rm_file.c
 *
 *  Created on: Aug 28, 2017
 *      Author: hico
 */
#define _SVID_SOURCE
#define  _GNU_SOURCE

/* Afficher le contenu du répertoire en ordre inverse */
#include <dirent.h>
#include <unistd.h>

#include <stdio.h>

#include "test.h"


static int rm_file_remove(int argc, char **argv)
{
   struct dirent **namelist;
   int ii   = 0;
   int vNb  = 0;
   char vName[1024];

   vNb = scandir(argv[2], &namelist, NULL, versionsort); // versionsort alphasort
   if (vNb < 0)
      perror("scandir");
   else {
      do
      {
         if( (namelist[ii]->d_type & DT_DIR )  == 0 )
         {
            printf("rm_file_remove %s \n",namelist[ii]->d_name);

            sprintf(vName,"%s/%s",
                       argv[2],
                       namelist[ii]->d_name);
            remove(vName);
         }

         ii++;
      }while (ii < vNb);
      free (namelist);
   }
   return 0;
}

static int rm_file_scan(int argc, char **argv)
{

   struct dirent **namelist;
   int ii = 0;
   int vNb = 0;

   vNb = scandir(argv[2], &namelist, NULL, versionsort); // versionsort alphasort
   if (vNb < 0)
      perror("scandir");
   else {
      do
      {
         printf("rm_file_scan %s : %s , type=%#X \n",
                (int)namelist[ii]->d_type & DT_DIR ? "dir " : "file",
                namelist[ii]->d_name,
                (int)namelist[ii]->d_type );
         ii++;
      }while (ii < vNb);
      free (namelist);
   }

   return 0;

}


int main_rm_file(int argc, char **argv)
{

   if( argc > 2 )
   {
      rm_file_scan(argc,argv);
      rm_file_remove(argc,argv);
      rm_file_scan(argc,argv);
   }
   else
   {
      PrintUsage();
   }
   return 0;

}
