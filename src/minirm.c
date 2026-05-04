#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


typedef struct{
	int mode; // mode 1 = file, mode 2 = dir
	int idir; // para saber si es dir
	int start;
	int rmproc; // el proceso de removido
	char *path; // bruh
} rm_options;

int isdir(const char *pathname){
	struct stat pathstat;
	if((stat(pathname, &pathstat)) != 0){
		printf("No se a podido acceder al archivo!\n");
		return -1;
	}
	return S_ISDIR(pathstat.st_mode);	
}

int removefile(const char *pathname){
	int flrm = 0;
	printf("Borrando archivo %s\n", pathname);
	flrm = unlink(pathname);
	
	if (flrm != 0){
		printf("Hubo un error al borrar el archivo!\n");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

int removedir(const char *pathname){
	int dirrm = 0;
	printf("Borrando dir %s\n", pathname);
	dirrm = rmdir(pathname);

	if (dirrm != 0){
		printf("Hubo un error al borrar el directorio!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int remove_recursive(char *pathname){
	DIR *dir;
	struct dirent *dp;
	char fullpath[1024];
	int delfile = 0;

	dir = opendir(pathname);

	if (dir == NULL){
		perror("Hubo un error al abrir el directorio!");
		return EXIT_FAILURE;
	}

	while((dp = readdir(dir)) != NULL){
		if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
			continue; // no hace nada lolololololol xd
		}

		snprintf(fullpath, sizeof(fullpath), "%s/%s", pathname, dp->d_name);

		if(isdir(fullpath)){
			delfile = remove_recursive(fullpath);
			if (delfile == EXIT_FAILURE){
				printf("No se pudo borrar archivo %s\n", fullpath);
				closedir(dir);
				return EXIT_FAILURE;
			}
		} else {
			delfile = removefile(fullpath);
			if (delfile == EXIT_FAILURE){
				printf("No se pudo borrar archivo %s\n", fullpath);
				closedir(dir);
				return EXIT_FAILURE;
			}
		}
	}

	closedir(dir);
	
	return removedir(pathname);
}

int main(int argc, char* argv[]){
	rm_options rmopt;

	rmopt.mode = 1;
	rmopt.start = 1;

	if (argc == 1) {
		printf("falta que hacer pdj\n");
		return EXIT_FAILURE;
	} else if  ((strcmp(argv[1], "-rf") == 0) && argc > 2){
		rmopt.mode = 2;
		rmopt.start = 2; 
	}
	
	for (int i=rmopt.start; i < argc; i++){
		rmopt.idir = isdir(argv[i]);

		if(rmopt.idir == -1){
			return EXIT_FAILURE;
		}

		if(rmopt.mode == 1){
			if(rmopt.idir == 1){
				printf("Porfavor use -rf para remover directorios!\n");
				return EXIT_FAILURE;
			}

			rmopt.path = argv[i];

			rmopt.rmproc = removefile(rmopt.path);

			if(rmopt.rmproc == 0){
				printf("se quito el archivo %s\n", rmopt.path);
			}

		} else if (rmopt.mode == 2){
			rmopt.path = argv[i];

			if(rmopt.idir == 1){
				rmopt.rmproc = remove_recursive(rmopt.path);
			} else {
				rmopt.rmproc = removefile(rmopt.path);
			}
			if (rmopt.rmproc == 0){
				printf("Se removio %s exitosamente\n", rmopt.path);
			}
		}	
	}


	return EXIT_SUCCESS;
}
